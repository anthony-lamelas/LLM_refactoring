from openai import OpenAI
from . import LLMProvider

class OpenAIProvider(LLMProvider):
    def __init__(self, api_key: str, model: str = "gpt-5.2"):
        super().__init__(api_key)
        self.client = OpenAI(api_key=api_key)
        self._model = model
    
    def refactor_code(self, code: str, system_prompt: str) -> str:
        response = self.client.chat.completions.create(
            model=self._model,
            messages=[
                {"role": "system", "content": system_prompt},
                {"role": "user", "content": f"Please refactor the following C code to improve readability, maintainability, and follow best practices:\n\n```c\n{code}\n```"}
            ],
            temperature=0.7,
            max_completion_tokens=4000
        )
        return response.choices[0].message.content or ""
    
    @property
    def provider_name(self) -> str:
        return "openai"
    
    @property
    def model_name(self) -> str:
        return self._model
