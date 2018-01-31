set(CMAKE_SWIG_FLAGS "")
set_source_files_properties(../daq.i PROPERTIES CPLUSPLUS ON)

# Add swig module
swig_add_library(daq LANGUAGE python SOURCES ../daq.i)
swig_link_libraries(daq ${JADE_LEGACY_CORE} ${PYTHON_LIBRARIES})

if(UNIX)
  # Files to install with Python
  set(PYTHON_INSTALL_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/daq.py
    ${CMAKE_CURRENT_BINARY_DIR}/_daq.so)
elseif(WIN32)
  set(PYTHON_INSTALL_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/daq.py
    ${CMAKE_CURRENT_BINARY_DIR}/Release/_daq.pyd)
endif()

# Configure setup.py and copy to output directory
set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in)
set(SETUP_PY_OUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
configure_file(${SETUP_PY_IN} ${SETUP_PY_OUT})

# Install target to call setup.py
add_custom_target(install-python
  DEPENDS _daq
  COMMAND python ${SETUP_PY_OUT} install)
