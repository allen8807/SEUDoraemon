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
#include"core/DoraWorldModel.h"
#include"stdio.h"

namespace dora_perception {
    using namespace std;
    using namespace seumath;
    using namespace dora_core;

    DoraCamera::DoraCamera() {
        mPtrFrame = NULL;
        mPtrCapture = NULL;
        mPtrNarrowImage = NULL;
        mPtrGrayImage = NULL;
        mPtrBinaryImage = NULL;
        mPtrShrinkImage = NULL;
        mPtrErodeDilateImage = NULL;
        mNarrowTimes = 4;
        mShrinkTimes = 4;

        memset((&mCamCfg), 0, sizeof (VideoCap_cfg));
        mCamCfg.width = 640;
        mCamCfg.height = 480;
    }

    DoraCamera::DoraCamera(const DoraCamera& orig) {
    }

    DoraCamera::~DoraCamera() {
        closeCamera();
    }

    void DoraCamera::update() {

    }

    void DoraCamera::compassImage(IplImage * pstImage) {
        if (NULL == pstImage) {
            return;
        }
        IplImage * tmpImage =
                cvCreateImage(
                cvSize(80, 60),
                pstImage->depth,
                1
                );

        cvResize(pstImage, tmpImage, CV_INTER_AREA);

        cout << "compress Image\n";
        printBinaryImage(tmpImage);
        cvReleaseImage(&tmpImage);

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
        //  saveImageToFile("small.bmp", mPtrNarrowImage);
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
        unsigned char threshold = minGray + (unsigned char) ((minGray + maxGray)*0.2f);
        mPtrBinaryImage =
                cvCreateImage(
                cvGetSize(mPtrGrayImage),
                mPtrGrayImage->depth,
                1
                );

        cvThreshold(mPtrGrayImage, mPtrBinaryImage, threshold, 255, CV_THRESH_BINARY);
        //     printBinaryImage(mPtrBinaryImage);
        saveImageToFile("binary.bmp", mPtrBinaryImage);
    }

    void DoraCamera::shrinkImage() {
        if (NULL != mPtrShrinkImage) {
            cvReleaseImage(&mPtrShrinkImage);
        }
        if (NULL == mPtrErodeDilateImage) {
            return;
        }
        mPtrShrinkImage =
                cvCreateImage(
                cvSize(mPtrErodeDilateImage->width / mShrinkTimes,
                mPtrErodeDilateImage->height / mShrinkTimes),
                mPtrErodeDilateImage->depth,
                1
                );
        IplImage * tmpImage =
                cvCreateImage(
                cvSize(mPtrErodeDilateImage->width / mShrinkTimes,
                mPtrErodeDilateImage->height / mShrinkTimes),
                mPtrErodeDilateImage->depth,
                1
                );

        cvResize(mPtrErodeDilateImage, tmpImage, CV_INTER_AREA);
        cvThreshold(tmpImage, mPtrShrinkImage, 128, 255, CV_THRESH_BINARY);
        //     printBinaryImage(mPtrShrinkImage);
        //      printGrayImage(mPtrShrinkImage);
        //      saveImageToFile("shrink.bmp", mPtrShrinkImage);
        cvReleaseImage(&tmpImage);
    }

    void DoraCamera::enhanceImage(IplImage * pstImage) {
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
        int i = 0;
        int j = 0;
        unsigned char tmp = 0;
        for (i = 0; i < pstImage->height; ++i) {
            for (j = 0; j < pstImage->width; ++j) {
                tmp = pstImage->imageData[i * (pstImage->widthStep) + j * (pstImage->nChannels)];
                if (tmp > 128) {
                    tmpImage->imageData[i * (tmpImage->widthStep) + j * (tmpImage->nChannels)] = 0;
                } else {
                    tmpImage->imageData[i * (tmpImage->widthStep) + j * (tmpImage->nChannels)] = 1;
                }
            }
        }
        cvReleaseImage(&tmpImage);
    }

    void DoraCamera::erodeAndDilateImage(IplImage * pstImage) {
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
        if (NULL != mPtrErodeDilateImage) {
            cvReleaseImage(&mPtrErodeDilateImage);
        }
        mPtrErodeDilateImage =
                cvCreateImage(
                cvSize(pstImage->width,
                pstImage->height),
                pstImage->depth,
                1
                );
        //   cvErode(pstImage,tmpImage,NULL,5);
        cvDilate(pstImage, tmpImage, NULL, 2);
        cvErode(tmpImage, tmpImage2, NULL, 2);
        cvCopy(tmpImage2, mPtrErodeDilateImage, NULL);
        printBinaryImage(tmpImage2);
        //     saveImageToFile("Erode.bmp", tmpImage2);
        //cvDilate

        cvReleaseImage(&tmpImage);
        cvReleaseImage(&tmpImage2);
    }

    void DoraCamera::printGrayImage(IplImage * pstImage) {
        if (NULL == pstImage) {
            return;
        }
        int i = 0;
        int j = 0;
        unsigned char tmp = 0;
        for (i = 0; i < pstImage->height; ++i) {
            for (j = 0; j < pstImage->width; ++j) {
                tmp = pstImage->imageData[i * (pstImage->widthStep) + j * (pstImage->nChannels)];

   //             printf("%3d ", tmp);
            }
    //        cout << "*" << endl;
        }

    }

    void DoraCamera::printBinaryImage(IplImage * pstImage) {
        if (NULL == pstImage) {
            return;
        }
        int i = 0;
        int j = 0;
        unsigned char tmp = 0;
        for (i = 0; i < pstImage->height; ++i) {
            for (j = 0; j < pstImage->width; ++j) {
                tmp = pstImage->imageData[i * (pstImage->widthStep) + j * (pstImage->nChannels)];
                if (tmp > 128) {
       //             printf("  ");
                } else {
       //             printf("1 ");
                }
            }
          //  cout << "*" << endl;
        }

    }

    void DoraCamera::notifyObservers() {
        MUTI_DATA.setBlocks(mWMBlocks);
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
        //    mPtrFrame = cvLoadImage("02.jpg",CV_LOAD_IMAGE_COLOR);
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
//        cvSaveImage(pstFilename, pstImage);
//        cout << "filename\n" << pstFilename
//                << "\ncolor model\n" << pstImage->colorModel
//                << "\ndata\n" << pstImage->imageSize
//                << " " << pstImage->height <<
//                " " << pstImage->width <<
//                " withstep" << pstImage->widthStep
//                << " dep" << pstImage->depth
//                << " channels" << pstImage->nChannels
//                << endl;
//        cout << "write to file" << endl;
    }

    void DoraCamera::setBlock(IplImage * pstImage) {
        if (NULL == pstImage) {
            return;
        }
        mBlocks.clear();
        IplImage * tmpImage =
                cvCreateImage(
                cvGetSize(pstImage),
                pstImage->depth,
                1
                );
        int width = pstImage->width;
        int height = pstImage->height;
        int widthStep = pstImage->widthStep;
        cvCopy(pstImage, tmpImage, NULL);
        this->printGrayImage(tmpImage);
      //  cout << "set bound" << endl;
        //填边
        const int whiteBit = 255;
        const int blackBit = 0;
        const int dealedBit = 160;
        const int targetBit = 80;
        int maxTargetHeight = height / 3;
        int i = 0;
        int j = 0;

        for (i = 0; i < height; ++i) {
            tmpImage->imageData[i * widthStep] = whiteBit;
            tmpImage->imageData[i * widthStep + width - 1] = whiteBit;
        }
        for (j = 0; j < width; ++j) {
            tmpImage->imageData[j] = whiteBit;
            tmpImage->imageData[(height - 1) * widthStep + j] = whiteBit;
        }
        this->printGrayImage(tmpImage);
        //处理
        char * boundaryImageData = tmpImage->imageData;
        for (i = height - 2; i > 0; --i) {
            for (j = 1; j < width - 1; ++j) {
                if (blackBit == boundaryImageData[i * widthStep + j]) {
                    int left = j;
                    int minLeft = j;
                    int right = j;
                    int maxRight = j;
                    int up = i;
                    int k = 0;
                    for (up = i; up > 0; --up) {
              //          cout << " i " << i << "j " << j << "up " << up << endl;
                        while (!((blackBit == boundaryImageData[up * widthStep + left])
                                && (blackBit != boundaryImageData[up * widthStep + left - 1]))) {
                            if (blackBit == boundaryImageData[up * widthStep + left]) {
                                --left;
                            } else {
                                ++left;
                            }
                            if (left <= 1 || left >= width - 2) {
                                break;
                            }
                        }
            //            cout << "left" << left << endl;
                        while (!((blackBit == boundaryImageData[up * widthStep + right])
                                && (blackBit != boundaryImageData[up * widthStep + right + 1]))) {
                            if (blackBit == boundaryImageData[up * widthStep + right]) {
                                ++right;
                            } else {
                                --right;
                            }
                            if (right <= 1 || right >= width - 2) {
                                break;
                            }
                        }
          //              cout << "right" << right << endl;
                        if ((i - up) <= maxTargetHeight) {
                            if (left < minLeft) {
                                minLeft = left;
                            }
                            if (right > maxRight) {
                                maxRight = right;
                            }
                        }
                        //如果左边界大于右边界那么循环结束，跳出
                        if (left > right) {
                            break;
                        }

                        for (k = left; k <= right; ++k) {
                            boundaryImageData[up * widthStep + k] = dealedBit;
                        }

                    }
                    VisionBlock tmpBlock;
                    tmpBlock.width = maxRight - minLeft + 1;
                    if (i - up < maxTargetHeight) {
                        tmpBlock.height = i - up;
                    } else {
                        tmpBlock.height = maxTargetHeight;
                    }
                    tmpBlock.leftDownPoint = Vector2i(i, minLeft);
                    mBlocks.push_back(tmpBlock);
                }
            }
        }
        //cout << "after deal" << endl;
        this->printGrayImage(tmpImage);
        int p = 0;
        int blocksNum = mBlocks.size();
        for (p = 0; p < blocksNum; ++p) {
       //     cout << "leftdown point" << (mBlocks[p]).leftDownPoint
         //           << "height" << (mBlocks[p]).height
           //         << "width" << (mBlocks[p]).width << endl;
        }

        cvReleaseImage(&tmpImage);



    }

    void DoraCamera::test() {

        clock_t begin;
        clock_t end;
        begin = clock();
        updateFrame();
  //      saveFrameToFile("test.bmp");
        end = clock();
        //cout << "update time is " << (end - begin)*1000 / CLOCKS_PER_SEC << "ms" << endl;
        begin = clock();
        narrowImage();
        grayImage();
        binaryImage();
        erodeAndDilateImage(mPtrBinaryImage);
        shrinkImage();
        setBlock(mPtrShrinkImage);
      //  compassImage(mPtrShrinkImage);
      
        calcWMBlock(mPtrShrinkImage);
          notifyObservers();
        end = clock();
       // cout << "time is " << (end - begin)*1000 / CLOCKS_PER_SEC << "ms" << endl;
    }

    void DoraCamera::calcWMBlock(IplImage * pstImage) {
        if(NULL == pstImage){
            return;
        }
        const float heightdMax = 1.0f;
        const float errHeight = 0.2f;
        const float widthMax = 1.0f;
        const float heightMAxDir = 46.0f;
        const float widthMaxDir = 60.0f;
        int width = pstImage->width;
        int height  = pstImage->height;
        int size = mBlocks.size();
        mWMBlocks.clear();
        for(int i =0;i<size;++i){
            int w = mBlocks[i].leftDownPoint.y();
            int h =mBlocks[i].leftDownPoint.x();
            WMBlocks tmp;
            tmp.leftDir = w*widthMaxDir/width - widthMaxDir/2;
            tmp.rightDir = (w+mBlocks[i].width)*widthMaxDir/width - widthMaxDir/2;
            tmp.widthOffset =  w*widthMax/width - widthMax/2;
            tmp.foroardOffset = h*heightdMax/height + errHeight;
            mWMBlocks.push_back(tmp);
        }
      //  MUTI_DATA.setBlocks(mWMBlocks);
    }
}//namespace dora_perception 
