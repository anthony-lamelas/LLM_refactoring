from openai import OpenAI
from .base_provider import LLMProvider

class DeepSeekProvider(LLMProvider):
    """DeepSeek provider."""
    
    def __init__(self, api_key: str, model: str = "deepseek-chat"):
        super().__init__(api_key)
        self.client = OpenAI(
            api_key=api_key,
            base_url="https://api.deepseek.com"
        )
        self._model = model
    
    def refactor_code(self, code: str, system_prompt: str) -> str:
        """Use DeepSeek API to refactor C code."""
        response = self.client.chat.completions.create(
            model=self._model,
            messages=[
                {"role": "system", "content": system_prompt},
                {"role": "user", "content": f"Please refactor the following C code to improve readability, maintainability, and follow best practices:\n\n```c\n{code}\n```"}
            ],
            temperature=0.7
        )
        
        return response.choices[0].message.content or ""
    
    @property
    def provider_name(self) -> str:
        return "deepseek"
    
    @property
    def model_name(self) -> str:
        return self._model
