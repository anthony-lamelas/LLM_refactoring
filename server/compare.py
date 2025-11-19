import os
import re
import subprocess
import json
from pathlib import Path
from datetime import datetime
from typing import Optional

from models.compare import CompilationResult, ExecutionResult, ComparisonResult

def extract_c_code_from_llm_output(content: str) -> str:
    """Extract C code from LLM output (handles markdown code blocks)."""
    patterns = [
        r'```c\s*\n(.*?)```',
        r'```cpp\s*\n(.*?)```',
        r'```\s*\n(.*?)```',
    ]
    
    for pattern in patterns:
        match = re.search(pattern, content, re.DOTALL)
        if match:
            return match.group(1).strip()
    
    return content.strip()

def compile_c_file(source_path: Path, output_dir: Path, compiler: str = "gcc") -> CompilationResult:
    """Compile a C file using gcc or other compiler."""
    output_dir.mkdir(parents=True, exist_ok=True)
    
    # Create executable path
    exe_name = source_path.stem + ".exe"
    exe_path = output_dir / exe_name
    
    # Compile command
    compile_cmd = [compiler, str(source_path), "-o", str(exe_path)]
    
    try:
        result = subprocess.run(
            compile_cmd,
            capture_output=True,
            text=True,
            timeout=30
        )
        
        if result.returncode == 0:
            return CompilationResult(
                file_path=str(source_path),
                compiled=True,
                executable_path=str(exe_path)
            )
        else:
            return CompilationResult(
                file_path=str(source_path),
                compiled=False,
                error_message=result.stderr
            )
    
    except subprocess.TimeoutExpired:
        return CompilationResult(
            file_path=str(source_path),
            compiled=False,
            error_message="Compilation timeout"
        )
    except FileNotFoundError:
        return CompilationResult(
            file_path=str(source_path),
            compiled=False,
            error_message=f"Compiler '{compiler}' not found. Install MinGW or MSVC."
        )
    except Exception as e:
        return CompilationResult(
            file_path=str(source_path),
            compiled=False,
            error_message=str(e)
        )

def run_executable(exe_path: str, input_data: Optional[str] = None, timeout: int = 5) -> ExecutionResult:
    """Run a compiled executable and capture output."""
    try:
        result = subprocess.run(
            [exe_path],
            input=input_data,
            capture_output=True,
            text=True,
            timeout=timeout
        )
        
        return ExecutionResult(
            ran=True,
            output=result.stdout,
            error=result.stderr,
            return_code=result.returncode
        )
    
    except subprocess.TimeoutExpired:
        return ExecutionResult(
            ran=False,
            error="Execution timeout"
        )
    except Exception as e:
        return ExecutionResult(
            ran=False,
            error=str(e)
        )

def compare_files(original_c: Path, refactored_txt: Path, compiler: str = "gcc") -> ComparisonResult:
    """Compare original and refactored C files."""
    
    # Create temp directory for refactored C file
    temp_dir = Path("temp_refactored")
    temp_dir.mkdir(exist_ok=True)
    
    with open(refactored_txt, 'r', encoding='utf-8') as f:
        refactored_content = f.read()
    
    refactored_code = extract_c_code_from_llm_output(refactored_content)
    
    refactored_c = temp_dir / f"{original_c.stem}_refactored.c"
    with open(refactored_c, 'w', encoding='utf-8') as f:
        f.write(refactored_code)
    
    build_dir = Path("build")
    original_result = compile_c_file(original_c, build_dir / "original", compiler)
    
    refactored_result = compile_c_file(refactored_c, build_dir / "refactored", compiler)
    
    comparison = ComparisonResult(
        original_file=str(original_c),
        refactored_file=str(refactored_txt),
        original_compilation=original_result,
        refactored_compilation=refactored_result,
        timestamp=datetime.now().isoformat()
    )
    
    # If both compiled successfully, try to run them
    if original_result.compiled and refactored_result.compiled:
        print(f"  Running executables...")
        
        # Run without input first
        if original_result.executable_path and refactored_result.executable_path:
            original_exec = run_executable(original_result.executable_path)
            refactored_exec = run_executable(refactored_result.executable_path)
        
            comparison.original_execution = original_exec
            comparison.refactored_execution = refactored_exec
            
            # Compare outputs
            if original_exec.ran and refactored_exec.ran:
                comparison.outputs_match = (
                    original_exec.output == refactored_exec.output and
                    original_exec.return_code == refactored_exec.return_code
                )
    
    return comparison

def get_all_c_files(directory: str) -> list[Path]:
    """Get all .c files from the specified directory."""
    data_dir = Path(directory)
    return list(data_dir.rglob("*.c"))

def find_corresponding_refactored_file(original_c: Path, original_dir: Path, results_dir: Path) -> Optional[Path]:
    """Find the corresponding refactored .txt file for an original .c file."""
    relative_path = original_c.relative_to(original_dir)
    refactored_path = results_dir / relative_path.with_suffix('.txt')
    
    return refactored_path if refactored_path.exists() else None

def generate_summary_report(comparisons: list[ComparisonResult], output_file: Path):
    """Generate a summary report of all comparisons."""
    total = len(comparisons)
    original_compiled = sum(1 for c in comparisons if c.original_compilation.compiled)
    refactored_compiled = sum(1 for c in comparisons if c.refactored_compilation.compiled)
    both_compiled = sum(1 for c in comparisons if c.original_compilation.compiled and c.refactored_compilation.compiled)
    outputs_match = sum(1 for c in comparisons if c.outputs_match is True)
    
    # Calculate percentages safely
    both_compiled_pct = f"{outputs_match/both_compiled*100:.1f}%" if both_compiled > 0 else "N/A"
    
    report = f"""
Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}

Total files compared: {total}

Compilation Results:
  Original files compiled:    {original_compiled}/{total} ({original_compiled/total*100:.1f}%)
  Refactored files compiled:  {refactored_compiled}/{total} ({refactored_compiled/total*100:.1f}%)
  Both compiled successfully: {both_compiled}/{total} ({both_compiled/total*100:.1f}%)

Execution Results:
  Outputs match: {outputs_match}/{both_compiled} ({both_compiled_pct} of successfully compiled pairs)

DETAILED RESULTS:
"""
    
    for i, comp in enumerate(comparisons, 1):
        report += f"\n{i}. {Path(comp.original_file).name}\n"
        report += f"   Original compiled: {'✓' if comp.original_compilation.compiled else '✗'}\n"
        report += f"   Refactored compiled: {'✓' if comp.refactored_compilation.compiled else '✗'}\n"
        
        if comp.outputs_match is not None:
            report += f"   Outputs match: {'✓' if comp.outputs_match else '✗'}\n"
        
        if not comp.original_compilation.compiled and comp.original_compilation.error_message:
            error_msg = comp.original_compilation.error_message[:100]
            report += f"   Original error: {error_msg}...\n"
        if not comp.refactored_compilation.compiled and comp.refactored_compilation.error_message:
            error_msg = comp.refactored_compilation.error_message[:100]
            report += f"   Refactored error: {error_msg}...\n"
        
    # Save report
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(report)
    
    print(report)
    print(f"\nReport saved to: {output_file}")

def save_detailed_json(comparisons: list[ComparisonResult], output_file: Path):
    """Save detailed comparison results as JSON."""
    data = [c.model_dump() for c in comparisons]
    
    with open(output_file, 'w', encoding='utf-8') as f:
        json.dump(data, f, indent=2)
    
    print(f"JSON results saved to: {output_file}")

def main():
    original_dir = Path("../data/2024")
    results_dir = Path("../results")
    reports_dir = Path("../reports")
    compiler = "gcc"  
    
    reports_dir.mkdir(exist_ok=True)
    
    # Get all original C files
    c_files = get_all_c_files(str(original_dir))
    print(f"Found {len(c_files)} original .c files\n")
    
    comparisons = []
    processed = 0
    
    for i, original_c in enumerate(c_files, 1):
        # Find corresponding refactored file
        refactored_txt = find_corresponding_refactored_file(original_c, original_dir, results_dir)
        
        if not refactored_txt:
            print(f"[{i}/{len(c_files)}] Skipping {original_c.name} - no refactored version found")
            continue
        
        print(f"[{i}/{len(c_files)}] Comparing: {original_c.name}")
        
        try:
            comparison = compare_files(original_c, refactored_txt, compiler)
            comparisons.append(comparison)
            processed += 1
            
            # Show immediate result
            if comparison.original_compilation.compiled and comparison.refactored_compilation.compiled:
                if comparison.outputs_match:
                    print(f" Both compiled and outputs match!\n")
                else:
                    print(f" Both compiled but outputs differ\n")
            elif comparison.refactored_compilation.compiled:
                print(f" Only refactored compiled\n")
            elif comparison.original_compilation.compiled:
                print(f" Only original compiled\n")
            else:
                print(f" Neither compiled\n")
                
        except Exception as e:
            print(f"  Error during comparison: {e}\n")
            continue
    
    if comparisons:
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        summary_file = reports_dir / f"comparison_summary_{timestamp}.txt"
        json_file = reports_dir / f"comparison_details_{timestamp}.json"
        
        generate_summary_report(comparisons, summary_file)
        save_detailed_json(comparisons, json_file)
    else:
        print("No files were compared. Make sure refactored files exist.")

if __name__ == "__main__":
    main()