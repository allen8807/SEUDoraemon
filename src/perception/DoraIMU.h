/* 
 * File:   DoraIMU.h
 * Author: allen
 *
 * Created on 2014年5月24日, 下午4:46
 */

#ifndef DORAIMU_H
#define	DORAIMU_H
#include"seumath/Math.hpp"
#include<ctime>
namespace dora_perception {

    class DoraIMU {
    public:
        DoraIMU();
        DoraIMU(const DoraIMU& orig);
        void update();
        void test();
        virtual ~DoraIMU();
    private:
        void updatePosture();
        void updateMotion();
        void updatePostionBySpeed();
        void updateIsWalking();
        clock_t mLastPosture;
        clock_t mLastAcc;
        clock_t mLastVelClock;;
        clock_t mNow;
        seumath::Vector3f mPosture; //x=roll,y=pitch,z=yaw
        //     seumath::Vector3f mGyro;
        seumath::Vector3f mVelocity;
        seumath::Vector3f mAcc;
        seumath::Vector3f mPostion;
        float mFirstBodyDir;
        seumath::Vector3f mPostionBySpeed;
        //各轴标度因数和补偿值
        int mKX;
        int mKY;
        int mKZ;
        int mOffsetX;
        int mOffsetY;
        int mOffsetZ;
        //重力加速度g
        float mG;
        //向前走的平均速度
        float mSpeedForward;
        
        bool mIsWalking;

    };
}//namespace dora_perception 

#endif	/* DORAIMU_H */
