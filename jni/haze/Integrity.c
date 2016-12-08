/*
 ============================================================================
 Name        : Integrity.c
 Author      : sushuai
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "inc/hmac_sha256.h"
#include "Reinforce.h"
#include "inc/base64_enc.h"
#include "inc/base64_dec.h"
/*#include "opensource/base64_dec.c"*/
#include "Key.h"
#include <sys/ptrace.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>
#include "inc/KeyIOApi.h"


#include "../libcmcc_haze.h"

#include "logcat.h"
#define LOG_TAG "hazeso_integrity.c"

int verify_Noah_Hmac(char *tmepOut, int length) {
	int ret, i, fd;
//	char newHmac[89];
	char oldHmac[88 + 1] = HMACVALUE;
	char filePath[128];
	unsigned char ikey[33];
	struct stat stat;
	char tmp[33] = { 0 };
	char result[45] = { 0 };
	char final[89];
	char *tmp1 = final;

	ret = getIKey(ikey);
	if (ret != 0) {
		return 1;
	}
	hmac_sha256(ikey, 32, tmepOut, length, tmp);
	tmp[32] = '\0';
	base64enc(result, tmp, 32);
	result[44] = '\0';

	LOGI("in integrity result is %s", result);

	for (i = 0; i < 44; i++) {
		sprintf(tmp1, "%02x", result[i]);
		tmp1 += 2;
	}
	final[88] = '\0';

	LOGI("in integrity final is %s", final);
//	LOGI("in integrity HMACVALUE is %s", HMACVALUE);
	LOGI("in integrity oldHmac is %s", oldHmac);
	LOGI("in integrity final is %s", final);

	for (i = 0; i < 88; i++) {
		if (final[i] != oldHmac[i]) {
			return 1;
		}
	}
	return 0;
}
//
void CharacterDecoder(char *pBuffer, char *rBuffer, int length, int randomNum) {
	int cTemp = 0;
	int iCount;
	if (randomNum % 2 == 0) {

		for (iCount = 0; iCount < length; iCount++) {
			cTemp = pBuffer[iCount];
			rBuffer[iCount] = cTemp + (-1) * (iCount % 2 ? 1 : -1) * randomNum;
		}
	} else {
		int iCount;
		for (iCount = 0; iCount < length; iCount++) {
			cTemp = pBuffer[iCount];
			rBuffer[iCount] = cTemp + (-1) * (iCount % 2 ? -1 : 1) * randomNum;
		}
	}
}

//int antiDebug() {
//	//*
//#ifdef _CMCC_OMP_SEC_DEBUG
//	LOGI("in antiDebug");
//#endif
//	int ret = 0;
//	char command[20] = { 0 };
//	char szBufGdb[64] = { 0 };
//	char buffer[256] = { 0 };
//	//  ps gdb
//	char BufGdb[6] = { 108, 119, 28, 107, 96, 102 };
//	CharacterDecoder(BufGdb, szBufGdb, 6, 4);
//
//#ifdef _CMCC_OMP_SEC_DEBUG
//	LOGI("in antiDebug szBufGdb is %s ", szBufGdb);
//#endif
//
//	strcpy(command, szBufGdb);
//	FILE *pp;
//
//	pp = popen(command, "r");
//	if (pp == NULL)
//		return -1;
//
//	while (fgets(buffer, sizeof(buffer), pp) != NULL) {
//		;
//	}
//#ifdef _CMCC_OMP_SEC_DEBUG
//	LOGI("in antiDebug buffer is %s ", buffer);
//#endif
//	char *tmp = strtok(buffer, " ");
//	char *pid = strtok(NULL, " ");
//
//#ifdef _CMCC_OMP_SEC_DEBUG
//	LOGI("in antiDebug tmp is %s ", tmp);
//	LOGI("in antiDebug pid is %s ", pid);
//#endif
//	if (pid != NULL && strcmp(pid, "PID") != NULL) {
//		ret = atoi(pid);
//		exit(21);
//	}
//
//	if (pp != NULL)
//		pclose(pp);
//	return ret;
//	//*/
//	//return 0;
//}
//
//int FindPID_GdbServer() {
//#ifdef _CMCC_OMP_SEC_DEBUG
//	LOGI("in FindPID_GdbServer");
//#endif
//	int RetPid = -1;
//	char command[20];
//	strcpy(command, "ps gdbserver");
//	FILE *pp;
//	char buffer[512] = { 0 };
//
//	pp = popen(command, "r");
//	if (pp == NULL) //ADD
//		return -1;
//
//	while (fgets(buffer, sizeof(buffer), pp) != NULL) {
//		;
//	}
////	fgets(buffer,sizeof(buffer),pp);
////	fgets(buffer,sizeof(buffer),pp);
//
//#ifdef _CMCC_OMP_SEC_DEBUG
//	LOGI("in FindPID_GdbServer buffer is %s ", buffer);
//#endif
//	char *tmp = strtok(buffer, " ");
//	char *pid = strtok(NULL, " ");
//
//#ifdef _CMCC_OMP_SEC_DEBUG
//	LOGI("in FindPID_GdbServer tmp is %s ", tmp);
//	LOGI("in FindPID_GdbServer pid is %s ", pid);
//#endif
//
//	if (pid != NULL && strcmp(pid, "PID") != NULL) {
//		RetPid = atoi(pid);
//	}
//
//	if (pp != NULL)
//		pclose(pp);
//
//	return RetPid;
//}
