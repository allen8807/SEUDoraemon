/* 
 * File:   DoraemonCtrl.cpp
 * Author: allen
 * 
 * Created on 2014年5月26日, 下午8:45
 */

#include "DoraemonCtrl.h"
#include "seumath/Math.hpp"
#include "DoraCommand.h"
#include  "DoraMutiThreadData.h"
namespace dora_core {
    using namespace seumath;
    DoraemonCtrl::DoraemonCtrl() {
    }

    DoraemonCtrl::DoraemonCtrl(const DoraemonCtrl& orig) {
    }

    DoraemonCtrl::~DoraemonCtrl() {
    }
    void DoraemonCtrl::run(){
        seumath::Vector2f tarPol ;
        mPathPlan.WalkRoundPostPlanning();
        tarPol= mPathPlan.getLocalTarget();
        DoraCommand cmd;       
        if(abs(tarPol.x())<0.2f){  
            cmd.WalkOnSamePlace(2);
        //    cout<<"WalkOnSamePlace"<<endl;
        }else if(tarPol.y()>15.0f){
            cmd.TurnLeftInCircle(2);
         //   cout<<"TurnLeftInCircle"<<endl;
        }else if(tarPol.y()<-15.0f){
            cmd.TurnRightInCircle(2);
       //                 cout<<"TurnRightInCircle"<<endl;
        }else{
            cmd.WalkForward(10);
         //               cout<<"WalkForward"<<endl;
        }
     //   cmd.TurnLeftInCircle(2);
      //  cmd.WalkOnSamePlace(2);
        MUTI_DATA.setCommand(cmd);
        
        
        
    }
}//namespace dora_core

