/* 
 * File:   DoraAction.cpp
 * Author: allen
 * 
 * Created on 2014年5月26日, 下午9:39
 */

#include "DoraAction.h"
#include"core/DoraCommand.h"
#include"com/CommUnit.h"
#include"core/DoraMutiThreadData.h"
#include<iostream>
#include<stdio.h>
namespace dora_action {
    using namespace std;

    DoraAction::DoraAction() {
        mPtrCmdBuff = NULL;
        mCmdSize = 0;
        mCmdID = ERR_CMD;
        mRobotState = dora_core::RS_STOP;
    }

    DoraAction::DoraAction(const DoraAction& orig) {
    }

    DoraAction::~DoraAction() {
        if (NULL != mPtrCmdBuff) {
            delete mPtrCmdBuff;
            mPtrCmdBuff = NULL;
        }
    }

    void DoraAction::setAction(DoraCommand& pstCmd) {
        //cout<<"setACtion"<<endl;
        if (NULL != mPtrCmdBuff) {
            delete mPtrCmdBuff;
            mPtrCmdBuff = NULL;
            mCmdSize = 0;
        }
        vector<char> vData = pstCmd.getCmdData();
        
        mCmdSize = vData.size();
      //  cout << "mCmdSize" << mCmdSize << endl;
        mRobotState = pstCmd.getRobotState();
        mCmdID = pstCmd.getCmdID();
        mPtrCmdBuff = new unsigned char[mCmdSize];
        int i = 0;
        for (i = 0; i < mCmdSize; ++i) {
            mPtrCmdBuff[i] = (unsigned char) vData[i];
            printf("%x ", mPtrCmdBuff[i]);
        }
          cout<<endl;
    }

    void DoraAction::sendAction() {
        static int ii = 0;
        ++ii;
        usleep(50*1000);
        cout<<"send action "<<ii<<" "<<mCmdID<<endl;
        if (true != (CommUnit::sendCmd(mPtrCmdBuff, mCmdSize))) {
            cerr << "send err" << endl;
        }

        //  cout<<"send buff"<< mPtrCmdBuff<<endl;

    }

    ReturnCode DoraAction::receiveReturnCode() {
        unsigned char value;

        while (true) {
            value = CommUnit::recieveReturnValue();
            printf("value1 %x\n", value);
            if (mCmdID == value) {
                cout<<"setSTATE$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<endl;
                MUTI_DATA.setRobotState(mRobotState);
                break;
            } else {
                if (CHECK_ERROR == value) {
                    cerr << "check err" << endl;
                    return (ReturnCode) value;
                } else if (CMD_ID_ERROR == value) {
                    cerr << "command not right" << endl;
                    return (ReturnCode) value;
                }

            }
        }
        while (true) {
            value = CommUnit::recieveReturnValue();
             printf("value2 %x\n", value);
            if (ACTION_FINISH == value) {
                cout << "cmd succ" << endl;
                MUTI_DATA.setRobotState(dora_core::RS_STOP);
                return (ReturnCode) value;
            } else if (ACTION_ERROR == value) {
                cerr << "action can not finish " << endl;
                return (ReturnCode) value;
            }
           

        }

    }

    void DoraAction::run() {
        DoraCommand cmd;
        MUTI_DATA.getCommand(cmd);
        cout<< "[action] ID "<<cmd.getCmdID()<<endl;
        cout<< cmd.getCmdData().size()<<endl;
     //     cmd.WalkForward(30);
     //   cmd.WalkOnSamePlace(100);
      //  while (1) {
        
            setAction(cmd);
            if(cmd.getCmdID() !=ERR_CMD ){
            sendAction();
            receiveReturnCode();
            }
      //  }


    }
}//namespace dora_action 

