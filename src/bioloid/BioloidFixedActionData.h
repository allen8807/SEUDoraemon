/* 
 * File:   BioloidFixedActionData.h
 * Author: allen
 *
 * Created on 2012年7月7日, 下午11:04
 */

#ifndef BIOLOIDFIXEDACTIONDATA_H
#define	BIOLOIDFIXEDACTIONDATA_H
#include "Singleton.hpp"
#include "BioloidJointsData.h"
#include<string>
#include<map>


namespace BioData {
    using namespace std;
    // using namespace math;

    class BioloidFixedActionData : public Singleton<BioloidFixedActionData> {
    public:

        /**
         * an angle step means to desired angle and needed time
         *
         * pose; // desired pose
         * time; // move in given time
         * priority; // can be changed by other tasks?
         *              //数量越小，优先级越高，最小为0
         *               //true（1）表示该pose执行结束后可以使用别的task而不一定要继续这个task，
         *               //false(0)表示该pose执行结束后必须使用该task指定的下一个pose
         *   int adjustLeg; // 仅用于walk相关的序列can adjust foot? which?
         *                  // 0 表示不是walk相关，不可以
         *                  // 1 表示可调整right leg
         *                  // 2 表示可调整left leg
         *                  // 3 表示可调整right&left leg
         *   std::string next; // next task name
         *   bool changeFoot; // will the foot changed in next task?保留暂时不用
         */
        struct BioFixedTask {
            std::string pose; // desired pose
            float time; // move in given time
            int priority; // can be changed by other tasks?
            int adjustLeg; // can adjust foot? which?
            std::string next; // next task name
            bool changeFoot; // will the foot changed in next task?
        };
        typedef std::map<std::string, BioFixedTask> TTaskMap;
        typedef std::map<std::string, BioData::BioloidJointsData> TPoseMap;

        BioloidFixedActionData();
        void init();
        ~BioloidFixedActionData();
               const TTaskMap& getTaskMap() const{
            return mTaskMap;
        }

       const TPoseMap& getPoseMap() const{
           return mPoseMap;
       }
       
    protected:
        bool loadConfigFile(const std::string& filename, std::map<std::string, BioFixedTask>& map);
        bool loadConfigFile(const std::string& filename, std::map<std::string, BioData::BioloidJointsData>& map);
        void writeToPoseMap(std::istream &p_Stream, BioData::BioloidJointsData& p_BJD);
        BioData::BioloidJointsData::HJ_ID getIDByHJName(const std::string& p_Name);
        //   BioloidFixedActionData(const BioloidFixedActionData& orig);
        bool loadConfigString(const std::string& p_String, std::map<std::string, BioFixedTask>& map);
        bool loadConfigString(const std::string& p_String, std::map<std::string, BioData::BioloidJointsData>& map);

    private:
        /** fixed angles */
        TTaskMap mTaskMap;
        TPoseMap mPoseMap;
    };
    std::ostream & operator<<(std::ostream &p_Stream, BioloidFixedActionData::BioFixedTask& p_Task);
    std::istream & operator>>(std::istream &p_Stream, BioloidFixedActionData::BioFixedTask& p_Task);

#define BFAD BioData::BioloidFixedActionData::GetSingleton()
}/* namespace BioData */

#endif	/* BIOLOIDFIXEDACTIONDATA_H */

