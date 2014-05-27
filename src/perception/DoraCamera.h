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

namespace dora_perception {
    using namespace std;
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
    
class DoraCamera {
public:
    DoraCamera();
    DoraCamera(const DoraCamera& orig);
    virtual ~DoraCamera();
    void initCamera();
    void openCamera();
    void updateFrame();
    void closeCamera();
    void saveFrameToFile(const char *  pstFilename);
    
    void test();
private:
    VideoCap_cfg mCamCfg;
    //opencv
    IplImage*  mPtrFrame;
    CvCapture* mPtrCapture;
    

};
}//namespace dora_perception
#endif	/* DORACAMERA_H */

