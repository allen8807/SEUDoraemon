/* 
 * File:   HingeJointEffector.cpp
 * Author: allen
 * 
 * Created on 2012年7月6日, 上午10:38
 */

#include "HingeJointEffector.h"
namespace BioEfect{
HingeJointEffector::HingeJointEffector()
:mMinDeg(-180.0f),
 mMaxDeg(180.0f),
 mMaxSpeed(360.0f),
mDeg(0.0f),
mSpeed(0.0f)
{
}

HingeJointEffector::~HingeJointEffector() {
}

    void HingeJointEffector::setMinDeg(math::AngDeg p_minDeg )
    {
        mMinDeg = math::normalizeAngle(p_minDeg);
    }

    void HingeJointEffector::setMaxDeg(math::AngDeg p_maxDeg )
    {
        mMaxDeg = math::normalizeAngle(p_maxDeg);
    }

    void HingeJointEffector::setMaxSpeed(float p_maxSpeed)
    {
        mMaxSpeed = p_maxSpeed;
    }
    void HingeJointEffector::setDeg(math::AngDeg p_deg){
        mDeg = math::normalizeAngle(p_deg);
    }
    void HingeJointEffector::setSpeed(float p_speed){
        mSpeed = p_speed;
    }
}/* namespace BioEfect */
