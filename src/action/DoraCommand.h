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
namespace dora_action {
    using namespace std;

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
        void setWalkCmd(WalkCommand& pstWalkCmd);
        void WalkForward(char pstStepNum);
        void TurnInCircle(char pstStepNum);
        void update();
    private:
        void constructCmdData();
        CmdType mCmdID;
        WalkCommand mWalkCmd;
        vector<char> mCmdData;
    };
}

#endif	/* DORACOMMAND_H */

