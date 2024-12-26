cmake_minimum_required(VERSION 3.14)
project(DSA_FINAL_PROJECT)

set(CMAKE_CXX_STANDARD 17)

# Locate SFML
set(SFML_DIR "C:/SFML/lib/cmake/SFML") # Adjust the path to your SFML location
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add your executable
add_executable(MySFMLApp main.cpp)

# Link SFML to your project
target_link_libraries(MySFMLApp sfml-graphics sfml-window sfml-system)
