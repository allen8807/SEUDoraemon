/* 
 * File:   ActionController.h
 * Author: allen
 *
 * Created on 2012年7月8日, 上午9:52
 */

#ifndef ACTIONCONTROLLER_H
#define	ACTIONCONTROLLER_H
#include<queue>
#include"BioloidJointsData.h"
#include"BioloidJointsEffector.h"

namespace BioCtrl {
    using namespace std;
    using namespace BioData;

    class ActionController {
    public:
        ActionController();
   
        virtual ~ActionController();

        typedef std::queue<BioData::BioloidJointsData> TActionQu;

        /**
         *
         * @param p_Now 当前的位置
         * @param p_Target 目标的位置
         * @param p_Cycle  期望完成的周期数
         * @return 实际需要的周期
         */
        int ctrlByDeg(const BioData::BioloidJointsData& p_Now, const BioData::BioloidJointsData& p_Target, int p_Cycle);

        int ctrlByRate(const BioData::BioloidJointsData& p_Now, const BioData::BioloidJointsData& p_Target, int p_Cycle);
        /**
         *
         * @return 剩余的周期数
         */
        int performDeg();
     //   BioEfect::BioloidJointsEffector& getBioEffector();
    protected:
        void GenerateActionDegQueue();



    private:
        BioData::BioloidJointsData mNow; //use both Deg and Speed
        BioData::BioloidJointsData mTargetPose; //only use mDeg
        TActionQu mAction;
        //float mDuration;
        int mCycle;
  //     BioEfect::BioloidJointsEffector mBioEffector;
    };
}/*namesapce BioCtrl*/

#endif	/* ACTIONCONTROLLER_H */

