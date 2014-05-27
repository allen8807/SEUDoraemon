INCLUDE_DIRECTORIES(./)
INCLUDE_DIRECTORIES(${DORA_SOURCE_DIR})
#OpenCV
INCLUDE_DIRECTORIES(/usr/local/include/opencv)

SET(SRCS 
DoraIMU.h
DoraIMU.cpp
DoraCamera.h
DoraCamera.cpp
) 
ADD_LIBRARY( dora_perception STATIC ${SRCS}  )
TARGET_LINK_LIBRARIES(dora_perception
com
cxcore
highgui
)
