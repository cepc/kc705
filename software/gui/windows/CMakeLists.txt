include_directories(../../lib/core/include)

find_package(Qt5 COMPONENTS Widgets PrintSupport REQUIRED)

include_directories(include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

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

install(TARGETS ${JADE_GUI}
    DESTINATION bin)
