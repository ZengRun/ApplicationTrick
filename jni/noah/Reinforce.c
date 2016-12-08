/*
 ============================================================================
 Name        : Reinforce.c
 Author      : sushuai
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "Reinforce.h"
#include "Alg.h"
#define LOG_TAG "core_reinforce.c"

static int func0() {
	int result = 0, i;
	result >>= 4;
	i = rand() % 5;
	switch (i) {
	case 0:
		i &= 0xffff;
		if (i >= 0)
			return 0;
		break;
	case 1:
		return i - 1;
		break;
	case 2:
		return i - 2;
		break;
	case 3:
		return i - 3;
		break;
	case 4:
		return 0;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}
static int func1() {
	int a = 32;
	a <<= 1;
	if (a > 20)
		return a - 63;
	else
		return 23;
}
static int func2() {
	int i, j = 0;
	for (i = 0; i < 5; i++) {
		j++;
	}
	return j - 3;
}
static int func3() {
	int i = rand() % 3;
	switch (i) {
	case 0:
		i += 2;
		break;
	case 1:
		i++;
		break;
	default:
		break;
	}
	return i + 1;
}
static int func4() {
	int b = 1;
	b ^= 0x0f;
	return b - 10;
}
static int func5() {
	int i = 0;
	while (i < 10) {
		i++;
		i <<= 1;
	}
	return i - 9;
}
static int func6() {
	int i, j, k;
	j = func3();
	k = func4();
	i = j + k + 5;
	return (i / 2);
}
static int func7() {
	int i, j, k;
	j = func2();
	k = func5();
	i = j + k;
	return i;
}
static int func8() {
	int i, j, k;
	j = func1();
	k = func7();
	i = j + k;
	return i;
}
static int func9() {
	int i, j, k;
	j = func2();
	k = func6();
	i = j + k + 1;
	return i;
}
static int func10() {
	int i, j, k;
	j = func1();
	k = func9();
	i = j + k;
	return i;
}
static int func11() {
	int i, j, k, l;
	j = func2();
	k = func3();
	l = func4();
	i = j + 2 + k + l;
	return i;
}
static int func12() {
	int i, j, k;
	j = func2();
	k = func10();
	i = j + k;
	return i;
}
static int func13() {
	int i, j, k;
	j = func1();
	k = func12();
	i = j + k;
	return i;
}
static int func14() {
	int i, j, k;
	j = func2();
	k = func7();
	i = j + k + 5;
	return i;
}
static int func15() {
	int i, j, k, l;
	j = func4();
	k = func5();
	l = func6();
	i = j + k + l;
	return i;
}
static int confuseFunc(int input) {
	int result;
	switch (input) {
	case 0:
		result = func13();
		break;
	case 1:
		result = func11();
		break;
	case 2:
		result = func3();
		break;
	case 3:
		result = func5();
		break;
	case 4:
		result = func7();
		break;
	case 5:
		result = func9();
		break;
	case 6:
		result = func1();
		break;
	case 7:
		result = func2();
		break;
	case 8:
		result = func15();
		break;
	case 9:
		result = func12();
		break;
	case 10:
		result = func4();
		break;
	case 11:
		result = func8();
		break;
	case 12:
		result = func10();
		break;
	case 13:
		result = func6();
		break;
	case 14:
		result = func0();
		break;
	case 15:
		result = func14();
		break;
	default:
		result = 255;
		break;
	}
	return result;
}

static int reconfuseFunc(int input1) {
	int result1;
	switch (input1) {
	case 0:
		result1 = func14();
		break;
	case 1:
		result1 = func6();
		break;
	case 2:
		result1 = func7();
		break;
	case 3:
		result1 = func2();
		break;
	case 4:
		result1 = func10();
		break;
	case 5:
		result1 = func3();
		break;
	case 6:
		result1 = func13();
		break;
	case 7:
		result1 = func4();
		break;
	case 8:
		result1 = func11();
		break;
	case 9:
		result1 = func5();
		break;
	case 10:
		result1 = func12();
		break;
	case 11:
		result1 = func1();
		break;
	case 12:
		result1 = func9();
		break;
	case 13:
		result1 = func0();
		break;
	case 14:
		result1 = func15();
		break;
	case 15:
		result1 = func8();
		break;
	default:
		result1 = 255;
		break;
	}
	return result1;
}

static int rebuildKey(a, length, appKey)
	char *a;int length;char *appKey; {
	int i, number, hnumber, lnumber;
	//replace appkey2
	int reverseMappingArray[] = { 8, 12, 1, 15, 13, 0, 14, 3, 6, 9, 5, 7, 10,
			11, 4, 2 };
	//int reverseMappingArray[] = {8,10,9,3,1,4,5,6,13,15,7,11,0,12,14,2};
	for (i = 0; i < length; i++) {
		hnumber = *(a + 2 * i);
		hnumber = reconfuseFunc(hnumber);
		hnumber = reverseMappingArray[hnumber];
		lnumber = *(a + 2 * i + 1);
		lnumber = reconfuseFunc(lnumber);
		lnumber = reverseMappingArray[lnumber];
		number = (hnumber << 4) + lnumber;
		*appKey++ = (char) number;
	}
	*appKey++ = '\0';
	return 0;
}

static int rebuildIKey(a, length, appKey)
	char *a;int length;char *appKey; {
	int i, number, hnumber, lnumber;
	//replace IKEY2
	int reverseMappingArray[] = { 8, 12, 1, 15, 13, 0, 14, 3, 6, 9, 5, 7, 10,
			11, 4, 2 };
	for (i = 0; i < length; i++) {
		hnumber = *(a + 2 * i);
		hnumber = reconfuseFunc(hnumber);
		hnumber = reverseMappingArray[hnumber];
		lnumber = *(a + 2 * i + 1);
		lnumber = reconfuseFunc(lnumber);
		lnumber = reverseMappingArray[lnumber];
		number = (hnumber << 4) + lnumber;
		*appKey++ = (char) number;
	}
	*appKey++ = '\0';
	return 0;
}
int getIKey(char *iKey) {
	char encodeKey2[120 + 1] = IKEYVALUE;
	char decKey[45] = { 0 };
	char *a = NULL;
	char encryKey[2 * 44 * sizeof(char)] = { 0 };

	LOGI("in getIKey IKEYVALUE is %s", encodeKey2);
	base64dec(encryKey, encodeKey2, 1);
	LOGI("in getIKey encryKey is %s", encryKey);
	a = (char *) encryKey;
	rebuildIKey(a, 44, decKey);
	base64dec(iKey, decKey, 1);
	iKey[32] = '\0';
	return 0;
}
int getIKey_1ST(char *iKey) {
	char encodeKey2[120+1] = IKEYVALUE_1ST;
	#ifdef _CMCC_OMP_SEC_DEBUG
	LOGI("in getIKey_1ST encodeKey2 is %s\n",encodeKey2);
	#endif
	char encryKey[2 * 44 * sizeof(char)];
	base64dec(encryKey, encodeKey2, 1);
	char *a = (char *) encryKey;
	char decKey[45];
	rebuildIKey(a, 44, decKey);
	base64dec(iKey, decKey, 1);

	iKey[32] = '\0';
	return 0;
}

int getIKey_2ST(char *iKey) {
	char encodeKey2[120+1] = IKEYVALUE_2ST;
	#ifdef _CMCC_OMP_SEC_DEBUG
	LOGI("in getIKey_2ST encodeKey2 is %s\n",encodeKey2);
	#endif
	char encryKey[2 * 44 * sizeof(char)];
	base64dec(encryKey, encodeKey2, 1);
	char *a = (char *) encryKey;
	char decKey[45];
	rebuildIKey(a, 44, decKey);
	base64dec(iKey, decKey, 1);

	iKey[32] = '\0';
	return 0;
}
int getIKey_Dec_WB_Key(char *iKey) {
	char encodeKey2[120+1] = IKEYVALUE_ENC_WB;
	#ifdef _CMCC_OMP_SEC_DEBUG
	LOGI("in getIKey_Dec_WB_Key encodeKey2 is %s\n",encodeKey2);
	#endif
	char encryKey[2 * 44 * sizeof(char)];
	base64dec(encryKey, encodeKey2, 1);
	char *a = (char *) encryKey;
	char decKey[45];
	rebuildIKey(a, 44, decKey);
	base64dec(iKey, decKey, 1);
	iKey[32] = '\0';
	return 0;
}
int getdecIKey(char *iKey) {
	char encodeKey2[120] = DECIKEYVALUE;

	char encryKey[2 * 44 * sizeof(char)];
	base64dec(encryKey, encodeKey2, 1);
	char *a = (char *) encryKey;
	char decKey[45];
	rebuildIKey(a, 44, decKey);
	base64dec(iKey, decKey, 1);

	iKey[32] = '\0';
	return 0;
}
int getIKeyInterface(char *iKey, char *iikey) {
	char szHMAC[256] = { 0 };
	int i;
	unsigned char ii[33] = { 0 };
	unsigned char dd[45] = { 0 };
	unsigned char decKey[45] = { 0 };
	char *a = NULL;
	for (i = 0; i < 120; i++) {
		szHMAC[i] = iikey[i];
	}
	char encryKey[256 * sizeof(char)] = "\0";
	base64dec(encryKey, szHMAC, 1);
	a = (char *) encryKey;

	rebuildIKey(a, 44, decKey);
	decKey[44] = '\0';

	for (i = 0; i < 44; i++) {
		dd[i] = decKey[i];
	}
	dd[44] = '\0';
	base64dec(ii, dd, 1);
	for (i = 0; i < 32; i++) {
		iKey[i] = ii[i];
	}
	iKey[32] = '\0';
	return 0;
}
