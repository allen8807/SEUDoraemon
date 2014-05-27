INCLUDE_DIRECTORIES(./)
INCLUDE_DIRECTORIES(${DORA_SOURCE_DIR})
INCLUDE_DIRECTORIES(${DORA_SOURCE_DIR}/libs)

SET(SRCS 
DoraAction.cpp
DoraAction.h
DoraCommand.cpp
DoraCommand.h
) 
ADD_LIBRARY( dora_action STATIC ${SRCS}  )
TARGET_LINK_LIBRARIES(dora_action
dora_perception
com
cxcore
highgui
)
