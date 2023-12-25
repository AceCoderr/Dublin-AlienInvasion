## This is a necessary command for every CMake Project.
## You cannot skip this command
# The file with this command must be present in the
# root directory for the project
cmake_minimum_required(VERSION 3.22)
## You can make comments using # just like we do in Python ##
## Defining the project settings, the parameters are in the following sequence
# Name of the projects
# Version of the project (Major.Minor.Patch)
# Supported Languages
project(
firstProject
VERSION 1.0.0
LANGUAGES C CXX
)
## We are setting in-built variables to match our
## project settings/requirements
# Setting the required language version
set(CMAKE_CXX_STANDARD 17)
# Making sure the mentioned language version is enforced
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# For external libraries, we do not need these for this tutorial
set(CMAKE_CXX_EXTENSIONS OFF)
# Setting the Executable and Library Name
set(EXECUTABLE_NAME Executable)
set(LIBRARY_NAME Library)
# Since src (Application) is dependent on include (Custom Libraries)
# We include them in a reverse order, otherwise
# The application will throw an error while building
add_subdirectory(include)
add_subdirectory(src)
# Adding external libraries
# We are using a module offered by CMake here
include(FetchContent)
## 2 subfunctions are required
# This will declare - get the required files from the GitHub Repo, the parameters
are:
# 1. Project name as present in GitHub repo CMakeLists.txt
# 2. GIT_REPOSITORY <link_of_repo> - The repo that we want to copy
# 3. GIT_TAG <release_tag> - This will get the particular tag/version/revision that is mentioned here
# 4. GIT_SHALLOW - To prevent recursive cloning of the repo
FetchContent_Declare(
    fmt
    GIT_REPOSITORY "https://github.com/fmtlib/fmt"
    GIT_TAG 9.1.0
    GIT_SHALLOW TRUE
)
# This will make sure that the external library is available to use with the app, the parameters are:
# 1. Project name as mentioned in FetchContent_Declare (1st parameter)
FetchContent_MakeAvailable(fmt)