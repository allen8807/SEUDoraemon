/* 
 * File:   DoraWorldModel.h
 * Author: allen
 *
 * Created on 2014年5月26日, 下午8:47
 */

#ifndef DORAWORLDMODEL_H
#define	DORAWORLDMODEL_H
#include"Singleton.hpp"
#include"seumath/Math.hpp"
namespace dora_core {
    using namespace seumath;
    
    class DoraWorldModel : public Singleton<DoraWorldModel> {
    public:
        DoraWorldModel();
        DoraWorldModel(const DoraWorldModel& orig);
        virtual ~DoraWorldModel();
    private:
        Vector3f mPosture;
        Vector3f mPostion;

    };
#define WM dora_core::DoraWorldModel::GetSingleton()
}


#endif	/* DORAWORLDMODEL_H */

