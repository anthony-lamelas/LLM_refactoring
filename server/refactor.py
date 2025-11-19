import os
from pathlib import Path
from dotenv import load_dotenv
from openai import OpenAI
from globals import system_prompt

load_dotenv()

def read_c_file(filepath: str) -> str:
    """Read the C code file to be refactored."""
    with open(filepath, 'r', encoding='utf-8') as f:
        return f.read()

def refactor_with_openai(code: str, api_key: str) -> str:
    """Use OpenAI API to refactor C code."""
    client = OpenAI(api_key=api_key)
    
    response = client.chat.completions.create(
        model="gpt-4o",  
        messages=[
            {"role": "system", "content": system_prompt},
            {"role": "user", "content": f"Please refactor the following C code to improve readability, maintainability, and follow best practices:\n\n```c\n{code}\n```"}
        ],
        temperature=0.7,
        max_tokens=4000
    )
    
    return response.choices[0].message.content or ""

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
    input_dir = "../data/2024"
    output_dir = "../results"
    
    api_key = os.getenv("OPENAI_API_KEY")
    if not api_key:
        raise ValueError("OPENAI_API_KEY not found in .env file")
    
    c_files = get_all_c_files(input_dir)
    
    if not c_files:
        print("No .c files found in data/2024 directory")
        return
    
    for i, c_file in enumerate(c_files, 1):
        print(f"[{i}/{len(c_files)}] Processing: {c_file}")
        
        try:
            c_code = read_c_file(str(c_file))
            
            refactored_output = refactor_with_openai(c_code, api_key)
            
            relative_path = c_file.relative_to(input_dir)
            output_file = Path(output_dir) / relative_path.with_suffix('.txt')
            
            save_refactored_code(refactored_output, str(output_file))
            
        except Exception as e:
            print(f" Error: {e}\n")
            continue
    
    print(f"Refactoring complete, Results saved to '{output_dir}' directory")

if __name__ == "__main__":
    main()