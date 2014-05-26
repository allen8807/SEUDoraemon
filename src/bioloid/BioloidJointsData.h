/* 
 * File:   BioloidJointsData.h
 * Author: allen
 *
 * Created on 2012年7月6日, 下午1:47
 */

#ifndef BIOLOIDJOINTSDATA_H
#define	BIOLOIDJOINTSDATA_H
#include"math/Math.hpp"

namespace BioData {
    using namespace math;
    using namespace std;

    struct JointData {
        math::AngDeg mDeg;
        float mSpeed;
    };

    class BioloidJointsData {
    public:

        enum HJ_ID {
            NECK = 0,
            RSHOULDER = 1,
            RUPPERARM = 2,
            RLOWERARM = 3,
            LSHOULDER = 4,
            LUPPERARM = 5,
            LLOWERARM = 6,
            RHIP1 = 7,
            RHIP2 = 8,
            RTHIGH = 9,
            RSHANK = 10,
            RANKLE = 11,
            RFOOT = 12,
            LHIP1 = 13,
            LHIP2 = 14,
            LTHIGH = 15,
            LSHANK = 16,
            LANKLE = 17,
            LFOOT = 18,
            DOF = 19
        };

        BioloidJointsData();
        //  BioloidJointsData(const BioloidJointsData& orig);
        virtual ~BioloidJointsData();
//可以用于修改
        JointData * getJointsData() {
            return mJointsData;
        }
//不能用于修改
        const JointData getJointData(HJ_ID p_hId) {
            return mJointsData[p_hId];
        }

        static BioloidJointsData plusJointsDeg(const BioloidJointsData& p_op1, const BioloidJointsData& p_op2);

        template<typename DATATYPE>
        static BioloidJointsData multiplyJointsDegwithNum(DATATYPE p_Num, const BioloidJointsData & p_op2) {
            BioloidJointsData res;
            for (int i = BioloidJointsData::NECK; i < BioloidJointsData::DOF; i++) {
                res.mJointsData[i].mDeg = math::normalizeAngle(p_op2.mJointsData[i].mDeg * p_Num);
            }
            return res;
        }
        BioloidJointsData & operator=(const BioloidJointsData& p_BJD);
        /**
         * only compare mDeg
         * @param p_BJD
         * @return 
         */
        bool operator==(const BioloidJointsData& p_BJD)const;
    private:
        JointData mJointsData[DOF];
    };

}/* namespace BioData */

#endif	/* BIOLOIDJOINTSDATA_H */

