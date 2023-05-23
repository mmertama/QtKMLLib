
set(NAME KmlQuickTest)

find_package(Qt6
    COMPONENTS quick positioning xml network opengl qml webview
    REQUIRED)

qt_standard_project_setup()

add_executable(${NAME}
	main.cpp
	qml.qrc
	Teknopolis.kml
	)

target_link_libraries(${NAME} PRIVATE
	Qt::Core
	Qt::Gui
	Qt::Qml
	Qt::Quick
        Qt::WebView
	qtkml)

target_include_directories(${NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/kmllib)

set_target_properties(${NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
