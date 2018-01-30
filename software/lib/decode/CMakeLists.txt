include_directories(include)
aux_source_directory(src ARP_SRC)

set(JADE_DECODE jadepix)

add_library(${JADE_DECODE} ${ARP_SRC})

install(TARGETS ${JADE_DECODE}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        PUBLIC_HEADER DESTINATION include)

add_subdirectory(swig)
