/* 
 * File:   DoraCommUnit.cpp
 * Author: allen
 * 
 * Created on 2014年5月27日, 上午1:40
 */

#include "CommUnit.h"
#include "roboard.h"
CommUnit::CommUnit() {
}

CommUnit::CommUnit(const CommUnit& orig) {
}

CommUnit::~CommUnit() {
}
static void recieveCmd(){
    
}
    static void sendCmd(unsigned char  * pstCmdBuf,int pstSize){
        com4_Send(pstCmdBuf, pstSize);
    }

