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
        WMBlocks postPol;
        memset(&postPol, 0, sizeof (WMBlocks));
       // cout << "mypos" << mypos << "target" << nowTar << "tarPol" << tarPol << "bodydir" << bodydir << endl;
 //       cout << "tarsize" << mWalkTargets.size() << endl;
        int mapsize = 0;
        bool isSee = false;
        // std::map<unsigned int, seumath::Vector3f> polMap;
        std::vector<WMBlocks> blockVec = WM.getBlocks();

        mapsize = blockVec.size();
        for (int i = 0; i < mapsize; ++i) {
            if (blockVec[i].foroardOffset < 0.8f) {
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

    }
}//namespace dora_core 
