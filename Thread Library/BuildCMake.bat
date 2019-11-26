cls

ECHO %cd%
ECHO Creating build directory
set buildDirectory = %CD%\build
mkdir "%CD%\build"

cd "%CD%\build"

ECHO Running CMakeLists.txt and generating makefiles
cmake ..\
@pause