#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<unistd.h>
#define USE_COMMON
#include "roboard.h"
#include "rmg146.h"
#define TEMPERATURE_FACTOR 0.025634765
#define GYRO_RATE_FACTOR   0.9765625 
#define G_FACTOR           0.000076			      // G sensor scale factor (g/LSB)
#define DEGREE_TO_RADIAN   57.2957795147199       // radian to degree
#define RADIAN_TO_DEGREE   0.01745329251944       // degree to radian
#define PI                 3.14159265		      

#define	GYRO_BIAS          0.65*RADIAN_TO_DEGREE	      

static int SIZE_INT = (sizeof (int) *8 - 1);
//static int EULER_ANGLE[3];
static int G_AXIS_VALUE[3], GY_AXIS_VALUE[3], M_AXIS_VALUE[3], TEMPERATURE;

static double AZIMUTH[2], PITCH[2], ROLL[2], DIRECT[3] = {0.0, 1.0, 0.0};
//static int OBSERVER_NUMBER[3];
static double BIAS[3];
static double PRIORI[3][2][2];
double PSD_1[2] = {0.001, 0.001}, PSD_2[2] = {0.001, 0.001};

const unsigned char acceler_addr = 0x30 >> 1;
const unsigned char magnetic_addr = 0x3C >> 1;
const unsigned char gyro_addr = 0xD0 >> 1;

bool InitDM163(void) {
    roboio_SetRBVer(RB_110); // use RB-100
    if (i2c_Initialize(I2CIRQ_DISABLE) == false) {
        printf("FALSE!!  %s\n", roboio_GetErrMsg());
        return false;
    }

    i2c0_SetSpeed(I2CMODE_AUTO, 200000L); // set i2c speed:400k
    // set acceler
    if (i2c0master_StartN(acceler_addr, I2C_WRITE, 2) == false) {
        printf("g-sensor error:%s !!\n", roboio_GetErrMsg());
        return false;
    }
    i2c0master_WriteN(0x20);
    i2c0master_WriteN(0x37);

    if (i2c0master_StartN(magnetic_addr, I2C_WRITE, 4) == false) {
        printf("mangnetic sensor error:%s !!\n", roboio_GetErrMsg());
        return false;
    }
    i2c0master_WriteN(0x00); //set CRA_REG_M
    i2c0master_WriteN(0x18); //DO2 = 1, DO1 = 1, DO0 = 0
    i2c0master_WriteN(0x01); //set CRB_REG_M
    i2c0master_WriteN(0x60); //GN0 = 1, GN1 = 1, GN2 = 0(-2.5g ~ +2.5g)
    if (i2c0master_StartN(gyro_addr, I2C_WRITE, 2) == false) {
        printf("gyro sensor error:%s !!\n", roboio_GetErrMsg());
        return false;
    }
    i2c0master_WriteN(0x16); //set DLPF, full scale
    i2c0master_WriteN(0x18); // bit3 = bit4 = 1
    printf("RM-G146 sensor module start...\n\n");
    return true;

}

/*  read x-axis,y-axis,z-axis values of Compass */
bool ReadMXYZ(void) {
    int d;
    // read M_AXIS_VALUE[0]
    if (i2c0master_StartN(magnetic_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x03) == false)
        return false;
    if (i2c0master_StartN(magnetic_addr, I2C_READ, 1) == false)
        return false;
    if ((M_AXIS_VALUE[0] = i2c0master_ReadN()) == 0xffff)
        return false;

    M_AXIS_VALUE[0] = M_AXIS_VALUE[0] << 8;

    if (i2c0master_StartN(magnetic_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x04) == false)
        return false;
    if (i2c0master_StartN(magnetic_addr, I2C_READ, 1) == false)
        return false;
    if ((d = i2c0master_ReadN()) == 0xffff)
        return false;

    M_AXIS_VALUE[0] = M_AXIS_VALUE[0] + d;
    // read M_AXIS_VALUE[1]
    if (i2c0master_StartN(magnetic_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x05) == false)
        return false;
    if (i2c0master_StartN(magnetic_addr, I2C_READ, 1) == false)
        return false;
    if ((M_AXIS_VALUE[1] = i2c0master_ReadN()) == 0xffff)
        return false;

    M_AXIS_VALUE[1] = M_AXIS_VALUE[1] << 8;

    if (i2c0master_StartN(magnetic_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x06) == false)
        return false;
    if (i2c0master_StartN(magnetic_addr, I2C_READ, 1) == false)
        return false;
    if ((d = i2c0master_ReadN()) == 0xffff)
        return false;

    M_AXIS_VALUE[1] = M_AXIS_VALUE[1] + d;

    // read M_AXIS_VALUE[2]
    if (i2c0master_StartN(magnetic_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x07) == false)
        return false;
    if (i2c0master_StartN(magnetic_addr, I2C_READ, 1) == false)
        return false;
    if ((M_AXIS_VALUE[2] = i2c0master_ReadN()) == 0xffff)
        return false;

    M_AXIS_VALUE[2] = M_AXIS_VALUE[2] << 8;

    if (i2c0master_StartN(magnetic_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x08) == false)
        return false;
    if (i2c0master_StartN(magnetic_addr, I2C_READ, 1) == false)
        return false;
    if ((d = i2c0master_ReadN()) == 0xffff)
        return false;

    M_AXIS_VALUE[2] = M_AXIS_VALUE[2] + d;

    if (M_AXIS_VALUE[0] == 0 && M_AXIS_VALUE[0] == 0 && M_AXIS_VALUE[2] == 0) {
        err_SetMsg(ERROR_I2CWRONGUSAGE, "Compass gets zero value of three axas!");
        return false;
    }
    return true;
}

/*  read x-axis,y-axis,z-axis values of G-sensor */
bool ReadGXYZ(void) {
    int d;
    // read G_AXIS_VALUE[0]
    if (i2c0master_StartN(acceler_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x29) == false)
        return false;
    if (i2c0master_StartN(acceler_addr, I2C_READ, 1) == false)
        return false;
    if ((G_AXIS_VALUE[0] = i2c0master_ReadN()) == 0xffff)
        return false;

    G_AXIS_VALUE[0] = (G_AXIS_VALUE[0] + 2) &(~256);

    G_AXIS_VALUE[0] = G_AXIS_VALUE[0] << 8;

    if (i2c0master_StartN(acceler_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x28) == false)
        return false;
    if (i2c0master_StartN(acceler_addr, I2C_READ, 1) == false)
        return false;
    if ((d = i2c0master_ReadN()) == 0xffff)
        return false;

    G_AXIS_VALUE[0] = G_AXIS_VALUE[0] + d;
    // read G_AXIS_VALUE[1]
    if (i2c0master_StartN(acceler_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x2b) == false)
        return false;
    if (i2c0master_StartN(acceler_addr, I2C_READ, 1) == false)
        return false;
    if ((G_AXIS_VALUE[1] = i2c0master_ReadN()) == 0xffff)
        return false;
    G_AXIS_VALUE[1] = (G_AXIS_VALUE[1] + 7) & (~256);

    G_AXIS_VALUE[1] = G_AXIS_VALUE[1] << 8;

    if (i2c0master_StartN(acceler_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x2a) == false)
        return false;
    if (i2c0master_StartN(acceler_addr, I2C_READ, 1) == false)
        return false;
    if ((d = i2c0master_ReadN()) == 0xffff)
        return false;

    G_AXIS_VALUE[1] = G_AXIS_VALUE[1] + d;

    // read G_AXIS_VALUE[2]
    if (i2c0master_StartN(acceler_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x2d) == false)
        return false;
    if (i2c0master_StartN(acceler_addr, I2C_READ, 1) == false)
        return false;
    if ((G_AXIS_VALUE[2] = i2c0master_ReadN()) == 0xffff)
        return false;

    G_AXIS_VALUE[2] = G_AXIS_VALUE[2] << 8;

    if (i2c0master_StartN(acceler_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x2c) == false)
        return false;
    if (i2c0master_StartN(acceler_addr, I2C_READ, 1) == false)
        return false;
    if ((d = i2c0master_ReadN()) == 0xffff)
        return false;

    G_AXIS_VALUE[2] = G_AXIS_VALUE[2] + d;

    if (G_AXIS_VALUE[0] == 0 && G_AXIS_VALUE[1] == 0 && G_AXIS_VALUE[2] == 0) {
        err_SetMsg(ERROR_I2CWRONGUSAGE, "G-sensor gets zero value of three axas!");
        return false;
    }
    return true;
}

/*  read x-axis,y-axis,z-axis values of Gyro-sensor */

bool ReadGyXYZ(void) {
    int d;
    // read TEMPERATURE
    if (i2c0master_StartN(gyro_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x1b) == false)
        return false;
    if (i2c0master_StartN(gyro_addr, I2C_READ, 1) == false)
        return false;
    if ((TEMPERATURE = i2c0master_ReadN()) == 0xffff)
        return false;
    TEMPERATURE = TEMPERATURE << 8;

    if (i2c0master_StartN(gyro_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x1c) == false)
        return false;
    if (i2c0master_StartN(gyro_addr, I2C_READ, 1) == false)
        return false;
    if ((d = i2c0master_ReadN()) == 0xffff)
        return false;
    TEMPERATURE = TEMPERATURE + d;
    if ((TEMPERATURE & 0x8000) != 0)
        TEMPERATURE = (~(TEMPERATURE & (~0x8000))) | (0x80000000);
    d = TEMPERATURE + 13200;
    TEMPERATURE = d / 280 + 35;

    // read GY_AXIS_VALUE[0]
    if (i2c0master_StartN(gyro_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x1d) == false)
        return false;
    if (i2c0master_StartN(gyro_addr, I2C_READ, 1) == false)
        return false;
    if ((GY_AXIS_VALUE[0] = i2c0master_ReadN()) == 0xffff)
        return false;

    if (i2c0master_StartN(gyro_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x1e) == false)
        return false;
    if (i2c0master_StartN(gyro_addr, I2C_READ, 1) == false)
        return false;
    if ((d = i2c0master_ReadN()) == 0xffff)
        return false;
    GY_AXIS_VALUE[0] = GY_AXIS_VALUE[0] + d;
    // read GY_AXIS_VALUE[1]
    if (i2c0master_StartN(gyro_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x1f) == false)
        return false;
    if (i2c0master_StartN(gyro_addr, I2C_READ, 1) == false)
        return false;
    if ((GY_AXIS_VALUE[1] = i2c0master_ReadN()) == 0xffff)
        return false;

    if (i2c0master_StartN(gyro_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x20) == false)
        return false;
    if (i2c0master_StartN(gyro_addr, I2C_READ, 1) == false)
        return false;
    if ((d = i2c0master_ReadN()) == 0xffff)
        return false;
    GY_AXIS_VALUE[1] = GY_AXIS_VALUE[1] + d;
    // read GY_AXIS_VALUE[2]
    if (i2c0master_StartN(gyro_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x21) == false)
        return false;
    if (i2c0master_StartN(gyro_addr, I2C_READ, 1) == false)
        return false;
    if ((GY_AXIS_VALUE[2] = i2c0master_ReadN()) == 0xffff)
        return false;

    if (i2c0master_StartN(gyro_addr, I2C_WRITE, 1) == false)
        return false;
    if (i2c0master_WriteN(0x22) == false)
        return false;
    if (i2c0master_StartN(gyro_addr, I2C_READ, 1) == false)
        return false;
    if ((d = i2c0master_ReadN()) == 0xffff)
        return false;
    GY_AXIS_VALUE[2] = GY_AXIS_VALUE[2] + d;

    return true;
}
/*void gotoxy(int x,int y)  
{
    CONSOLE_SCREEN_BUFFER_INFO    csbiInfo;                            
    HANDLE    hConsoleOut;
    hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);
    csbiInfo.dwCursorPosition.X = x;                                    
    csbiInfo.dwCursorPosition.Y = y;                                    
    SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition);   
} */

/*void clrscr(void) 
{
    CONSOLE_SCREEN_BUFFER_INFO    csbiInfo;                            
    HANDLE    hConsoleOut;
    COORD    Home = {0,0};
    DWORD    dummy;
    hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);
    FillConsoleOutputCharacter(hConsoleOut,' ',csbiInfo.dwSize.X * csbiInfo.dwSize.Y,Home,&dummy); 
    csbiInfo.dwCursorPosition.X = 0;                                    
    csbiInfo.dwCursorPosition.Y = 0;                                    
    SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition);   
}*/
/*  
 *	computes the azimuth angle , the angle between -180 and 180 degrees
 *	the north is    0  degree
 *	the east  is   90  degrees
 *	the south is  180  degrees
 *	the weat  is  -90  degrees
 */

/*  computes the pitch angle, the angle between -180 and 180 degrees  */
double GetPitch(double *gval) {
    double p;
    double angle;
    p = gval[1] * gval[1] + gval[2] * gval[2];
    p = sqrt(p);
    if (gval[2] > 0)
        angle = atan2(gval[0], p);
    else
        angle = atan2(gval[0], -p);


    return angle;
}

/*  compute the roll angle, the angle between -180 and 180 degrees  */
double GetRoll(double *gval) {
    if (gval[2] == 0) {
        if (gval[1] == 0)
            return 0;
        return gval[1] > 0 ? PI / 2 : -PI / 2;
    }
    double angle = atan2(gval[1], gval[2]);
    return angle;
}

/*  
 *	computes the azimuth angle , the angle between -180 and 180 degrees
 *	the north is    0  degree
 *	the east  is   90  degrees
 *	the south is  180  degrees
 *	the weat  is  -90  degrees
 */
double GetAzimuth(double *mval, double pitch, double roll) {
    double x, y;
    double cp, sp, cr, sr;
    cp = cos(pitch);
    sp = sin(pitch);
    cr = cos(roll);
    sr = sin(roll);
    x = mval[0] * cp - mval[1] * sr * sp - mval[2] * cr*sp;
    y = mval[1] * cr - mval[2] * sr;

    if (roll > PI * 0.5 || roll < -PI * 0.5)
        return atan2(x, -y);
    else
        return atan2(-x, y);
}

double Abs(double v) {
    return v > 0 ? v : -v;
}

/*
double KalmanUpdata(double gyro, double angle_m, double bias_m, double *psd, double (*priori_k)[2], double bias_p, double angle_p)
{
	
        double gain_k[2], sq;
        double diff;
	
        //hat x_k
        angle_p += (gyro - bias_p)*DELTA_TIME;
	
        //hat p_k
        priori_k[0][0] = priori_k[0][0] + (priori_k[1][0] + priori_k[0][1])*DELTA_TIME + psd[0];
        priori_k[0][1] = priori_k[0][1] + priori_k[1][1]*DELTA_TIME;
        priori_k[1][0] = priori_k[1][0] + priori_k[1][1]*DELTA_TIME;
        priori_k[1][1] = psd[1];
	
        //k_k
        sq = priori_k[0][0] + bias_m;
        gain_k[0] = priori_k[0][0]/sq;
        gain_k[1] = priori_k[1][0]/sq;
	
        //x_k
        if(angle_p - angle_m > PI)
        {
                angle_m += 2*PI;
                diff = angle_m - angle_p;
        }
        else if(angle_m - angle_p > PI)
        {
                angle_p += 2*PI;
                diff = angle_m - angle_p;
        }
        else
        {
                diff = angle_m - angle_p;
        }
        angle_p += gain_k[0]*diff;
        bias_p += gain_k[1]*diff;
        //p_k
        priori_k[0][0] -= gain_k[0]* priori_k[0][0];
        priori_k[0][1] -= gain_k[0]* priori_k[0][1];
        priori_k[1][0] -= gain_k[1]* priori_k[0][0];
        priori_k[1][1] -= gain_k[1]* priori_k[0][1];
	
        if(angle_p > 2*PI)
                angle_p -= 2*PI;
        return  angle_p;
}
 */


void EnsembleKalman(double gyro, double *angle_m, double bias_m, double *psd, double (*priori_k)[2], double bias_p, double *angle_p) {

    double gain_k[2], sq;
    double diff[2];

    //hat x_k
    angle_p[0] += (gyro - bias_p) * DELTA_TIME;

    //hat p_k
    priori_k[0][0] = priori_k[0][0] + (priori_k[1][0] + priori_k[0][1]) * DELTA_TIME + psd[0];
    priori_k[0][1] = priori_k[0][1] + priori_k[1][1] * DELTA_TIME;
    priori_k[1][0] = priori_k[1][0] + priori_k[1][1] * DELTA_TIME;
    priori_k[1][1] = psd[1];

    //k_k
    sq = priori_k[0][0] + bias_m;
    gain_k[0] = priori_k[0][0] / sq;
    gain_k[1] = priori_k[1][0] / sq;


    //x_k
    if (angle_m[1] - angle_m[0] > PI) {
        angle_m[0] += 2 * PI;
        diff[0] = angle_m[0] - angle_m[1];
    } else if (angle_m[0] - angle_m[1] > PI) {
        angle_m[1] += 2 * PI;
        diff[0] = angle_m[0] - angle_m[1];
    } else {
        diff[0] = angle_m[0] - angle_m[1];
    }

    //
    if (angle_p[0] - angle_m[0] > PI) {
        angle_m[0] += 2 * PI;
        diff[1] = angle_m[0] - angle_p[0];
    } else if (angle_m[0] - angle_p[0] > PI) {
        angle_p[0] += 2 * PI;
        diff[1] = angle_m[0] - angle_p[0];
    } else {
        diff[1] = angle_m[0] - angle_p[0];
    }

    angle_p[0] += (1 - gain_k[0])*(angle_p[0] - angle_p[1]) + gain_k[0] * diff[0] + gain_k[0] * diff[1];
    bias_p += (1 - gain_k[1])*(angle_p[0] - angle_p[1]) + gain_k[1] * diff[1];

    //p_k
    priori_k[0][0] -= gain_k[0] * priori_k[0][0];
    priori_k[0][1] -= gain_k[0] * priori_k[0][1];
    priori_k[1][0] -= gain_k[1] * priori_k[0][0];
    priori_k[1][1] -= gain_k[1] * priori_k[0][1];

    if (angle_p[0] > PI)
        angle_p[0] -= 2 * PI;
    if (angle_m[0] > PI)
        angle_m[0] -= 2 * PI;
    if (angle_p[0] < -PI)
        angle_p[0] += 2 * PI;
    if (angle_m[0] < -PI)
        angle_m[0] += 2 * PI;
    angle_p[1] = angle_p[0];
    angle_m[1] = angle_m[0];

}

void EnsembleKalmanAzimuth(double gyro, double *angle_m, double bias_m, double *psd, double (*priori_k)[2], double bias_p, double *angle_p) {

    double gain_k[2], sq;
    double diff[2];

    //hat x_k
    angle_p[0] += (gyro - bias_p) * DELTA_TIME;

    //hat p_k
    priori_k[0][0] = priori_k[0][0] + (priori_k[1][0] + priori_k[0][1]) * DELTA_TIME + psd[0];
    priori_k[0][1] = priori_k[0][1] + priori_k[1][1] * DELTA_TIME;
    priori_k[1][0] = priori_k[1][0] + priori_k[1][1] * DELTA_TIME;
    priori_k[1][1] = psd[1];

    //k_k
    sq = priori_k[0][0] + bias_m;
    gain_k[0] = priori_k[0][0] / sq;
    gain_k[1] = priori_k[1][0] / sq;


    //x_k
    if (angle_m[1] - angle_m[0] > PI) {
        angle_m[0] += 2 * PI;
        diff[0] = angle_m[0] - angle_m[1];
    } else if (angle_m[0] - angle_m[1] > PI) {
        angle_m[1] += 2 * PI;
        diff[0] = angle_m[0] - angle_m[1];
    } else {
        diff[0] = angle_m[0] - angle_m[1];
    }

    //
    if (angle_p[0] - angle_m[0] > PI) {
        angle_m[0] += 2 * PI;
        diff[1] = angle_m[0] - angle_p[0];
    } else if (angle_m[0] - angle_p[0] > PI) {
        angle_p[0] += 2 * PI;
        diff[1] = angle_m[0] - angle_p[0];
    } else {
        diff[1] = angle_m[0] - angle_p[0];
    }

    angle_p[0] += (1 - gain_k[0])*(angle_p[0] - angle_p[1]) + gain_k[0] * diff[0] + gain_k[0] * diff[1];
    bias_p += (1 - gain_k[1])*(angle_p[0] - angle_p[1]) + gain_k[1] * diff[1];

    //p_k
    priori_k[0][0] -= gain_k[0] * priori_k[0][0];
    priori_k[0][1] -= gain_k[0] * priori_k[0][1];
    priori_k[1][0] -= gain_k[1] * priori_k[0][0];
    priori_k[1][1] -= gain_k[1] * priori_k[0][1];

    if (angle_p[0] > 2 * PI)
        angle_p[0] -= 2 * PI;
    if (angle_p[0] < -2 * PI)
        angle_p[0] += 2 * PI;
    if (angle_m[0] > 2 * PI)
        angle_m[0] -= 2 * PI;
    if (angle_m[0] < -2 * PI)
        angle_m[0] += 2 * PI;
    angle_p[1] = angle_p[0];
    angle_m[1] = angle_m[0];

}

void ReformInteger(int *v, int n) {
    for (int i = 0; i < n; i++) {
        if ((v[i] & 0x8000) != 0)
            v[i] = v[i] | 0xffff0000;
    }

}

//  get the heading angle

void Compass(double *azimuth_k, double *pitch_k, double *roll_k)
 {
    double gval[3], gyval[3], mval[3];
    static double pitch[2], roll[2], azimuth[2], pit;
    if (ReadGXYZ() == false) {
        printf("g-sensor error:%s !!\n", roboio_GetErrMsg());
//        i2c_Close();
//        exit(1);
    }
    if (ReadMXYZ() == false) {
        printf("mangnetic sensor error:%s !!\n", roboio_GetErrMsg());
//        i2c_Close();
//        exit(1);
    }
    if (ReadGyXYZ() == false) {
        printf("gyro sensor error:%s !!\n", roboio_GetErrMsg());
//        i2c_Close();
//        exit(1);
    }

    if (SIZE_INT > 16) {
        ReformInteger(G_AXIS_VALUE, 3);
        ReformInteger(M_AXIS_VALUE, 3);
        ReformInteger(GY_AXIS_VALUE, 3);
    }

    gval[0] = (double) (G_AXIS_VALUE[0] / 1000);
    gval[1] = (double) (G_AXIS_VALUE[1] / 1000);
    gval[2] = (double) (G_AXIS_VALUE[2] / 1000);
    mval[0] = (double) (M_AXIS_VALUE[0] / 10);
    mval[1] = (double) (M_AXIS_VALUE[1] / 10);
    mval[2] = (double) (M_AXIS_VALUE[2] / 10);
    gyval[0] = (double) (GY_AXIS_VALUE[0] / 1000);
    gyval[1] = (double) (GY_AXIS_VALUE[1] / 1000);
    gyval[2] = (double) (GY_AXIS_VALUE[2] / 1000);

    pitch[0] = GetPitch(gval);
    roll[0] = GetRoll(gval);

    if (pitch[0] > PI / 2 && pitch[0] < PI)
        pit = PI - pitch[0];
    else if (pitch[0] >= PI)
        pit = 0.0;
    else if (pitch[0] <= -PI / 2)
        pit = -PI - pitch[0];

    azimuth[0] = GetAzimuth(mval, pit, roll[0]);

    if (azimuth[0] < 0)
        azimuth[0] = azimuth[0] + 2 * PI;
    azimuth_k[0] = azimuth[0];
    // pitch_k[0]=pitch[0];
    // roll_k[0]=roll[0];

    EnsembleKalman(gyval[1] * RADIAN_TO_DEGREE, pitch, GYRO_BIAS, PSD_1, PRIORI[1], BIAS[1], pitch_k);
    EnsembleKalman(gyval[0] * RADIAN_TO_DEGREE, roll, GYRO_BIAS, PSD_1, PRIORI[0], BIAS[0], roll_k);
    /*EnsembleKalmanAzimuth(gyval[2]*RADIAN_TO_DEGREE,azimuth,GYRO_BIAS,PSD_2,PRIORI[2],BIAS[2],azimuth_k);*/


}

//int main(void) {
//    if (InitDM163() == false) {
//        printf("  Error:%s !!\n", roboio_GetErrMsg());
//        return -1;
//    }
//    do {
//        Compass(AZIMUTH, PITCH, ROLL);
//    } while (getchar() != 27);
//}



