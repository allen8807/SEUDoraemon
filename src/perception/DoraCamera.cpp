/* 
 * File:   DoraCamera.cpp
 * Author: allen
 * 
 * Created on 2014年5月26日, 下午5:41
 */

#include "DoraCamera.h"
#include <cstring>
#include<iostream>

namespace dora_perception {
    using namespace std;
    DoraCamera::DoraCamera() {
     mPtrFrame = NULL;
     mPtrCapture = NULL;
     
     memset((&mCamCfg),0,sizeof(VideoCap_cfg));
     mCamCfg.width = 640;
     mCamCfg.height = 480;
}


DoraCamera::DoraCamera(const DoraCamera& orig) {
}

DoraCamera::~DoraCamera() {
    closeCamera();
}

void DoraCamera::initCamera(){
    openCamera();
}

void DoraCamera::openCamera(){
    if(NULL != mPtrCapture){
        return;
    }
      mPtrCapture = cvCreateCameraCapture(CV_CAP_ANY);  
      cvSetCaptureProperty( mPtrCapture, CV_CAP_PROP_FRAME_WIDTH ,  mCamCfg.width );
      cvSetCaptureProperty( mPtrCapture, CV_CAP_PROP_FRAME_HEIGHT , mCamCfg.height );
      if(NULL != mPtrCapture){
      cout<<"open cam succ"<<endl;
      }else{
      cout<<"open cam failed"<<endl;
      }
}
 void DoraCamera::updateFrame(){
     	mPtrFrame = cvQueryFrame( mPtrCapture );
        if(NULL != mPtrFrame){
      cout<<"update frame succ"<<endl;
      }else{
      cout<<"update frame failed"<<endl;
      }
 }
    void DoraCamera::closeCamera(){
         //释放图像    
  //    cvReleaseImage(&mPtrFrame);         
      //释放摄像设备    
      cvReleaseCapture(&mPtrCapture);  
    }
    
     void DoraCamera::saveFrameToFile(const char* pstFilename){
         cvSaveImage(pstFilename,mPtrFrame);
          cout<<"write to file"<<endl;
     }
    
    void DoraCamera::test(){
        sleep(30);
        updateFrame();
        saveFrameToFile("test.bmp");
    }


}//namespace dora_perception 
