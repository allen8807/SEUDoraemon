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

namespace dora_core {
    using namespace std;
    using namespace seumath;
    enum RobotState {
        RS_STOP = 0,
        RS_WALK_FORWARD = 1,
        RS_TURN_CIRCLE = 2
    };

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

    private:

        pthread_mutex_t mPostureMutex;
        Vector3f mPosture;
        pthread_mutex_t mPostionMutex;
        Vector3f mPostion;
        pthread_mutex_t mRobotStateMutex;
        RobotState mRobotState;

    };
#define MUTI_DATA dora_core::DoraMutiThreadData::GetSingleton()
}//namespace dora_core 

#endif	/* DORAMUTITHREADDATA_H */

