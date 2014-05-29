/* 
 * File:   DoraMutiThreadData.cpp
 * Author: allen
 * 
 * Created on 2014年5月26日, 下午11:31
 */

#include "DoraMutiThreadData.h"
#include "core/DoraCommand.h"
#include "DoraWorldModel.h"
#include<ctime>
namespace dora_core {
    DoraMutiThreadData::DoraMutiThreadData() {
        mPosition = Vector3f(0.0f, 0.0f, 0.0f);
        mPosture = Vector3f(0.0f, 0.0f, 0.0f);
        mRobotState = RS_STOP;
        //初始化互斥信号锁
        pthread_mutex_init(&mPostureMutex, NULL);
        pthread_mutex_init(&mPositionMutex, NULL);
        pthread_mutex_init(&mRobotStateMutex, NULL);
        pthread_mutex_init(&mBlocksMutex, NULL);
        pthread_mutex_init(&mCommandMutex, NULL);
        
    }

    void DoraMutiThreadData::setCommand(DoraCommand& pstCommand) {
         pthread_mutex_lock(&mCommandMutex);
         mCommand.setDoraCmmand(pstCommand);
          pthread_mutex_unlock(&mCommandMutex);
    }

    void DoraMutiThreadData::getCommand(DoraCommand& pstCommand){
        pthread_mutex_lock(&mCommandMutex);
        pstCommand.setDoraCmmand(mCommand);
          pthread_mutex_unlock(&mCommandMutex);
    }

    DoraMutiThreadData::DoraMutiThreadData(const DoraMutiThreadData& orig) {
    }

    DoraMutiThreadData::~DoraMutiThreadData() {
    }

    void DoraMutiThreadData::setPostion(Vector3f pstPosition) {
        pthread_mutex_lock(&mPositionMutex);
        mPosition = pstPosition;
        pthread_mutex_unlock(&mPositionMutex);
    }

    void DoraMutiThreadData::getPostion(Vector3f* pstPtrPostion) {
        pthread_mutex_lock(&mPositionMutex);
        (*pstPtrPostion) = mPosition;
        pthread_mutex_unlock(&mPositionMutex);
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

    void DoraMutiThreadData::setRobotState(RobotState pstRobotState) {
        pthread_mutex_lock(&mRobotStateMutex);
        mRobotState = pstRobotState;
        pthread_mutex_unlock(&mRobotStateMutex);
    }

    void DoraMutiThreadData::getRobotState(RobotState* pstPtrRobotState) {
        pthread_mutex_lock(&mRobotStateMutex);
        (*pstPtrRobotState) = mRobotState;
        pthread_mutex_unlock(&mRobotStateMutex);
    }

    void DoraMutiThreadData::setBlocks(vector<WMBlocks>& pstBlocks) {
        pthread_mutex_lock(&mBlocksMutex);
        assignBlocks(pstBlocks, mBlocks);
        pthread_mutex_unlock(&mBlocksMutex);

    }

    void DoraMutiThreadData::getBlocks(vector<WMBlocks>& pstBlocks) {
        pthread_mutex_lock(&mBlocksMutex);
        assignBlocks(mBlocks, pstBlocks);
        pthread_mutex_unlock(&mBlocksMutex);
    }

    void DoraMutiThreadData::assignBlocks(const vector<WMBlocks>& pstSrcBlocks, vector<WMBlocks>& pstTarBlocks) {
        pstTarBlocks.clear();
        int size = pstSrcBlocks.size();
        int i = 0;
        for (i = 0; i < size; ++i) {
            pstTarBlocks.push_back(pstSrcBlocks[i]);
        }
    }
}//namespace dora_core 

