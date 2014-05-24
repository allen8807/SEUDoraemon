#ifndef __AX12_C3_H__
#define __AX12_C3_H__

#define Broadcast_ID  0xfe

extern int InitCom3(void);
extern int InitCom4(void);
extern void SetSpeed(unsigned char ID,unsigned int v);
extern void SetPosition(unsigned char ID,float Degree);
extern void RegSetPosition(unsigned char ID,float Degree); //Write but not action
extern void Action(unsigned char ID);  //Action(Broadcast_ID) All Servo action
extern float ReadPosition(unsigned char ID);

#endif
