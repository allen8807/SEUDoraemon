/* 
 * File:   BioloidJointsData.cpp
 * Author: allen
 * 
 * Created on 2012年7月6日, 下午1:47
 */

#include "BioloidJointsData.h"

namespace BioData {
    using namespace std;

    //  std::map<std::string,int>mBioNameToID;

    BioloidJointsData::BioloidJointsData() {
        for (int i = BioloidJointsData::NECK; i < BioloidJointsData::DOF; i++) {
            mJointsData[i].mDeg = 0.0f;
            mJointsData[i].mSpeed = 0.0f;
        }
    }

    //    BioloidJointsData::BioloidJointsData(const BioloidJointsData& orig) {
    //    }

    BioloidJointsData::~BioloidJointsData() {
    }

     BioloidJointsData BioloidJointsData::plusJointsDeg(const BioloidJointsData& p_op1, const BioloidJointsData& p_op2) {
        BioloidJointsData res;
        for (int i = BioloidJointsData::NECK; i < BioloidJointsData::DOF; i++) {
            res.mJointsData[i].mDeg = math::normalizeAngle(p_op1.mJointsData[i].mDeg + p_op2.mJointsData[i].mDeg);

        }
        return res;
    }

    BioloidJointsData& BioloidJointsData::operator=(const BioloidJointsData& p_BJD) {
        for (int i = BioloidJointsData::NECK; i < BioloidJointsData::DOF; i++) {
            mJointsData[i].mDeg = p_BJD.mJointsData[i].mDeg;
            mJointsData[i].mSpeed = p_BJD.mJointsData[i].mSpeed;
        }
        return *this;
    }

    bool BioloidJointsData::operator==(const BioloidJointsData& p_BJD)const {
        for (int i = BioloidJointsData::NECK; i < BioloidJointsData::DOF; i++) {
            if (abs(mJointsData[i].mDeg - p_BJD.mJointsData[i].mDeg) > 0.5f) {
                return false;
            }
        }
        return true;
    }

}/* namespace BioData */

