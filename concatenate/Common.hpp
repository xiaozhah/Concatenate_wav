//
//  Common.hpp
//  concatenate
//
//  Created by 骁 on 2019/6/19.
//  Copyright © 2019 zhou xiao. All rights reserved.
//

#ifndef Common_hpp
#define Common_hpp

#define   NO_ERR          0
#define   PI              3.14159265358979323846

#include <stdio.h>
#include "assert.h"
#include "unistd.h"
#include "math.h"

#endif /* Common_hpp */

int ReadWavFile(char* szWavFile, short* pData);
void WriteWavFile(char *szOptFile, int nDataLen, short *pData, int nSampleRate);
