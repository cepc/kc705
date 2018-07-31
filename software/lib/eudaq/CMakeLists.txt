option(JADE_BUILD_EUDAQ_PRODUCER "build JadeProducer against EUDAQ2?" ON)
if(NOT JADE_BUILD_EUDAQ_PRODUCER)
  return()
endif()

include_directories(../core/include)
include_directories(include)
aux_source_directory(src MODULE_SRC)

set(EUDAQ_INSTALL_DIR ${PROJECT_SOURCE_DIR}/software/extern/eudaq_install CACHE
  PATH "eudaq install directory")

# set(EUDAQ_INSTALL_DIR ../../extern/eudaq_install)
include_directories(${EUDAQ_INSTALL_DIR}/include)
include_directories(${EUDAQ_INSTALL_DIR}/extern/include)

find_library(EUDAQ_CORE_LIBRARY eudaq_core
  HINTS ${EUDAQ_INSTALL_DIR}/lib)
if(EUDAQ_CORE_LIBRARY)
  list(APPEND EUDAQ_LIBRARIES ${EUDAQ_CORE_LIBRARY})
else()
  message("unable to find eudaq core")
  return()
endif()

set(EUDAQ_MODULE eudaq_module_jade)
add_library(${EUDAQ_MODULE} SHARED ${MODULE_SRC})
target_link_libraries(${EUDAQ_MODULE} ${EUDAQ_CORE_LIBRARY} ${JADE_CORE_LIBRARY})

install(TARGETS
  ${EUDAQ_MODULE}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)


