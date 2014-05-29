/* 
 * File:   DoraCamera.h
 * Author: allen
 *
 * Created on 2014年5月26日, 下午5:41
 */

#ifndef DORACAMERA_H
#define	DORACAMERA_H
#include<string>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include"DoraBasicPerception.h"
#include"seumath/Math.hpp"
#include<vector>
#include"core/DoraWorldModel.h"
namespace dora_perception {
    using namespace std;
    using namespace dora_core;
typedef struct VideoCap_cfg
{
    bool use_cv;
    const char *dev_name;
    unsigned int index;
    unsigned int buff_num;
    unsigned int width;
    unsigned int height;
    unsigned int format; 
}VideoCap_cfg;
    
typedef struct VisionBlock
{
    seumath::Vector2i leftDownPoint;//(height,width)
    int height;
    int width;
    
}VisionBlock;
class DoraCamera :public DoraBasicPerception {
public:
    DoraCamera();
    DoraCamera(const DoraCamera& orig);
    virtual ~DoraCamera();
    void update();
    void notifyObservers();
    void initCamera();
    void openCamera();
    void updateFrame();
    void closeCamera();
    void saveFrameToFile(const char *  pstFilename);
    void saveImageToFile(const char* pstFilename,IplImage * pstImage);
    void setBlock(IplImage * pstImage);//二值图像
    void compassImage(IplImage * pstImage);
    void test();
    
private:
    void narrowImage();
    void grayImage();
    void binaryImage();
    void shrinkImage();
    void enhanceImage(IplImage * pstImage);
    void erodeAndDilateImage(IplImage * pstImage);
    void printGrayImage(IplImage * pstImage);
    void printBinaryImage(IplImage * pstImage);
    void calcWMBlock(IplImage * pstImage);
    
    VideoCap_cfg mCamCfg;
    int mNarrowTimes;
    int mShrinkTimes;
    IplImage*  mPtrNarrowImage;
    IplImage*  mPtrGrayImage;
    IplImage*  mPtrBinaryImage;
    IplImage*  mPtrShrinkImage;
    IplImage*  mPtrErodeDilateImage;
    
    vector<VisionBlock> mBlocks;
    vector<WMBlocks> mWMBlocks;
    //opencv
    IplImage*  mPtrFrame;
    CvCapture* mPtrCapture;
    

};
}//namespace dora_perception
#endif	/* DORACAMERA_H */

