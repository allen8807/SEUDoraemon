/* 
 * File:   DoraPathPlanning.cpp
 * Author: allen
 * 
 * Created on 2014年5月27日, 上午12:35
 */

#include "DoraPathPlanning.h"
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

    }
}//namespace dora_core 
