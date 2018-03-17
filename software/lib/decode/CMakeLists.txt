include_directories(include)
aux_source_directory(src JADE_LEGACY_DECODE_SRC)

set(JADE_LEGACY_DECODE jadepix)
add_library(${JADE_LEGACY_DECODE} ${JADE_LEGACY_DECODE_SRC})
add_subdirectory(swig)
