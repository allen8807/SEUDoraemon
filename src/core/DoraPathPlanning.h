/* 
 * File:   DoraPathPlanning.h
 * Author: allen
 *
 * Created on 2014年5月27日, 上午12:35
 */

#ifndef DORAPATHPLANNING_H
#define	DORAPATHPLANNING_H
#include "seumath/Math.hpp"
#include <stack>
namespace dora_core {
 using namespace seumath;
    enum WRP_ST {
 ST_BEGIN =0,
 ST_ROUND =1,
 ST_BACK =2
 };
class DoraPathPlanning {
    
public:
    DoraPathPlanning();
    DoraPathPlanning(const DoraPathPlanning& orig);
    virtual ~DoraPathPlanning();

    void WalkRoundPostPlanning();

private:

       Vector2f mFinalTarget;
       std::stack<Vector2f> mPathPlanningTargets;
 
        
       WRP_ST mWalkRoundPostState;
       std::stack<Vector2f> mWalkTargets;
};
}//namespace dora_core 

#endif	/* DORAPATHPLANNING_H */

