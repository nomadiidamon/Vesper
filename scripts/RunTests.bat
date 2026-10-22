@echo off
python "%~dp0..\tools\run_tests.py" %*
set EXIT_CODE=%errorlevel%
echo.
pause
exit /b %EXIT_CODE%
