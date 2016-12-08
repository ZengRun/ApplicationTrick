/*
 ============================================================================
 Name        : descrypt.c
 Author      : sushuai
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include"descrypt.h"
#include "Decryption.h"

#define LOG_TAG "core_decrypt.c"

unsigned char DB[592][256] = { 0 };
char parameter1[33] = "paramemterdestroydestroyparamemt";
char parameter2[33] = "destroyparamemterdestroyparamemt";

int descryptdata_wb(char* data,int len,char* outdata);

#define KEYBITS 256
int descryptFile_traAES(char* data, int len, char* pwd, char* outFile) {
	LOGI("in descryptFile");
	LOGI("outFile is %s", outFile);

	unsigned long rk[RKLENGTH(KEYBITS)];
	unsigned char key[KEYLENGTH(KEYBITS)];
	int i;
	int nrounds;
	char *password;
	FILE *output;
	password = pwd;
	for (i = 0; i < sizeof(key); i++)
		key[i] = *password != 0 ? *password++ : 0;
	output = fopen(outFile, "wb+");

	if (output == NULL) {
		LOGI("File open error");
		return 5350;
	}

	nrounds = rijndaelSetupDecrypt(rk, key, 256);
	int ngroups = len / 16;
	int lastgroup = len % 16;
	int k;

	for (k = 0; k < ngroups; k++) {
		unsigned char plaintext[16];
		unsigned char ciphertext[16];
		int j;
		for (j = 0; j < sizeof(ciphertext); j++) {
			ciphertext[j] = data[k * 16 + j];
		}
		if (j == 0)
			break;
		rijndaelDecrypt(rk, nrounds, ciphertext, plaintext);
		if (fwrite(plaintext, sizeof(plaintext), 1, output) != 1) {
			fclose(output);
			LOGI("File write error");
			output = NULL;
			return 5351;
		}
	}

	if (lastgroup > 0) {
		unsigned char last[lastgroup];
		int start = 16 * ngroups;
		int l;
		for (l = 0; l < lastgroup; l++) {

			last[l] = data[start + l];
		}
		if (fwrite(last, sizeof(last), 1, output) != 1) {
			fclose(output);
			LOGI("File write error");
			output = NULL;
			return 5351;
		}
	}
	fclose(output);
	output = NULL;
	return 0;
}

int descryptdata_traAES(char* data, int len, char* pwd, char* outdata) {
	LOGI("enter descryptFile");
	unsigned long rk[RKLENGTH(KEYBITS)];
	unsigned char key[KEYLENGTH(KEYBITS)];
	int i;
	int nrounds;
	char *password;
	FILE *output;
	password = pwd;
	for (i = 0; i < sizeof(key); i++) {
		key[i] = *password != 0 ? *password++ : 0;
	}
	nrounds = rijndaelSetupDecrypt(rk, key, 256);
	int ngroups = len / 16;
	int lastgroup = len % 16;
	int k;

	for (k = 0; k < ngroups; k++) {
		unsigned char plaintext[16];
		unsigned char ciphertext[16];
		int j;
		for (j = 0; j < sizeof(ciphertext); j++) {
			ciphertext[j] = data[k * 16 + j];
		}
		if (j == 0)
			break;
		rijndaelDecrypt(rk, nrounds, ciphertext, plaintext);
		for (j = 0; j < sizeof(ciphertext); j++) {
			outdata[k * 16 + j] = plaintext[j];
		}
	}

	if (lastgroup > 0) {
		unsigned char last[lastgroup];
		int start = 16 * ngroups;
		int l;
		for (l = 0; l < lastgroup; l++) {
			outdata[start + l] = data[start + l];
		}

	}
	LOGI("exit descryptFile");
	return 0;
}

int getSoLidPath_traAES(char* path,char *innertest) {
	LOGI("in get1SoLidPath");
	int fd, length;
	char dataPth[128];
	char key[33] = "";
	char key1[33] = "";
	int ret;
	char depara[5] = "\0";
	int seppar = 0;
	int i;
	//char corePath[128] = "";
	struct stat filestat;
	//snprintf(dataPth, 128, "/data/data/%s/MyReflector_reinforce", path);
	snprintf(dataPth, 128, "/data/data/%s/cmcc_march", path);

	LOGI("dataPth is %s", dataPth);

	if ((fd = open(dataPth, O_RDONLY)) < 0)
		return -2;
	if ((fstat(fd, &filestat)) < 0)
		return -3;
	length = filestat.st_size;
	close(fd);
	//char tmepOut[length-151552];
	char *tmepOut = calloc(length-151552, sizeof(char));
	char *InnerOut= calloc(length-151552,sizeof(char));
	char *data = calloc(length-151552, sizeof(char));
	ompReadFile(dataPth, length-151552, data);

	for (i = 0; i < 5; i++) {
		depara[i] = parameter2[i];
	}
	//seppar=	atoi(depara);
	LOGI("depara[0] is %c", depara[0]);

	seppar = depara[0] - 0x30;

	LOGI("seppar is %d", seppar);

	int l = rebuildDataLen(data, seppar, length-151552, tmepOut);
	ret = getdecIKey(key1);

	LOGI("key1 is %s", key1);

	descryptdata_wb(key1, 32, key);
	LOGI("key is %s", key);

	LOGI("length is %d", length);

	int a = descryptdata_traAES(tmepOut, length-151552, key, InnerOut);
	for(i=0;i<length-151552;i++){
	innertest[i]=InnerOut[i];
	}
	free(InnerOut);
	free(tmepOut);
	free(data);
	return a;
}

int getSoLidPath_traAES_File(char* path) {
	LOGI("in get1SoLidPath_File");
	int fd, length;
	char dataPth[128];
	char hidePth[128];
	char key[33] = "";
	char key1[33] = "";
	int ret;
	char depara[5] = "\0";
	int seppar = 0;
	int i;
	char corePath[128] = "";
	struct stat filestat;
	snprintf(dataPth, 128, "/data/data/%s/cmcc_march", path);
	snprintf(hidePth,128,"/data/data/%s/.cmcc_march",path);

	LOGI("dataPth is %s", dataPth);

	if ((fd = open(dataPth, O_RDONLY)) < 0)
		return -2;
	if ((fstat(fd, &filestat)) < 0)
		return -3;
	length = filestat.st_size;
	close(fd);
	char *tmepOut = calloc(length-151552, sizeof(char));
	char *data = calloc(length-151552, sizeof(char));
	ompReadFile(dataPth, length-151552, data);
	rename(dataPth,hidePth);//rename cmcc_march into .cmcc_march,

		for (i = 0; i < 5; i++) {
			depara[i] = parameter2[i];
		}
		//seppar=	atoi(depara);
		LOGI("depara[0] is %c", depara[0]);
		seppar = depara[0] - 0x30;
		LOGI("seppar is %d", seppar);
		int l = rebuildDataLen(data, seppar, length - 151552, tmepOut);
		snprintf(corePath, 128, "/data/data/%s/cmcc_march", path);// generate dexfile cmcc_march.zip
		ret = getdecIKey(key1);
		LOGI("key1 is %s", key1);
		descryptdata_wb(key1, 32, key);
		LOGI("key is %s", key);
		LOGI("length is %d", length);

		int a = descryptFile_traAES(tmepOut, length - 151552, key, corePath);
		free(tmepOut);
		free(data);
		return a;
}

int initialize_WB_Key(char *path) {

	int fd, i, j, ret = 1;
	int length = 0;
	char dataPth[128] = "\0";
	unsigned char key[33] = "\0";
	char depara[5] = "\0";
	int seppar = 0;
	struct stat filestat;
	unsigned char Tmp_Wb_Key[151552 + 1] = "\0";
	unsigned char Wb_Key_tmepOut[151552 + 1] = "\0";
	snprintf(dataPth, 128, "/data/data/%s/cmcc_march", path);
#ifdef _CMCC_OMP_SEC_DEBUG
	LOGI("dataPth is %s", dataPth);
#endif
	if ((fd = open(dataPth, O_RDONLY)) < 0)
		return -1;
	if ((fstat(fd, &filestat)) < 0)
		return -1;
	length = filestat.st_size;
	char *data = calloc(length, sizeof(char));
	ompReadFile(dataPth, length, data);
	for (i = 0; i < 151552; i++) {
		Tmp_Wb_Key[i] = data[length - 151552 + i];
	}

	for (i = 0; i < 5; i++) {
		depara[i] = parameter1[i];
	}
	seppar = depara[0] - 0x30;
#ifdef _CMCC_OMP_SEC_DEBUG
	LOGI("seppar is %d", seppar);
#endif

	int l = rebuildDataLen(Tmp_Wb_Key, seppar, 151552, Wb_Key_tmepOut);
	ret = getIKey_Dec_WB_Key(key);
#ifdef _CMCC_OMP_SEC_DEBUG
	LOGI("in initialize_WB_Key key is %s", key);
#endif

	int a = descryptdata_traAES(Wb_Key_tmepOut, 151552, key, Tmp_Wb_Key);

	for (i = 0; i < 592; i++) {
		for (j = 0; j < 256; j++) {
			DB[i][j] = Tmp_Wb_Key[256 * i + j];
		}
	}
	return 0;
}

int descryptdata_wb(char* data, int len, char* outdata) {
#ifdef _CMCC_OMP_SEC_DEBUG
	LOGI("in descryptdata_wb");
#endif
	int i;
	int nrounds;
	char *password;

	// nrounds = rijndaelSetupDecrypt(rk, key, 256);
	int ngroups = len / 16;
	int lastgroup = len % 16;
	int k;

	for (k = 0; k < ngroups; k++) {
		unsigned char plaintext[16];
		unsigned char ciphertext[16];
		int j;
		for (j = 0; j < sizeof(ciphertext); j++) {
			ciphertext[j] = data[k * 16 + j];
		}
		if (j == 0)
			break;

		Decrypt(ciphertext, plaintext);

		for (j = 0; j < sizeof(ciphertext); j++) {
			outdata[k * 16 + j] = plaintext[j];
		}
	}

	return 0;
}


