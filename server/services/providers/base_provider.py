from abc import ABC, abstractmethod

class LLMProvider(ABC):
    """Base class for all LLM providers."""
    
    def __init__(self, api_key: str):
        self.api_key = api_key
    
    @abstractmethod
    def refactor_code(self, code: str, system_prompt: str) -> str:
        """
        Send code to the LLM for refactoring.
        
        Args:
            code: The C code to refactor
            system_prompt: The system prompt with refactoring guidelines
            
        Returns:
            The refactored code response from the LLM
        """
        pass
    
    @property
    @abstractmethod
    def provider_name(self) -> str:
        """Return the name of the provider (e.g., 'openai', 'gemini')."""
        pass
    
    @property
    @abstractmethod
    def model_name(self) -> str:
        """Return the model name being used (e.g., 'gpt-4', 'gemini-pro')."""
        pass
