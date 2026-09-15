cmake_minimum_required(VERSION 3.20)
project(SudokuProject LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

# Find wxWidgets core and base modules
find_package(wxWidgets REQUIRED COMPONENTS core base)

# Create the executable
add_executable(Sudoku src/main.cpp
                      src/Sudoku.hpp
                      src/Sudoku.cpp
                      src/SudokuPanel.hpp
                      src/SudokuPanel.cpp
                      src/SudokuFrame.hpp
                      src/SudokuFrame.cpp
                      src/Themes.h
                      src/algos.hpp
                      src/algos.cpp)

add_custom_command(TARGET Sudoku POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:Sudoku>/data"
)

# Link wxWidgets (CMake handles include paths, defines, and libs automatically)
target_link_libraries(Sudoku PRIVATE ${wxWidgets_LIBRARIES})
target_include_directories(Sudoku PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_compile_definitions(Sudoku PRIVATE ${wxWidgets_DEFINITIONS})

######################################################################
#                             TESTS                                  #
######################################################################
add_executable(SudokuTests src/algos.hpp
                           src/algos.cpp
                           tests/algorithmTests.cpp)
  
target_link_libraries(SudokuTests PRIVATE ${wxWidgets_LIBRARIES})
target_include_directories(SudokuTests PRIVATE src ${wxWidgets_INCLUDE_DIRS})
target_compile_definitions(SudokuTests PRIVATE ${wxWidgets_DEFINITIONS})
