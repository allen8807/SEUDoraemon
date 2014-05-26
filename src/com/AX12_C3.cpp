// RB-100: Controll AX-12 by COM3
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "roboard.h"




#define ERROR_BYTE		4
#define NO_ERROR		0

// AX-12 commands
#define READ_DATA		0x02
#define WRITE_DATA		0x03
#define REG_WRITE_DATA		0x04
#define ACTION		0x05

// AX-12 registers
#define ID_ADDR			0x03
#define GOAL_POSITION	0x1e
#define MOVING_SPEED	0x20

unsigned char chk_sum(unsigned char *data, int len) {
    unsigned int i, sum = 0;
    for (i = 2; i < (len - 1); i++)
        sum += data[i];
    sum = (~sum) & 0x00ff;
    return (unsigned char) sum;
}

void err_msg(unsigned char err) {
    if ((err & 1) > 0)
        printf("Input Voltage Error.\n");
    if ((err & 2) > 0)
        printf("Angle Limit Error.\n");
    if ((err & 4) > 0)
        printf("Overheating Error.\n");
    if ((err & 8) > 0)
        printf("Range Error.\n");
    if ((err & 16) > 0)
        printf("Checksum Error.\n");
    if ((err & 32) > 0)
        printf("Overload Error.\n");
    if ((err & 64) > 0)
        printf("Instruction Error.\n");
}

void err_msg1(unsigned char ID, unsigned char err) {
    printf("errID%d: \n", ID);
    if ((err & 1) > 0)
        printf("Input Voltage Error.\n");
    if ((err & 2) > 0)
        printf("Angle Limit Error.\n");
    if ((err & 4) > 0)
        printf("Overheating Error.\n");
    if ((err & 8) > 0)
        printf("Range Error.\n");
    if ((err & 16) > 0)
        printf("Checksum Error.\n");
    if ((err & 32) > 0)
        printf("Overload Error.\n");
    if ((err & 64) > 0)
        printf("Instruction Error.\n");
}

int InitCom3(void) {
    roboio_SetRBVer(RB_110);
    if (!com3_Init(COM_HDUPLEX)) {
        printf("ERROR: %s\n", roboio_GetErrMsg());
        return -1;
    }

    // Set data format as 8 bits, 1 stop bit, no parity
    com3_SetFormat(COM_BYTESIZE8, COM_STOPBIT1, COM_NOPARITY);

    // Set COM3 baudrate to 115200bps (note: you should ensure that the baudrate of your AX-12 is also set as 115200bps)
    com3_SetBaud(COMBAUD_115200BPS);
    return 0;
}

int InitCom4(void) {
    roboio_SetRBVer(RB_110);
    if (!com4_Init(COM_HDUPLEX)) {
        printf("ERROR: %s\n", roboio_GetErrMsg());
        return -1;
    }

    // Set data format as 8 bits, 1 stop bit, no parity
    com4_SetFormat(COM_BYTESIZE8, COM_STOPBIT1, COM_NOPARITY);

    // Set COM3 baudrate to 115200bps (note: you should ensure that the baudrate of your AX-12 is also set as 115200bps)
    com4_SetBaud(COMBAUD_115200BPS);
    return 0;
}

void SetSpeed(unsigned char ID, unsigned int v) {
    unsigned char comd[128];
    unsigned char rx_status[128];
    comd[0] = 0xff;
    comd[1] = 0xff;
    comd[2] = ID; // ID
    comd[3] = 5; // LENGTH : LENGTH INSTRUCTION PARAMETER1 PARAMETER2 PARAMETER3
    comd[4] = WRITE_DATA; // INSTRUCTION
    comd[5] = MOVING_SPEED; // MOVING SPEED register
    comd[6] = v; // Low byte of MOVING SPEED
    comd[7] = v >> 8; // High byte of MOVING SPEED
    comd[8] = chk_sum(comd, 9); // Calculate the checksum.

    com3_ServoTRX(comd, 9, rx_status, 6);
    if (rx_status[ERROR_BYTE] != 0) err_msg(rx_status[ERROR_BYTE]);
}

void SetPosition(unsigned char ID, float Degree) {
    printf("ID %d, Degree %f", ID, Degree);
    unsigned char comd[128];
    unsigned char rx_status[128];
    unsigned int p;
    Degree = Degree + 150;
    p = (int) (Degree * 1023 / 300);

    comd[0] = 0xff;
    comd[1] = 0xff;
    comd[2] = ID; // ID
    comd[3] = 5; // LENGTH : LENGTH INSTRUCTION PARAMETER1 PARAMETER2 PARAMETER3
    comd[4] = WRITE_DATA; // INSTRUCTION
    comd[5] = GOAL_POSITION; // GOAL POSITION register
    comd[6] = p; // Low byte of GOAL POSITION
    comd[7] = p >> 8; // High byte of GOAL POSITION
    comd[8] = chk_sum(comd, 9); // Calculate the checksum.
    com3_ClearRFIFO();
    com3_Send(comd, 9);
    usleep(500);

    //   com3_ServoTRX(comd, 9, rx_status, 6);
    // if( rx_status[ERROR_BYTE]!=0 ) err_msg1( ID,rx_status[ERROR_BYTE] );
}

void RegSetPosition(unsigned char ID, float Degree) {
    unsigned char comd[128];
    unsigned char rx_status[128];
    unsigned int p;
    Degree = Degree + 150;
    p = (int) (Degree * 1023 / 300);

    comd[0] = 0xff;
    comd[1] = 0xff;
    comd[2] = ID; // ID
    comd[3] = 5; // LENGTH : LENGTH INSTRUCTION PARAMETER1 PARAMETER2 PARAMETER3
    comd[4] = REG_WRITE_DATA; // INSTRUCTION
    comd[5] = GOAL_POSITION; // GOAL POSITION register
    comd[6] = p; // Low byte of GOAL POSITION
    comd[7] = p >> 8; // High byte of GOAL POSITION
    comd[8] = chk_sum(comd, 9); // Calculate the checksum.

    com3_ClearRFIFO();
    com3_Send(comd, 9);
    usleep(500);

    //    com3_ServoTRX(comd, 9, rx_status, 6);
    //    if (rx_status[ERROR_BYTE] != 0) err_msg(rx_status[ERROR_BYTE]);
}

void Action(unsigned char ID) {
    unsigned char comd[128];
    unsigned char rx_status[128];
    comd[0] = 0xff;
    comd[1] = 0xff;
    comd[2] = ID; // ID
    comd[3] = 2; // LENGTH : LENGTH INSTRUCTION PARAMETER1 PARAMETER2 PARAMETER3
    comd[4] = ACTION; // INSTRUCTION
    comd[5] = chk_sum(comd, 6); // Calculate the checksum.

    com3_ClearRFIFO();
    com3_Send(comd, 9);
    usleep(500);

    //    com3_ServoTRX(comd, 6, rx_status, 6);
    //    if (rx_status[ERROR_BYTE] != 0) err_msg(rx_status[ERROR_BYTE]);
}

float ReadPosition(unsigned char ID) {
    unsigned char comd[128];
    unsigned char rx_status[128];
    unsigned int p;
    float Degree;
    comd[0] = 0xff;
    comd[1] = 0xff;
    comd[2] = ID; // ID
    comd[3] = 4; // LENGTH : LENGTH INSTRUCTION PARAMETER1 PARAMETER2
    comd[4] = READ_DATA; // INSTRUCTION
    comd[5] = GOAL_POSITION; // GOAL POSITION register
    comd[6] = 2; // Read 2 bytes from Address of GOAL POSITION register
    comd[7] = chk_sum(comd, 8); // Calculate the checksum

    com3_ServoTRX(comd, 8, rx_status, 8);
    if (rx_status[ERROR_BYTE] != 0)
        err_msg(rx_status[ERROR_BYTE]);
    else {
        p = (((unsigned int) rx_status[6]) << 8) + (unsigned int) rx_status[5];
        Degree = (float) (p * 300.0f / 1023);
        Degree = Degree - 150;
        printf("The read %d current position: %f\n", ID, Degree);
    }
    return Degree;
}
/*int main()
{


	

    // first set the correct RoBoard version
    //roboio_SetRBVer(RB_100);      // if your RoBoard is the newest RB-100 (model: DM-118D)
    //roboio_SetRBVer(RB_100b2);  // if your RoBoard is old RB-100 (model: DM-118A/DM-118B/DM-118C)
    //roboio_SetRBVer(RB_100RD);  // if your RoBoard is RB-100RD
    roboio_SetRBVer(RB_110);    // if your RoBoard is RB-110
    //roboio_SetRBVer(RB_050);    // if your RoBoard is RB-050

	
         InitCom3();

        // Set MOVING SPEED
        SetSpeed(0,200);
	
        

        // Set GOAL POSITION (moving AX-12)
        RegSetPosition(0,-150);
        usleep(1000000);
        // Read the position of AX-12
        ReadPosition(0);
        Action(Broadcast_ID);
        usleep(3000000);
        // Read the position of AX-12
        ReadPosition(0);
        

        com3_Close();
        return 0;
}*/
