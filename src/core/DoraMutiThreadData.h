/* 
 * File:   DoraMutiThreadData.h
 * Author: allen
 *
 * Created on 2014年5月26日, 下午11:31
 */

#ifndef DORAMUTITHREADDATA_H
#define	DORAMUTITHREADDATA_H
#include <pthread.h>
#include"Singleton.hpp"
#include"seumath/Math.hpp"
#include"DoraCommand.h"
#include"DoraWorldModel.h"
#include <vector>
namespace dora_core {
    using namespace std;
    using namespace seumath;
  
    class DoraMutiThreadData : public Singleton<DoraMutiThreadData> {
    public:  
        DoraMutiThreadData();
        DoraMutiThreadData(const DoraMutiThreadData& orig);
        virtual ~DoraMutiThreadData();

        void setPostion(Vector3f pstPostion);
        void getPostion(Vector3f* pstPtrPostion);
        
        void setPosture(Vector3f pstPosture);
        void getPosture(Vector3f* pstPtrPosture);

        void setRobotState(RobotState mRobotState);
        void getRobotState(RobotState* mRobotState);

        void assignBlocks(const vector<WMBlocks>& pstSrcBlocks,vector<WMBlocks>& pstTarBlocks);
        void setBlocks(vector<WMBlocks>& pstBlocks); 
        void getBlocks(vector<WMBlocks>& pstBlocks);
        
       
        void setCommand(DoraCommand& mCommand);
        void getCommand(DoraCommand& mCommand);

    private:

        pthread_mutex_t mPostureMutex;
        Vector3f mPosture;
        pthread_mutex_t mPositionMutex;
        Vector3f mPosition;
        pthread_mutex_t mRobotStateMutex;
        RobotState mRobotState;
        pthread_mutex_t mBlocksMutex;
        vector<WMBlocks> mBlocks;
        pthread_mutex_t mCommandMutex;
        DoraCommand mCommand;

    };
#define MUTI_DATA dora_core::DoraMutiThreadData::GetSingleton()
}//namespace dora_core 

#endif	/* DORAMUTITHREADDATA_H */