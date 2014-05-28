/* 
 * File:   DoraCommUnit.cpp
 * Author: allen
 * 
 * Created on 2014年5月27日, 上午1:40
 */

#include "CommUnit.h"
#include "roboard.h"
#include<iostream>

CommUnit::CommUnit() {
}

CommUnit::CommUnit(const CommUnit& orig) {
}

CommUnit::~CommUnit() {
}

 unsigned char CommUnit::recieveReturnValue() {
    return (unsigned char) com3_Read();
}

 bool CommUnit::sendCmd(unsigned char * pstCmdBuf, int pstSize) {
    if (NULL == pstCmdBuf) {
        std::cerr << "send buff is NULL" << std::endl;
        return false;
    }
    return com3_Send(pstCmdBuf, pstSize);
}

