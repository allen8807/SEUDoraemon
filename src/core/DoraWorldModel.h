/* 
 * File:   DoraWorldModel.h
 * Author: allen
 *
 * Created on 2014年5月26日, 下午8:47
 */

#ifndef DORAWORLDMODEL_H
#define	DORAWORLDMODEL_H
#include "Singleton.hpp"
#include "seumath/Math.hpp"
#include <vector>

namespace dora_core {
    
    using namespace seumath;
    using namespace std;
    typedef   struct WMBlocks{
        float widthOffset;//width
        float forwardOffset;//height
        float leftDir;
        float rightDir;
    }WMBlocks;
    
    class DoraWorldModel : public Singleton<DoraWorldModel> {
    public:
        DoraWorldModel();
        DoraWorldModel(const DoraWorldModel& orig);
        virtual ~DoraWorldModel();
        void updateWorldModel();

        seumath::AngDeg getMyBodyDirection() const {
            return mMyBodyDirection;
        }

        seumath::Vector2f getMyGlobalPos2D() {
            Vector2f res(mPosition.x(), mPosition.y());
            return res;
        }

        vector<dora_core::WMBlocks> getBlocks() const {
            return mBlocks;
        }
    private:
        Vector3f mPosture;
        Vector3f mPosition;
        seumath::AngDeg mMyBodyDirection;
        vector<dora_core::WMBlocks> mBlocks;

    };
#define WM dora_core::DoraWorldModel::GetSingleton()
}


#endif	/* DORAWORLDMODEL_H */

