from typing import Optional
from pydantic import BaseModel, Field


class CompilationResult(BaseModel):
    """Store compilation results for a file."""
    file_path: str
    compiled: bool
    error_message: Optional[str] = None
    executable_path: Optional[str] = None


class ExecutionResult(BaseModel):
    """Store execution results."""
    ran: bool
    output: Optional[str] = None
    error: Optional[str] = None
    return_code: Optional[int] = None


class ComparisonResult(BaseModel):
    """Store complete comparison results."""
    original_file: str
    refactored_file: str
    original_compilation: CompilationResult
    refactored_compilation: CompilationResult
    original_execution: Optional[ExecutionResult] = None
    refactored_execution: Optional[ExecutionResult] = None
    outputs_match: Optional[bool] = None
    timestamp: str = ""
