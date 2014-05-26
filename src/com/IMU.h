#ifndef __IMU_H
#define __IMU_H

typedef  short int16_t;

extern float 	imu_calc_yaw,  //偏航角
		imu_calc_pitch,//俯仰
		imu_calc_roll, //滚转
		imu_calc_alt,  //高度
		imu_calc_tempr,//温度
		imu_calc_press;//气压
//ADC值
extern int16_t imu_orig_ax, imu_orig_ay, imu_orig_az;//加速度计
extern int16_t imu_orig_gx, imu_orig_gy, imu_orig_gz;//陀螺仪
extern int16_t imu_orig_hx, imu_orig_hy, imu_orig_hz;//磁力计

extern bool imu_is_update_imu;
extern bool imu_is_update_motion;

extern void USART2_IRQHandler(void); //接收中断子程序
extern void UART2_CommandRoute(void);

#endif
