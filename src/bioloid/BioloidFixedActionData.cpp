/* 
 * File:   BioloidFixedActionData.cpp
 * Author: allen
 * 
 * Created on 2012年7月7日, 下午11:04
 */
#include<iostream>
#include <fstream>
#include <sstream>
#include "BioloidFixedActionData.h"
#include "BioloidJointsData.h"
#include "FixedTaskString.h"
#include "FixedPoseString.h"
namespace BioData {
#define TASK_FILE "data/bioloid.txt"
#define POSE_FILE "data/bioloid_pose.txt"
BioloidFixedActionData::BioloidFixedActionData() {
        init();
    }

    //BioloidFixedActionData::BioloidFixedActionData(const BioloidFixedActionData& orig) {
    //}

    BioloidFixedActionData::~BioloidFixedActionData() {
    }

    void BioloidFixedActionData::init() {
        mTaskMap.clear();
        mPoseMap.clear();

     //   mTaskStr = TASK_STR;
      //  mPoseStr = POSE_STR;

    //    loadConfigString(TASK_STR, mTaskMap);
      //  loadConfigString(POSE_STR, mPoseMap);
        loadConfigFile(TASK_FILE, mTaskMap);
        loadConfigFile(POSE_FILE, mPoseMap);
    }

    bool BioloidFixedActionData::loadConfigFile(const string& filename, map<string, BioFixedTask>& map) {
        ifstream ifs;
        ifs.open(filename.c_str());
        if (!ifs) {
            cerr << "BioloidFixedActionData can not open file: " << filename << endl;
            return false;
        }

        while (!ifs.eof()) {
            string nameLine, dataLine;
            getline(ifs, nameLine);
            if (nameLine.empty()) continue; // skip empty line
            if ('#' == nameLine[0]) continue; // skip comment line
            if (ifs.eof()) {
                cerr << "BioloidFixedActionData data breaks: " << nameLine << endl;
                return false;
            }
            stringstream ss;
            do {
                dataLine.clear();
                getline(ifs, dataLine);
                if ('#' != dataLine[0]) {
                    ss << dataLine << ' ';
                }
            } while (!dataLine.empty());

     //       cout << ss.str() << endl;
            ss >> map[nameLine];
        }
        return true;
    }

    bool BioloidFixedActionData::loadConfigFile(const string& filename, map<string, BioData::BioloidJointsData>& map) {
        ifstream ifs;
        ifs.open(filename.c_str());
        if (!ifs) {
            cerr << "BioloidFixedActionData can not open file: " << filename << endl;
            return false;
        }

        while (!ifs.eof()) {
            string nameLine, dataLine;
            getline(ifs, nameLine);
            if (nameLine.empty()) continue; // skip empty line
            if ('#' == nameLine[0]) continue; // skip comment line
            if (ifs.eof()) {
                cerr << "BioloidFixedActionData data breaks: " << nameLine << endl;
                return false;
            }
            stringstream ss;
            do {
                dataLine.clear();
                getline(ifs, dataLine);
                if ('#' != dataLine[0]) {
                    ss << dataLine << ' ';
                }
            } while (!dataLine.empty());

            //cout<<ss.str()<<endl;
            writeToPoseMap(ss, map[nameLine]);
        }
        return true;
    }

    bool BioloidFixedActionData::loadConfigString(const std::string& p_String, std::map<std::string, BioFixedTask>& map) {
   //     std::ifstream ifs;
      //  p_string
        std::stringstream ifs;
        ifs.str("");
        ifs<<p_String;
   //     cout<<"test&&&&&&&&" <<ifs.str()<<"%%%%%%%%test"<< endl;
        if (!ifs) {
            cerr << "BioloidFixedActionData can not open string task " << endl;
            return false;
        }

        while (!ifs.eof()) {
            string nameLine, dataLine;
            getline(ifs, nameLine);
            if (nameLine.empty()) continue; // skip empty line
            if ('#' == nameLine[0]) continue; // skip comment line
            if (ifs.eof()) {
                cerr << "BioloidFixedActionData data breaks: " << nameLine << endl;
                return false;
            }
            stringstream ss;
            do {
                dataLine.clear();
                getline(ifs, dataLine);
                if ('#' != dataLine[0]) {
                    ss << dataLine << ' ';
                }
            } while (!dataLine.empty());

            //   cout<<ss.str()<<endl;
            ss >> map[nameLine];
        }
        return true;
    }

    bool BioloidFixedActionData::loadConfigString(const std::string& p_String, std::map<std::string, BioData::BioloidJointsData>& map) {
        std::stringstream ifs;
        ifs.str("");
        ifs<<p_String;
        
        if (!ifs) {
            cerr << "BioloidFixedActionData can not open string pose: "<< endl;
            return false;
        }

        while (!ifs.eof()) {
            string nameLine, dataLine;
            getline(ifs, nameLine);
            if (nameLine.empty()) continue; // skip empty line
            if ('#' == nameLine[0]) continue; // skip comment line
            if (ifs.eof()) {
                cerr << "BioloidFixedActionData data breaks: " << nameLine << endl;
                return false;
            }
            stringstream ss;
            do {
                dataLine.clear();
                getline(ifs, dataLine);
                if ('#' != dataLine[0]) {
                    ss << dataLine << ' ';
                }
            } while (!dataLine.empty());

            //cout<<ss.str()<<endl;
            writeToPoseMap(ss, map[nameLine]);
        }
        return true;
    }

    void BioloidFixedActionData::writeToPoseMap(std::istream &p_Stream, BioData::BioloidJointsData& p_BJD) {
        while (!p_Stream.eof()) {
            std::string name;
            float val = 0;
            p_Stream >> name >> val;
            if (name.empty()) break;
            //p.mJointMap[HUMANOID.getJointSensorId(name)].setAngle(val);
            int hj_id = getIDByHJName(name);
            if (hj_id >= BioData::BioloidJointsData::NECK && hj_id < BioData::BioloidJointsData::DOF) {
                p_BJD.getJointsData()[hj_id].mDeg = val;
            }
        }
    }

    BioData::BioloidJointsData::HJ_ID BioloidFixedActionData::getIDByHJName(const string& p_Name) {
        if (p_Name.compare("hj1") == 0) {
            return BioData::BioloidJointsData::NECK;
        } else if (p_Name.compare("raj1") == 0) {
            return BioData::BioloidJointsData::RSHOULDER;
        } else if (p_Name.compare("raj2") == 0) {
            return BioData::BioloidJointsData::RUPPERARM;
        } else if (p_Name.compare("raj4") == 0) {
            return BioData::BioloidJointsData::RLOWERARM;
        } else if (p_Name.compare("laj1") == 0) {
            return BioData::BioloidJointsData::LSHOULDER;
        } else if (p_Name.compare("laj2") == 0) {
            return BioData::BioloidJointsData::LUPPERARM;
        } else if (p_Name.compare("laj4") == 0) {
            return BioData::BioloidJointsData::LLOWERARM;
        } else if (p_Name.compare("rlj1") == 0) {
            return BioData::BioloidJointsData::RHIP1;
        } else if (p_Name.compare("rlj2") == 0) {
            return BioData::BioloidJointsData::RHIP2;
        } else if (p_Name.compare("rlj3") == 0) {
            return BioData::BioloidJointsData::RTHIGH;
        } else if (p_Name.compare("rlj4") == 0) {
            return BioData::BioloidJointsData::RSHANK;
        } else if (p_Name.compare("rlj5") == 0) {
            return BioData::BioloidJointsData::RANKLE;
        } else if (p_Name.compare("rlj6") == 0) {
            return BioData::BioloidJointsData::RFOOT;
        } else if (p_Name.compare("llj1") == 0) {
            return BioData::BioloidJointsData::LHIP1;
        } else if (p_Name.compare("llj2") == 0) {
            return BioData::BioloidJointsData::LHIP2;
        } else if (p_Name.compare("llj3") == 0) {
            return BioData::BioloidJointsData::LTHIGH;
        } else if (p_Name.compare("llj4") == 0) {
            return BioData::BioloidJointsData::LSHANK;
        } else if (p_Name.compare("llj5") == 0) {
            return BioData::BioloidJointsData::LANKLE;
        } else if (p_Name.compare("llj6") == 0) {
            return BioData::BioloidJointsData::LFOOT;
        } else {
            return BioData::BioloidJointsData::DOF;
        }
    }

    std::ostream & operator<<(std::ostream &p_Stream, BioloidFixedActionData::BioFixedTask& p_Task) {
        p_Stream
                << p_Task.pose << ' '
                << p_Task.time << ' '
                << p_Task.priority << ' '
                << p_Task.adjustLeg << ' '
                << p_Task.next << ' '
                << p_Task.changeFoot;
        return p_Stream;
    }

    std::istream & operator>>(std::istream &p_Stream, BioloidFixedActionData::BioFixedTask& p_Task) {
        p_Stream
                >> p_Task.pose
                >> p_Task.time
                >> p_Task.priority
                >> p_Task.adjustLeg
                >> p_Task.next
                >> p_Task.changeFoot;
        return p_Stream;
    }


}/* namespace BioData */

