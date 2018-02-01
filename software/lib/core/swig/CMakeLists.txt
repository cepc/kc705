option(JADE_BUILD_LEGACY_CORE_PYTHON_SWIG "JADE_BUILD_CORE_PYTHON_SWIG" ON)
if(NOT JADE_BUILD_LEGACY_CORE_PYTHON_SWIG)
  return()
endif()

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

set_source_files_properties(daq.i PROPERTIES CPLUSPLUS ON)
swig_add_library(daq LANGUAGE python SOURCES daq.i)
swig_link_libraries(daq ${JADE_LEGACY_CORE} ${PYTHON_LIBRARIES})

if(WIN32)
  set(LIB_FILE_PYTHON Release/_daq.pyd)
else()
  set(LIB_FILE_PYTHON _daq.so)
endif()

set(PYTHON_INSTALL_FILES
  ${CMAKE_CURRENT_BINARY_DIR}/daq.py
  ${CMAKE_CURRENT_BINARY_DIR}/${LIB_FILE_PYTHON})

# Configure setup.py and copy to output directory
set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in)
set(SETUP_PY_OUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
configure_file(${SETUP_PY_IN} ${SETUP_PY_OUT})

# Install target to call setup.py
add_custom_target(install-python
  DEPENDS _daq
  COMMAND python ${SETUP_PY_OUT} install)
