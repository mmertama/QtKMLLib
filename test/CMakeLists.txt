
set(LINEDRAW_PATH ${CMAKE_CURRENT_SOURCE_DIR}/linedraw)
set(LINEDRAW
	${LINEDRAW_PATH}/geoline.h
    	${LINEDRAW_PATH}/line_p.h
    	${LINEDRAW_PATH}/line.h
    	${LINEDRAW_PATH}/linecollection.h
    	${LINEDRAW_PATH}/lineitem.h
	
	${LINEDRAW_PATH}/geoline.cpp
    	${LINEDRAW_PATH}/line.cpp 
    	${LINEDRAW_PATH}/linecollection.cpp 
    	${LINEDRAW_PATH}/lineitem.cpp)

add_subdirectory(KmlImageTest)
#add_subdirectory(KMLTest)
