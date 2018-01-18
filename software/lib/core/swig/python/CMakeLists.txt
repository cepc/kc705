# Include python
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})


set(CMAKE_SWIG_FLAGS "")
set_source_files_properties(../${PROJECT_NAME}.i PROPERTIES CPLUSPLUS ON)
include_directories(../..)

# Add swig module
swig_add_module(daq python ../${PROJECT_NAME}.i)
swig_link_libraries(daq ${PROJECT_NAME} ${PYTHON_LIBRARIES})

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

## Declare install target for python
#install(TARGETS daq
#        COMMAND "${PYTHON_EXECUTABLE} setup.py"
#        COMPONENT python)

# Install target to call setup.py
add_custom_target(install-python
        DEPENDS _daq
        COMMAND python ${SETUP_PY_OUT} install)
