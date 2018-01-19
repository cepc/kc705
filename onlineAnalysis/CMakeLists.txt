cmake_minimum_required(VERSION 3.6)

# Make a name for the project
project(jadepix)

option(BUILD_SWIG_LIBS "Build swig library" ON)

# Include all the header files in the directory of the CMakeLists.txt 
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

# Set all the source files to ARP_SRC 
# aux_source_directory only include the dir not sub dir
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src ARP_SRC)

# Set the CXX compile option
# similar to gmake: g++ -std=c++1y ...
if(UNIX)
    set(CMAKE_CXX_FLAGS "-std=c++1y -Wall -Wextra -Wno-unused-parameter -fPIC ${CMAKE_CXX_FLAGS}")
endif()

# Create main and link 
add_library(${PROJECT_NAME} ${ARP_SRC})

# Add swig subdirectory
if(BUILD_SWIG_LIBS)
  add_subdirectory(swig)
endif(BUILD_SWIG_LIBS)

# Add install target
install(TARGETS ${PROJECT_NAME} 
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        PUBLIC_HEADER DESTINATION include)
