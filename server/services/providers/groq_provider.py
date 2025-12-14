from groq import Groq
from . import LLMProvider

class GroqProvider(LLMProvider):
    def __init__(self, api_key: str, model: str = "meta-llama/llama-4-maverick-17b-128e-instruct"):
        super().__init__(api_key)
        self.client = Groq(api_key=api_key)
        self._model = model
    
    def refactor_code(self, code: str, system_prompt: str) -> str:
        response = self.client.chat.completions.create(
            model=self._model,
            messages=[
                {"role": "system", "content": system_prompt},
                {"role": "user", "content": f"Please refactor the following C code to improve readability, maintainability, and follow best practices:\n\n```c\n{code}\n```"}
            ],
            temperature=0.7,
            max_tokens=4000
        )
        return response.choices[0].message.content or ""
    
    @property
    def provider_name(self) -> str:
        return "groq"
    
    @property
    def model_name(self) -> str:
        return self._model
