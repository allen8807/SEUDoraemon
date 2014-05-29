/* 
 * File:   DoraIMU.cpp
 * Author: allen
 * 
 * Created on 2014年5月24日, 下午4:46
 */
#include<iostream>
#include<fstream>
#include "DoraIMU.h"
#include "com/IMU.h"
#include<ctime>
#include"seumath/Vector.hpp"
namespace dora_perception {
    using namespace seumath;
    using namespace std;
    using namespace dora_core;

    DoraIMU::DoraIMU() {
        mPosture = Vector3f(0.0f, 0.0f, 0.0f);
        mFirstBodyDir = 140.0f;
        mAcc = Vector3f(0.0f, 0.0f, 0.0f);
        mAccVelocity=Vector3f(0.0f, 0.0f, 0.0f);
        mVelocity = Vector3f(0.0f, 0.0f, 0.0f);
        mPostion = Vector3f(0.0f, 0.0f, 0.0f);
        mKX = 16302;
        mKY = 16480;
        mKZ = 16342;
        // mOffsetX = 401;
        // mOffsetY = -37;
        mOffsetX = 489;
        mOffsetY = -285;
        mOffsetZ = 3920;
        //重力加速度g
        mG = 9.79f;
        mNow = clock();
        mLastPosture = mNow;
        mLastAcc = mNow;
        mLastVelClock = mNow;
        
        mSpeedForward = 0.0618f;
        mIsWalking = false;
        mPostionBySpeed = Vector3f(0.0f, 0.0f, 0.0f);

        mRobotState = dora_core::RS_STOP;
    }

    DoraIMU::DoraIMU(const DoraIMU& orig) {
    }

    DoraIMU::~DoraIMU() {
    }

    void DoraIMU::updatePosture() {
        if (true == imu_is_update_imu) {
            mPosture.x() = imu_calc_roll;
            mPosture.y() = imu_calc_pitch;
            mPosture.z() = imu_calc_yaw;
//            cout << "posture\t"
//                    << imu_calc_roll << '\t'
//                    << imu_calc_pitch << '\t'
//                    << imu_calc_yaw << '\t'
//                    << "time" << (mNow - mLastPosture)*1000 / CLOCKS_PER_SEC << "ms"
//                    << endl;
            this->mLastPosture = mNow;
        } else {
            //   cout<<"not update imu!!!" <<endl;
        }
    }
    
    void DoraIMU::updateMotion(){
     //    mNow = clock();
          if (true == ::imu_is_update_motion) {
            mAcc.x() = 0.1f * mG * (imu_orig_ax + mOffsetX) / mKX + 0.9f * mAcc.x();
            mAcc.y() = 0.1f * mG * (imu_orig_ay + mOffsetY) / mKY + 0.9f * mAcc.y();
            mAcc.z() = 0.1f * mG * (imu_orig_az + mOffsetZ) / mKZ + 0.9f * mAcc.z();
            TransMatrix<float> tm;
            tm.rotationX(-mPosture.x());
            mAcc = tm.rotate(mAcc);
            tm.rotationY(-mPosture.y());
            mAcc = tm.rotate(mAcc);
            tm.rotationZ(-mPosture.z());
            mAcc = tm.rotate(mAcc);
            //   mAcc.z() = mAcc.z()+mG;
            double duration = ((double) (mNow - mLastAcc)) / CLOCKS_PER_SEC;
            Vector3f newVel = mAccVelocity + mAcc*duration;

                mPostion = mPostion + (newVel + mAccVelocity)*(duration / 2);

            mAccVelocity = newVel;
//            cout<<"cljorig"
//                    <<"\thx\t"<<imu_orig_hx
//                     <<"\thy\t"<<imu_orig_hy
//                     <<"\thz\t"<<imu_orig_hz
//                    <<endl;

//            cout << "dur\t" << duration <<
//                    "accorig\t" <<
//                    imu_orig_ax + mOffsetX << '\t'
//                    << imu_orig_ay + mOffsetY << '\t'
//                    << imu_orig_az + mOffsetZ << '\t'
//                    << "Acc\t"
//                    << mAcc.x() << '\t'
//                    << mAcc.y() << '\t'
//                    << mAcc.z() << '\t'
//                    << "Postion\t"
//                    << mPostion.x() << '\t'
//                    << mPostion.y() << '\t'
//                    << mPostion.z() << '\t'
                    //               <<"posture\t"
                    //               <<imu_calc_roll<<'\t'
                    //               <<imu_calc_pitch<<'\t'
                    //               <<imu_calc_yaw<<'\t'
//                    << "time" << (mNow - mLastAcc)*1000 / CLOCKS_PER_SEC << "ms"
//                    << endl;
            this->mLastAcc = mNow;
            //}
            //++j;
        } else {
            //      cout<<"not update motion!!!" <<endl;
        }
    }

    void DoraIMU::updateRobotState(){
        if(dora_core::RS_WALK_FORWARD != mRobotState ){
            mLastVelClock = mNow;
        }
        MUTI_DATA.getRobotState(&mRobotState);     
  //      cout<<"mRobotState"<<mRobotState<<endl;
    }
    void DoraIMU::updatePostionBySpeed(){
        if(dora_core::RS_WALK_FORWARD == mRobotState){
        this->mVelocity = Vector3f(this->mSpeedForward,0.0f,0.0f);
        TransMatrix<float> tm;
        tm.rotationZ(mPosture.z()-mFirstBodyDir);
       mVelocity= tm.rotate(mVelocity);
       mPostionBySpeed= mPostionBySpeed+mVelocity*((mNow-mLastVelClock)*1000.0f/ CLOCKS_PER_SEC/1000);
        mLastVelClock=mNow;
        }     
    }
    void DoraIMU::update() {
        mNow = clock();
        USART2_IRQHandler();
        UART2_CommandRoute();
        updatePosture();
        
     //   updateMotion();
        updateRobotState();
        updatePostionBySpeed();
        notifyObservers();
        writePosToFile();
    }
    
     void DoraIMU::notifyObservers(){
         MUTI_DATA.setPosture(mPosture);
         MUTI_DATA.setPostion(mPostion);
     }

    void DoraIMU::test() {
        Vector3f p1(0.0f, 0.0f, 1.414f);
        Vector3f p2(0.0f, 0.0f, 1.0f);
        TransMatrix<float> tm;
        tm.rotationY(-45.0f);
        p2 = tm.rotate(p1);
        cout << "p1\t" << p1 << "p2\t" << p2 << endl;
    }
    
    void DoraIMU::writePosToFile(){
        static clock_t lasttime = mNow;
        if((mNow - lasttime)/CLOCKS_PER_SEC >= 1){
            lasttime = mNow;
             cout<<"mRobotState!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<mRobotState<<endl;
//        ofstream   ofacc( "posacc.txt",iostream::app); 
//        ofacc<<mPostion.x()<<'\t'
//                <<mPostion.y()<<'\t'
//                <<mPostion.z()<<'\t'
//                <<"time\t"<<mNow<<'\t'
//                <<mPosture<<'\t'
//                <<endl;
//        ofacc.close();
//          ofstream   ofspeed( "posspeed.txt",iostream::app); 
//        ofspeed<<mPostionBySpeed.x()<<'\t'
//                <<mPostionBySpeed.y()<<'\t'
//                <<mPostionBySpeed.z()<<'\t'
//                 <<mPosture<<'\t'
//                <<"time\t"<<mNow<<'\t'
//                
//                <<endl;
//        ofspeed.close();
        }
        
        
    }
}//end namespace dora_perception

