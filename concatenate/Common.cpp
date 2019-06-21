//
//  Common.cpp
//  concatenate
//
//  Created by 骁 on 2019/6/19.
//  Copyright © 2019 zhou xiao. All rights reserved.
//

#include "Common.hpp"

int ReadWavFile(char* szWavFile, short* pData)
{
    assert(access(szWavFile, 0) == 0 && "Wav file not found!\n");
    FILE  *fpWav = fopen(szWavFile, "rb");
    int nLen;
    
    fseek(fpWav, 0, SEEK_END);
    nLen = (int)ftell(fpWav);
    nLen = (nLen - 44) / 2;
    fseek(fpWav, 44, SEEK_SET);
    fread(pData, nLen, sizeof(short), fpWav);
    fclose(fpWav);
    
    return  nLen;
}

void WriteWavFile(char *szOptFile, int nDataLen, short *pData, int nSampleRate)
{
    char riff[5] = "RIFF";
    char riffsize[4] = "";
    char wave[5] = "WAVE";
    char fmt[5] = "fmt ";
    char fmtsize[4] = "";
    unsigned short format = 1;
    unsigned short channel = 1;
    char sampling[4] = "";
    char bps[4] = "";
    char block = 2;
    char dummy1 = 0;
    char bit = 16;
    char dummy2 = 0;
    char data[5] = "data";
    char datasize[4] = "";
    
    unsigned long tmp;
    FILE *fp;
    
    fmtsize[3] = 0x00;    fmtsize[2] = 0x00;
    fmtsize[1] = 0x00;    fmtsize[0] = 0x10;
    
    tmp = (unsigned long)(nDataLen * 2);
    datasize[3] = (char)(tmp >> 24);    datasize[2] = (char)(tmp >> 16);
    datasize[1] = (char)(tmp >> 8);    datasize[0] = (char)tmp;
    
    tmp += (unsigned long)36;
    riffsize[3] = (char)(tmp >> 24);    riffsize[2] = (char)(tmp >> 16);
    riffsize[1] = (char)(tmp >> 8);    riffsize[0] = (char)tmp;
    
    tmp = (unsigned long)nSampleRate;
    sampling[3] = (char)(tmp >> 24);    sampling[2] = (char)(tmp >> 16);
    sampling[1] = (char)(tmp >> 8);    sampling[0] = (char)tmp;
    
    tmp += tmp;
    bps[3] = (char)(tmp >> 24);    bps[2] = (char)(tmp >> 16);
    bps[1] = (char)(tmp >> 8);    bps[0] = (char)tmp;
    
    fp = fopen(szOptFile, "wb");
    
    /* write header */
    fwrite(riff, sizeof(char), 4, fp);
    fwrite(riffsize, sizeof(char), 4, fp);
    fwrite(wave, sizeof(char), 4, fp);
    fwrite(fmt, sizeof(char), 4, fp);
    fwrite(fmtsize, sizeof(char), 4, fp);
    fwrite(&format, sizeof(unsigned short), 1, fp);
    fwrite(&channel, sizeof(unsigned short), 1, fp);
    fwrite(sampling, sizeof(char), 4, fp);
    fwrite(bps, sizeof(char), 4, fp);
    fwrite(&block, sizeof(char), 1, fp);
    fwrite(&dummy1, sizeof(char), 1, fp);
    fwrite(&bit, sizeof(char), 1, fp);
    fwrite(&dummy2, sizeof(char), 1, fp);
    fwrite(data, sizeof(char), 4, fp);
    fwrite(datasize, sizeof(char), 4, fp);
    
    /* write data */
    int i;
    short nTmp;
    for (i = 0; i < nDataLen; i++)
    {
        nTmp = (short)pData[i];
        fwrite(&nTmp, sizeof(short), 1, fp);
    }
    
    /* close file */
    fclose(fp);
    
    return;
}
