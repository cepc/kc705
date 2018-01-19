option(JADE_BUILD_DECODE_PYTHON_SWIG "JADE_BUILD_DECODE_PYTHON_SWIG" ON)
if(NOT JADE_BUILD_DECODE_PYTHON_SWIG)
  return()
endif()

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

add_subdirectory(python)
