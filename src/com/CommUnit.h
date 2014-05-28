/* 
 * File:   DoraCommUnit.h
 * Author: allen
 *
 * Created on 2014年5月27日, 上午1:40
 */

#ifndef DORACOMMUNIT_H
#define	DORACOMMUNIT_H
class CommUnit {
public:
    CommUnit();
    CommUnit(const CommUnit& orig);
    virtual ~CommUnit();
    static unsigned char recieveReturnValue();
    static bool sendCmd(unsigned char * pstCmdBuf,int pstSize);
    
private:

};

#endif	/* DORACOMMUNIT_H */

