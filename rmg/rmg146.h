#ifndef __RMG146_H__
#define __RMG146_H__

#define DELTA_TIME	  	   0.01//10ms

bool InitDM163(void);
/**
 * 计算姿态，由于有滤波，传入参数为上周期的数值
 * @param azimuth_k
 * @param pitch_k
 * @param roll_k
 */
void Compass(double *azimuth_k, double *pitch_k, double *roll_k);

#endif
