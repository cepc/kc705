include_directories(include)
add_definitions("-DJADE_DLL_EXPORT")

set(JADE_LEGACY_CORE_SRC src/DataTakingThread.cpp src/daq.cpp)
set(JADE_LEGACY_CORE jadelegacy)
add_library(${JADE_LEGACY_CORE} ${JADE_LEGACY_CORE_SRC})

aux_source_directory(src JADE_CORE_SRC)
list(REMOVE_ITEM JADE_CORE_SRC ${JADE_LEGACY_CORE_SRC})
set(JADE_CORE_LIBRARY jadecore CACHE INTERNAL "Jade core library")
add_library(${JADE_CORE_LIBRARY} SHARED ${JADE_CORE_SRC})
target_link_libraries(${JADE_CORE_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

add_subdirectory(pybind11)
add_subdirectory(swig)

install(TARGETS
  ${JADE_CORE_LIBRARY}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
