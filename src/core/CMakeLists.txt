INCLUDE_DIRECTORIES(./)
INCLUDE_DIRECTORIES(${DORA_SOURCE_DIR})
INCLUDE_DIRECTORIES(${DORA_SOURCE_DIR}/libs)

SET(SRCS 
DoraMutiThreadData.h
DoraMutiThreadData.cpp
DoraPathPlanning.h
DoraPathPlanning.cpp
DoraWorldModel.h
DoraWorldModel.cpp
DoraemonCtrl.h
DoraemonCtrl.cpp
DoraCommand.h
DoraCommand.cpp
) 
ADD_LIBRARY( dora_core STATIC ${SRCS}  )
TARGET_LINK_LIBRARIES(dora_core
com
dora_perception
dora_action
pthread
)
