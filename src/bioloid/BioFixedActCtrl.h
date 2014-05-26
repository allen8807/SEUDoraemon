/* 
 * File:   BioFixedActCtrl.h
 * Author: allen
 *
 * Created on 2012年7月8日, 下午5:33
 */

#ifndef BIOFIXEDACTCTRL_H
#define	BIOFIXEDACTCTRL_H
#include<string>
#include"BioloidFixedActionData.h"
#include"math/Math.hpp"
#include"BioloidJointsData.h"
namespace BioCtrl {
    using namespace std;
    using namespace BioData;
    //Pose是原子操作，也就是说Pose必须被完成才能进行下一个Pose，但task不是原子操作，所以Pose后的Pose可以不是task所规定的

    class BioFixedActCtrl {
    public:
        BioFixedActCtrl();
        //  BioFixedActCtrl(const BioFixedActCtrl& orig);
        virtual ~BioFixedActCtrl();
        //Blind函数中的处理于其余不同，不能在非Blind的函数中调用，仅调试时使用
        void getUpFromLieBlindDeg();
        void getUpFromDiveBlindDeg();
        void walkBlindDeg();
        void turnLeftBlindDeg();
        void turnRightBlindDeg();
        /////////////////////////////////////////////////////
        void getUpFromLie();
        void getUpFromDive();
        void walkStraight();
        void turnLeft();
        void turnRight();
	void yuanditurnLeft();
     	void yuanditurnRight();
        void walk(math::AngDeg p_turnDeg);
    private:
        void performTaskBlindDeg(std::string p_FisrtTaskName);
        void performTaskDeg(std::string p_FisrtTaskName);
        /**
         *该函数用于处理那些不会调整Pose的序列如getup之类的
         * 
         */
        void unadjustPoseAction(std::string p_FisrtTaskName);
        /**
         * 查看当前的Pose是否执行完毕
         * @return
         */
        bool isNowPoseCompleted();
        bool isNowTaskChangeable(std::string p_NewTaskName);
        int mRemainCycle;
        std::string mNowTaskName;
        std::string mNextTaskName;
        std::string mPreTaskName;
        BioData::BioloidJointsData mTargrtPose; //现在的目标Pose
        BioData::BioloidJointsData mLastPose; //上一个完成的Pose
    };
}

#endif	/* BIOFIXEDACTCTRL_H */

