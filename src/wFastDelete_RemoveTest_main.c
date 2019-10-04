//===========================================================================
// 20190921...RemoveTest
//===========================================================================
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <Shlwapi.h>

#include "wFastDelete_FileFind.h"
//===========================================================================
FILE *fp;
DWORD TotalDirectoryCount;
//===========================================================================
void MessageOut(char *str)
{
  fprintf(stderr,str);
  fprintf(fp,str);
}
//===========================================================================
void FFCB_ShowWhileWorking(wchar_t *dn, DWORD cdct)
{
  char mbs[999],mbdn[999];

  wcstombs(mbdn,dn,999);
  sprintf(mbs,"%lu/%lu %s\n",cdct,TotalDirectoryCount,mbdn);
  MessageOut(mbs);
}
//===========================================================================
int main()
{
  int wac,tv;
  int RandSeed, MaxDepth, DirRate, MaxFile;
  int rc,wm;
  DWORD wtcs,wtce,wtms;
  LPWSTR *wcs, wcl;
  char mbs[999],rfn[999],cs1[999],cs2[999],cs3[999],cs4[999];
  wchar_t WorkingDirectoryName[999],wstr[999],wcs1[999],wcs2[999],wcs3[999],wcs4[999];

  WorkingDirectoryName[0]=0;

  wcl = GetCommandLineW(); wcs = CommandLineToArgvW(wcl,&wac);
  if(wac>6){
    wcstombs(mbs,wcs[1],999); tv = atoi(mbs); wm = tv;
    wcstombs(mbs,wcs[2],999); tv = atoi(mbs); RandSeed = tv;
    wcstombs(mbs,wcs[3],999); tv = atoi(mbs); MaxDepth = tv;
    wcstombs(mbs,wcs[4],999); tv = atoi(mbs); DirRate = tv;
    wcstombs(mbs,wcs[5],999); tv = atoi(mbs); MaxFile = tv;
    lstrcpyW(WorkingDirectoryName,wcs[6]);
    LocalFree(wcs);

    sprintf(rfn,"wFastDelete_RemoveTest_%d_%d_%d_%d.txt",RandSeed, MaxDepth, DirRate, MaxFile);
    fp=fopen(rfn,"wt");
    if(fp==NULL){
      sprintf(mbs,"Output File Create Error!!\n%s\n",rfn);
      fprintf(stderr,mbs);
      Beep(2000,500);
      getch();
      return -1;
    }

    sprintf(mbs,"wFastDelete_RemoveTest\n%d %d %d %d\n\n",RandSeed, MaxDepth, DirRate, MaxFile); MessageOut(mbs);
    swprintf(wstr,L"%ls\n\n",WorkingDirectoryName); wcstombs(mbs,wstr,999); MessageOut(mbs);

//PathFileExists
    if(PathFileExistsW(WorkingDirectoryName)==TRUE){
      swprintf(wstr,L"Path Exist!!\n");
      wcstombs(mbs,wstr,999); MessageOut(mbs);
      fclose(fp);
      Beep(2000,500);
      getch();
      return -2;
    };

//CreateDirectory
    if(CreateDirectoryW(WorkingDirectoryName,NULL)==FALSE){
      swprintf(wstr,L"CreateDirectory Error!!\n");
      wcstombs(mbs,wstr,999); MessageOut(mbs);
      fclose(fp);
      Beep(2000,500);
      getch();
      return -3;
    }
    RemoveDirectoryW(WorkingDirectoryName);

//MT
    MessageOut("[RT_MakeTemp]\n");
    wtcs = GetTickCount();
    if(MakeTestDirectory(RandSeed, MaxDepth, DirRate, MaxFile, WorkingDirectoryName)<0){
      swprintf(wstr,L"MakeTemp Error!!\n");
      wcstombs(mbs,wstr,999); MessageOut(mbs);
      fclose(fp);
      Beep(2000,500);
      getch();
      return -3;
    }
    wtce = GetTickCount(); wtms = (wtcs<=wtce?wtce-wtcs:wtce);
    CommaString(cs1,3,MTD_dct); mbstowcs(wcs1,cs1,999);
    CommaString(cs2,3,MTD_fct); mbstowcs(wcs2,cs2,999);
    CommaString64(cs3,3,MTD_fsz); mbstowcs(wcs3,cs3,999);
    CommaString(cs4,3,wtms); mbstowcs(wcs4,cs4,999);
    swprintf(wstr,L"MTD_dct:%s  MTD_fct:%s  MTD_fsz:%s\nTime:%s\n\n",wcs1,wcs2,wcs3,wcs4);
    wcstombs(mbs,wstr,999); MessageOut(mbs);

//FF
    if(wm==1||wm==2){
      MessageOut("[RT_FileFind, FFM_ShowWhileWorking=0]\n");
      FFM = FFM_Info; FFM_ShowWhileWorking = 0;
      FileFind(WorkingDirectoryName,NULL,NULL,FFCB_ShowWhileWorking);
      CommaString(cs1,3,FF_DirectoryCount); mbstowcs(wcs1,cs1,999);
      CommaString(cs2,3,FF_FileCount); mbstowcs(wcs2,cs2,999);
      CommaString64(cs3,3,FF_FileSize); mbstowcs(wcs3,cs3,999);
      CommaString(cs4,3,FF_TotalTime); mbstowcs(wcs4,cs4,999);
      swprintf(wstr,L"dct:%s  fct:%s  fsz:%s\nTime:%s\n\n",wcs1,wcs2,wcs3,wcs4);
      wcstombs(mbs,wstr,999); MessageOut(mbs);

      MessageOut("[RT_FileFind, FFM_ShowWhileWorking=1]\n");
      TotalDirectoryCount = GetDirectoryCount(WorkingDirectoryName);
      FFM = FFM_Info; FFM_ShowWhileWorking = 1;
      FileFind(WorkingDirectoryName,NULL,NULL,FFCB_ShowWhileWorking);
      CommaString(cs1,3,FF_DirectoryCount); mbstowcs(wcs1,cs1,999);
      CommaString(cs2,3,FF_FileCount); mbstowcs(wcs2,cs2,999);
      CommaString64(cs3,3,FF_FileSize); mbstowcs(wcs3,cs3,999);
      CommaString(cs4,3,FF_TotalTime); mbstowcs(wcs4,cs4,999);
      swprintf(wstr,L"dct:%s  fct:%s  fsz:%s\nTime:%s\n\n",wcs1,wcs2,wcs3,wcs4);
      wcstombs(mbs,wstr,999); MessageOut(mbs);
    }

//CRD
    if(wm==2||wm==3){
      MessageOut("[RT_RemoveDirectory, ShowWhileRemove=0]\n");
      wtcs = GetTickCount();
      FFM = FFM_Remove; FFM_ShowWhileRemove=0;
      rc = cmdRemoveDirectory(WorkingDirectoryName);
      if(rc!=0) Beep(2000,500);
      wtce = GetTickCount(); wtms = (wtcs<=wtce?wtce-wtcs:wtce);
      CommaString(cs4,3,wtms); mbstowcs(wcs4,cs4,999);
      swprintf(wstr,L"Time:%s (%ls)\n\n",wcs4,(rc==0 ? L"Succ" : L"Fail"));
      wcstombs(mbs,wstr,999); MessageOut(mbs);
    }

//MT
    if(wm==2||wm==3){
      MessageOut("[RT_MakeTemp]\n");
      wtcs = GetTickCount();
      if(MakeTestDirectory(RandSeed, MaxDepth, DirRate, MaxFile, WorkingDirectoryName)<0){
        swprintf(wstr,L"MakeTemp Error!!\n");
        wcstombs(mbs,wstr,999); MessageOut(mbs);
        fclose(fp);
        Beep(2000,500);
        getch();
        return -4;
      }
      wtce = GetTickCount(); wtms = (wtcs<=wtce?wtce-wtcs:wtce);
      CommaString(cs1,3,MTD_dct); mbstowcs(wcs1,cs1,999);
      CommaString(cs2,3,MTD_fct); mbstowcs(wcs2,cs2,999);
      CommaString64(cs3,3,MTD_fsz); mbstowcs(wcs3,cs3,999);
      CommaString(cs4,3,wtms); mbstowcs(wcs4,cs4,999);
      swprintf(wstr,L"MTD_dct:%s  MTD_fct:%s  MTD_fsz:%s\nTime:%s\n\n",wcs1,wcs2,wcs3,wcs4);
      wcstombs(mbs,wstr,999); MessageOut(mbs);
    }

//FRD
    if(wm==2||wm==3){
      MessageOut("[RT_RemoveDirectory, ShowWhileRemove=1]\n");
      wtcs = GetTickCount();
      TotalDirectoryCount = GetDirectoryCount(WorkingDirectoryName);
      FFM = FFM_Remove; FFM_ShowWhileRemove=1;
      FileFind(WorkingDirectoryName,NULL,NULL,FFCB_ShowWhileWorking);
      wtce = GetTickCount(); wtms = (wtcs<=wtce?wtce-wtcs:wtce);
      CommaString(cs4,3,wtms); mbstowcs(wcs4,cs4,999);
      swprintf(wstr,L"Time:%s (%ls)\n\n",wcs4,(rc==0 ? L"Succ" : L"Fail"));
      wcstombs(mbs,wstr,999); MessageOut(mbs);
    }

    fclose(fp);
  }else{
    swprintf(wstr,L"Usage: wFastDelete_RemoveTest  Mode RandSeed MaxDepth DirRate MaxFile WorkingDirectoryName\nMode= 0 MakeTemp  1 MakeTemp/FileFind  2 MakeTemp/FileFind/RemoveDirectory  3 MakeTemp/RemoveDirectory\n\n");
    wcstombs(mbs,wstr,999); fprintf(stderr,mbs);
    Beep(2000,500);
    getch();
  }
  return 0;
}
//===========================================================================
