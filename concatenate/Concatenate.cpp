//
//  Concatenate.cpp
//  concatenate
//
//  Created by 骁 on 2019/6/19.
//  Copyright © 2019 zhou xiao. All rights reserved.
//

#include "Concatenate.hpp"
#include "Basic.hpp"

// 直接拼接
void Concatenate(char* audio_dir, char* frame_file_path, char* out_audio)
{
    printf("Concatenating ...\n");
    int nSegLen, nOLPoint, nPrevSegLen, nNextSegLen, nPrefixLen, i, j;
    nSegLen = FRAME_SHIFT*SAMPLE_RATE / 1000;
    nOLPoint = int(double(SAMPLE_RATE) / fLowestF0 + 0.5);
    nOLPoint = nOLPoint / 4 * 4;
    nPrefixLen = std::max(0, nOLPoint / 2 - nSegLen);
    nPrevSegLen = nPrefixLen + nSegLen + nOLPoint / 2;
    assert(nPrevSegLen >> 1 << 1 == nPrevSegLen);
    nNextSegLen = nOLPoint + nSegLen + nOLPoint + nPrefixLen;
    assert(nNextSegLen >> 1 << 1 == nNextSegLen);
    
    char szTmpStr[MAX_STR_LEN];
    int nFrmNum = 0, *pSenNo, *pFrmNo, *pUnitNo;
    float *pPowNorm;
    
    assert(access(frame_file_path, 0) == 0 && "frame file not found!\n");
    FILE* fpIn = fopen(frame_file_path, "rt");
    while (fgets(szTmpStr, MAX_STR_LEN, fpIn))
        nFrmNum++;
    
    fseek(fpIn, 0, SEEK_SET);
    pSenNo = new int[nFrmNum];
    pFrmNo = new int[nFrmNum];
    pUnitNo = new int[nFrmNum];
    pPowNorm = new float[nFrmNum];
    for (i = 0; i < nFrmNum; i++)   // read unit list file
    {
        fgets(szTmpStr, MAX_STR_LEN, fpIn);
        sscanf(szTmpStr, " %d %d %f ", pSenNo + i, pFrmNo + i, pPowNorm + i);
    }
    fclose(fpIn);
    
    int nSenPoint;
    short* pSenWav = new short[MAX_SEN_POINT];
    double* pSynWav = new double[nPrefixLen + nFrmNum*nSegLen + nOLPoint];
    short* pShortWav = new short[nPrefixLen + nFrmNum*nSegLen + nOLPoint];
    double* pPrevSeg = new double[nPrevSegLen];
    double* pNextSeg = new double[nNextSegLen];
    int nBestShift;
    double dBestCorr, dCurCorr;
    
    double* pOneWin = new double[nNextSegLen];
    for (i = 0; i < nNextSegLen; i++)
        pOneWin[i] = 1;
    
    char szWavFile[MAX_PATH_LEN];
    sprintf(szWavFile, "%s/%08d.wav", audio_dir, pSenNo[0]);
    
    nSenPoint = ReadWavFile(szWavFile, pSenWav);
    assert(nSenPoint <= MAX_SEN_POINT);
    //存储语音切分得到的数据至pPrevSeg中，数据长度是nPrevSegLen（这里是320）
    //也就是说pPrevSeg存储的是当前帧的起点往前80点，往后240点的结果
    CutWinData(nSenPoint, pSenWav, pFrmNo[0] * nSegLen - nPrefixLen + nPrevSegLen / 2,
               nPrevSegLen, pOneWin, nPrevSegLen, pPrevSeg);
    
    // power normalization
    //先做能量规整 再拼接 不然肯定拼接处就会有咔哒声
    for (j = 0; j < nPrevSegLen; j++)
        pPrevSeg[j] *= pPowNorm[0];
    
    memset(pSynWav, 0, sizeof(double)*(nFrmNum*nSegLen + nOLPoint + nPrefixLen));
    memcpy(pSynWav + nPrefixLen, pPrevSeg + nPrefixLen, sizeof(double)*nSegLen);
    
    double* pAllCorr = new double[nOLPoint];
    for (i = 1; i < nFrmNum; i++)
    {
        sprintf(szWavFile, "%s/%08d.wav", audio_dir, pSenNo[i]);
        
        nSenPoint = ReadWavFile(szWavFile, pSenWav);
        assert(nSenPoint <= MAX_SEN_POINT);
        CutWinData(nSenPoint, pSenWav, pFrmNo[i] * nSegLen - nOLPoint + nNextSegLen / 2,
                   nNextSegLen, pOneWin, nNextSegLen, pNextSeg);
        
        // power normalization
        for (j = 0; j < nNextSegLen; j++)
            pNextSeg[j] *= pPowNorm[i];
        
        dBestCorr = -1;
        nBestShift = -1;
        for (j = 0; j < nOLPoint; j++)   // shift to find optimum concatenation position
        {
            dCurCorr = GetCorrCoef(nOLPoint, pPrevSeg + nPrefixLen + nSegLen - nOLPoint / 2, pNextSeg + j);
            pAllCorr[j] = dCurCorr;
            if (dCurCorr > dBestCorr)
            {
                dBestCorr = dCurCorr;
                nBestShift = j;
            }
        }
        // generate waveform by overlap
        if (dBestCorr < CORR_THRES)
            nBestShift = int(double(rand()) / double(RAND_MAX)*double(nOLPoint));
        
        OverlapWaveForm((nSegLen + nPrefixLen) * 2, pSynWav + (i - 1)*nSegLen, nPrevSegLen, nOLPoint, pPrevSeg, pNextSeg + nBestShift);
        if (i < nFrmNum - 1)
            memcpy(pPrevSeg, pSynWav + i*nSegLen, sizeof(double)*nPrevSegLen);
    }
    
    for (i = 0; i < nFrmNum*nSegLen + nPrefixLen; i++)
        pShortWav[i] = short(pSynWav[i]);
    WriteWavFile(out_audio, nFrmNum*nSegLen, pShortWav + nPrefixLen, SAMPLE_RATE);
    
    delete[] pSenNo;
    delete[] pFrmNo;
    delete[] pPowNorm;
    delete[] pSenWav;
    delete[] pSynWav;
    delete[] pPrevSeg;
    delete[] pNextSeg;
    delete[] pAllCorr;
    delete[] pOneWin;
    
    return;
}

double GetCorrCoef(int nLen, double* pA, double* pB)
{
    int i;
    double dCorr, dEA, dEB;
    for (i = 0, dCorr = dEA = dEB = 0; i < nLen; i++)
    {
        dCorr += pA[i] * pB[i];
        dEA += pA[i] * pA[i];
        dEB += pB[i] * pB[i];
    }
    dCorr = dCorr / sqrt(dEA*dEB);
    
    return dCorr;
}

// nDataLen = 2*(nSegLen+nPrefixLen)
// nDataLen = 2*nLRLen-nOLLen
void OverlapWaveForm(int nDataLen, double* pData, int nLRLen, int nOLLen, double* pLeft, double* pRight)
{
    int i;
    double* pHanW = new double[nOLLen * 2];
    GetHannWindow(nOLLen * 2, nOLLen, pHanW);
    for (i = 0; i < nLRLen - nOLLen; i++)
        pData[i] = pLeft[i];
    for (; i < nLRLen; i++)    // overlap portion
        pData[i] = pLeft[i] * pHanW[nOLLen + i - nLRLen + nOLLen] + pRight[i - nLRLen + nOLLen] * (1 - pHanW[nOLLen + i + nOLLen - nLRLen]);
    for (; i < nDataLen + nOLLen / 2; i++)
        pData[i] = pRight[i - nLRLen + nOLLen];
    
    delete[] pHanW;
    
    return;
}
