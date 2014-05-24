/* 
 * File:   DoraIMU.h
 * Author: allen
 *
 * Created on 2014年5月24日, 下午4:46
 */

#ifndef DORAIMU_H
#define	DORAIMU_H
#include"seumath/Math.hpp"
namespace dora_localization {
    class DoraIMU {
    public:
        DoraIMU();
        DoraIMU(const DoraIMU& orig);
       void update();
        virtual ~DoraIMU();
    private:
        seumath::Vector3f mPosture;//x=roll,y=pitch,z=yaw
   //     seumath::Vector3f mGyro;
        seumath::Vector3f mVelocity;
        seumath::Vector3f mAcc;
    };
}//namespace dora_localization 

#endif	/* DORAIMU_H */

