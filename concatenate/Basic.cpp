//
//  Basic.cpp
//  concatenate
//
//  Created by 骁 on 2019/6/19.
//  Copyright © 2019 zhou xiao. All rights reserved.
//

#include "Basic.hpp"

int CutWinData(int nWavLen,short* pWavData,int nCenPos,int nWinLen,double* pWin,int nSTLen,double* pSTData)
{
    int nHalfWin = nWinLen/2;
    memset(pSTData,0,sizeof(double)*nSTLen);
    
    int i;
    for(i=-nHalfWin;i<nHalfWin;i++)
    {
        if(nCenPos+i>=0 && nCenPos+i<nWavLen)
            pSTData[nSTLen/2+i] = double(pWavData[nCenPos+i])*pWin[i+nHalfWin];
    }
    return nCenPos;
}

int GetHannWindow(int nDataLen,int nHalfLen,double* pW)
{
    memset(pW,0,sizeof(double)*nDataLen);
    
    int i;
    for(i=0;i<nHalfLen*2;i++)
        pW[nDataLen/2-nHalfLen+i] = 0.5*(1-cos(PI*double(i)/double(nHalfLen)));
    
    return NO_ERR;
}
