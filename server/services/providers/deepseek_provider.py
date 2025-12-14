from openai import OpenAI
import time
from .base_provider import LLMProvider

class DeepSeekProvider(LLMProvider):
    """DeepSeek provider."""
    
    def __init__(self, api_key: str, model: str = "deepseek-chat"):
        super().__init__(api_key)
        self.client = OpenAI(
            api_key=api_key,
            base_url="https://api.deepseek.com",
            timeout=120.0  # Increase timeout to 2 minutes
        )
        self._model = model
    
    def refactor_code(self, code: str, system_prompt: str) -> str:
        """Use DeepSeek API to refactor C code with retry logic."""
        max_retries = 3
        retry_delay = 5
        
        for attempt in range(max_retries):
            try:
                response = self.client.chat.completions.create(
                    model=self._model,
                    messages=[
                        {"role": "system", "content": system_prompt},
                        {"role": "user", "content": f"Please refactor the following C code to improve readability, maintainability, and follow best practices:\n\n```c\n{code}\n```"}
                    ],
                    temperature=0.7
                )
                
                return response.choices[0].message.content or ""
                
            except Exception as e:
                if attempt < max_retries - 1:
                    print(f"  Retry {attempt + 1}/{max_retries} after {retry_delay}s...")
                    time.sleep(retry_delay)
                    retry_delay *= 2  # Exponential backoff
                else:
                    raise  # Re-raise on final attempt
    
    @property
    def provider_name(self) -> str:
        return "deepseek"
    
    @property
    def model_name(self) -> str:
        return self._model
