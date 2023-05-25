
set(NAME KmlImageTest)

find_package(Qt6
    COMPONENTS Quick Positioning Xml Network OpenGL Qml Widgets QuickWidgets
    REQUIRED)


qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

qt_add_executable(${NAME}
	mainwindow.cpp
        main.cpp
        linepath.cpp
        polymesh.cpp
	mainwindow.h
    	linepath.h
    	polymesh.h
	${LINEDRAW}
	mainwindow.ui
	kmlimage.qrc
	)

target_link_libraries(${NAME} PRIVATE
	Qt::Core
	Qt::Gui
	Qt::Qml
	Qt::Quick
        Qt::Widgets
        Qt6::OpenGL
        Qt6::QuickWidgets
	qtkml
        )

target_include_directories(${NAME} PRIVATE
	${LINEDRAW_PATH}
	${CMAKE_SOURCE_DIR}/kmllib)


set_target_properties(${NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
