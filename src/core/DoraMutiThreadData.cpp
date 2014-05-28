/* 
 * File:   DoraMutiThreadData.cpp
 * Author: allen
 * 
 * Created on 2014年5月26日, 下午11:31
 */

#include "DoraMutiThreadData.h"
#include<ctime>
namespace dora_core {

    DoraMutiThreadData::DoraMutiThreadData() {
        mPostion = Vector3f(0.0f,0.0f,0.0f);
        mPosture = Vector3f(0.0f,0.0f,0.0f);
        mRobotState = RS_STOP;
        //初始化互斥信号锁
        pthread_mutex_init(&mPostureMutex, NULL);
     pthread_mutex_init(&mPostionMutex, NULL);
       pthread_mutex_init(&mRobotStateMutex, NULL);
     
    }

    DoraMutiThreadData::DoraMutiThreadData(const DoraMutiThreadData& orig) {
    }

    DoraMutiThreadData::~DoraMutiThreadData() {
    }

    void DoraMutiThreadData::setPostion(Vector3f pstPostion) {
        pthread_mutex_lock(&mPostionMutex);
        mPostion = pstPostion;
        pthread_mutex_unlock(&mPostionMutex);
    }

    void DoraMutiThreadData::getPostion(Vector3f* pstPtrPostion) {
        pthread_mutex_lock(&mPostionMutex);
        (*pstPtrPostion) = mPostion;
        pthread_mutex_unlock(&mPostionMutex);
    }

    void DoraMutiThreadData::setPosture(Vector3f pstPosture) {
        pthread_mutex_lock(&mPostureMutex);
        mPosture = pstPosture;
        pthread_mutex_unlock(&mPostureMutex);
    }

    void DoraMutiThreadData::getPosture(Vector3f* pstPtrPosture) {
        pthread_mutex_lock(&mPostureMutex);
        (*pstPtrPosture) = mPosture;
        pthread_mutex_unlock(&mPostureMutex);

    }
    
     void DoraMutiThreadData::setRobotState(RobotState pstRobotState){
          pthread_mutex_lock(&mRobotStateMutex);
            mRobotState = pstRobotState;
            pthread_mutex_unlock(&mRobotStateMutex);
        }

        void DoraMutiThreadData::getRobotState(RobotState* pstPtrRobotState){
             pthread_mutex_lock(&mRobotStateMutex);
            (*pstPtrRobotState) = mRobotState;
             pthread_mutex_unlock(&mRobotStateMutex);
        }

}//namespace dora_core 

