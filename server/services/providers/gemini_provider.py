import google.generativeai as genai
from google.generativeai.types import HarmCategory, HarmBlockThreshold
from .base_provider import LLMProvider

class GeminiProvider(LLMProvider):
    def __init__(self, api_key: str, model: str = "gemini-2.5-flash"):
        super().__init__(api_key)
        genai.configure(api_key=api_key)
        
        # Configure safety settings to be more permissive for code
        safety_settings = {
            HarmCategory.HARM_CATEGORY_HARASSMENT: HarmBlockThreshold.BLOCK_NONE,
            HarmCategory.HARM_CATEGORY_HATE_SPEECH: HarmBlockThreshold.BLOCK_NONE,
            HarmCategory.HARM_CATEGORY_SEXUALLY_EXPLICIT: HarmBlockThreshold.BLOCK_NONE,
            HarmCategory.HARM_CATEGORY_DANGEROUS_CONTENT: HarmBlockThreshold.BLOCK_NONE,
        }
        
        self.model = genai.GenerativeModel(
            model,
            safety_settings=safety_settings
        )
        self._model_name = model
    
    def refactor_code(self, code: str, system_prompt: str) -> str:
        prompt = f"{system_prompt}\n\nPlease refactor the following C code to improve readability, maintainability, and follow best practices:\n\n```c\n{code}\n```"
        
        response = self.model.generate_content(
            prompt,
            generation_config={
                "temperature": 0.7,
                "max_output_tokens": 4000,
            }
        )
        
        # Check if response has valid content
        if not response.candidates or not response.candidates[0].content.parts:
            if response.candidates:
                finish_reason = response.candidates[0].finish_reason
                feedback = response.prompt_feedback if hasattr(response, 'prompt_feedback') else None
                raise ValueError(f"Gemini blocked response. Finish reason: {finish_reason}, Feedback: {feedback}")
            raise ValueError("Gemini returned empty response")
        
        return response.text
    
    @property
    def provider_name(self) -> str:
        return "gemini"
    
    @property
    def model_name(self) -> str:
        return self._model_name
