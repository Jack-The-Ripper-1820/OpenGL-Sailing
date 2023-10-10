#!/bin/bash

# Check if "dependencies" directory exists, if not, create it
[ -d "dependencies" ] || mkdir dependencies

cd dependencies

# Clone each repository into its respective directory
[ -d "glfw" ] || git clone https://github.com/glfw/glfw.git glfw
[ -d "glm" ] || git clone https://github.com/g-truc/glm.git glm
[ -d "assimp" ] || git clone https://github.com/assimp/assimp.git assimp
[ -d "glew" ] || git clone https://github.com/Perlmint/glew-cmake.git glew

cd ..

echo "Dependencies have been cloned successfully."
read -p "Press enter to continue"
