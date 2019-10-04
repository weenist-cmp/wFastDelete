#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = %cd%

CC = gcc.exe
CXX = g++.exe
AR = ar.exe
LD = g++.exe
WINDRES = windres.exe

INC = 
CFLAGS = -Wall
RESINC = 
LIBDIR = 
LIB = \\MinGW\\lib\\libcomctl32.a \\MinGW\\lib\\libshlwapi.a \\MinGW\\lib\\libcomdlg32.a
LDFLAGS = -static-libstdc++ -static-libgcc -static

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O2
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB) 
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj
DEP_RELEASE = 
OUT_RELEASE = ..\\wFastDelete.exe

OBJ_RELEASE = $(OBJDIR_RELEASE)\\wFastDelete_FileFind.o $(OBJDIR_RELEASE)\\wFastDelete_main.o $(OBJDIR_RELEASE)\\wFastDelete_module.o $(OBJDIR_RELEASE)\\wFastDelete_private.o

all: release

clean: clean_release

before_release: 
	cmd /c if not exist .. md ..
	cmd /c if not exist $(OBJDIR_RELEASE) md $(OBJDIR_RELEASE)

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) -mwindows $(LIB_RELEASE)

$(OBJDIR_RELEASE)\\wFastDelete_FileFind.o: wFastDelete_FileFind.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c wFastDelete_FileFind.c -o $(OBJDIR_RELEASE)\\wFastDelete_FileFind.o

$(OBJDIR_RELEASE)\\wFastDelete_main.o: wFastDelete_main.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c wFastDelete_main.c -o $(OBJDIR_RELEASE)\\wFastDelete_main.o

$(OBJDIR_RELEASE)\\wFastDelete_module.o: wFastDelete_module.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c wFastDelete_module.c -o $(OBJDIR_RELEASE)\\wFastDelete_module.o

$(OBJDIR_RELEASE)\\wFastDelete_private.o: wFastDelete_private.rc
	$(WINDRES) -i wFastDelete_private.rc -J rc -o $(OBJDIR_RELEASE)\\wFastDelete_private.o -O coff $(INC_RELEASE)

clean_release: 
	cmd /c del /f $(OBJ_RELEASE) $(OUT_RELEASE)
	cmd /c rd ..
	cmd /c rd $(OBJDIR_RELEASE)

.PHONY: before_release after_release clean_release
