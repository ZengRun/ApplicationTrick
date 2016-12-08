/*
 ============================================================================
 Name        : descrypt.c
 Author      : sushuai
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include"decrypt.h"

#define LOG_TAG "hazeso_decrypt.c"

char parameter[33] = "destroyparamemterdestroyparamemt";
#define KEYBITS 256
int descryptFile(char* data, int len, char* pwd, char* outFile) {

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
		fputs("File open error\n", stderr);
		return 1;
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
			fputs("File write error", stderr);
			return 1;
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
			fputs("File write error", stderr);
			return 1;
		}
	}
	fclose(output);
	return 0;
}

void* getSoLidPath(char* path) {

#ifdef _CMCC_OMP_SEC_TIME
	double t = 0.00;
	clock_t start = 0, end = 0;
	start = clock();
#endif

	LOGI("in getSoLidPath");

	int fd, length, ret;
	int noah_length;
	char *tmepOut = NULL;
	char *tmepOut2 = NULL;
	char dataPth[128] = { 0 };
	struct stat filestat;
	char password[33] = { 0 };
	char corePath[128];
	char depara[5] = { 0 };
	int seppar, i;
	char newHmac[89] = { 0 };
	char newIkey[121] = { 0 };
	snprintf(dataPth, 128, "%s/lib/libcmcc_haze.so", path);

	LOGI("dataPth is %s", dataPth);

	if ((fd = open(dataPth, O_RDONLY)) < 0)
		return SoLid;
	if ((fstat(fd, &filestat)) < 0)
		return SoLid;
	length = filestat.st_size;
	close(fd);
	char *data = calloc(length, sizeof(char));
	ompReadFile(dataPth, length, data);

	LOGI("noah length is %d", length);

//check integrity
	memcpy(&noah_length, data + (length - sizeof(int)), sizeof(int));
	tmepOut = calloc(noah_length, sizeof(char));
	tmepOut2 = calloc(noah_length, sizeof(char));
	memcpy(tmepOut, data + (length - 208 - sizeof(int) - noah_length),
			noah_length);   //ikey:120 hmac:88

	ret = verify_Noah_Hmac(tmepOut, noah_length);

	LOGI("after verify_Noah_Hmac ret is %d", ret);

	if (ret != 0) {
		return SoLid;
	}

	for (i = 0; i < 5; i++) {
		depara[i] = parameter[i];
	}
	//seppar=	atoi(depara);
	seppar = depara[0] - 0x30;

	LOGI("seppar is %d", seppar);

	int l = rebuildDataLen(tmepOut, seppar, noah_length, tmepOut2);

	LOGI("l is %d", l);

	getdecIKey(password);

	LOGI("password is %s", password);

	snprintf(corePath, 128, "%s/htc.so", path);
	int a = descryptFile(tmepOut2, l, password, corePath);

	if ((SoLid = dlopen(corePath, RTLD_LAZY)) == NULL) {
		LOGI("dlopen - %s", dlerror());
		exit(-1);
	}

	LOGI("out get1SoLi1dPath1");
	remove(corePath);
	free(data);
	free(tmepOut);
	free(tmepOut2);
#ifdef _CMCC_OMP_SEC_TIME
	end = clock();
	t = (double) (end - start) / CLOCKS_PER_SEC;
	LOGW("7. decrypt consume time is %lf", t);
#endif
	return SoLid;
}

