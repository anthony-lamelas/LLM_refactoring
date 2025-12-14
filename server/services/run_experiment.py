#!/usr/bin/env python3
"""
Batch experiment runner for comparing multiple LLM providers on C code refactoring.
"""
import os
import subprocess
import json
from pathlib import Path
from datetime import datetime
import time

# Available providers to test
PROVIDERS = ['openai', 'gemini', 'claude', 'groq', 'deepseek']

def check_api_keys():
    """Check which API keys are available."""
    key_mapping = {
        'openai': 'OPENAI_API_KEY',
        'gemini': 'GEMINI_API_KEY',
        'claude': 'CLAUDE_API_KEY',
        'groq': 'GROQ_API_KEY',
        'deepseek': 'DEEPSEEK_API_KEY',
    }
    
    available = []
    missing = []
    
    for provider, env_var in key_mapping.items():
        if os.getenv(env_var):
            available.append(provider)
        else:
            missing.append(provider)
    
    return available, missing

def run_refactoring(provider: str):
    """Run refactoring for a specific provider."""
    print(f"\n{'='*60}")
    print(f"REFACTORING WITH {provider.upper()}")
    print(f"{'='*60}\n")
    
    start_time = time.time()
    
    result = subprocess.run(
        ['python', 'refactor.py', '--provider', provider],
        capture_output=True,
        text=True
    )
    
    duration = time.time() - start_time
    
    if result.returncode == 0:
        print(f"✓ {provider} refactoring completed in {duration:.1f}s")
        return True, duration
    else:
        print(f"✗ {provider} refactoring failed:")
        print(result.stderr)
        return False, duration

def run_comparison(provider: str):
    """Run comparison for a specific provider."""
    print(f"\n{'='*60}")
    print(f"COMPARING {provider.upper()} RESULTS")
    print(f"{'='*60}\n")
    
    start_time = time.time()
    
    result = subprocess.run(
        ['python', 'compare.py', '--provider', provider],
        capture_output=True,
        text=True
    )
    
    duration = time.time() - start_time
    
    if result.returncode == 0:
        print(f"✓ {provider} comparison completed in {duration:.1f}s")
        return True, duration
    else:
        print(f"✗ {provider} comparison failed:")
        print(result.stderr)
        return False, duration

def extract_results_from_report(provider: str):
    """Extract key metrics from the comparison report."""
    reports_dir = Path(f"../../reports_{provider}")
    
    # Find the most recent report
    reports = list(reports_dir.glob("comparison_details_*.json"))
    if not reports:
        return None
    
    latest_report = max(reports, key=lambda p: p.stat().st_mtime)
    
    with open(latest_report, 'r') as f:
        data = json.load(f)
    
    total = len(data)
    original_compiled = sum(1 for d in data if d['original_compilation']['compiled'])
    refactored_compiled = sum(1 for d in data if d['refactored_compilation']['compiled'])
    both_compiled = sum(1 for d in data if d['original_compilation']['compiled'] and d['refactored_compilation']['compiled'])
    outputs_match = sum(1 for d in data if d.get('outputs_match') is True)
    
    return {
        'provider': provider,
        'total_files': total,
        'original_compiled': original_compiled,
        'refactored_compiled': refactored_compiled,
        'both_compiled': both_compiled,
        'outputs_match': outputs_match,
        'original_pct': f"{original_compiled/total*100:.1f}%",
        'refactored_pct': f"{refactored_compiled/total*100:.1f}%",
        'match_pct': f"{outputs_match/both_compiled*100:.1f}%" if both_compiled > 0 else "N/A"
    }

def generate_cross_provider_report(results: dict):
    """Generate a comparison report across all providers."""
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    report_path = Path(f"../../reports_combined/experiment_summary_{timestamp}.txt")
    report_path.parent.mkdir(exist_ok=True)
    
    report = f"""
{'='*80}
MULTI-PROVIDER C CODE REFACTORING EXPERIMENT
Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}
{'='*80}

PROVIDERS TESTED: {', '.join(results.keys())}

COMPILATION SUCCESS RATES:
"""
    
    # Table header
    report += f"\n{'Provider':<15} {'Original':<12} {'Refactored':<12} {'Both':<12} {'Outputs Match':<15}\n"
    report += "-" * 80 + "\n"
    
    for provider, data in results.items():
        if data:
            report += f"{provider:<15} {data['original_pct']:<12} {data['refactored_pct']:<12} "
            report += f"{data['both_compiled']}/{data['total_files']:<9} {data['match_pct']:<15}\n"
    
    report += "\n\nDETAILED METRICS:\n" + "="*80 + "\n\n"
    
    for provider, data in results.items():
        if data:
            report += f"{provider.upper()}:\n"
            report += f"  Total files: {data['total_files']}\n"
            report += f"  Original compiled: {data['original_compiled']} ({data['original_pct']})\n"
            report += f"  Refactored compiled: {data['refactored_compiled']} ({data['refactored_pct']})\n"
            report += f"  Both compiled: {data['both_compiled']}\n"
            report += f"  Outputs match: {data['outputs_match']} ({data['match_pct']})\n\n"
    
    # Find best performer
    valid_results = {k: v for k, v in results.items() if v}
    if valid_results:
        best_compilation = max(valid_results.items(), key=lambda x: x[1]['refactored_compiled'])
        best_accuracy = max(
            [(k, v) for k, v in valid_results.items() if v['both_compiled'] > 0],
            key=lambda x: x[1]['outputs_match']
        )
        
        report += f"\nBEST PERFORMERS:\n"
        report += f"  Highest compilation rate: {best_compilation[0]} ({best_compilation[1]['refactored_pct']})\n"
        report += f"  Highest output accuracy: {best_accuracy[0]} ({best_accuracy[1]['match_pct']})\n"
    
    with open(report_path, 'w') as f:
        f.write(report)
    
    print(report)
    print(f"\nCombined report saved to: {report_path}")

def main():
    print("="*80)
    print("MULTI-PROVIDER C CODE REFACTORING EXPERIMENT")
    print("="*80)
    
    # Check available API keys
    available, missing = check_api_keys()
    
    print(f"\nAvailable providers: {', '.join(available)}")
    if missing:
        print(f"Missing API keys for: {', '.join(missing)}")
    
    if not available:
        print("\nError: No API keys found. Please configure .env file.")
        return
    
    print(f"\nThis experiment will:")
    print(f"1. Refactor all C files using each provider")
    print(f"2. Compile and compare results")
    print(f"3. Generate a cross-provider comparison report")
    print(f"\nEstimated time: 15-30 minutes per provider")
    
    input("\nPress Enter to start the experiment...")
    
    experiment_start = time.time()
    results = {}
    
    for provider in available:
        provider_start = time.time()
        
        # Run refactoring
        refactor_success, refactor_time = run_refactoring(provider)
        
        if not refactor_success:
            print(f"\nSkipping comparison for {provider} due to refactoring failure")
            results[provider] = None
            continue
        
        # Run comparison
        compare_success, compare_time = run_comparison(provider)
        
        if not compare_success:
            print(f"\nWarning: Comparison for {provider} failed")
            results[provider] = None
            continue
        
        # Extract results
        metrics = extract_results_from_report(provider)
        results[provider] = metrics
        
        provider_duration = time.time() - provider_start
        print(f"\n{provider} completed in {provider_duration:.1f}s")
        print(f"  Refactored compiled: {metrics['refactored_pct']}")
        print(f"  Outputs match: {metrics['match_pct']}")
    
    experiment_duration = time.time() - experiment_start
    
    print(f"\n{'='*80}")
    print(f"EXPERIMENT COMPLETE")
    print(f"Total duration: {experiment_duration/60:.1f} minutes")
    print(f"{'='*80}\n")
    
    # Generate cross-provider report
    if any(results.values()):
        generate_cross_provider_report(results)
    else:
        print("No results to compare. All providers failed.")

if __name__ == "__main__":
    main()
