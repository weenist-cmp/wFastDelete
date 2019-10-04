//===========================================================================
#ifndef wFastDelete_FileFind_H
#define wFastDelete_FileFind_H
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
//===========================================================================
#define FFM_Remove         1
#define FFM_Info           2

#define FFM_InfoDirectory  21

#define FFM_ListDirectory  31
#define FFM_ListFile       32
#define FFM_ListFileSize   33

#if wFastDelete_RemoveTest==1
extern DWORD MTD_dct, MTD_fct;
extern uint64_t MTD_fsz;
#endif


extern int FFM,FFM_ShowWhileWorking,FFM_ShowWhileRemove; // FFM=RunMode
extern FILE *FFFP;
extern DWORD FF_TotalTime, FF_DirectoryCount, FF_FileCount;
extern uint64_t FF_FileSize;
typedef void (*__FFCB_Directory)(wchar_t *dn, int ds);
typedef void (*__FFCB_File)(wchar_t *fn, uint64_t fs);
typedef void (*__FFCB_ShowWhileWorking)(wchar_t *dn, DWORD cdct);
//===========================================================================
#ifdef __cplusplus
extern "C" {
#endif

char *CommaString(char *cs, int cn, DWORD dw);
char *CommaString64(char *cs, int cn, uint64_t ui64);

//int SilentlyRemoveDirectory(wchar_t *dn);
int cmdRemoveDirectory(wchar_t *dn);

void FFCB_Directory(wchar_t *dn, int ds);
void FFCB_File(wchar_t *fn, uint64_t fs);

void FileFind(wchar_t *dn, __FFCB_Directory _FFCB_Directory, __FFCB_File _FFCB_File, __FFCB_ShowWhileWorking _FFCB_ShowWhileWorking);
int GetDirectoryCount(wchar_t *dn);


#if wFastDelete_RemoveTest==1
int MakeTestDirectory(int RandSeed, int MaxDepth, int DirRate, int MaxFile, wchar_t *dn);
#endif

#ifdef __cplusplus
}
#endif
//===========================================================================
#endif
//===========================================================================
