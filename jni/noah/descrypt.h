#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include  "dlfcn.h" 
#include <sys/ptrace.h>
#include <dlfcn.h>
#include"Key.h"
#include "inc/KeyIOApi.h"
#include "inc/rijndael.h"
#include "Integrity.h"
#include "../logcat.h"

void* getSoLid();
int descryptFile_traAES(char* data, int len, char* pwd, char* outFile);
int descryptdata_traAES(char* data, int len, char* pwd, char* outdata);
int getSoLidPath_traAES(char* path,char *innertest);
int removeMyReflector(char* path);
