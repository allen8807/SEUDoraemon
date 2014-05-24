/* 
 * File:   BioloidJointsEffector.cpp
 * Author: allen
 * 
 * Created on 2012年7月6日, 下午12:25
 */
#include <iostream>
#include "BioloidJointsEffector.h"

namespace BioEfect {

    BioloidJointsEffector::BioloidJointsEffector() {
        mBioJoints[BioloidJointsEffector::NECK].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::NECK].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::NECK].setMinDeg(-150.0f);

        mBioJoints[BioloidJointsEffector::RSHOULDER].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::RSHOULDER].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::RSHOULDER].setMinDeg(-150.0f);

        mBioJoints[BioloidJointsEffector::RUPPERARM].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::RUPPERARM].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::RUPPERARM].setMinDeg(-150.0f);

        mBioJoints[BioloidJointsEffector::RLOWERARM].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::RLOWERARM].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::RLOWERARM].setMinDeg(-150.0f);

        mBioJoints[BioloidJointsEffector::LSHOULDER].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::LSHOULDER].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::LSHOULDER].setMinDeg(-150.0f);

        mBioJoints[BioloidJointsEffector::LUPPERARM].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::LUPPERARM].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::LUPPERARM].setMinDeg(-150.0f);

        mBioJoints[BioloidJointsEffector::LLOWERARM].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::LLOWERARM].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::LLOWERARM].setMinDeg(-150.0f);

        mBioJoints[BioloidJointsEffector::RHIP1].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::RHIP1].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::RHIP1].setMinDeg(-150.0f);

        mBioJoints[BioloidJointsEffector::RHIP2].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::RHIP2].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::RHIP2].setMinDeg(-150.0f);

        mBioJoints[BioloidJointsEffector::RTHIGH].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::RTHIGH].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::RTHIGH].setMinDeg(-150.0f);

        mBioJoints[BioloidJointsEffector::RSHANK].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::RSHANK].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::RSHANK].setMinDeg(-150.0f);

        mBioJoints[BioloidJointsEffector::RANKLE].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::RANKLE].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::RANKLE].setMinDeg(-150.0f);

        mBioJoints[BioloidJointsEffector::RFOOT].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::RFOOT].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::RFOOT].setMinDeg(-150.0f);

        mBioJoints[BioloidJointsEffector::LHIP1].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::LHIP1].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::LHIP1].setMinDeg(-150.0f);

        mBioJoints[BioloidJointsEffector::LHIP2].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::LHIP2].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::LHIP2].setMinDeg(-150.0f);

        mBioJoints[BioloidJointsEffector::LTHIGH].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::LTHIGH].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::LTHIGH].setMinDeg(-150.0f);

        mBioJoints[BioloidJointsEffector::LSHANK].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::LSHANK].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::LSHANK].setMinDeg(-150.0f);

        mBioJoints[BioloidJointsEffector::LANKLE].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::LANKLE].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::LANKLE].setMinDeg(-150.0f);

        mBioJoints[BioloidJointsEffector::LFOOT].setMaxSpeed(300.0f);
        mBioJoints[BioloidJointsEffector::LFOOT].setMaxDeg(150.0f);
        mBioJoints[BioloidJointsEffector::LFOOT].setMinDeg(-150.0f);

        initFlag();
    }

    BioloidJointsEffector::~BioloidJointsEffector() {
    }

    void BioloidJointsEffector::setBioJointsDeg(BioData::BioloidJointsData& p_JsData) {
        setBioJointDeg(BioloidJointsEffector::NECK, p_JsData.getJointData(BioloidJointsData::NECK).mDeg);

        setBioJointDeg(BioloidJointsEffector::RSHOULDER, p_JsData.getJointData(BioloidJointsData::RSHOULDER).mDeg);
        setBioJointDeg(BioloidJointsEffector::RUPPERARM, p_JsData.getJointData(BioloidJointsData::RUPPERARM).mDeg);
        setBioJointDeg(BioloidJointsEffector::RLOWERARM, p_JsData.getJointData(BioloidJointsData::RLOWERARM).mDeg);

        setBioJointDeg(BioloidJointsEffector::LSHOULDER, p_JsData.getJointData(BioloidJointsData::LSHOULDER).mDeg);
        setBioJointDeg(BioloidJointsEffector::LUPPERARM, p_JsData.getJointData(BioloidJointsData::LUPPERARM).mDeg);
        setBioJointDeg(BioloidJointsEffector::LLOWERARM, p_JsData.getJointData(BioloidJointsData::LLOWERARM).mDeg);

        setBioJointDeg(BioloidJointsEffector::RHIP1, p_JsData.getJointData(BioloidJointsData::RHIP1).mDeg);
        setBioJointDeg(BioloidJointsEffector::RHIP2, p_JsData.getJointData(BioloidJointsData::RHIP2).mDeg);
        setBioJointDeg(BioloidJointsEffector::RTHIGH, p_JsData.getJointData(BioloidJointsData::RTHIGH).mDeg);
        setBioJointDeg(BioloidJointsEffector::RSHANK, p_JsData.getJointData(BioloidJointsData::RSHANK).mDeg);
        setBioJointDeg(BioloidJointsEffector::RANKLE, p_JsData.getJointData(BioloidJointsData::RANKLE).mDeg);
        setBioJointDeg(BioloidJointsEffector::RFOOT, p_JsData.getJointData(BioloidJointsData::RFOOT).mDeg);

        setBioJointDeg(BioloidJointsEffector::LHIP1, p_JsData.getJointData(BioloidJointsData::LHIP1).mDeg);
        setBioJointDeg(BioloidJointsEffector::LHIP2, p_JsData.getJointData(BioloidJointsData::LHIP2).mDeg);
        setBioJointDeg(BioloidJointsEffector::LTHIGH, p_JsData.getJointData(BioloidJointsData::LTHIGH).mDeg);
        setBioJointDeg(BioloidJointsEffector::LSHANK, p_JsData.getJointData(BioloidJointsData::LSHANK).mDeg);
        setBioJointDeg(BioloidJointsEffector::LANKLE, p_JsData.getJointData(BioloidJointsData::LANKLE).mDeg);
        setBioJointDeg(BioloidJointsEffector::LFOOT, p_JsData.getJointData(BioloidJointsData::LFOOT).mDeg);

        mIsJointsDegOn = true;

    }

    void BioloidJointsEffector::setBioJointsSpeed(BioData::BioloidJointsData& p_JsData) {

        setBioJointSpeed(BioloidJointsEffector::NECK, p_JsData.getJointData(BioloidJointsData::NECK).mSpeed);

        setBioJointSpeed(BioloidJointsEffector::RSHOULDER, p_JsData.getJointData(BioloidJointsData::RSHOULDER).mSpeed);
        setBioJointSpeed(BioloidJointsEffector::RUPPERARM, p_JsData.getJointData(BioloidJointsData::RUPPERARM).mSpeed);
        setBioJointSpeed(BioloidJointsEffector::RLOWERARM, p_JsData.getJointData(BioloidJointsData::RLOWERARM).mSpeed);

        setBioJointSpeed(BioloidJointsEffector::LSHOULDER, p_JsData.getJointData(BioloidJointsData::LSHOULDER).mSpeed);
        setBioJointSpeed(BioloidJointsEffector::LUPPERARM, p_JsData.getJointData(BioloidJointsData::LUPPERARM).mSpeed);
        setBioJointSpeed(BioloidJointsEffector::LLOWERARM, p_JsData.getJointData(BioloidJointsData::LLOWERARM).mSpeed);

        setBioJointSpeed(BioloidJointsEffector::RHIP1, p_JsData.getJointData(BioloidJointsData::RHIP1).mSpeed);
        setBioJointSpeed(BioloidJointsEffector::RHIP2, p_JsData.getJointData(BioloidJointsData::RHIP2).mSpeed);
        setBioJointSpeed(BioloidJointsEffector::RTHIGH, p_JsData.getJointData(BioloidJointsData::RTHIGH).mSpeed);
        setBioJointSpeed(BioloidJointsEffector::RSHANK, p_JsData.getJointData(BioloidJointsData::RSHANK).mSpeed);
        setBioJointSpeed(BioloidJointsEffector::RANKLE, p_JsData.getJointData(BioloidJointsData::RANKLE).mSpeed);
        setBioJointSpeed(BioloidJointsEffector::RFOOT, p_JsData.getJointData(BioloidJointsData::RFOOT).mSpeed);

        setBioJointSpeed(BioloidJointsEffector::LHIP1, p_JsData.getJointData(BioloidJointsData::LHIP1).mSpeed);
        setBioJointSpeed(BioloidJointsEffector::LHIP2, p_JsData.getJointData(BioloidJointsData::LHIP2).mSpeed);
        setBioJointSpeed(BioloidJointsEffector::LTHIGH, p_JsData.getJointData(BioloidJointsData::LTHIGH).mSpeed);
        setBioJointSpeed(BioloidJointsEffector::LSHANK, p_JsData.getJointData(BioloidJointsData::LSHANK).mSpeed);
        setBioJointSpeed(BioloidJointsEffector::LANKLE, p_JsData.getJointData(BioloidJointsData::LANKLE).mSpeed);
        setBioJointSpeed(BioloidJointsEffector::LFOOT, p_JsData.getJointData(BioloidJointsData::LFOOT).mSpeed);
        mIsJointsSpeedOn = true;
    }

    void BioloidJointsEffector::setBioJointDeg(HJE_ID p_Id, math::AngDeg p_Deg) {
        if (p_Deg < mBioJoints[p_Id].getMinDeg()) {
            p_Deg = mBioJoints[p_Id].getMinDeg();
            std::cerr << "[Warnning]" << "Joint" << p_Id << " degree is too low and set it to " << p_Deg << std::endl;
        } else if (p_Deg > mBioJoints[p_Id].getMaxDeg()) {
            p_Deg = mBioJoints[p_Id].getMaxDeg();
            std::cerr << "[Warnning]" << "Joint" << p_Id << " degree is too high and set it to " << p_Deg << std::endl;
        }
        mBioJoints[p_Id].setDeg(p_Deg);
    }

    void BioloidJointsEffector::setBioJointSpeed(HJE_ID p_Id, float p_Speed) {
        if (p_Speed < 0.0f) {
            p_Speed = 0.0f;
            std::cerr << "[Warnning]" << "Joint" << p_Id << " speed is too low and set it to " << p_Speed << std::endl;
        } else if (p_Speed > mBioJoints[p_Id].getMaxSpeed()) {
            p_Speed = mBioJoints[p_Id].getMaxSpeed();
            std::cerr << "[Warnning]" << "Joint" << p_Id << " speed is too high and set it to " << p_Speed << std::endl;
        }
        mBioJoints[p_Id].setSpeed(p_Speed);

    }

    void BioloidJointsEffector::setJointsFlag() {
        if (mIsJointsDegOn && mIsJointsSpeedOn) {
            mJointsFlag = DEG_SPEED;
            return;
        } else if (mIsJointsDegOn) {
            mJointsFlag = DEG;
        } else {
            mJointsFlag = DEG;
            std::cerr << "[Warnning]" << "No Joints DegInfo,set flag to DEG\n" << std::endl;
        }
    }
}/* namespace BioEfect */
