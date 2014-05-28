/* 
 * File:   main.cpp
 * Author: allen
 *
 * Created on 2012年7月21日, 下午3:26
 */

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include<iostream>
#include<math.h>
//#include"bioloid/IntfBioloid.h"
//#include"rmg/rmg146.h"
#include"com/AX12_C3.h"
#include"com/AD.h"
#include"com/IMU.h"
//#include"camera/camera.h"
#include"roboard.h"
#include"perception/DoraPerceptions.h"
#include"action/DoraCommand.h"
#include"action/DoraAction.h"
using namespace std;
using namespace dora_perception;
using namespace dora_action;
//pthread_mutex_t camera_mutex;
//pthread_mutex_t posture_mutex;
pthread_t camera_tid;
pthread_t posture_tid;
pthread_t ctrl_tid;



int camera_flag = 1;
int posture_flag = 1;
int ctrl_flag = 1;



void * cameraThread(void *pa) {
  //  float cameraInfo[2];
    //   int cycles = 10;
  //  while (camera_flag == 1) {
        struct timeval timeStart;
        gettimeofday(&timeStart, NULL);
    //    camera_flag = mainLoop();

   //     cameraInfo[0] = Get_xielv();
    //    cameraInfo[1] = Get_piancha();
  //      cout << "cameraInfo[0] : " << cameraInfo[0] << endl;
 //       cout << "cameraInfo[1] : " << cameraInfo[1] << endl;
        //setBioJointDeg(0,20);



        // usleep(30000);
        struct timeval timeEnd;
        gettimeofday(&timeEnd, NULL);
        long timeCost = 1000000 * (timeEnd.tv_sec - timeStart.tv_sec)+(timeEnd.tv_usec - timeStart.tv_usec);
        long timeCostms = timeCost / 1000;
   //     printf("cameraThread cost %ld ms %ld us\n", timeCostms, timeCost);
        /*
         *以下操作camera交互数据
         *
         */
//        pthread_mutex_lock(&camera_mutex);
//        camera_info[0] = cameraInfo[0];
//        camera_info[1] = cameraInfo[1];
//        pthread_mutex_unlock(&camera_mutex);
  //  }
}

void * postureThread(void *pa) {
  
DoraIMU doraimu;
    while (1) {
        struct timeval timeStart;
        gettimeofday(&timeStart, NULL);
   doraimu.update();
        
        
        struct timeval timeEnd;
        gettimeofday(&timeEnd, NULL);
//#define DELTA_TIME	  	   0.01//10ms
        long timeCost = 1000000 * (timeEnd.tv_sec - timeStart.tv_sec)+(timeEnd.tv_usec - timeStart.tv_usec);
        long timeCostms = timeCost / 1000;
 //       long timeComps = DELTA_TIME * 1000000 - timeCost;
     
 //       printf("postureThread cost %ld ms %ld us\n", timeCostms, timeCost);

    }

}

void *ctrlThread(void* pa) {
    //  int cycles = 100;
   DoraAction doraAct;
  //  IB.setTimeOfCycle(0.02f);
 
 //   while (ctrl_flag == 1) {
//    while(1){
        struct timeval timeStart;
        gettimeofday(&timeStart, NULL);
 
 //  doraAct.run();
      
        //以下为控制主程序
/*
        if (zhuangtouflag == 0)//
        {
            if (cameraInfo[1] > -3 || cameraInfo[0]<-5)//如果机器人太靠右或者方向过于偏向右边则转头
            {
                zhuangtouflag = 1;
            }
        } else {
            if (cameraInfo[1]<-8) {
                zhuangtouflag = 0;
            }
            cameraInfo[0] = cameraInfo[0] + 7;
            cameraInfo[1] = cameraInfo[1] + 3;

        }
*/
      

        //  usleep(20000);
        struct timeval timeEnd;
        gettimeofday(&timeEnd, NULL);
        long timeCost = 1000000 * (timeEnd.tv_sec - timeStart.tv_sec)+(timeEnd.tv_usec - timeStart.tv_usec);
        long timeCostms = timeCost / 1000;
        long timeComps = 20000 - timeCost;
//        if (timeComps > 0) {
//            usleep(timeComps);
//        }
    //    printf("ctrlThread cost %ld ms %ld us\n", timeCostms, timeCost);
 //   }
}

void initRoboard() {
    // first set the correct RoBoard version
    //roboio_SetRBVer(RB_100);      // if your RoBoard is the newest RB-100 (model: DM-118D)
    //roboio_SetRBVer(RB_100b2);  // if your RoBoard is old RB-100 (model: DM-118A/DM-118B/DM-118C)
    //roboio_SetRBVer(RB_100RD);  // if your RoBoard is RB-100RD
    roboio_SetRBVer(RB_110); // if your RoBoard is RB-110
    //roboio_SetRBVer(RB_050);    // if your RoBoard is RB-050
// InitAD();
//     if (InitDM163() == false) {
//         printf(" [gyr] Error:%s !!\n", roboio_GetErrMsg());
//         posture_flag = 0;
//     }
// 
//     if (init_video() != 1) {
//         printf("[video] init  Error !!\n");
//         camera_flag = 0;
//     }
    if (InitCom3() != 0) {
        printf("[com3] init  Error !!\n");
        ctrl_flag = 0;
    }
    if (InitCom4() != 0) {
	printf("[com4] init  Error !!\n");
        ctrl_flag = 0;
    }
    
}

void runMultiplyThread(void) {
    int temp;
    memset(&camera_tid, 0, sizeof (pthread_t));
    memset(&posture_tid, 0, sizeof (pthread_t));
    memset(&ctrl_tid, 0, sizeof (pthread_t));
    /*创建线程*/
    if ((temp = pthread_create(&camera_tid,
            NULL,
            cameraThread,
            NULL)) != 0) //comment2
    {
        printf("线程camera创建失败!\n");
    } else {
        printf("线程camera被创建\n");
    }
    if ((temp = pthread_create(&posture_tid,
            NULL,
            postureThread,
            NULL)) != 0) //comment2
    {
        printf("线程posture创建失败!\n");
    } else {
        printf("线程posture被创建\n");
    }
    if ((temp = pthread_create(&ctrl_tid,
            NULL,
            ctrlThread,
            NULL)) != 0) //comment2
    {
        printf("线程control创建失败!\n");
    } else {
        printf("线程control被创建\n");
    }

    if (camera_tid != 0) { //comment4
        pthread_join(camera_tid, NULL);
        printf("线程camera已经结束\n");
    }
    if (posture_tid != 0) { //comment5
        pthread_join(posture_tid, NULL);
        printf("线程posture已经结束\n");
    }
    if (posture_tid != 0) { //comment5
        pthread_join(ctrl_tid, NULL);
        printf("线程ctrl_tid已经结束\n");
    }
}

int main() {
    /*用默认属性初始化互斥锁*/
//     pthread_mutex_init(&camera_mutex, NULL);
//     pthread_mutex_init(&posture_mutex, NULL);
  //  unsigned char comd[11] ={0xFF,0xFF,0x08,0x11,0x02,0x00,0x00,0x00,0x00,0x00,0x1B};
  //  unsigned char state;
    initRoboard();
  //  DoraIMU doraimu;
     runMultiplyThread();
  //  while(1)
  //  {
//	
   //     doraimu.update();
      //  doraimu.test();
        
  //  }
//    DoraCamera doraCam;
//    doraCam.initCamera();
//    sleep(1);
//    int a = 20;
// //   while(--a){
//    doraCam.test();
 //   }
//    DoraAction doraAct;
//    doraAct.run();
    return 0;
}
