option(JADE_BUILD_LEGACY_DECODE_PYTHON_SWIG "JADE_BUILD_LEGACY_DECODE_PYTHON_SWIG" ON)
if(NOT JADE_BUILD_LEGACY_DECODE_PYTHON_SWIG)
  return()
endif()

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

set_source_files_properties(jadepix.i PROPERTIES CPLUSPLUS ON)
swig_add_library(jadepix LANGUAGE python SOURCES jadepix.i)
swig_link_libraries(jadepix jadepix ${PYTHON_LIBRARIES})

if(WIN32)
  set(LIB_FILE_PYTHON Release/_jadepix.pyd)
else()
  set(LIB_FILE_PYTHON _jadepix.so)
endif()

set(PYTHON_INSTALL_FILES
  ${CMAKE_CURRENT_BINARY_DIR}/jadepix.py
  ${CMAKE_CURRENT_BINARY_DIR}/${LIB_FILE_PYTHON})

# Configure setup.py and copy to output directory
set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in)
set(SETUP_PY_OUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
configure_file(${SETUP_PY_IN} ${SETUP_PY_OUT})

# Install target to call setup.py
add_custom_target(install-python-decode
        DEPENDS _jadepix
        COMMAND python ${SETUP_PY_OUT} install)
