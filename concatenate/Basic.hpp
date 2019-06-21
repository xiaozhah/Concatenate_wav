//
//  Basic.hpp
//  concatenate
//
//  Created by 骁 on 2019/6/19.
//  Copyright © 2019 zhou xiao. All rights reserved.
//

#ifndef Basic_hpp
#define Basic_hpp

#include <stdio.h>
#include "memory.h"
#include "Common.hpp"
#include "setting.hpp"

#endif /* Basic_hpp */

int CutWinData(int nWavLen,short* pWavData,int nCenPos,int nWinLen,double* pWin,int nSTLen,double* pSTData);
int GetHannWindow(int nDataLen,int nHalfLen,double* pW);
