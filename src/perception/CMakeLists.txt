INCLUDE_DIRECTORIES(./)
INCLUDE_DIRECTORIES(${DORA_SOURCE_DIR})
#OpenCV
INCLUDE_DIRECTORIES(/usr/local/include/opencv)

SET(SRCS 
DoraBasicPerception.h
DoraPerceptions.h
DoraIMU.h
DoraIMU.cpp
DoraCamera.h
DoraCamera.cpp
) 
ADD_LIBRARY( dora_perception STATIC ${SRCS}  )
TARGET_LINK_LIBRARIES(dora_perception
com
dora_core
cxcore
highgui
)
