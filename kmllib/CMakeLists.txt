set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ZLIB REQUIRED)
find_package(Boost REQUIRED)
find_library(MiniZip NAMES minizip REQUIRED)
find_library(UriParser NAMES uriparser REQUIRED)
find_library(Expat NAMES expat REQUIRED)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libkml/cmake;${CMAKE_MODULE_PATH}")
add_subdirectory(third_party)

find_package(Qt6 REQUIRED COMPONENTS gui quick positioning REQUIRED)

qt_standard_project_setup()


set(HEADERS
    qtkml.h
    rendervisitor.h
    graphicsvisitor.h
    stylevisitor.h
    mercatorprojection.h
    kmldocument.h
    coord.h
    kmlelement.h
    kmlgraphics.h
    kmlrenderer.h
    qmlkml.h)

set(SOURCES
    rendervisitor.cpp
    graphicsvisitor.cpp
    stylevisitor.cpp
    mercatorprojection.cpp
    kmldocument.cpp
    kmlelement.cpp
    kmlgraphics.cpp
    qmlimage.cpp
    kmlrenderer.cpp
    kmlitem.cpp)

#add_library(libkml OBJECT IMPORTED)

#set_property(TARGET libkml PROPERTY IMPORTED_OBJECTS
#  # Quotes are required because of the ()
#  "/path/to/somewhere/objects$(EFFECTIVE_PLATFORM_NAME)/$(CURRENT_ARCH)/func.o"
#)

add_library(${PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
    )

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libkml/src
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    Qt::Core
    Qt::Gui
    Qt::Positioning
    Qt::Quick
    ZLIB::ZLIB
    ${Expat}
    ${UriParser}
    ${MiniZip}
    $<TARGET_OBJECTS:kmlbase>
    $<TARGET_OBJECTS:kmlconvenience>
    $<TARGET_OBJECTS:kmldom>
    $<TARGET_OBJECTS:kmlengine>
    $<TARGET_OBJECTS:kmlregionator>
    $<TARGET_OBJECTS:kmlxsd>
    )

