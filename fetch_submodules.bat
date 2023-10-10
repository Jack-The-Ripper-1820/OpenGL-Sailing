@echo off

git init

git remote add origin https://github.com/Jack-The-Ripper-1820/OpenGL-Sailing.git

git pull origin main

git submodule update --init --recursive

git add . 

git commit -m "local commit"
