/*
 ============================================================================
 Name        : Alg.c
 Author      : sushuai
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "Alg.h"
char SubStituteMark[33] = "havereplacedhavereplacedhaverepl";
#define LOG_TAG "Alg.c"
#define MAXFILENAME (256)

void itostr(str, i)
	//convert int to char*
	char *str;int *i; {
	LOGI("in itostr");
	snprintf(str, 128, "%d", *i);
}

int calculate(uf, src, HMAC, cer_length, keyword)
	unzFile uf;char *src;char *HMAC;int *cer_length;char *keyword; {
	LOGI("in calculate");
	uLong i;
	int length = 0;
	unz_global_info64 gi;
	int err;
	int total_length = 0;
//	int listnum = 0;
	char keyword_new[20] = { 0 };
	unsigned char TmpCerMac[33] = { 0 };
	//unsigned char *filename_inzip = NULL;
	int j = 0;
	//uLong ratio = 0;
	//const char *string_method;
	//char charCrypt = ' ';
	unsigned char *innmem = NULL;
	err = unzGetGlobalInfo64(uf, &gi);
	if (err != UNZ_OK) {
		LOGI("err != UNZ_OK");
	}

	if (strlen(keyword) == 9) { //META-INF
		memcpy(keyword_new, keyword, 9);
	} else if (strlen(keyword) == 11) { //classes.dex
		memcpy(keyword_new, keyword, 11);
	}

	for (i = 0; i < gi.number_entry; i++) {
		unz_file_info64 file_info;
		char filename_inzip[256];
		unsigned char cer1tmpmac[33] = { 0 };
		err = unzGetCurrentFileInfo64(uf, &file_info, filename_inzip,
				sizeof(filename_inzip), NULL, 0, NULL, 0);
		if (err != UNZ_OK) {
			break;
		}

		if (strlen(keyword) == 9) {
			if (memcmp(filename_inzip, keyword_new, 9) == 0) {
				innmem = (unsigned char *) extractFile_inmem(src,
						filename_inzip, &length);

				LOGI("in calculate filename_inzip is %s", filename_inzip);
				LOGI("in calculate length is %d", length);

				sha256(cer1tmpmac, innmem, length);

				for (j = 0; j < 32; j++) {
					TmpCerMac[j] ^= cer1tmpmac[j];
				}
				total_length += length;

				LOGI("in calculate TmpCerMac is %s", TmpCerMac);
				LOGI("in calculate total_length is %d", total_length);

				if (innmem != NULL) {
					free(innmem);
				}
			}
		}

		else if (strlen(keyword) == 11) {
			if (strcmp(filename_inzip, keyword_new) == 0) {
				innmem = (unsigned char *) extractFile_inmem(src,
						filename_inzip, &length);

				LOGI("in calculate filename_inzip is %s", filename_inzip);
				LOGI("in calculate length is %d", length);

				sha256(cer1tmpmac, innmem, length);

				for (j = 0; j < 32; j++) {
					TmpCerMac[j] ^= cer1tmpmac[j];
				}
				total_length += length;

				LOGI("in calculate TmpCerMac is %s", TmpCerMac);
				LOGI("in calculate total_length is %d", total_length);

				if (innmem != NULL) {
					free(innmem);
				}
			}
		}

		if ((i + 1) < gi.number_entry) {
			err = unzGoToNextFile(uf);
			if (err != UNZ_OK) {
				break;
			}
		}
	}
	*cer_length = total_length;
	for (j = 0; j < 32; j++) {
		HMAC[j] ^= TmpCerMac[j];
	}

	LOGI("in calculate HMAC is %s", HMAC);

	return 0;
}

int Cal_HMAC(zipfilename, HMAC, cer_length, keyword)
	char *zipfilename;char *HMAC;int *cer_length;char *keyword; {

	LOGI("in Cal_CER_HMAC");

	char filename_try[MAXFILENAME + 16] = "";
	int ret_value = 0;
	int j;
	char HMAC_tmp[33] = { 0 };
	unzFile uf = NULL;
	uf = unzOpen64(zipfilename);
	if (uf == NULL) {
		//printf("cannot open %s\n", zipfilename);
		return -2;
	}
	//printf("%s opened\n", zipfilename);
	ret_value = calculate(uf, zipfilename, HMAC_tmp, cer_length, keyword);

	LOGI("in Cal_CER_HMAC HMAC_tmp is %s", HMAC_tmp);

	for (j = 0; j < 32; j++) {
		HMAC[j] = HMAC_tmp[j];
	}

	LOGI("in Cal_CER_HMAC HMAC is %s", HMAC);

	unzClose(uf);

	return ret_value;
}

int CreateFile(filename, pkname, writebuf)
	const char *filename;const char *pkname;char *writebuf; {
	char fullpath[128];
	memset(fullpath, 0, sizeof(fullpath));
	sprintf(fullpath, "/data/data/%s/%s", pkname, filename);
	FILE *fp = fopen(fullpath, "w+");
	if (fp == NULL) {
		return 5346;
	}
	if (fputs(writebuf, fp) == EOF) {
		return 5346;
	}
	fclose(fp);

	return 0;
}

int GetFileLength(buf, length)
	char *buf;char *length; {
	int len = 0;
	char *pstart;
	char *pfind;
	char *startpr, *endpr;
	char tag[64];
	memset(tag, 0, sizeof(tag));
	strcpy(tag, ",");
	pstart = buf;
	if ((pfind = (char *) strstr(pstart, tag)) != NULL) {
		len = pfind - pstart;
		memcpy(length, pstart, len);
	} else {
		return 1;
	}
	return 0;
}

int GetFileMAC(buf, MAC)
	char *buf;char *MAC; {
	int len = 0, i;
	char *pstart;
	char *pfind;
	char *startpr, *endpr;
	char tag[64];
	memset(tag, 0, sizeof(tag));
	strcpy(tag, ",");
	pstart = buf;
	for (i = 0; i < 2; i++) {
		if ((pfind = (char *) strstr(pstart, tag)) != NULL) {
			if (i == 1) {
				len = pfind - pstart;
				if (len > 0) {
					memcpy(MAC, pstart, len);
				}
			} else {
				pfind++;
				pstart = pfind;
			}
		}
	}

	return 0;
}

int HexToStr(pbHex, Len, pbStr)
	unsigned char*pbHex;int Len;char *pbStr; {
	int i;
	for (i = 0; i < Len; i++) {
		if (((pbHex[i] & 0xf0) >> 4) >= 0 && ((pbHex[i] & 0xf0) >> 4) <= 9)
			pbStr[2 * i] = ((pbHex[i] & 0xf0) >> 4) + 0x30;
		else if (((pbHex[i] & 0xf0) >> 4) >= 10
				&& ((pbHex[i] & 0xf0) >> 4) <= 16)
			pbStr[2 * i] = ((pbHex[i] & 0xf0) >> 4) + 0x37;
		else
			return 1; //won't happen

		if ((pbHex[i] & 0x0f) >= 0 && (pbHex[i] & 0x0f) <= 9)
			pbStr[2 * i + 1] = (pbHex[i] & 0x0f) + 0x30;
		else if ((pbHex[i] & 0x0f) >= 10 && (pbHex[i] & 0x0f) <= 16)
			pbStr[2 * i + 1] = (pbHex[i] & 0x0f) + 0x37;
		else
			return 1; //won't happen
	}
	return OMP_SUCCESS;
}

int decryptseed(APPkey, encryptedseed, seed)
	const char *APPkey;const char *encryptedseed;char *seed; {
	unsigned char iv[16] = "";
	unsigned char ucencryptedseed[128] = "";
	unsigned char ucResult[256] = "";
	unsigned char pubKey[16] = "";
	unsigned char ucseed[45] = "";
	AES_KEY key;
	int len, ret;

	if (!encryptedseed || !APPkey) {
		return 5058;
	}

	memcpy(pubKey, APPkey + 16, 16);

//get the encryption key
	ret = AES_set_decrypt_key((const unsigned char *) pubKey, 128, &key);
	memcpy(iv, APPkey, 16);

//decrypt seed
	AES_cbc_encrypt((const unsigned char *) encryptedseed, seed, 48, &key, iv,
			AES_DECRYPT);
	seed[44] = '\0';

	return 0;
}

int GetRegMac(appkey, req, RegMac)
	char *appkey;char *req;char *RegMac; {

	LOGI("in GetRegMac");

	unsigned char ucmac[33];
	if (!appkey || !req) {
		return OMP_NULL_PTR_ERR;
	}
	hmac_sha256(appkey, OMP_APPKEY_LEN, req, strlen(req), ucmac);
	base64enc(RegMac, (void *) ucmac, (unsigned short) 32);

	LOGI("int GetRegMac,RegMac is %s", RegMac);

	return 0;
}

/*
 Name:			encryptIMSI
 Description:	Encrypt IMSI with aes-128-cbc)
 Paramters:
 IMSI:16
 APPkey:32
 timestamp:12
 reval:44*/
int encryptedimsi(plaintext, APPkey, timestamp, reval1)
	const char *plaintext;const char *APPkey;const char *timestamp;char *reval1; {

	LOGI("in encryptedplaintext");

	unsigned char iv[16] = { 1 };
	char data[256] = { 0 };
	unsigned char ucResult[256] = { 0x00 };
	AES_KEY key;
	int len = 0;
	int ret = 0;
	char reval[100] = "";
	unsigned char pucKey[16] = { 0x00 };
	if (!plaintext || !APPkey || !timestamp) {
		return OMP_NULL_PTR_ERR;
	}

	if (strlen(timestamp) != OMP_TIME_STAMP_LEN) {
		return OMP_PARAM_LEN_ERR;
	}
//	if(strlen(IMSI) != OMP_IMSI_LEN && strlen(IMSI) !=OMP_DEVICEID_LEN && strlen(IMSI) !=11)
//	{
//		return OMP_PARAM_LEN_ERR;
//	}

	LOGI("strlen(IMSI) is %d", strlen(plaintext));
	LOGI("plaintext is %s", plaintext);

	memcpy(pucKey, APPkey + 16, 16);
	ret = AES_set_encrypt_key((const unsigned char*) pucKey, 128, &key);
	if (ret < 0) {
		return OMP_IMSI_INVALID_ERR;
	}

	strcpy(data, plaintext);
	strcat(data, timestamp);

	len = strlen(data); // 28 bytes or 44 bytes or 23 bytes
	if (len == 23) {

		LOGI("encrypt IMEI");

		memset(data + len, 0x09, 9);
		len += 9;
	} else if (len == 28) {

		LOGI("encrypt IMSI");

		memset(data + len, 0x04, 4);
		len += 4;
	} else if (len == 44) {

		LOGI("encrypt deviceid");

		memset(data + len, 0x04, 4);
		len += 4;
	} else {
		exit(1);
	}
//memset(iv, 0x03, sizeof(iv));
	memcpy(iv, APPkey, 16);

	AES_cbc_encrypt((const unsigned char*) data, ucResult, len, &key, iv,
			AES_ENCRYPT);

	base64enc(reval, (void *) ucResult, (unsigned short) len);

	LOGI("reval is %s", reval);

	strcpy(reval1, reval);
	return OMP_SUCCESS;
}

int encryptdata(plaintext, APPkey, reval)
	const char *plaintext;const unsigned char *APPkey;unsigned char *reval; {
	unsigned char iv[16] = { 1 };
	char data[1024] = "\0";
	unsigned char datatmp[1024] = "\0";
	int i;
	int pading;
	AES_KEY key;
	int len = 0;
	int ret = 0;
	unsigned char pucKey[16] = "\0";
	if (!plaintext || !APPkey || !reval) {
		return 1;
	}
	memcpy(pucKey, APPkey + 16, 16);
	ret = AES_set_encrypt_key((const unsigned char*) pucKey, 128, &key);
	if (ret < 0) {
		return 1;
	}
	len = strlen(plaintext);
	if (len % 16 != 0) {
		pading = ((int) len / 16 + 1) * 16;
		strcpy(data, plaintext);
		memset(data + len, pading - len, pading - len);
		len = pading;
	} else {
		strcpy(data, plaintext);
	}
	memcpy(iv, APPkey, 16);
	AES_cbc_encrypt((const unsigned char*) data, datatmp, len, &key, iv,
			AES_ENCRYPT);

	base64enc(reval, (void *) datatmp, (unsigned short) len);
	return 0;
}

int decryptdata(ciphertext, APPkey, plaintext)
	unsigned char *ciphertext;unsigned char *APPkey;unsigned char *plaintext; {
	unsigned char datatmp[1024] = "\0";
	unsigned char iv[16] = "\0";
	unsigned char pubKey[16] = "\0";
	AES_KEY key;
	int len, ret;

	if (!ciphertext || !APPkey || !plaintext) {
		return 5058;
	}

	len = base64_binlength(ciphertext, sizeof(unsigned char));
	ret = base64dec((void*) datatmp, ciphertext, sizeof(unsigned char));
	if (ret != 0) {
		return 1;
	}

	memcpy(pubKey, APPkey + 16, 16);
//get the encryption key
	ret = AES_set_decrypt_key((const unsigned char *) pubKey, 128, &key);
	memcpy(iv, APPkey, 16);
//decrypt seed
	AES_cbc_encrypt((const unsigned char *) datatmp, plaintext, len, &key, iv,
			AES_DECRYPT);
	plaintext[len] = '\0';
	return 0;
}

int calMACandSAVE(length, lengthchar, strcounter, apkpath, strpkname,
		privatekey, mac, apkkey, FileName)
	int length;char *lengthchar;char *strcounter;char *apkpath;char *strpkname;unsigned char *privatekey;char *mac;char *apkkey;char *FileName; {
	char *content = NULL;
	char tmpmac[33] = "\0";
	char mac32[33] = "\0";
	char tmpmac1[45] = "\0";
	int i;
	unsigned char Splicebuf[256] = "\0";
	unsigned char writebuf[256] = "\0";
	content = calloc(length + 1, sizeof(char));
	ompReadFile(apkpath, length, content);
//	sha256(tmpmac, content, strlen(content));
	sha256(tmpmac, content, length);
	hmac_sha256(tmpmac, 32, strcounter, strlen(strcounter), mac32);
	for (i = 0; i < 32; i++) {
		apkkey[i] = tmpmac[i];
	}
	base64enc(mac, (void *) mac32, (unsigned short) 32);
	base64enc(tmpmac1, (void *) tmpmac, (unsigned short) 32);
//save
//splice
	sprintf(Splicebuf, "%s,%s,", lengthchar, tmpmac1);
//encrypt

	LOGI("Splicebuf is %s", Splicebuf);

	encryptdata(Splicebuf, privatekey, writebuf);
	CreateFile(FileName, strpkname, writebuf);
	if (content != NULL) {
		free(content);
	}
	return 0;
}
