/* 
 * File:   ActionController.cpp
 * Author: allen
 * 
 * Created on 2012年7月8日, 上午9:52
 */

#include "ActionController.h"
#include"BioloidCtrl.h"
namespace BioCtrl {

    ActionController::ActionController() {
        while (!mAction.empty()) {
            mAction.pop();
        }
        //float mDuration;
        mCycle = 0;
    }
    ActionController::~ActionController() {
    }

    int ActionController::ctrlByDeg(const BioData::BioloidJointsData& p_Now, const BioData::BioloidJointsData& p_Target, int p_Cycle) {
        mNow = p_Now;
      //  cout<<"[ctrlByDeg] mCycle"<<mCycle<<endl;
        if (mTargetPose == p_Target) {
     //       cout<<"[ctrlByDeg] ==tar"<<mCycle<<endl;
            return mCycle;
        } else {
            mTargetPose = p_Target;
            mCycle = p_Cycle;
            GenerateActionDegQueue();
    //         cout<<"[ctrlByDeg] !=tar"<<mCycle<<endl;
            return mCycle;
        }
    }

    void ActionController::GenerateActionDegQueue() {
        BioloidJointsData deltaJionts;
        for (int i = BioloidJointsData::NECK; i < BioloidJointsData::DOF; i++) {
            deltaJionts.getJointsData()[i].mDeg = (mTargetPose.getJointsData()[i].mDeg - mNow.getJointsData()[i].mDeg) / mCycle;
        }
        while (!mAction.empty()) {
            mAction.pop();
        }
        for (int cycleNum = 1; cycleNum < mCycle + 1; cycleNum++) {
            mAction.push(
                    BioloidJointsData::plusJointsDeg(
                    mNow,
                    BioloidJointsData::multiplyJointsDegwithNum<int>(cycleNum, deltaJionts)
                    )
                    );
        }
    }

    int ActionController::performDeg() {
        BIO_ROBOT.getBioEffect().setBioJointsDeg(mAction.front());
        if (mAction.size() > 1) {
            mAction.pop();
        }
        mCycle--;
        return mCycle;
    }
//    BioEfect::BioloidJointsEffector& ActionController::getBioEffector(){
//        return mBioEffector;
//    }


}/*namesapce BioCtrl*/

