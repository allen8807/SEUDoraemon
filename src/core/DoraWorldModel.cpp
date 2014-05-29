/* 
 * File:   DoraWorldModel.cpp
 * Author: allen
 * 
 * Created on 2014年5月26日, 下午8:47
 */

#include "DoraWorldModel.h"
#include "DoraMutiThreadData.h"

namespace dora_core {
    using namespace std;
    using namespace seumath;
    DoraWorldModel::DoraWorldModel() {
        mPosition = Vector3f(0.0f,0.0f,0.0f);
        mPosture = Vector3f(0.0f,0.0f,0.0f);
        mMyBodyDirection=0.0f;
        mBlocks.clear();
    }

    DoraWorldModel::DoraWorldModel(const DoraWorldModel& orig) {
    }

    DoraWorldModel::~DoraWorldModel() {
    }
    
    void DoraWorldModel::updateWorldModel(){
        MUTI_DATA.getPostion(&mPosition);
        MUTI_DATA.getPosture(&mPosture);
        MUTI_DATA.getBlocks(mBlocks);
        mMyBodyDirection = mPosture.z();
    }
}

