@echo off

REM Check if "dependencies" folder exists, if not, create it
if not exist "dependencies" mkdir dependencies

cd dependencies

REM Clone each repository into its respective directory
git clone https://github.com/glfw/glfw.git glfw
git clone https://github.com/g-truc/glm.git glm
git clone https://github.com/assimp/assimp.git assimp
git clone https://github.com/Perlmint/glew-cmake.git glew

cd ..

echo Dependencies have been cloned successfully.
pause
