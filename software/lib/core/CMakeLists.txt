include_directories(include)
aux_source_directory(src ARP_SRC)

set(JADE_CORE daq)

add_library(${JADE_CORE} ${ARP_SRC})

install(TARGETS ${JADE_CORE}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        PUBLIC_HEADER DESTINATION include)

add_subdirectory(swig)
