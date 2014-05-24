/* 
 * File:   BioloidJointsEffector.h
 * Author: allen
 *
 * Created on 2012年7月6日, 下午12:25
 */

#ifndef BIOLOIDJOINTSEFFECTOR_H
#define	BIOLOIDJOINTSEFFECTOR_H
#include "HingeJointEffector.h"
#include "BioloidJointsData.h"
namespace BioEfect {
    using namespace BioData;

    class BioloidJointsEffector {
    public:
        //HingeJointEffectID

        enum HJE_ID {
            NECK = 0,
            RSHOULDER = 1,
            RUPPERARM = 2,
            RLOWERARM = 3,
            LSHOULDER = 4,
            LUPPERARM = 5,
            LLOWERARM = 6,
            RHIP1 = 7,
            RHIP2 = 8,
            RTHIGH = 9,
            RSHANK = 10,
            RANKLE = 11,
            RFOOT = 12,
            LHIP1 = 13,
            LHIP2 = 14,
            LTHIGH = 15,
            LSHANK = 16,
            LANKLE = 17,
            LFOOT = 18,
            DOF = 19
        };

        enum EFFECTOR_FLAG {
            DEG = 0,
            DEG_SPEED = 1
            //    DEG_SPEED = 0x3
        };
        BioloidJointsEffector();
        ~BioloidJointsEffector();
        void initFlag(){
             mIsJointsSpeedOn = false;
            mIsJointsDegOn = false;
        }
        void setBioJointsDeg(BioData::BioloidJointsData& p_JsData);
        void setBioJointsSpeed(BioData::BioloidJointsData& p_JsData);

        EFFECTOR_FLAG& getJointsFlag() {

            return mJointsFlag;
        }

        BioEfect::HingeJointEffector * getBioJoints() {
            return mBioJoints;
        }

        //   update();
    private:

        void setBioJointDeg(HJE_ID p_Id, math::AngDeg p_Deg);
        void setBioJointSpeed(HJE_ID p_Id, float p_Speed);
        void setJointsFlag();
        HingeJointEffector mBioJoints[DOF];
        bool mIsJointsSpeedOn;
        bool mIsJointsDegOn;
        EFFECTOR_FLAG mJointsFlag;
    };


}/* namespace BioEfect*/

#endif	/* BIOLOIDJOINTSEFFECTOR_H */

