/*
 ============================================================================
 Name        : Integrity.c
 Author      : sushuai
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "Integrity.h"

#define LENGTH_HMAC	88

#define LOG_TAG "core_integrity.c"

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
//original component

int calHmac(char *filePath, char *result) {
	int fd, length, ret;
	struct stat stat;
	int klen = 32;
	char tmp[33] = { 0 };
	char key[33] = { 0 };
	if ((fd = open(filePath, O_RDONLY)) < 0)
		return -2;
	if ((fstat(fd, &stat)) < 0)
		return -3;
	length = stat.st_size;
	char *out = calloc(length, sizeof(char));
	ret = ompReadFile(filePath, length, out);
	if (ret != 0) {
		free(out);
		return -4;
	}
	getIKey(key);
	hmac_sha256(key, klen, out, length, tmp);
	base64enc(result, tmp, 32);
	free(out);
	return 0;
}

int calHmac_1ST(char *filePath, char *result) {

	int ret=1;
	char key1[33]="";
	char key[33]="";
	int fd,length;
	int i;
	struct stat stat;
	int klen = 32;
	char tmp[33]="";

	ret = getIKey_1ST(key1);
	#ifdef _CMCC_OMP_SEC_DEBUG
	LOGI("in calHmac_1ST key1 is %s",key1);
	#endif
	if(ret!=0){
	#ifdef _CMCC_OMP_SEC_DEBUG
	LOGI("ret!=0");
	#endif
	}
	descryptdata_wb(key1,32,key);
	#ifdef _CMCC_OMP_SEC_DEBUG
	LOGI("in calHmac_1ST key is %s",key);
	LOGI("in calHmac_1ST  filePath is %s",filePath);
	#endif


	if((fd = open(filePath, O_RDONLY)) < 0){
	  #ifdef _CMCC_OMP_SEC_DEBUG
	  LOGI("open filePath fail");
	  #endif
		return -2;
	}
	if((fstat(fd,&stat)) < 0){
		#ifdef _CMCC_OMP_SEC_DEBUG
	  LOGI("fstat(fd,&stat) error");
	  #endif
		return -3;
	}
	length = stat.st_size;
	close(fd);
	char *out = malloc(sizeof(char) * length);
	ret = ompReadFile(filePath, length, out);
	if(ret != 0){
		free(out);
		return -4;
	}
	hmac_sha256(key,klen,out,length,tmp);
	base64enc(result,tmp,32);
	free(out);
	return 0;
}

int calHmac_2ST(char *filePath, char *result) {

	int ret=1;
	char key1[33]="";
	char key[33]="";
	int fd,length;
	int i;
	struct stat stat;
	int klen = 32;
	char tmp[33]="";

	ret = getIKey_2ST(key1);
	#ifdef _CMCC_OMP_SEC_DEBUG
	LOGI("in calHmac_2ST key1 is %s",key1);
	#endif
	if(ret!=0){
	#ifdef _CMCC_OMP_SEC_DEBUG
	LOGI("ret!=0");
	#endif
	}
	descryptdata_wb(key1,32,key);

	#ifdef _CMCC_OMP_SEC_DEBUG
	LOGI("in calHmac_1ST key is %s",key);
	#endif


	if((fd = open(filePath, O_RDONLY)) < 0)
		return -2;
	if((fstat(fd,&stat)) < 0)
		return -3;
	length = stat.st_size;
        close(fd);
	char *out = malloc(sizeof(char) * length);
	ret = ompReadFile(filePath, length, out);
	if(ret != 0){
		free(out);
		return -4;
	}
	hmac_sha256(key,klen,out,length,tmp);
	base64enc(result,tmp,32);
	free(out);
	return 0;
}
int verify_1st_ClassesHmac(char *filePath) {
	int ret = 0, i;
	char oldHmac[89] = HMACVALUE_1ST;
	char newHmac[89];
	char tmpHmac[45];
	char dexpath[128]="";
	snprintf(dexpath, 128, "/data/data/%s/classes.dex", filePath);

	#ifdef _CMCC_OMP_SEC_DEBUG
	LOGI("dexpath is %s",dexpath);
	#endif

	ret = calHmac_1ST(dexpath, tmpHmac);

	if (ret != 0)
		return ret;

	char *tmp = newHmac;
	for (i = 0; i < 44; i++) {
		sprintf(tmp, "%2x", tmpHmac[i]);
		tmp += 2;
	}


	#ifdef _CMCC_OMP_SEC_DEBUG
	LOGI("1st oldHmac is %s",oldHmac);
	LOGI("1st newHmac is %s",newHmac);
	#endif


	for (i = 0; i < 88; i++) {
		if (oldHmac[i] != newHmac[i]) {
			return -1;
		}
	}
	return 0;
}


int verify_2st_ClassesHmac(char *filePath) {
	int ret = 0, i;
	char oldHmac[89] = HMACVALUE_2ST;
	char newHmac[89];
	char tmpHmac[45];
	char dexpath[128]="";
	snprintf(dexpath, 128, "/data/data/%s/cmcc_march.zip", filePath);

	#ifdef _CMCC_OMP_SEC_DEBUG
	LOGI("dexpath is %s",dexpath);
	#endif

	ret = calHmac_2ST(dexpath, tmpHmac);

	if (ret != 0)
		return ret;

	char *tmp = newHmac;
	for (i = 0; i < 44; i++) {
		sprintf(tmp, "%2x", tmpHmac[i]);
		tmp += 2;
	}

	#ifdef _CMCC_OMP_SEC_DEBUG
	LOGI("2st oldHmac is %s",oldHmac);
	LOGI("2st newHmac is %s",newHmac);
	#endif


	for (i = 0; i < 88; i++) {
		if (oldHmac[i] != newHmac[i]) {
			return -1;
		}
	}
	return 0;
}
int verifyClassesHmac(char *filePath) {
	int ret = 0, i;
	char oldHmac[89] = HMACVALUE;
	char newHmac[89] = "\0";
	char tmpHmac[45] = "\0";
	char dexpath[128] = "\0";
	char *tmp = NULL;
	snprintf(dexpath, 128, "/data/data/%s", filePath);

	LOGI("verifyClassesHmac original path is %s", dexpath);

	ret = calHmac(dexpath, tmpHmac);
	if (ret != 0)
		return ret;

	tmp = newHmac;
	for (i = 0; i < 44; i++) {
		sprintf(tmp, "%02x", tmpHmac[i]);
		tmp += 2;
	}

	LOGI("oldHmac is %s", oldHmac);
	LOGI("newHmac is %s", newHmac);

	for (i = 0; i < 88; i++) {
		if (oldHmac[i] != newHmac[i]) {
			return -1;
		}
	}
	return 0;
}
int Verify_Haze_Hmac(char *strpkname) {
	char dataPth[256] = { 0 };
	int fd, i, length;
	struct stat filestat;
	char oldHmac[89] = { 0 };
	char newIkey[121] = { 0 };
	int Noah_Length;
	char newHmac[89] = { 0 };
	char tmp[33] = { 0 };
	char key[33] = { 0 };
	char result[45] = { 0 };
	char *data = NULL;
	char *Tmpdata = NULL;
	char *tmp1 = NULL;
	snprintf(dataPth, 128, "/data/data/%s/lib/libcmcc_haze.so", strpkname);

	LOGI("Verify_Haze_Hmac enter");

	if ((fd = open(dataPth, O_RDONLY)) < 0)
		return -2;
	if ((fstat(fd, &filestat)) < 0)
		return -3;
	length = filestat.st_size;
	close(fd);

	data = calloc(length, sizeof(char));
	ompReadFile(dataPth, length, data);

	LOGI("length is %d", length);

	for (i = 0; i < 88; i++) {
		oldHmac[i] = data[i + length - 208 - sizeof(int)];
	}
	for (i = 0; i < 120; i++) {
		newIkey[i] = data[i + length - 120 - sizeof(int)];
	}
	oldHmac[88] = '\0';
	newIkey[120] = '\0';

	LOGI("oldHmac is %s", oldHmac);
	LOGI("newIkey is %s", newIkey);


	memcpy(&Noah_Length, data + length - sizeof(int), sizeof(int));

	LOGI("Noah_Length is %d", Noah_Length);

	Tmpdata = calloc(length - Noah_Length - 208 - sizeof(int), sizeof(char));
	memcpy(Tmpdata, data, length - Noah_Length - 208 - sizeof(int));
	if (data != NULL) {
		free(data);
	}
	getIKeyInterface(key, newIkey);

	LOGI("key is %s", key);

	hmac_sha256(key, 32, Tmpdata, length - Noah_Length - 208 - sizeof(int), tmp);
	base64enc(result, tmp, 32);
	tmp1 = newHmac;
	for (i = 0; i < 44; i++) {
		sprintf(tmp1, "%02x", result[i]);
		tmp1 += 2;
	}

	LOGI("newHmac is %s", newHmac);

	if (Tmpdata != NULL) {
		free(Tmpdata);
	}
	for (i = 0; i < 88; i++) {
		if (oldHmac[i] != newHmac[i]) {
			return -1;
		}
	}

	LOGI("Verify_Haze_Hmac OK.");
	return 0;
}

