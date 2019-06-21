//
//  Concatenate.hpp
//  concatenate
//
//  Created by 骁 on 2019/6/19.
//  Copyright © 2019 zhou xiao. All rights reserved.
//

#ifndef Concatenate_hpp
#define Concatenate_hpp

#include <stdio.h>
#include "algorithm"
#include "setting.hpp"

#endif /* Concatenate_hpp */

void Concatenate(char* audio_dir, char* frame_file_path, char* synthesized_audio_file_path);
double GetCorrCoef(int nLen, double* pA, double* pB);
void OverlapWaveForm(int nDataLen, double* pData, int nLRLen, int nOLLen, double* pLeft, double* pRight);

