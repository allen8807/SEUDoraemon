/* 
 * File:   DoraCamera.cpp
 * Author: allen
 * 
 * Created on 2014年5月26日, 下午5:41
 */

#include "DoraCamera.h"
#include <cstring>
#include<iostream>
#include"core/DoraMutiThreadData.h"
#include"stdio.h"

namespace dora_perception {
    using namespace std;

    DoraCamera::DoraCamera() {
        mPtrFrame = NULL;
        mPtrCapture = NULL;
        mPtrNarrowImage = NULL;
        mPtrGrayImage = NULL;
        mPtrBinaryImage = NULL;
        mPtrShrinkImage = NULL;
        mPtrErodeDilateImage = NULL;
        mNarrowTimes = 2;
        mShrinkTimes = 2;

        memset((&mCamCfg), 0, sizeof (VideoCap_cfg));
        mCamCfg.width = 160;
        mCamCfg.height = 120;
    }

    DoraCamera::DoraCamera(const DoraCamera& orig) {
    }

    DoraCamera::~DoraCamera() {
        closeCamera();
    }

    void DoraCamera::update() {

    }

    void DoraCamera::narrowImage() {
        int i = 0;
        int j = 0;
        int k = 0;
        int narW = mPtrFrame->width / mNarrowTimes;
        int narH = mPtrFrame->height / mNarrowTimes;
        if (NULL != mPtrNarrowImage) {
            cvReleaseImage(&mPtrNarrowImage);
        }
        mPtrNarrowImage = cvCreateImage(
                cvSize(narW, narH),
                mPtrFrame->depth,
                mPtrFrame->nChannels
                );
        for (i = 0; i < narH; ++i) {
            for (j = 0; j < narW; ++j) {
                for (k = 0; k < mPtrNarrowImage->nChannels; ++k) {
                    mPtrNarrowImage->imageData[i * (mPtrNarrowImage->widthStep) + j * (mPtrNarrowImage->nChannels) + k]
                            =
                            mPtrFrame->imageData[i * mNarrowTimes * (mPtrFrame->widthStep) + j * mNarrowTimes * (mPtrNarrowImage->nChannels) + k];
                }
            }
        }
        saveImageToFile("small.bmp", mPtrNarrowImage);
    }

    void DoraCamera::grayImage() {
        if (NULL != mPtrGrayImage) {
            cvReleaseImage(&mPtrGrayImage);
        }
        if (NULL == mPtrNarrowImage) {
            return;
        }
        mPtrGrayImage =
                cvCreateImage(
                cvGetSize(mPtrNarrowImage),
                mPtrNarrowImage->depth,
                1
                );

        cvCvtColor(mPtrNarrowImage, mPtrGrayImage, CV_RGB2GRAY);
        saveImageToFile("gray.bmp", mPtrGrayImage);
    }

    void DoraCamera::binaryImage() {
        if (NULL != mPtrBinaryImage) {
            cvReleaseImage(&mPtrBinaryImage);
        }
        if (NULL == mPtrGrayImage) {
            return;
        }
        unsigned char maxGray = 0;
        unsigned char minGray = 255;
        int i = 0;
        int j = 0;
        unsigned char tmp = 0;
        for (i = 0; i < mPtrGrayImage->height; ++i) {
            for (j = 0; j < mPtrGrayImage->width; ++j) {
                tmp = mPtrGrayImage->imageData[i * (mPtrGrayImage->widthStep) + j * (mPtrGrayImage->nChannels)];
                if (maxGray < tmp) {
                    maxGray = tmp;
                }
                if (minGray > tmp) {
                    minGray = tmp;
                }
            }
        }
        unsigned char threshold = minGray+ (unsigned char)((minGray+maxGray)*0.5f);
         mPtrBinaryImage =
                cvCreateImage(
                cvGetSize(mPtrGrayImage),
                mPtrGrayImage->depth,
                1
                );
        
        cvThreshold(mPtrGrayImage,mPtrBinaryImage,threshold,255,CV_THRESH_BINARY);
        printBinaryImage(mPtrBinaryImage);
        saveImageToFile("binary.bmp", mPtrBinaryImage);
    }
    
    void DoraCamera::shrinkImage(){
        cout<<"ttttt"<<endl;
         if (NULL != mPtrShrinkImage) {
            cvReleaseImage(&mPtrShrinkImage);
        }
        if (NULL == mPtrErodeDilateImage) {
            return;
        }
         mPtrShrinkImage =
                cvCreateImage(
                 cvSize(mPtrErodeDilateImage->width/mShrinkTimes,
                mPtrErodeDilateImage->height/mShrinkTimes),
                mPtrErodeDilateImage->depth,
                1
                );
         IplImage * tmpImage =
                cvCreateImage(
                 cvSize(mPtrErodeDilateImage->width/mShrinkTimes,
                mPtrErodeDilateImage->height/mShrinkTimes),
                mPtrErodeDilateImage->depth,
                1
                );

        cvResize(mPtrErodeDilateImage,tmpImage,CV_INTER_AREA);
        cvThreshold(tmpImage,mPtrShrinkImage,128,255,CV_THRESH_BINARY);
        printBinaryImage(mPtrShrinkImage);
        printGrayImage(mPtrShrinkImage);
         saveImageToFile("shrink.bmp", mPtrShrinkImage);
    }
    
    void DoraCamera::enhanceImage(IplImage * pstImage){
         if (NULL == pstImage) {
            return;
        }
         IplImage * tmpImage = 
                 cvCreateImage(
                 cvSize(pstImage->width,
                pstImage->height),
                pstImage->depth,
                1
                );
          int i =0;
        int j =0;
       unsigned char tmp = 0; 
          for (i = 0; i < pstImage->height; ++i) {
            for (j = 0; j < pstImage->width; ++j) {
                tmp  = pstImage->imageData[i * (pstImage->widthStep) + j * (pstImage->nChannels)];
                if(tmp>128){
                    tmpImage->imageData[i * (tmpImage->widthStep) + j * (tmpImage->nChannels)] = 0;
                }else{
                    tmpImage->imageData[i * (tmpImage->widthStep) + j * (tmpImage->nChannels)] = 1;
                }
            }
        }   
    }
    
    void DoraCamera::erodeAndDilateImage(IplImage * pstImage){
        if (NULL == pstImage) {
            return;
        }
         IplImage * tmpImage = 
                 cvCreateImage(
                 cvSize(pstImage->width,
                pstImage->height),
                pstImage->depth,
                1
                );
          IplImage * tmpImage2 = 
                 cvCreateImage(
                 cvSize(pstImage->width,
                pstImage->height),
                pstImage->depth,
                1
                );
          if(NULL!=mPtrErodeDilateImage){
          cvReleaseImage(&mPtrErodeDilateImage);
          }
          mPtrErodeDilateImage=
                  cvCreateImage(
                 cvSize(pstImage->width,
                pstImage->height),
                pstImage->depth,
                1
                );
      //   cvErode(pstImage,tmpImage,NULL,5);
         cvDilate(pstImage,tmpImage,NULL,2);
         cvErode(tmpImage,tmpImage2,NULL,2);
         cvCopy(tmpImage2,mPtrErodeDilateImage,NULL);
           printBinaryImage(tmpImage2);
         saveImageToFile("Erode.bmp", tmpImage2);
         //cvDilate

        
    }
    
    void DoraCamera::printGrayImage(IplImage * pstImage){
//         if (NULL == pstImage) {
//             return;
//        }
//        int i =0;
//        int j =0;
//       unsigned char tmp = 0; 
//          for (i = 0; i < pstImage->height; ++i) {
//            for (j = 0; j < pstImage->width; ++j) {
//                tmp = pstImage->imageData[i * (pstImage->widthStep) + j * (pstImage->nChannels)];
//                
//                printf("%3d ",tmp);
//            }
//            cout<<"*"<<endl;
//        }
//        
    }
    
     void DoraCamera::printBinaryImage(IplImage * pstImage){
//         if (NULL == pstImage) {
//             return;
//        }
//        int i =0;
//        int j =0;
//       unsigned char tmp = 0; 
//          for (i = 0; i < pstImage->height; ++i) {
//            for (j = 0; j < pstImage->width; ++j) {
//                tmp = pstImage->imageData[i * (pstImage->widthStep) + j * (pstImage->nChannels)];
//                if(tmp>128){
//                printf("  ");
//                }else{
//                    printf("1 ");
//                }
//            }
//            cout<<"*"<<endl;
//        }
//        
    }

    void DoraCamera::notifyObservers() {

    }

    void DoraCamera::initCamera() {
        openCamera();
    }

    void DoraCamera::openCamera() {
        if (NULL != mPtrCapture) {
            return;
        }
        mPtrCapture = cvCreateCameraCapture(CV_CAP_ANY);
       // mPtrCapture = cvCreateFileCapture("/dev/video0");
        cvSetCaptureProperty(mPtrCapture, CV_CAP_PROP_FRAME_WIDTH, mCamCfg.width);
        cvSetCaptureProperty(mPtrCapture, CV_CAP_PROP_FRAME_HEIGHT, mCamCfg.height);
        if (NULL != mPtrCapture) {
            cout << "open cam succ" << endl;
        } else {
            cout << "open cam failed" << endl;
        }
    }

    void DoraCamera::updateFrame() {
        mPtrFrame = cvQueryFrame(mPtrCapture);
   //      mPtrFrame = cvLoadImage("01.jpg",CV_LOAD_IMAGE_COLOR);
        if (NULL != mPtrFrame) {
            cout << "update frame succ" << endl;
        } else {
            cout << "update frame failed" << endl;
        }
    }

    void DoraCamera::closeCamera() {
        //释放图像    
        //    cvReleaseImage(&mPtrFrame);         
        //释放摄像设备    
        cvReleaseCapture(&mPtrCapture);
    }

    void DoraCamera::saveFrameToFile(const char* pstFilename) {
        saveImageToFile(pstFilename, mPtrFrame);
    }

    void DoraCamera::saveImageToFile(const char* pstFilename, IplImage * pstImage) {
     //   cvSaveImage(pstFilename, pstImage);
        cout << "filename\n" << pstFilename
                << "\ncolor model\n" << pstImage->colorModel
                << "\ndata\n" << pstImage->imageSize
                << " " << pstImage->height <<
                " " << pstImage->width <<
                " withstep" << pstImage->widthStep
                << " dep" << pstImage->depth
                << " channels" << pstImage->nChannels
                << endl;
        cout << "write to file" << endl;
    }

    void DoraCamera::test() {
       
        clock_t begin;
        clock_t end;
          begin = clock();
        updateFrame();
        saveFrameToFile("test.bmp");
         end = clock();
        cout<<"update time is "<< (end - begin)*1000/CLOCKS_PER_SEC<<"ms"<<endl;  
  begin = clock();
        narrowImage();
        grayImage();
        binaryImage();
        erodeAndDilateImage(mPtrBinaryImage);
        shrinkImage();
       end = clock();
        cout<<"time is "<< (end - begin)*1000/CLOCKS_PER_SEC<<"ms"<<endl;
    }
}//namespace dora_perception 
