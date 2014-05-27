/* 
 * File:   DoraAction.h
 * Author: allen
 *
 * Created on 2014年5月26日, 下午9:39
 */

#ifndef DORAACTION_H
#define	DORAACTION_H
namespace dora_action {
class DoraAction {
public:
    DoraAction();
    DoraAction(const DoraAction& orig);
    virtual ~DoraAction();
    void run();
private:

};
}//namespace dora_action 

#endif	/* DORAACTION_H */

