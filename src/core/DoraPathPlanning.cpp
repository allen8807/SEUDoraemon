/* 
 * File:   DoraPathPlanning.cpp
 * Author: allen
 * 
 * Created on 2014年5月27日, 上午12:35
 */

#include "DoraPathPlanning.h"
#include "DoraWorldModel.h"
#include "DoraMutiThreadData.h"
namespace dora_core {

    DoraPathPlanning::DoraPathPlanning() {
        mFinalTarget = Vector2f(8.0f, 0.0f);
        mPathPlanningTargets.push(mFinalTarget);
        mWalkRoundPostState = ST_BEGIN;
        mWalkTargets.push(Vector2f(0.0f, 0.0f));
        mWalkTargets.push(Vector2f(0.0f, 9.0f));
    }

    DoraPathPlanning::DoraPathPlanning(const DoraPathPlanning& orig) {
    }

    DoraPathPlanning::~DoraPathPlanning() {
    }

    void DoraPathPlanning::WalkRoundPostPlanning() {

        Vector2f nowTar = mWalkTargets.top();
        Vector2f mypos = WM.getMyGlobalPos2D();
        float bodydir = WM.getMyBodyDirection();
        Vector2f tarPol(0.0f, 0.0f);
        WMBlocks postPol;//柱子极坐标
        memset(&postPol, 0, sizeof (WMBlocks));
       // cout << "mypos" << mypos << "target" << nowTar << "tarPol" << tarPol << "bodydir" << bodydir << endl;
 //       cout << "tarsize" << mWalkTargets.size() << endl;
        int mapsize = 0;
        bool isSee = false;
        // std::map<unsigned int, seumath::Vector3f> polMap;
        std::vector<WMBlocks> blockVec = WM.getBlocks();

        mapsize = blockVec.size();
        for (int i = 0; i < mapsize; ++i) {
            if (blockVec[i].forwardOffset < 0.8f) {
                isSee = true;
                memcpy(&postPol, &(blockVec[i]), sizeof (WMBlocks));
                break;
            }
        }


        if (ST_BEGIN == mWalkRoundPostState) {
            tarPol = xyz2pol(nowTar - mypos);
            tarPol.y() = tarPol.y() - bodydir;

            mLocalTarget = tarPol;


            if (isSee == true) {
                mWalkRoundPostState = ST_ROUND;
                mWalkTargets.pop();
                Vector2f p1(0.0f, 0.0f);
                Vector2f tmpTarPol(0.0f, 0.0f);
                //first sub target
                tmpTarPol.x() = 1.5f;
                tmpTarPol.y() = (postPol.leftDir + postPol.rightDir) / 2 - 45.0f;
                tmpTarPol.y() += bodydir;
                p1 = pol2xyz(tmpTarPol);
                p1.x() += mypos.x();
                p1.y() += mypos.y();
                mWalkTargets.push(p1);
                //second sub target
                tmpTarPol.x() = 2.5f;
                tmpTarPol.y() = (postPol.leftDir + postPol.rightDir) / 2;
                tmpTarPol.y() += bodydir;
                p1 = pol2xyz(tmpTarPol);
                p1.x() += mypos.x();
                p1.y() += mypos.y();
                mWalkTargets.push(p1);
                //third sub target
                tmpTarPol.x() = 1.5f;
                tmpTarPol.y() = (postPol.leftDir + postPol.rightDir) / 2 + 45.0f;
                tmpTarPol.y() += bodydir;
                p1 = pol2xyz(tmpTarPol);
                p1.x() += mypos.x();
                p1.y() += mypos.y();
                mWalkTargets.push(p1);
            }

            return;

        } else if (ST_ROUND == mWalkRoundPostState) {
            tarPol = xyz2pol(nowTar - mypos);
            tarPol.y() = tarPol.y() - bodydir;

            if (abs(tarPol.x()) < 0.2f) {

                if (mWalkTargets.size() >= 2) {
                    mWalkTargets.pop();
                }
                if (mWalkTargets.size() <= 1) {
                    mWalkRoundPostState = ST_BACK;
                }

            }
            mLocalTarget = tarPol;
            return;
        } else {
            tarPol = xyz2pol(nowTar - mypos);
            tarPol.y() = tarPol.y() - bodydir;
            mLocalTarget = tarPol;
            return;
        }
    }

    void DoraPathPlanning::WalkAvoidBlocks() {
  //1.get target point
        Vector2f target = mPathPlanningTargets.top();
         Vector2f mypos =  WM.getMyGlobalPos2D();  
       float bodydir = WM.getMyBodyDirection();
       Vector2f tarPol = xyz2pol(target-mypos);
       
       tarPol.y() = tarPol.y()-bodydir;
       Vector2f finalTarPol = xyz2pol(mFinalTarget-mypos);
       finalTarPol.y() = finalTarPol.y()-bodydir;
       cout<<"mypos"<<mypos<<"target" <<target <<"tarPol" <<tarPol<<"bodydir"<<bodydir<<endl;
   cout<<"tarSize"<<mPathPlanningTargets.size()<<endl;
       //2.check block
       int mapsize = 0;
       bool isBlock = false;
       int blockNum = 0;
   //     std::map<unsigned int, math::Vector3f> polMap;
        std::vector<WMBlocks> blockVec = WM.getBlocks();
    //     if (WM.lastPerception().vision() != NULL) {
      //     polMap = WM.lastPerception().vision()->getPostGaolPolMap();
             mapsize= blockVec.size();  
                for (int i = 0; i < mapsize; ++i) {
                    if(blockVec[i].forwardOffset>0.8f || blockVec[i].forwardOffset>tarPol.x()){
                        continue;
                    }
                    if (abs((blockVec[i].leftDir+blockVec[i].rightDir)/2- tarPol.y()) < 20.0f) {
                        isBlock = true;
                        blockNum = i;
                        break;
                    }
                }
  //      }
       
  //  shared_ptr<Action> act;
    if(!isBlock){
//        if(abs(tarPol.x())<0.5f){
            if(mPathPlanningTargets.size()>1){
             mPathPlanningTargets.pop();
            }
//             cout<<"pop@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
//        act = goToRel(Vector2f(0.0f, 0.0f), 0.0f);
//       
//        }else if(tarPol.y()>5.0f){
//            act = goToRel(Vector2f(0.0f, 0.0f), 5.0f);
//        }else if(tarPol.y()<-5.0f){
//            act = goToRel(Vector2f(0.0f, 0.0f), -5.0f);
//        }else{
//            act = goToRel(Vector2f(0.0f, 0.1f), 0.0f);
//        }
        this->mLocalTarget = tarPol;
    //    return act;
    }else{
        Vector2f p1(0.0f, 0.0f);
        Vector2f tmpTarPol(0.0f, 0.0f);
        int nextBlockNum=blockNum;
     //   if(1 == mapsize){
        /*    tmpTarPol.x() = 1.5f;
            tmpTarPol.y() = tarPol.y();
            if(finalTarPol.y()>=tmpTarPol.y()){
               
                tmpTarPol.y()+=45;
            }else{
                 tmpTarPol.y()-=45;
            }*/
            tmpTarPol.x() = 1.5f;
            tmpTarPol.y() = (blockVec[blockNum].leftDir+blockVec[blockNum].rightDir)/2;
            if(finalTarPol.y()>=tmpTarPol.y()){
                
                for(int i = 0;i<mapsize;++i){
                    if(i!=blockNum && blockVec[i].forwardOffset <=0.8f && 
                            ((blockVec[i].leftDir+blockVec[i].rightDir)/2
                            > (blockVec[blockNum].leftDir+blockVec[blockNum].rightDir)/2)){
                        
                        if(nextBlockNum != blockNum  &&
                                ((blockVec[i].leftDir+blockVec[i].rightDir)/2
                                <  (blockVec[blockNum].leftDir+blockVec[blockNum].rightDir)/2)){
                            nextBlockNum = i;
                        }
                        if(nextBlockNum == blockNum){
                            nextBlockNum = i;
                        }
                    }
                }
                if(nextBlockNum != blockNum ){
                    tmpTarPol.y() =(((blockVec[blockNum].leftDir+blockVec[blockNum].rightDir)/2)
                            +(blockVec[nextBlockNum].leftDir+blockVec[nextBlockNum].rightDir)/2)/2.0f;
                    cerr<<"$$$$$$$$$$$##############$$$$$$$$$$$$$$$$$$$"<<endl;
                }else{
                tmpTarPol.y()+=45;
                }
                
            }else{
                  for(int i = 0;i<mapsize;++i){
                    if(i!=blockNum && blockVec[i].forwardOffset <=0.8f && 
                           ((blockVec[i].leftDir+blockVec[i].rightDir)/2
                            < (blockVec[blockNum].leftDir+blockVec[blockNum].rightDir)/2)
                            
                            ){
                        
                        if(nextBlockNum != blockNum  &&
                               ((blockVec[i].leftDir+blockVec[i].rightDir)/2
                                >  (blockVec[blockNum].leftDir+blockVec[blockNum].rightDir)/2)
                                ){
                            nextBlockNum = i;
                        }
                        if(nextBlockNum == blockNum){
                            nextBlockNum = i;
                        }
                    }
                }
                if(nextBlockNum != blockNum ){
                    tmpTarPol.y() = (((blockVec[blockNum].leftDir+blockVec[blockNum].rightDir)/2)
                            +(blockVec[nextBlockNum].leftDir+blockVec[nextBlockNum].rightDir)/2)/2.0f;
                    cerr<<"$$$$$$$$$$#############$$$$$$$$$$$$$$$$$$$$###################"<<endl;
                }else{
                tmpTarPol.y()-=45;
                }
            }        
        
            tmpTarPol.y() += bodydir;
            p1 = pol2xyz(tmpTarPol);
            p1.x() +=mypos.x();
            p1.y() +=mypos.y();
        //    if(p1.y()>mFinalTarget.y()){
         //       p1.y()-=0.3f;
         //   }else{
         //       p1.y()+=0.3f;
          //  }
            while(mPathPlanningTargets.size()>=2){
                mPathPlanningTargets.pop();
            }
            mPathPlanningTargets.push(p1);
            
            cout<<"p1"<<p1<<"size"<<mPathPlanningTargets.size() <<endl;
            return  WalkAvoidBlocks();
    }
    }
}//namespace dora_core 
