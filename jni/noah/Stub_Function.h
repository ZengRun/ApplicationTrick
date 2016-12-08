#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "dlfcn.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <memory.h>
#include "Key.h"
#include "../logcat.h"
#include "Integrity.h"
#include "Alg.h"
#include "../libcmcc_noah.h"
int Ver_Jar_Mark = 1;
int Ver1_Mark = 1;
int Ver2_Mark = 1;
JNIEXPORT int Stub_DecryptDex(char *strpkname, char *apkpath ,char *innertest);
JNIEXPORT int Stub_DecryptDex_File(char *strpkname, char *apkpath);
JNIEXPORT int Stub_RMMyReflector(char *strpkname, char *apkpath);

