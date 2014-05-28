/* 
 * File:   DoraBasicPerception.h
 * Author: allen
 *
 * Created on 2014年5月27日, 上午11:46
 */

#ifndef DORABASICPERCEPTION_H
#define	DORABASICPERCEPTION_H
namespace dora_perception {
class DoraBasicPerception {
public:
    DoraBasicPerception(){}
 //   DoraBasicPerception(const DoraBasicPerception& orig);
    virtual ~DoraBasicPerception(){}
    virtual void update() = 0;
    virtual void notifyObservers() = 0;
private:

};
}//namespace dora_perception 

#endif	/* DORABASICPERCEPTION_H */

