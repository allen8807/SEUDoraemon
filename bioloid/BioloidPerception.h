/* 
 * File:   BioloidPerception.h
 * Author: allen
 *
 * Created on 2012年7月8日, 上午10:06
 */

#ifndef BIOLOIDPERCEPTION_H
#define	BIOLOIDPERCEPTION_H
#include"math/Math.hpp"
#include"BioloidJointsData.h"

namespace BioPcept {
    using namespace BioData;
    using namespace math;

    class BioloidPerception {
    public:

        enum BalanceState {
            LIED_STATE = 0, //lied state
            //	LYING_STATE,				//lying state
            DIVED_STATE = 1, //dived state
            //	DIVING_STATE,				//diving state
            //LRROLLED_STATE,
            LEFTFALL_STATE = 2,
            RIGHTFALL_STATE = 3,
            BALANCE_STATE = 4,
            STATE_NUM = 5
        };
        BioloidPerception();
        //  BioloidPerception(const BioloidPerception& orig);
        virtual ~BioloidPerception();
        void setRawJoints(const BioData::BioloidJointsData & p_Joints);

        void setTimeOfCycle(float p_t) {
            mTimeOfCycle = p_t;
        }

        void setTorsoAcc(math::Vector3f p_acc) {
            mTorsoAcc = p_acc;
        }

        const math::Vector3f & getTorsoAcc() {
            return mTorsoAcc;
        }

        const float getTimeOfCycle() const {
            return mTimeOfCycle;
        }

        const BioData::BioloidJointsData& getPredictJoints() {
            return mPredictJoints;
        }

        const BioData::BioloidJointsData& getRawJoints() {
            return mRawJoints;
        }

        const BalanceState getNowBalanceState() const {
            return mNowBalanceState;
        }

        void update();

        void setTorsoPosture(math::Vector3f p_Posture) {
            mTorsoPosture = p_Posture;
        }

        math::Vector3f getTorsoPosture() const {
            return mTorsoPosture;
        }

        void setCameraOffset(float p_CameraOffset) {
            mCameraOffset = p_CameraOffset;
        }

        float getCameraOffset() const {
            return mCameraOffset;
        }

        void setCameraSlope(float p_CameraSlope) {
            mCameraSlope = p_CameraSlope;
        }

        float getCameraSlope() const {
            return mCameraSlope;
        }

    protected:
        void predictJoints();
        void upadteState();

        void upadteStateDuration(BalanceState p_state);
    private:

        BioData::BioloidJointsData mRawJoints;
        BioData::BioloidJointsData mPredictJoints;
        math::Vector3f mTorsoAcc;
        math::Vector3f mTorsoGyr;
        math::Vector3f mTorsoPosture;
        float mCameraSlope;
        float mCameraOffset;
        float mTimeOfCycle;
        BalanceState mNowBalanceState;
        int mStateDuration[STATE_NUM];
    };
}/*namespace BioPcept*/

#endif	/* BIOLOIDPERCEPTION_H */

