/* 
 * File:   DoraAction.h
 * Author: allen
 *
 * Created on 2014年5月26日, 下午9:39
 */

#ifndef DORAACTION_H
#define	DORAACTION_H
#include"DoraCommand.h"
namespace dora_action {
    
      enum ReturnCode{
        CHECK_ERROR = 0xFE,
        CMD_ID_ERROR = 0xFD,
        ACTION_ERROR = 0xF1,
        ACTION_FINISH= 0xF0
    };
    
class DoraAction {
public:
    DoraAction();
    DoraAction(const DoraAction& orig);
    virtual ~DoraAction();
    void run();
    void setAction(DoraCommand& pstCmd);
private:
    void sendAction();
    ReturnCode receiveReturnCode();
    unsigned char * mPtrCmdBuff;
    unsigned int mCmdSize;
    CmdType mCmdID;
    dora_core::RobotState mRobotState;

};
}//namespace dora_action 

#endif	/* DORAACTION_H */

