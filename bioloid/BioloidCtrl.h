/* 
 * File:   BioloidCtrl.h
 * Author: allen
 *
 * Created on 2012年7月9日, 上午10:40
 */

#ifndef BIOLOIDCTRL_H
#define	BIOLOIDCTRL_H
#include "Singleton.hpp"
#include"BioloidPerception.h"
#include"BioloidJointsEffector.h"
#include "BioFixedActCtrl.h"
#include "ActionController.h"
#include "BioFixedActCtrl.h"
namespace BioCtrl {
    using namespace BioPcept;
    using namespace BioEfect;

    class BioloidCtrl : public Singleton<BioloidCtrl> {
    public:

        enum BLIND_MODE {
            GETUP_LIE,
            GETUP_DIVE,
            WALK_STRAIGHT,
            TURN_LEFT,
            TURN_RIGHT
        };
        BioloidCtrl();
        //   BioloidCtrl(const BioloidCtrl& orig);
        virtual ~BioloidCtrl();

        BioPcept::BioloidPerception& getBioPcepetion() {
            return mPecpetion;
        }

        BioEfect::BioloidJointsEffector& getBioEffect();

        ActionController & getActCtrl() {
            return mActCtrl;
        }
        void BioBlindMode(BLIND_MODE p_Mode);
        void setTimeOfCycle(float mTimeOfCycle);
        float getTimeOfCycle() const;
        void BioNormalLoop();

    private:
        BioPcept::BioloidPerception mPecpetion;
        BioCtrl::BioFixedActCtrl mFixedCtrl;
        BioCtrl::ActionController mActCtrl;
        BioEfect::BioloidJointsEffector mBioEffector;

	int cylinderflag;//left : 0 right :1
	int changeflag[3];
	double xielvmemo[3];
	double pianchamemo[3];
        //     float mTimeOfCycle;
    };
#define BIO_ROBOT BioCtrl::BioloidCtrl::GetSingleton()
}/*namesapce BioCtrl*/

#endif	/* BIOLOIDCTRL_H */

