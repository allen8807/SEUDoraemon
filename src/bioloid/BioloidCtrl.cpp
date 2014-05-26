/* 
 * File:   BioloidCtrl.cpp
 * Author: allen
 * 
 * Created on 2012年7月9日, 上午10:40
 */

#include "BioloidCtrl.h"
namespace BioCtrl {

    BioloidCtrl::BioloidCtrl() {
        //       mTimeOfCycle = 0.2f;
    }

    void BioloidCtrl::setTimeOfCycle(float p_timeOfCycle) {
        mPecpetion.setTimeOfCycle(p_timeOfCycle);

    }

    float BioloidCtrl::getTimeOfCycle() const {
        return mPecpetion.getTimeOfCycle();
    }

    BioEfect::BioloidJointsEffector& BioloidCtrl::getBioEffect() {
        return mBioEffector;
    }


    //BioloidCtrl::BioloidCtrl(const BioloidCtrl& orig) {
    //}

    BioloidCtrl::~BioloidCtrl() {
    }

    void BioloidCtrl::BioBlindMode(BLIND_MODE p_Mode) {
        switch (p_Mode) {
            case GETUP_LIE:
                this->mFixedCtrl.getUpFromLieBlindDeg();
                break;
            case GETUP_DIVE:
                this->mFixedCtrl.getUpFromDiveBlindDeg();
                break;
            case WALK_STRAIGHT:
                this->mFixedCtrl.walkBlindDeg();
                break;
            case TURN_LEFT:
                this->mFixedCtrl.turnLeftBlindDeg();
                break;
            case TURN_RIGHT:
                this->mFixedCtrl.turnRightBlindDeg();
                break;
            default:
                break;
        }
        return;
    }

    void BioloidCtrl::BioNormalLoop() {
        //更新传感器的信息
        mPecpetion.update();

        BioPcept::BioloidPerception::BalanceState nowState = mPecpetion.getNowBalanceState();
        if (nowState == BioPcept::BioloidPerception::LIED_STATE) {
            //       std::cout<<"Loop before Lie"<<std::endl;
            this->mFixedCtrl.getUpFromLie();
        } else if (nowState == BioPcept::BioloidPerception::DIVED_STATE) {
            //         std::cout<<"Loop before Dive"<<std::endl;
            this->mFixedCtrl.getUpFromDive();
        } else
              if (nowState == BioPcept::BioloidPerception::BALANCE_STATE)
        {
            //    std::cout<<"Loop before walk"<<std::endl;
            
/*
		float turnCoef = 4 * mPecpetion.getCameraSlope() + mPecpetion.getCameraOffset();
            if (abs(turnCoef) < 11) {
              // this->mFixedCtrl.turnLeft();
		// this->mFixedCtrl.walkStraight();
		this->mFixedCtrl.turnRight();
            } else if (abs(turnCoef) >= 11) {
                //this->mFixedCtrl.turnLeft();
		//this->mFixedCtrl.walkStraight();
		this->mFixedCtrl.turnRight();
            } else {
                this->mFixedCtrl.turnRight();
            }
*/

	xielvmemo[2] = xielvmemo[1]; //for memo distance
	xielvmemo[1] = xielvmemo[0];
	xielvmemo[0] = 	mPecpetion.getCameraSlope();

	pianchamemo[2] = pianchamemo[1];
	pianchamemo[1] = pianchamemo[0];
	if(fabs(pianchamemo[0]) > 6 && mPecpetion.getCameraOffset() < 1)
	{
	if(cylinderflag  == 0 )pianchamemo[0] = -20.0;
	else pianchamemo[0] =20.0;
	cout<<"LOST LOST LOST LOST LOST LOST"<<endl;
	}
	else pianchamemo[0] = mPecpetion.getCameraOffset();

	if (xielvmemo[0]+ 60 < xielvmemo[2] && xielvmemo[1] + 60 < xielvmemo[2]&& xielvmemo[0]<80 &&xielvmemo [1] < 80)
	{
		if(cylinderflag == 0)cylinderflag =1;
		else cylinderflag =0;
	}	
cylinderflag =0;
double averagedistance = (xielvmemo[0]+xielvmemo[1]+xielvmemo[2])/3;
double averagepiancha =(pianchamemo[0]+pianchamemo[1]+pianchamemo[2])/3;



	if (cylinderflag == 0)//go left
{
	if(averagedistance < 60)
	{
	if(pianchamemo[0] > -6)	this->mFixedCtrl.turnRight();
	else if(pianchamemo[0] < -19)	this->mFixedCtrl.turnLeft();
	else this->mFixedCtrl.walkStraight();
	}
	else
	{
	 if(pianchamemo[0] > -10)   this->mFixedCtrl.turnRight();
        else if(pianchamemo[0] < -19)     this->mFixedCtrl.turnLeft();
        else this->mFixedCtrl.walkStraight();
	}	


}

	else
{
	if(averagedistance < 60)
	{
	if(pianchamemo[0] < 6)	this->mFixedCtrl.turnLeft();
	else if(pianchamemo[0] > 19)	this->mFixedCtrl.turnRight();
	else this->mFixedCtrl.walkStraight();
	}
	else
	{
	if(pianchamemo[0] < 10)    this->mFixedCtrl.turnLeft();
        else if(pianchamemo[0] > 19)      this->mFixedCtrl.turnRight();
        else this->mFixedCtrl.walkStraight();
	}

}  


cout<< "cylinderflag : "<<cylinderflag<<endl;

      }else{

            this->mFixedCtrl.getUpFromDive();    
}
}
}

