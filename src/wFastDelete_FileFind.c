//===========================================================================
// 2019/09/15...FileFind / FF_TotalTime,FF_DirectoryCount,FF_FileCount,FF_FileSize
// 20190921...MakeTestDirectory, _wsystem (Hide Console ==> CreateProcessW)
//===========================================================================
#define UNICODE
//#include <tchar.h>
//#include <Shlwapi.h>
//===========================================================================
//#pragma comment(lib, "libshlwapi.a")
//\MinGW\lib\libshlwapi.a
//===========================================================================
#include "wFastDelete_FileFind.h"
//===========================================================================
FILE *FFFP;
int FFM,FFM_ShowWhileWorking,FFM_ShowWhileRemove;
DWORD FF_TotalTime, FF_DirectoryCount, FF_FileCount;
uint64_t FF_FileSize;
char CT_Locale[9999];

#if wFastDelete_RemoveTest==1
DWORD MTD_dct, MTD_fct;
uint64_t MTD_fsz;
#endif

//==============================================================================
//20190924
char *CommaString64(char *cs, int cn, uint64_t ui64)
{
  int sl,sp,ct;
  char ns[9999],rcs[9999];

  if(cs==NULL) return NULL;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat="
#pragma GCC diagnostic ignored "-Wformat-extra-args"

  sprintf(ns,"%llu",ui64);
#pragma GCC diagnostic pop
  sl=strlen(ns);
  if(cn<=0 || sl<=cn){ strcpy(cs,ns); return cs; }

  for(ct=0,sp=0;ct<sl;ct++){
    if(ct>0 && ct%cn==0){
      rcs[sp++]=',';
    }
    rcs[sp++]=ns[sl-ct-1];

//sprintf(cs,"%d/%d",ct,sl);
//MessageBoxA(NULL,cs,cs,MB_OK);

  }
  rcs[sp]=0;
//MessageBoxA(NULL,rcs,rcs,MB_OK);

  sl=strlen(rcs);
  for(ct=0;ct<sl;ct++){
    cs[ct]=rcs[sl-ct-1];
  }
  cs[sl]=0;
//MessageBoxA(NULL,cs,cs,MB_OK);

  return cs;
}
char *CommaString(char *cs, int cn, DWORD dw)
{
  return CommaString64(cs, cn, (uint64_t)dw);
}
//==============================================================================
//20190921...Wait until Remove
int cmdRemoveDirectory(wchar_t *dn)
{
  wchar_t wcs[9999];
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  DWORD ec;

  ZeroMemory(&si,sizeof(si)); si.cb = sizeof(si); ZeroMemory(&pi,sizeof(pi));
  swprintf(wcs,L"cmd.exe /c rd /s/q \"%ls\"",dn);
  CreateProcessW(NULL,wcs,NULL,NULL,FALSE,CREATE_NO_WINDOW,NULL,NULL,&si,&pi);
  while(1){ GetExitCodeProcess(pi.hProcess,&ec); if(ec!=STILL_ACTIVE) break; }
  CloseHandle(pi.hProcess);
  return ec;
}
//==============================================================================
/*
//20190916 wchar_t
// cache 때문에 안지워질경우가 있어서 한번 더 실행
//      cv=0; while(1){ if(cv>1 || SilentlyRemoveDirectory(WorkingDirectoryName)==0) break; cv++; Beep(2000,500); }
int SilentlyRemoveDirectory(wchar_t *dn)
{
  int ret;
  wchar_t tempdir[9999];

  memset(tempdir,0,sizeof(wchar_t)*9999); lstrcpyW(tempdir,dn);
  SHFILEOPSTRUCTW file_op = { NULL, FO_DELETE, tempdir, NULL, FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT, 0, 0, NULL };
  ret = SHFileOperationW(&file_op);
  return ret; // returns 0 on success
}
*/
//===========================================================================
// CallBack 함수 예제
// ds=0 최초진입
// ds=1 재진입
// ds=2 모든 탐색 완료
/*
출력은 어떻게 해도 상간없고 어떻게 보느냐에따라 문자가 깨질 수 있다
1...wprintf(L"%ls\n",dn);
2...wcstombs(mbstr,dn,9999); printf("%s\n",mbstr);
*/
void FFCB_Directory(wchar_t *dn, int ds)
{
  char mbstr[9999];

  if(ds==2 && FFM==FFM_ListDirectory){
    wcstombs(mbstr,dn,9999);
    fprintf(FFFP,"%s\n",mbstr);
  }else if(ds==2 && FFM==FFM_Remove){
//    RemoveDirectoryW(dn);
  }
}
//===========================================================================
void FFCB_File(wchar_t *fn, uint64_t fs)
{
  char mbstr[9999];//,ts[9999];
//  HANDLE fh;

  if(FFM==FFM_ListFile||FFM==FFM_ListFileSize){
    wcstombs(mbstr,fn,9999);
    if(FFM==FFM_ListFile) fprintf(FFFP,"%s\n",mbstr);
    if(FFM==FFM_ListFileSize){
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat="
#pragma GCC diagnostic ignored "-Wformat-extra-args"
      fprintf(FFFP,"%9llu %s\n",fs,mbstr);
//      fprintf(FFFP,"%s\n",ts);
#pragma GCC diagnostic pop
    }
  }else if(FFM==FFM_Remove){
/*
    SetFileAttributesW(fn,FILE_ATTRIBUTE_NORMAL);
    fh = CreateFileW(fn,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    if(fh!=INVALID_HANDLE_VALUE){
      if(SetFilePointer(fh,0,NULL,FILE_BEGIN)!=INVALID_SET_FILE_POINTER) SetEndOfFile(fh);
      CloseHandle(fh);
      DeleteFileW(fn);
    }
*/
  }
}
//===========================================================================
void _FileFind(wchar_t *dn, __FFCB_Directory _FFCB_Directory, __FFCB_File _FFCB_File, __FFCB_ShowWhileWorking _FFCB_ShowWhileWorking)
{
  wchar_t ffn[MAX_PATH];
  wchar_t tstr[MAX_PATH];
  HANDLE hff;
  WIN32_FIND_DATAW dff;
  uint64_t fs;

  if(_FFCB_Directory!=NULL) _FFCB_Directory(dn,0); // 최초진입

  swprintf(ffn,L"%ls\\*.*",dn);
  hff = FindFirstFileW(ffn, &dff);
  while(hff!=INVALID_HANDLE_VALUE){
    if(FFM!=FFM_InfoDirectory && FFM!=FFM_Remove && ((dff.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)){
      fs  = ((uint64_t)dff.nFileSizeLow);
      fs += (((uint64_t)dff.nFileSizeHigh)<<32);
      FF_FileSize += fs;
      FF_FileCount++;
      swprintf(tstr,L"%ls\\%ls",dn,dff.cFileName);
      if(_FFCB_File!=NULL) _FFCB_File(tstr,fs);
    }else if((dff.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY){
      if(!(
         (dff.cFileName[0]=='.' && dff.cFileName[1]==0) ||
         (dff.cFileName[0]=='.' &&  dff.cFileName[1]=='.' && dff.cFileName[2] == 0)
         )){
        swprintf(tstr,L"%ls\\%ls",dn,dff.cFileName);
        _FileFind(tstr,_FFCB_Directory,_FFCB_File,_FFCB_ShowWhileWorking);
        if(_FFCB_Directory!=NULL) _FFCB_Directory(dn,1); // 재진입
      }
    }
    if(FindNextFileW(hff,&dff)==FALSE) break;
  }
  FindClose(hff);
  FF_DirectoryCount++;
  if(_FFCB_Directory!=NULL) _FFCB_Directory(dn,2); // 모든 탐색 완료
  if(FFM==FFM_Remove) cmdRemoveDirectory(dn);
  if(_FFCB_ShowWhileWorking!=NULL) _FFCB_ShowWhileWorking(dn,FF_DirectoryCount);
}
//===========================================================================
void FileFind(wchar_t *dn, __FFCB_Directory _FFCB_Directory, __FFCB_File _FFCB_File, __FFCB_ShowWhileWorking _FFCB_ShowWhileWorking)
{
  DWORD tcs,tce;

  FF_TotalTime = 0UL; FF_DirectoryCount=0UL; FF_FileCount=0UL; FF_FileSize=0ULL;

  if(
    ((FFM==FFM_Remove && FFM_ShowWhileRemove==0)||
     (FFM==FFM_Info   && FFM_ShowWhileWorking==0))
    && _FFCB_ShowWhileWorking != NULL
  ) _FFCB_ShowWhileWorking(dn,0);

  tcs = GetTickCount();
  _FileFind(dn, _FFCB_Directory, _FFCB_File,
    ((FFM==FFM_Remove && FFM_ShowWhileRemove==1)||(FFM==FFM_Info && FFM_ShowWhileWorking==1) ? _FFCB_ShowWhileWorking : NULL));
  tce = GetTickCount();
  FF_TotalTime = (tcs<=tce?tce-tcs:tce);
}
//===========================================================================
int GetDirectoryCount(wchar_t *dn)
{
  FFM = FFM_InfoDirectory;
  FileFind(dn, NULL, NULL, NULL);
  return FF_DirectoryCount;
}
//===========================================================================
#if wFastDelete_RemoveTest==1
DWORD MTD_dct, MTD_fct;
uint64_t MTD_fsz;
//===========================================================================
int _MakeTestDirectory(int MaxDepth, int depth, int DirRate, int MaxFile, wchar_t *dn)
{
  int ct,fs,mfct,ec;
  wchar_t fn[9999];
  HANDLE fh;

  if(CreateDirectoryW(dn,NULL)==FALSE) return -1;

  ec=0;
  mfct=rand()%MaxFile;
  for(ct=0;ct<(depth<MaxDepth ? 10 : mfct );ct++){
    fs=rand()%100;
    if(depth<MaxDepth && fs<DirRate){
      swprintf(fn,L"%ls\\dir%d",dn,ct);
      if(_MakeTestDirectory(MaxDepth,depth+1,DirRate,MaxFile,fn)<0) return -2;
    }else{
      if(rand()%100<50) fs=(rand()*rand())%1000000; else fs=rand()%1000;
      swprintf(fn,L"%ls\\file%d.bin",dn,ct);
      fh = CreateFileW(fn,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
      if(SetFilePointer(fh,fs,NULL,FILE_BEGIN)==INVALID_SET_FILE_POINTER||SetEndOfFile(fh)==FALSE){ Beep(1000,500);  ec=1; }
      CloseHandle(fh);
      if(ec==1) break;
      MTD_fct++;
      MTD_fsz+=((uint64_t)fs);
    }
  }
  MTD_dct++;

  return 1;
}
int MakeTestDirectory(int RandSeed, int MaxDepth, int DirRate, int MaxFile, wchar_t *dn)
{
  MTD_dct=0UL; MTD_fct=0UL;
  MTD_fsz=0ULL;

  srand(RandSeed);
  return _MakeTestDirectory(MaxDepth,0,DirRate,MaxFile,dn);
}
#endif
//===========================================================================
