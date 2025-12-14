from anthropic import Anthropic
from . import LLMProvider

class ClaudeProvider(LLMProvider):
    def __init__(self, api_key: str, model: str = "claude-sonnet-4-5-20250929"):
        super().__init__(api_key)
        self.client = Anthropic(api_key=api_key)
        self._model = model
    
    def refactor_code(self, code: str, system_prompt: str) -> str:
        response = self.client.messages.create(
            model=self._model,
            max_tokens=4000,
            temperature=0.7,
            system=system_prompt,
            messages=[
                {"role": "user", "content": f"Please refactor the following C code to improve readability, maintainability, and follow best practices:\n\n```c\n{code}\n```"}
            ]
        )
        return response.content[0].text
    
    @property
    def provider_name(self) -> str:
        return "claude"
    
    @property
    def model_name(self) -> str:
        return self._model
