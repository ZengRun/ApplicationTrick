#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dlfcn.h"
#include <unistd.h>
#include "Key.h"
#include <stdio.h>
#include <sys/ptrace.h>
#include <dlfcn.h>
#include "../inc/KeyIOApi.h"
#include "../inc/rijndael.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Integrity.h"
#include "../logcat.h"
#include "time.h"
#include "../libcmcc_haze.h"
//#include "CallDecrypt.h"
//void* getSoLid();
//void* getSoLidPath(char* path);
void* SoLid = NULL;
