/* 
 * File:   DoraCommand.h
 * Author: allen
 *
 * Created on 2014年5月26日, 下午9:09
 */

#ifndef DORACOMMAND_H
#define	DORACOMMAND_H

#include<string>
#include<vector>

namespace dora_core {
     using namespace std;
    enum RobotState {
        RS_STOP = 0,
        RS_WALK_FORWARD = 1,
        RS_TURN_CIRCLE = 2,
        RS_WALK=3
    };
   


 
    typedef struct WalkCommand {
        char x; //0-100
        char y; //0-50
        char dir; //0-10
        char h; //50
        char num; //
    } WalkCommand;

    enum CmdType{
        ERR_CMD = 0x00,
        WALK_CMD = 0x10,
        SERVO_CTRL_CMD = 0x11
    };
    
    class DoraCommand {
    public:
        DoraCommand();
        DoraCommand(const DoraCommand& orig);
        virtual ~DoraCommand();
        void setDoraCmmand(const DoraCommand& orig);
        void setWalkCmd(WalkCommand& pstWalkCmd);
        void WalkForward(char pstStepNum);
        void WalkOnSamePlace(char pstStepNum);
        void TurnRightInCircle(char pstStepNum);
        void TurnLeftInCircle(char pstStepNum);
      

        
        vector<char> getCmdData() const {
            return mCmdData;
        }

        CmdType getCmdID() const {
            return mCmdID;
        }

        RobotState getRobotState() const {
            return mRobotState;
        }
        
    private:
        void update();
        void constructCmdData();
        CmdType mCmdID;
        WalkCommand mWalkCmd;
        vector<char> mCmdData;
        RobotState mRobotState;
    };
}

#endif	/* DORACOMMAND_H */

