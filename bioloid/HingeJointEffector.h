/* 
 * File:   HingeJointEffector.h
 * Author: allen
 *
 * Created on 2012年7月6日, 上午10:38
 */

#ifndef HINGEJOINTEFFECTOR_H
#define	HINGEJOINTEFFECTOR_H
#include"math/Angle.hpp"
namespace BioEfect{
using namespace math;
class HingeJointEffector {
public:
    HingeJointEffector();
    ~HingeJointEffector();
    const math::AngDeg getMinDeg() const
    {
        return mMinDeg;
    }

    const math::AngDeg getMaxDeg() const
    {
        return mMaxDeg;
    }

    const float getMaxSpeed() const
    {
        return mMaxSpeed;
    }

    const math::AngDeg getDeg() const
    {
        return mDeg;
    }

    const float getSpeed() const
    {
        return mSpeed;
    }

    void setMinDeg(math::AngDeg p_minDeg );
    void setMaxDeg(math::AngDeg p_maxDeg );
    void setMaxSpeed(float p_maxSpeed);
    void setDeg(math::AngDeg p_deg);
    void setSpeed(float p_speed);

private:
    //unit of speed is deg/s
    math::AngDeg mMinDeg;
    math::AngDeg mMaxDeg;
    float mMaxSpeed;

    math::AngDeg mDeg;
    float mSpeed;
};
}/* namespace BioEfect */
#endif	/* HINGEJOINTEFFECTOR_H */

