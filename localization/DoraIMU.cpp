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
#include"seumath/Vector.hpp"
namespace dora_localization {
    using namespace seumath;
    using namespace std;
    DoraIMU::DoraIMU() {
        mPosture = Vector3f(0.0f,0.0f,0.0f);
        mAcc =  Vector3f(0.0f,0.0f,0.0f);
        mVelocity = Vector3f(0.0f,0.0f,0.0f);
        
    }

    DoraIMU::DoraIMU(const DoraIMU& orig) {
    }

    DoraIMU::~DoraIMU() {
    }
    
    void DoraIMU::update(){
       ::USART2_IRQHandler();
       ::UART2_CommandRoute();
       mPosture.x() = imu_calc_roll;
       mPosture.y() = imu_calc_pitch;
       mPosture.z() = imu_calc_yaw;
       
       cout<<imu_orig_ax<<'\t'
               <<imu_orig_ay<<'\t'
               <<imu_orig_az<<'\t'
               <<"posture\t"
               <<imu_calc_roll<<'\t'
               <<imu_calc_pitch<<'\t'
               <<imu_calc_yaw<<'\t'
               <<endl;
    }
}

