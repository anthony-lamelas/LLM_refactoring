import os
from dotenv import load_dotenv
from openai import OpenAI
from globals import system_prompt

load_dotenv()

def read_c_file(filepath: str) -> str:
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
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(output)
    print(f"Refactored code saved to: {output_path}")

def main():
    input_file = "data/2024_prog.c"
    output_file = "data/2024_prog_refactored.txt"
    
    c_code = read_c_file(input_file)
    
    api_key = os.getenv("OPENAI_API_KEY")
    if not api_key:
        raise ValueError("OPENAI_API_KEY not found in .env file")
    
    refactored_output = refactor_with_openai(c_code, api_key)
    
    print("REFACTORED CODE:")
    print(refactored_output)
    
    save_refactored_code(refactored_output, output_file)

if __name__ == "__main__":
    main()