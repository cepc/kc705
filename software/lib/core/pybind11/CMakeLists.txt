option(JADE_BUILD_CORE_PYBIND "build python binding library with pybind11?" ON)
if(NOT JADE_BUILD_CORE_PYBIND)
  return()
endif()

find_package(PythonInterp 3 EXACT REQUIRED)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

include_directories(../include include)

aux_source_directory(src JADE_CORE_BIND_SRC)

set(PYBIND_NAME_VER pybind11-2.2.1)
set(PYBIND_ZIP_FILE ${PYBIND_NAME_VER}.zip)

set(PYBIND_HEADER_FILE ${CMAKE_CURRENT_BINARY_DIR}/${PYBIND_NAME_VER}/include/pybind11/pybind11.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/${PYBIND_NAME_VER}/include)

add_custom_command(
  OUTPUT ${PYBIND_HEADER_FILE}
  COMMAND ${CMAKE_COMMAND} -E tar xf ${CMAKE_CURRENT_LIST_DIR}/${PYBIND_ZIP_FILE} 
  COMMAND ${CMAKE_COMMAND} -E touch_nocreate ${PYBIND_HEADER_FILE}
  DEPENDS ${PYBIND_ZIP_FILE}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Unpacking ${PYBIND_ZIP_FILE}"
  VERBATIM
  )

set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_MODULE_PREFIX  "")
add_library(pyjade SHARED ${JADE_CORE_BIND_SRC} ${PYBIND_HEADER_FILE})
target_link_libraries(pyjade ${JADE_CORE_LIBRARY} ${PYTHON_LIBRARIES})

install(TARGETS pyjade
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
