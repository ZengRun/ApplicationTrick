/*
 ============================================================================
 Name        : Stub_Function.c
 Author      : sushuai
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "Stub_Function.h"
#define LOG_TAG "Stub_Function.c"
int rrrr = 1;

int Stub_DecryptDex(strpkname, apkpath, innertest)
	char *strpkname;char *apkpath; char* innertest;{

	LOGI("in Stub_DecryptDex");

	int ret;
	ret = initialize_WB_Key(strpkname);
	rrrr = Verify_Haze_Hmac(strpkname);
	if (rrrr != 0) {
		return rrrr;
	}

	ret = getSoLidPath_traAES(strpkname,innertest); //traditional AES
	return ret;
}


int Stub_DecryptDex_File(strpkname, apkpath)
	char *strpkname;char *apkpath;{

	LOGI("in Stub_DecryptDex_File");

	int ret;
	ret = initialize_WB_Key(strpkname);
	rrrr = Verify_Haze_Hmac(strpkname);
	if (rrrr != 0) {
		return rrrr;
	}

	ret = getSoLidPath_traAES_File(strpkname); //traditional AES
	return ret;
}

int Stub_RMMyReflector(strpkname, apkpath)
	char *strpkname;char *apkpath; {

	LOGI("in Stub_RMMyReflector");

	int ret = 1;
	return ret;
}

