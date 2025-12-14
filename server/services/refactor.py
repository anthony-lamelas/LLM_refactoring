import os
import argparse
from pathlib import Path
from dotenv import load_dotenv
from globals import system_prompt
from providers import OpenAIProvider, GeminiProvider, ClaudeProvider, GroqProvider, DeepSeekProvider

load_dotenv()

def read_c_file(filepath: str) -> str:
    """Read the C code file to be refactored."""
    with open(filepath, 'r', encoding='utf-8') as f:
        return f.read()

def get_provider(provider_name: str):
    """Get the appropriate LLM provider based on name."""
    providers_map = {
        'openai': ('OPENAI_API_KEY', OpenAIProvider, "gpt-5.2"),
        'gemini': ('GEMINI_API_KEY', GeminiProvider, "gemini-3-pro-preview"),
        'claude': ('CLAUDE_API_KEY', ClaudeProvider, "claude-sonnet-4-5-20250929"),
        'groq': ('GROQ_API_KEY', GroqProvider, "meta-llama/llama-4-maverick-17b-128e-instruct"),
        'deepseek': ('DEEPSEEK_API_KEY', DeepSeekProvider, "deepseek-chat"),
    }
    
    if provider_name not in providers_map:
        raise ValueError(f"Unknown provider: {provider_name}. Available: {list(providers_map.keys())}")
    
    env_var, provider_class, model = providers_map[provider_name]
    api_key = os.getenv(env_var)
    
    if not api_key:
        raise ValueError(f"{env_var} not found in .env file")
    
    return provider_class(api_key, model)

def refactor_with_llm(code: str, provider) -> str:
    """Use LLM provider to refactor C code."""
    return provider.refactor_code(code, system_prompt)

def save_refactored_code(output: str, output_path: str):
    """Save the refactored code to a file."""
    output_path_obj = Path(output_path)
    output_path_obj.parent.mkdir(parents=True, exist_ok=True)
    
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(output)
    print(f"Refactored code saved to: {output_path}")

def get_all_c_files(directory: str) -> list[Path]:
    """Get all .c files from the specified directory."""
    data_dir = Path(directory)
    c_files = list(data_dir.rglob("*.c"))
    return c_files

def main():
    parser = argparse.ArgumentParser(description='Refactor C code using various LLM providers')
    parser.add_argument('--provider', type=str, default='openai',
                       choices=['openai', 'gemini', 'claude', 'groq', 'deepseek'],
                       help='LLM provider to use (default: openai)')
    args = parser.parse_args()
    
    input_dir = "../../data/2024"
    output_dir = f"../../results_{args.provider}"
    
    try:
        provider = get_provider(args.provider)
        print(f"Using {provider.provider_name} with model: {provider.model_name}\n")
    except ValueError as e:
        print(f"Error: {e}")
        return
    
    c_files = get_all_c_files(input_dir)
    
    if not c_files:
        print("No .c files found in data/2024 directory")
        return
    
    print(f"Found {len(c_files)} files to refactor\n")
    
    for i, c_file in enumerate(c_files, 1):
        print(f"[{i}/{len(c_files)}] Processing: {c_file}")
        
        try:
            c_code = read_c_file(str(c_file))
            
            refactored_output = refactor_with_llm(c_code, provider)
            
            relative_path = c_file.relative_to(input_dir)
            output_file = Path(output_dir) / relative_path.with_suffix('.txt')
            
            save_refactored_code(refactored_output, str(output_file))
            
        except Exception as e:
            print(f" Error: {e}\n")
            continue
    
    print(f"\nRefactoring complete! Results saved to '{output_dir}' directory")

if __name__ == "__main__":
    main()