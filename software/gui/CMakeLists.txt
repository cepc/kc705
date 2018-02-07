option(JADE_BUILD_GUI "JADE_GUI_BUILD" ON)
if(NOT JADE_BUILD_GUI)
    return()
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src GUI_SRC)

file(GLOB GUI_UIS form/mainwindow.ui)
qt5_wrap_ui(GUI_FORMS_UIC ${GUI_UIS})

file(GLOB GUI_MOCS include/mainwindow.h)
qt5_wrap_cpp(GUI_HEADERS_MOC ${GUI_MOCS})

file(GLOB GUI_RES qrc/*.qrc)
qt5_add_resources(GUI_RES_RCC ${GUI_RES})

set(GUI_APP app/main_gui.cpp)

set(JADE_GUI JadeGUI)

# Executable
add_executable(${JADE_GUI} ${GUI_APP} ${GUI_SRC} ${GUI_HEADERS_MOC} ${GUI_FORMS_UIC} ${GUI_RES_RCC})
qt5_use_modules(${JADE_GUI} Widgets)

install(TARGETS ${JADE_GUI}
    DESTINATION bin)
