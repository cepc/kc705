include_directories(../lib/core/include)
option(JADE_BUILD_GUI "JADE_GUI_BUILD" ON)
if(NOT JADE_BUILD_GUI)
    return()
endif()

find_package(Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
find_package(ROOT REQUIRED)

include_directories(include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${ROOT_INCLUDE_DIRS})
set(USEROOT true)
if(USEROOT)
  execute_process(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE) 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${ROOT_LD_FLAGS}")
endif(USEROOT)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src GUI_SRC)

file(GLOB GUI_UIS form/mainwindow.ui)
qt5_wrap_ui(GUI_FORMS_UIC ${GUI_UIS})

file(GLOB GUI_MOCS include/mainwindow.h include/qcustomplot.h include/GUIManager.hh)
qt5_wrap_cpp(GUI_HEADERS_MOC ${GUI_MOCS})

file(GLOB GUI_RES qrc/*.qrc)
qt5_add_resources(GUI_RES_RCC ${GUI_RES})

set(GUI_APP app/main_gui.cxx)

set(JADE_GUI JadeGUI)

# Executable
add_executable(${JADE_GUI} ${GUI_APP} ${GUI_SRC} ${GUI_HEADERS_MOC} ${GUI_FORMS_UIC} ${GUI_RES_RCC})
qt5_use_modules(${JADE_GUI} Widgets PrintSupport)

target_link_libraries(${JADE_GUI} ${JADE_CORE_LIBRARY})
target_link_libraries(${JADE_GUI} ${ROOT_LIBRARIES})

install(TARGETS ${JADE_GUI}
    DESTINATION bin)
