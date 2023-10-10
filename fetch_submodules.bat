@echo off

REM Initialize the repo as a Git repository
git init

REM Add the remote
git remote add origin https://github.com/Jack-The-Ripper-1820/OpenGL-Sailing.git

REM Fetch all the data from the repo (not changing any files in the working directory)
git fetch origin

REM Set the local branch to track the remote main branch without checking it out
git branch --track main origin/main

REM Update the submodules
git submodule update --init --recursive

REM Add all changes
git add .

REM Commit the changes
git commit -m "local commit"
