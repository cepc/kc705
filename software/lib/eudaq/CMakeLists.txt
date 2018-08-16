option(JADE_BUILD_EUDAQ_PRODUCER "build JadeProducer against EUDAQ2?" ON)
if(NOT JADE_BUILD_EUDAQ_PRODUCER)
  return()
endif()

include_directories(../core/include)
include_directories(include)
aux_source_directory(src MODULE_SRC)

find_package(eudaq CONFIG REQUIRED NO_DEFAULT_PATH)
include_directories(${EUDAQ_INCLUDE_DIRS})

set(EUDAQ_MODULE eudaq_module_jade)
add_library(${EUDAQ_MODULE} SHARED ${MODULE_SRC})
target_link_libraries(${EUDAQ_MODULE} ${EUDAQ_LIBRARIES} ${JADE_CORE_LIBRARY})
set_property( TARGET ${EUDAQ_MODULE} APPEND PROPERTY INSTALL_RPATH ${EUDAQ_LIBRARIES_DIRS})

install(TARGETS
  ${EUDAQ_MODULE}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

