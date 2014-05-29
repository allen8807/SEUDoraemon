/* 
 * File:   DoraCommand.cpp
 * Author: allen
 * 
 * Created on 2014年5月26日, 下午9:09
 */

#include "DoraCommand.h"
#include<cstring>
#include<sstream>
#include<iostream>

namespace dora_core {
using namespace std;
    DoraCommand::DoraCommand() {
        mCmdID = ERR_CMD;
        memset(&mWalkCmd, 0, sizeof (mWalkCmd));
        mCmdData.clear();
    }

    DoraCommand::DoraCommand(const DoraCommand& orig) {
    }

    DoraCommand::~DoraCommand() {
    }
    
     void DoraCommand::setDoraCmmand(const DoraCommand& pstCmd){
           mCmdID=pstCmd.mCmdID;
        memcpy(&mWalkCmd,&(pstCmd.mWalkCmd),sizeof(WalkCommand));
        mRobotState = pstCmd.mRobotState;
        int size = pstCmd.mCmdData.size();
        mCmdData.clear();
        for(int i =0;i<size;++i){
         mCmdData.push_back(pstCmd.mCmdData[i]);
        }
     }

    void DoraCommand::setWalkCmd(WalkCommand& pstWalkCmd) {
        mCmdID = WALK_CMD;
        memcpy(&mWalkCmd, &pstWalkCmd, sizeof (WalkCommand));
        mRobotState = dora_core::RS_WALK;
        
    }

    void DoraCommand::WalkForward(char pstStepNum) {
        mCmdID = WALK_CMD;
        mWalkCmd.x = 20;
        mWalkCmd.y = 0;
        mWalkCmd.dir = 0;
        mWalkCmd.h = 40;
        mWalkCmd.num = pstStepNum;
        mRobotState = dora_core::RS_WALK_FORWARD;
        constructCmdData();
        
    }
    
     void DoraCommand::WalkOnSamePlace(char pstStepNum) {
        mCmdID = WALK_CMD;
        mWalkCmd.x = 0;
        mWalkCmd.y = 0;
        mWalkCmd.dir = 0;
        mWalkCmd.h = 40;
        mWalkCmd.num = pstStepNum;
         mRobotState = dora_core::RS_TURN_CIRCLE;
        constructCmdData();
    }

    void DoraCommand::TurnRightInCircle(char pstStepNum) {
        mCmdID = WALK_CMD;
        mWalkCmd.x = 0;
        mWalkCmd.y = 0;
        mWalkCmd.dir = -10;
        mWalkCmd.h = 40;
        mWalkCmd.num = pstStepNum;
        mRobotState = dora_core::RS_TURN_CIRCLE;
        constructCmdData();
    }
    
     void DoraCommand::TurnLeftInCircle(char pstStepNum) {
        mCmdID = WALK_CMD;
        mWalkCmd.x = 0;
        mWalkCmd.y = 0;
        mWalkCmd.dir = 10;
        mWalkCmd.h = 40;
        mWalkCmd.num = pstStepNum;
        mRobotState = dora_core::RS_TURN_CIRCLE;
        constructCmdData();
        
    }
    void DoraCommand::update(){
        constructCmdData();
    }
    
    void DoraCommand::constructCmdData(){
        char length = 0;
        char sum = 0;
        char tmp =0;
        stringstream ss;
        ss.clear();
        if(WALK_CMD==mCmdID ){
            mCmdData.push_back(0xFF);
           // ++length;
            mCmdData.push_back(0xFF);
           // ++length;
            mCmdData.push_back(length); 
           // ++length;
            mCmdData.push_back((char)mCmdID);
            ++length;
            sum+=(char)mCmdID;
            mCmdData.push_back(mWalkCmd.x);
            ++length;
             sum+=mWalkCmd.x;
            mCmdData.push_back(mWalkCmd.y);
            ++length;
            sum+=mWalkCmd.y;
            mCmdData.push_back(mWalkCmd.dir);
            ++length;
            sum+=mWalkCmd.dir;
            mCmdData.push_back(mWalkCmd.h);
            ++length;
            sum+=mWalkCmd.h;
            mCmdData.push_back(mWalkCmd.num);
            ++length;
            sum+=mWalkCmd.num;
            ++length;//校验位
            mCmdData[2]=length;
            sum+=length;
            mCmdData.push_back(sum);    
           
        }
    //     cout<<"length" << length<<"mCmdDataSize"<<mCmdData.size()<<endl;
    }
}//namespace dora_action
