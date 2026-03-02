/**
 * @file matrixmul_validation_unit.h
 * @brief a validation unit which handles errors caused by multiplying incompatible matrices
 * @author Praneeth Kodavti , https://github.com/Praneeth-iith
 */

#include "vm/matrixmul_validation_unit.h"


#include <iostream>
#include <string>
#include <vector>


bool Validator::validate(uint64_t addr1, uint64_t addr2){

    auto row1 = memory_controller_.ReadWord(addr1);
    addr1+=4;
    auto col1 = memory_controller_.ReadWord(addr1);
    auto row2 = memory_controller_.ReadWord(addr2);
    addr2+=4;
    auto col2 = memory_controller_.ReadWord(addr2);
    if(row1<=0 || col1<=0 || row2<=0 || col2<=0 || col1!=row2){
        return false;
    }

    return true;
}