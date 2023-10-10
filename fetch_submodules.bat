@echo off

git init

git remote add origin https://github.com/Jack-The-Ripper-1820/OpenGL-Sailing.git
git fetch --all

git checkout main

git submodule update --init --recursive
