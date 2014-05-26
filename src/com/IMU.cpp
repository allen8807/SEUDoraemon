#ifndef __IMU_H
#define __IMU_H
/* 
编写者：lisn3188
网址：www.chiplab7.com
作者E-mail：lisn3188@163.com
编译环境：MDK-Lite  Version: 4.23
初版时间: 2012-05-25
测试： 本程序已在第七实验室的mini IMU上完成测试
功能：
使用串行中断程序接收mini IMU 的数据。
移植者需在自己编写对应的串口初始化程序，并开启接收中断。
在中断子程序中 需要做相应的改动

为了使用这个程序，你需要：
1.初始化你的串口，将波特率与模块的波特率设置成一致的
2.开启串口的接收中断，并参考以下程序编写中断子程序
  参考 void USART2_IRQHandler(void) 
3.在主程序的循环体中，定时调用子程序：
  void UART2_CommandRoute(void)
  以检查是否收到新的数据帧
  所有的姿态数据和ADC值都会在 UART2_CommandRoute
  中进行更新。
4.使用 以下值会自动更新：

float 	imu_calc_yaw,  //偏航角
		imu_calc_pitch,//俯仰
		imu_calc_roll, //滚转
		imu_calc_alt,  //高度
		imu_calc_tempr,//温度
		imu_calc_press;//气压
int16_t imu_orig_ax, imu_orig_ay, imu_orig_az;//加速度计
int16_t imu_orig_gx, imu_orig_gy, imu_orig_gz;//陀螺仪
int16_t imu_orig_hx, imu_orig_hy, imu_orig_hz;//磁力计
------------------------------------
*/
#include <stdio.h>
#include "roboard.h"
//uart reicer flag
#define b_uart_head  0x80  //收到A5 头 标志位
#define b_rx_over    0x40  //收到完整的帧标志
// USART Receiver buffer
#define RX_BUFFER_SIZE 100 //接收缓冲区字节数

volatile unsigned char rx_buffer[RX_BUFFER_SIZE]; //接收数据缓冲区
volatile unsigned char rx_wr_index; //缓冲写指针
volatile unsigned char RC_Flag;  //接收状态标志字节
//typedef int int16_t;
float 	imu_calc_yaw,  //偏航角
		imu_calc_pitch,//俯仰
		imu_calc_roll, //滚转
		imu_calc_alt,  //高度
		imu_calc_tempr,//温度
		imu_calc_press;//气压
//在接收完一帧IMU姿态报告后，调用这个子程序来取出姿态数据
void UART2_Get_IMU(void)
{
	int16_t temp;
	
	temp = 0;
	temp = rx_buffer[2];
	temp <<= 8;
	temp |= rx_buffer[3];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	imu_calc_yaw=(float)temp / 10.0f; //偏航角
	
	temp = 0;
	temp = rx_buffer[4];
	temp <<= 8;
	temp |= rx_buffer[5];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	imu_calc_pitch=(float)temp / 10.0f;//俯仰
	
	temp = 0;
	temp = rx_buffer[6];
	temp <<= 8;
	temp |= rx_buffer[7];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	imu_calc_roll=(float)temp / 10.0f;//滚转

	temp = 0;
	temp = rx_buffer[8];
	temp <<= 8;
	temp |= rx_buffer[9];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	imu_calc_alt=(float)temp / 10.0f;//高度
	
	temp = 0;
	temp = rx_buffer[10];
	temp <<= 8;
	temp |= rx_buffer[11];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	imu_calc_tempr=(float)temp / 10.0f;//温度
	
	temp = 0;
	temp = rx_buffer[12];
	temp <<= 8;
	temp |= rx_buffer[13];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	imu_calc_press=(float)temp * 10.0f;//气压

}

//ADC值
int16_t imu_orig_ax, imu_orig_ay, imu_orig_az;//加速度计
int16_t imu_orig_gx, imu_orig_gy, imu_orig_gz;//陀螺仪
int16_t imu_orig_hx, imu_orig_hy, imu_orig_hz;//磁力计
//在接收一帧ReportMotion 后调用这个子程序来取出ADC数据
void UART2_Get_Motion(void)
{
	int16_t temp;
	
	temp = 0;
	temp = rx_buffer[2];
	temp <<= 8;
	temp |= rx_buffer[3];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	imu_orig_ax=temp;//加速度计 X轴的ADC值
	
	temp = 0;
	temp = rx_buffer[4];
	temp <<= 8;
	temp |= rx_buffer[5];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	imu_orig_ay=temp;//加速度计 Y轴的ADC值
	
	temp = 0;
	temp = rx_buffer[6];
	temp <<= 8;
	temp |= rx_buffer[7];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	imu_orig_az=temp;//加速度计 Z轴的ADC值
	
	temp = 0;
	temp = rx_buffer[8];
	temp <<= 8;
	temp |= rx_buffer[9];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	imu_orig_gx=temp;//陀螺仪 X轴的ADC值
	
	temp = 0;
	temp = rx_buffer[10];
	temp <<= 8;
	temp |= rx_buffer[11];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	imu_orig_gy=temp;//陀螺仪 Y轴的ADC值
	
	temp = 0;
	temp = rx_buffer[12];
	temp <<= 8;
	temp |= rx_buffer[13];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	imu_orig_gz=temp;//陀螺仪 Z轴的ADC值
	
	temp = 0;
	temp = rx_buffer[14];
	temp <<= 8;
	temp |= rx_buffer[15];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	imu_orig_hx=temp;//磁力计 X轴的ADC值
	
	temp = 0;
	temp = rx_buffer[16];
	temp <<= 8;
	temp |= rx_buffer[17];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	imu_orig_hy=temp;//磁力计 Y轴的ADC值
	
	temp = 0;
	temp = rx_buffer[18];
	temp <<= 8;
	temp |= rx_buffer[19];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	imu_orig_hz=temp;//磁力计 Z轴的ADC值
}


//以下中断子程序是基于STM32的，如果你使用的不是STM32 请做相应的修改
//------------------------------------------------------
void USART2_IRQHandler(void) //接收中断子程序
{
  unsigned char data;
  data=(unsigned char)com4_Read(); //读取接收到的字节
  if(data==0xa5) 
  { 
      RC_Flag|=b_uart_head; //如果接收到A5 置位帧头标专位
      rx_buffer[rx_wr_index++]=data; //保存这个字节.
  }
  else if(data==0x5a)
       { 
	   if(RC_Flag&b_uart_head) //如果上一个字节是A5 那么认定 这个是帧起始字节
	     { rx_wr_index=0;  //重置 缓冲区指针
		   RC_Flag&=~b_rx_over; //这个帧才刚刚开始收
         }
         else //上一个字节不是A5
		  rx_buffer[rx_wr_index++]=data;
         RC_Flag&=~b_uart_head; //清帧头标志
       }
	   else
	   { rx_buffer[rx_wr_index++]=data;
		 RC_Flag&=~b_uart_head;
		 if(rx_wr_index==rx_buffer[0]) //收够了字节数.
	     {  
			RC_Flag|=b_rx_over; //置位 接收完整的一帧数据
          }
	   }

  if(rx_wr_index==RX_BUFFER_SIZE) //防止缓冲区溢出
  rx_wr_index--;
}

//--校验当前接收到的一帧数据是否 与帧校验字节一致
unsigned char Sum_check(void)
{ 
  unsigned char i;
  int16_t checksum=0; 
  for(i=0;i<rx_buffer[0]-2;i++)
   checksum+=rx_buffer[i];
  if((checksum%256)==rx_buffer[rx_buffer[0]-2])
   return(0x01); //Checksum successful
  else
   return(0x00); //Checksum error
}
bool imu_is_update_imu;
bool imu_is_update_motion;

//--这个子程序需要在主程序中 定时调用,以检查 串口是否接收完一帧数据
void UART2_CommandRoute(void)
{
     imu_is_update_imu=false;
 imu_is_update_motion=false;
  if(RC_Flag&b_rx_over){  //已经接收完一帧?
	RC_Flag&=~b_rx_over; //清标志先
	if(Sum_check()){ 
	//校验通过
	  if(rx_buffer[1]==0xA1){ //UART2_ReportIMU 的数据
// 	    printf("get IMU succ\n");
              imu_is_update_imu=true;
	    UART2_Get_IMU();	//取数据
	  }
	  if(rx_buffer[1]==0xA2){ //UART2_ReportMotion 的数据
// 	    printf("get Motion succ\n");
              imu_is_update_motion=true;
	    UART2_Get_Motion();	 //取数据
	  } 
	
	}//校验是否通过?
  }
}

#endif

