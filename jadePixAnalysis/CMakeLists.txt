cmake_minimum_required(VERSION 3.6)

# Make a name for the project
project(jadepixAna)

# Include all the header files in the directory of the CMakeLists.txt 
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# aux_source_directory only include the dir not sub dir
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

option(WITH_ROOT_BUILD "Build example with ROOT" ON)
if(WITH_ROOT_BUILD)
    find_package(ROOT)
    include_directories(${ROOT_INCLUDE_DIRS})
endif()

# Set the CXX compile option
# similar to gmake: g++ -std=c++1y ...
if(WIN32)
    set(CMAKE_CXX_FLGAS "-D_hypot=hypot -std=c++1y -Wall -Wextra -Wno-unused-parameter -fPIC ${CMAKE_CXX_FLAGS}")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "-std=c++1y -Wall -Wextra -Wno-unused-parameter -fPIC ${CMAKE_CXX_FLAGS}")
endif()

# Create main and link 
add_executable(decode2root decode2root.cc ${sources} ${headers})
add_library(libdecode2root decode2root.cc ${sources})

if(WITH_ROOT_BUILD)
    target_link_libraries(decode2root ${ROOT_LIBRARIES})
    target_link_libraries(libdecode2root ${ROOT_LIBRARIES})
endif()

#add_executable(pixelana pixelana.cc ${sources} ${headers})
#add_library(libpixelana pixelana.cc ${sources})
#if(WITH_ROOT_BUILD)
#    target_link_libraries(pixelana ${ROOT_LIBRARIES})
#    target_link_libraries(libpixelana ${ROOT_LIBRARIES})
#endif()

option(WITH_SWIG_BUILD "Build example with SWIG" ON)
if(WITH_SWIG_BUILD)
    add_subdirectory(swig)
endif()

# Add install target
#install(TARGETS decode2root pixelana  libdecode2root libpixelana
install(TARGETS decode2root libdecode2root
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        PUBLIC_HEADER DESTINATION include)

