/* 
 * File:   DoraemonCtrl.h
 * Author: allen
 *
 * Created on 2014年5月26日, 下午8:45
 */

#ifndef DORAEMONCTRL_H
#define	DORAEMONCTRL_H
#include "Singleton.hpp"
#include "DoraPathPlanning.h"
namespace dora_core {

    class DoraemonCtrl : public Singleton<DoraemonCtrl> {
    public:
        DoraemonCtrl();
        DoraemonCtrl(const DoraemonCtrl& orig);
        virtual ~DoraemonCtrl();
        void run();
    private:
        DoraPathPlanning mPathPlan;


    };
#define DORA_ROBOT dora_core::DoraemonCtrl::GetSingleton()
}//namespace dora_core

#endif	/* DORAEMONCTRL_H */

