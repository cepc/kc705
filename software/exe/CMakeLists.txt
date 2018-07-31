include_directories(../lib/core/include)

add_executable(OptionTest src/OptionTest.cxx)
target_link_libraries(OptionTest ${JADE_CORE_LIBRARY})
list(APPEND INSTALL_TARGETS OptionTest)

add_executable(SystemTest src/SystemTest.cxx)
target_link_libraries(SystemTest ${JADE_CORE_LIBRARY})
list(APPEND INSTALL_TARGETS SystemTest)

add_executable(ReaderTest src/ReaderTest.cxx)
target_link_libraries(ReaderTest ${JADE_CORE_LIBRARY})
list(APPEND INSTALL_TARGETS ReaderTest)

install(TARGETS ${INSTALL_TARGETS}
  DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

