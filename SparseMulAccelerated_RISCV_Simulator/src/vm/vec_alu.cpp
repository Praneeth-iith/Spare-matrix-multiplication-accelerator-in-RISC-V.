/**
 * File Name: alu.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */

#include "vm/vec_alu.h"
#include <cfenv>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

using namespace vec_alu;

[[nodiscard]] std::array<int64_t, RegisterFile::VEC_DIM> alu::vsmul(std::array<int64_t, RegisterFile::VEC_DIM> v1,int64_t s){
     std::array<int64_t, RegisterFile::VEC_DIM> res;
        for(int i = 0 ; i < (int)RegisterFile::VEC_DIM ; i++){
            auto sa = v1[i];
            auto sb = s;
            res[i] = sa*sb;
        }
        return res;
}

[[nodiscard]] std::array<int64_t, RegisterFile::VEC_DIM> alu::vadd(std::array<int64_t, RegisterFile::VEC_DIM> v1, std::array<int64_t, RegisterFile::VEC_DIM> v2) {
    std::array<int64_t, RegisterFile::VEC_DIM> res;
            for(int i = 0 ; i < (int)RegisterFile::VEC_DIM ; i++){
                auto sa = v1[i];
                auto sb = v2[i];
                res[i] = sa+sb;
            }
            return res;
    }