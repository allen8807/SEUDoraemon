/* 
 * File:   BioFixedActCtrl.cpp
 * Author: allen
 * 
 * Created on 2012年7月8日, 下午5:33
 */

#include<map>
#include <utility>
#include"math/Math.hpp"
#include "BioFixedActCtrl.h"
#include"BioloidCtrl.h"
#include "BioloidFixedActionData.h"

namespace BioCtrl {
    using namespace std;

    BioFixedActCtrl::BioFixedActCtrl() {
        mPreTaskName = "null";
        mNowTaskName = "null";
        mNextTaskName = "null";
        mRemainCycle = 0;
    }

    //    BioFixedActCtrl::BioFixedActCtrl(const BioFixedActCtrl& orig) {
    //    }

    BioFixedActCtrl::~BioFixedActCtrl() {
    }

    void BioFixedActCtrl::getUpFromDiveBlindDeg() {
        if (!isNowPoseCompleted()) {
            mRemainCycle = BIO_ROBOT.getActCtrl().performDeg();
            return;
        } else {
            if (mNowTaskName == "null") {
                performTaskBlindDeg("qianqi_zbl_1");
            } else {
                performTaskBlindDeg(mNowTaskName);
            }
        }
    }

    void BioFixedActCtrl::getUpFromLieBlindDeg() {
        if (!isNowPoseCompleted()) {
            mRemainCycle = BIO_ROBOT.getActCtrl().performDeg();
            return;
        } else {
            if (mNowTaskName == "null") {
                performTaskBlindDeg("houqi_zbl_1");
            } else {
                performTaskBlindDeg(mNowTaskName);
            }
        }
    }

    void BioFixedActCtrl::walkBlindDeg() {
        cout<<"walk"<<endl;
        if (!isNowPoseCompleted()) {
             cout<<"walknot"<<endl;
            mRemainCycle = BIO_ROBOT.getActCtrl().performDeg();
            return;
        } else {
            if (mNowTaskName == "null") {
                cout<<"walk null"<<endl;
                performTaskBlindDeg("walk_zbl_4");
            } else {
                 cout<<"walk wrong null"<<endl;
                performTaskBlindDeg(mNowTaskName);
            }
        }
    }

        void BioFixedActCtrl::turnLeftBlindDeg() {
        if (!isNowPoseCompleted()) {
            mRemainCycle = BIO_ROBOT.getActCtrl().performDeg();
            return;
        } else {
            if (mNowTaskName == "null") {
                performTaskBlindDeg("turn_left_4");
            } else {
                performTaskBlindDeg(mNowTaskName);
            }
        }
    }
    void BioFixedActCtrl::turnRightBlindDeg() {
        if (!isNowPoseCompleted()) {
            mRemainCycle = BIO_ROBOT.getActCtrl().performDeg();
            return;
        } else {
            if (mNowTaskName == "null") {
                performTaskBlindDeg("turn_right_4");
            } else {
                performTaskBlindDeg(mNowTaskName);
            }
        }
    }

    void BioFixedActCtrl::performTaskBlindDeg(std::string p_TaskName) {
        cout<<"task name"<<p_TaskName << endl;
        BioData::BioloidFixedActionData::TTaskMap::const_iterator iterTask = BFAD.getTaskMap().find(p_TaskName);
        mNowTaskName = p_TaskName;
        mNextTaskName = iterTask->second.next;
        //  float duration = iterTask->second.time;
        std::string poseName = iterTask->second.pose;
        std::string nowPoseName;

        //  BioData::BioloidFixedActionData::TPoseMap::const_iterator iterPose = BFAD.getPoseMap().find(poseName);
        //如果前一个task不存在，就认为当前的pose是目标的pose（因为是角度控制，pose达到了也仍然可以发送
        //如果前一个task存在，就认为当前pose是上一个task的目标pose，因为只有完成了上一个task才会进入下一个task的pose
        if (mPreTaskName == "null") {
            nowPoseName = poseName;
        } else {
            nowPoseName = BFAD.getTaskMap().find(mPreTaskName)->second.pose;
        }
        mRemainCycle = std::ceil(iterTask->second.time / BIO_ROBOT.getTimeOfCycle());
        mRemainCycle = BIO_ROBOT.getActCtrl().ctrlByDeg(BFAD.getPoseMap().find(nowPoseName)->second,
                BFAD.getPoseMap().find(poseName)->second, mRemainCycle);
        mRemainCycle = BIO_ROBOT.getActCtrl().performDeg();
    }

    /**
     * 根据mRemainCycle判断Pose是否完成
     * @return
     */
    bool BioFixedActCtrl::isNowPoseCompleted() {
        if (mRemainCycle <= 0 && mNextTaskName == "null") {
            mPreTaskName = mNowTaskName;
            mNowTaskName = "null";
         //   mNextTaskName = "null";//blind mode use
            return true;
        } else if (mRemainCycle <= 0 && mNextTaskName != "null") {
            mPreTaskName = mNowTaskName;
            mNowTaskName = mNextTaskName;
            return true;
        } else {
            return false;
        }
    }

    /**
     * 通过优先级判断是否可以改变task
     * @param p_NewTaskName
     * @return 可以打断返回true否则返回false
     */
    bool BioFixedActCtrl::isNowTaskChangeable(std::string p_NewTaskName) {
        BioData::BioloidFixedActionData::TTaskMap::const_iterator iterNowTask = BFAD.getTaskMap().find(mNowTaskName);
        BioData::BioloidFixedActionData::TTaskMap::const_iterator iterNewTask = BFAD.getTaskMap().find(p_NewTaskName);
        //return iterTask
        if (iterNewTask->second.priority < iterNowTask->second.priority) {
            return true;
        } else {
            return false;
        }
        //  iterTask->second.changeable;
        //   return true;
    }

    void BioFixedActCtrl::performTaskDeg(std::string p_TaskName) {
        cout << "[perform] nowtask "<<p_TaskName<<endl;
        BioData::BioloidFixedActionData::TTaskMap::const_iterator iterTask = BFAD.getTaskMap().find(p_TaskName);
        mNowTaskName = p_TaskName;
        mNextTaskName = iterTask->second.next;
        mRemainCycle = std::ceil(iterTask->second.time / BIO_ROBOT.getTimeOfCycle());

        // 不考虑电机反馈
               std::string poseName = iterTask->second.pose;
               std::string nowPoseName;

                //  BioData::BioloidFixedActionData::TPoseMap::const_iterator iterPose = BFAD.getPoseMap().find(poseName);
                        if (mPreTaskName == "null") {
                            nowPoseName = poseName;
                        } else {
                            nowPoseName = BFAD.getTaskMap().find(mPreTaskName)->second.pose;
                        }

        
             mRemainCycle = BIO_ROBOT.getActCtrl().ctrlByDeg(BFAD.getPoseMap().find(nowPoseName)->second,
                        mTargrtPose, mRemainCycle);

        //        //仿真中没有电机速度
        //        mRemainCycle = BIO_ROBOT.getActCtrl().ctrlByDeg(BIO_ROBOT.getBioPcepetion().getRawJoints(),
        //                mTargrtPose, mRemainCycle);

        //仿真中有电机速度
//        mRemainCycle = BIO_ROBOT.getActCtrl().ctrlByDeg(BIO_ROBOT.getBioPcepetion().getPredictJoints(),
//                mTargrtPose, mRemainCycle);
        mRemainCycle = BIO_ROBOT.getActCtrl().performDeg();
        std::cout << "[perf]mRemainCycle" << mRemainCycle << endl;
    }

    void BioFixedActCtrl::getUpFromLie() {
        unadjustPoseAction("houqi_zbl_1");
    }

    void BioFixedActCtrl::getUpFromDive() {
        unadjustPoseAction("qianqi_zbl_1");
    }

    void BioFixedActCtrl::walkStraight() {
        unadjustPoseAction("walk_zbl_4");
    }

    void BioFixedActCtrl::unadjustPoseAction(std::string p_FisrtTaskName) {
        //先判断要执行的task是否可以打断当前的task，如果可以将下一个task换成要替换的task
        if (mNowTaskName != "null" && isNowTaskChangeable(p_FisrtTaskName)) {
            mNextTaskName = p_FisrtTaskName;
        }
        //判断当前的pose是否完成，未完成则继续完成当前的pose
        if (!isNowPoseCompleted()) {
            mRemainCycle = BIO_ROBOT.getActCtrl().performDeg();
            return;
        } else {
            //pose完成后根据当前的taskname（这个已经在判断pose完成的函数中修改）生成新的目标pose
            BioData::BioloidFixedActionData::TTaskMap::const_iterator iterTask;
            BioData::BioloidFixedActionData::TPoseMap::const_iterator iterPose;
            std::string nowPoseName;
            //如果当前的task是null
            if (mNowTaskName == "null") {
                iterTask = BFAD.getTaskMap().find(p_FisrtTaskName);
                nowPoseName = BFAD.getTaskMap().find(iterTask->second.pose)->second.pose;
                iterPose = BFAD.getPoseMap().find(nowPoseName);
                mTargrtPose = iterPose->second;
                performTaskDeg(p_FisrtTaskName);
            } else {
                iterTask = BFAD.getTaskMap().find(mNowTaskName);
                nowPoseName = BFAD.getTaskMap().find(iterTask->second.pose)->second.pose;
                iterPose = BFAD.getPoseMap().find(nowPoseName);
                mTargrtPose = iterPose->second;
                performTaskDeg(mNowTaskName);
            }
        }
    }

    void BioFixedActCtrl::turnLeft(){
         unadjustPoseAction("turn_left_4");
    }


     void BioFixedActCtrl::yuanditurnLeft(){
         unadjustPoseAction("yuandi_left_1");
    }

        void BioFixedActCtrl::turnRight(){
         unadjustPoseAction("turn_right_4");
    }
	 void BioFixedActCtrl::yuanditurnRight(){
         unadjustPoseAction("yuandi_right_1");
    }

    /**
     *   该函数应该对输入角限制，并且应该分成左转右转两个函数，调用的固定序列也不应该一样，因为不同的序列优先级不同
     * 未测试
     * @param p_turnDeg 负数向右转，正数向左转
     */
    void BioFixedActCtrl::walk(math::AngDeg p_turnDeg) {
        //先判断要执行的task是否可以打断当前的task，如果可以将下一个task换成要替换的task
        if (mNowTaskName != "null" && isNowTaskChangeable("walk_zbl_1")) {
            mNextTaskName = "walk_zbl_1";
        }
        //判断当前的pose是否完成，未完成则继续完成当前的pose
        if (!isNowPoseCompleted()) {
            mRemainCycle = BIO_ROBOT.getActCtrl().performDeg();
            return;
        } else {
            //pose完成后根据当前的taskname（这个已经在判断pose完成的函数中修改）生成新的目标pose
            BioData::BioloidFixedActionData::TTaskMap::const_iterator iterTask;
            BioData::BioloidFixedActionData::TPoseMap::const_iterator iterPose;
            std::string nowPoseName;
            //如果当前的task是null
            if (mNowTaskName == "null") {
                iterTask = BFAD.getTaskMap().find("walk_zbl_1");
                nowPoseName = BFAD.getTaskMap().find(iterTask->second.pose)->second.pose;
                iterPose = BFAD.getPoseMap().find(nowPoseName);
                mTargrtPose = iterPose->second;

                if (p_turnDeg < 0 && iterTask->second.adjustLeg == 1) {
                    mTargrtPose.getJointsData()[BioData::BioloidJointsData::RHIP1].mDeg = -p_turnDeg;
                }

                if (p_turnDeg > 0 && iterTask->second.adjustLeg == 2) {
                    mTargrtPose.getJointsData()[BioData::BioloidJointsData::LHIP1].mDeg = p_turnDeg;
                }

                performTaskDeg("walk_zbl_1");
            } else {
                iterTask = BFAD.getTaskMap().find(mNowTaskName);
                nowPoseName = BFAD.getTaskMap().find(iterTask->second.pose)->second.pose;
                iterPose = BFAD.getPoseMap().find(nowPoseName);
                mTargrtPose = iterPose->second;
                if (p_turnDeg < 0 && iterTask->second.adjustLeg == 1) {
                    mTargrtPose.getJointsData()[BioData::BioloidJointsData::RHIP1].mDeg = -p_turnDeg;
                }

                if (p_turnDeg > 0 && iterTask->second.adjustLeg == 2) {
                    mTargrtPose.getJointsData()[BioData::BioloidJointsData::LHIP1].mDeg = p_turnDeg;
                }
                performTaskDeg(mNowTaskName);
            }
        }
    }
}
