/*
 ============================================================================
 Name        : unzip.c
 Author      : sushuai
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "time.h"
#include "../logcat.h"
#include <stdio.h>

#include "../libcmcc_haze.h"


#define LOG_TAG "unzip"

//http://www.doc88.com/p-618600879232.html
//extern char _path_21_ori[15];
//extern char _path_18_ori[14];
//extern char _path_19_ori[13];
//extern char _path_20_ori[18];

int unzip(char *pkname, char *apkpath) {
	char classdexpath[128] = { 0 };
	char cmcc_marchpath[128] = { 0 };
//	char cmcc_noahpath[128]="\0";
//	char cmcc_zoepath[128]="\0";
//	char cmcc_noah_v7path[128]="\0";
//	char cmcc_zoe_v7path[128]="\0";
//	char cmcc_noah_mipspath[128]="\0";
//	char cmcc_zoe_mipspath[128]="\0";
//	char cmcc_noah_x86path[128]="\0";
//	char cmcc_zoe_x86path[128]="\0";
	char srcpath[128] = { 0 };
	char dstpath[128] = { 0 };
	snprintf(srcpath, 128, "%s", apkpath);
	snprintf(dstpath, 128, "/data/data/%s/", pkname);
	snprintf(classdexpath, 128, "%sclasses.dex", dstpath);
	snprintf(cmcc_marchpath, 128, "%scmcc_march", dstpath);
#ifdef _CMCC_OMP_SEC_TIME
	double t = 0.00;
	clock_t start = 0, end = 0;
	double t_unzip = 0.00;
	clock_t start_unzip = 0, end_unzip = 0;
	clock_t start_unzip_classes_dex = 0, end_unzip_classes_dex = 0;
	clock_t start_unzip_cmcc_march = 0, end_unzip_cmcc_march = 0;
	clock_t start_unzip_cmcc_noah = 0, end_unzip_cmcc_noah = 0;
	clock_t start_unzip_cmcc_zoeso = 0, end_unzip_cmcc_zoeso = 0;
	clock_t start_unzip_cmcc_noah_v7 = 0, end_unzip_cmcc_noah_v7 = 0;
	clock_t start_unzip_cmcc_zoe_v7so = 0, end_unzip_cmcc_zoe_v7so = 0;
	clock_t start_unzip_cmcc_zoe_mipsso = 0, end_unzip_cmcc_zoe_mipsso = 0;
	clock_t start_unzip_cmcc_noah_mips = 0, end_unzip_cmcc_noah_mips = 0;
	clock_t start_unzip_cmcc_noah_mipsso = 0, end_unzip_cmcc_noah_mipsso = 0;
	clock_t start_unzip_cmcc_noah_x86 = 0, end_unzip_cmcc_noah_x86 = 0;
	clock_t start_unzip_cmcc_noah_x86so = 0, end_unzip_cmcc_noah_x86so = 0;
	clock_t start_unzip_cmcc_zoe_x86so = 0, end_unzip_cmcc_zoe_x86so = 0;
#endif

#ifdef _CMCC_OMP_SEC_TIME
	start_unzip = clock();
#endif

//	if(access(classdexpath,0)!=0){
//		#ifdef _CMCC_OMP_SEC_TIME
//		start_unzip_classes_dex = clock();
//		#endif
//		extractFile_For_OMP(srcpath, dstpath, "classes.dex");
//		#ifdef _CMCC_OMP_SEC_TIME
//		end_unzip_classes_dex = clock();
//		t_unzip = (double)(end_unzip_classes_dex - start_unzip_classes_dex) / CLOCKS_PER_SEC;
//		LOGW("5.1. unzip classes.dex consume time is %lf",t_unzip);
//		#endif
//		}
	if (access(cmcc_marchpath, 0) != 0) {
#ifdef _CMCC_OMP_SEC_TIME
		start_unzip_cmcc_march = clock();
#endif
		extractFile_For_OMP(srcpath, dstpath, "assets/cmcc_march");
#ifdef _CMCC_OMP_SEC_TIME
		end_unzip_cmcc_march = clock();
		t_unzip = (double) (end_unzip_cmcc_march - start_unzip_cmcc_march)
				/ CLOCKS_PER_SEC;
		LOGW("5.2. unzip cmcc_march consume time is %lf", t_unzip);
#endif
	}
//		if(access(cmcc_noahpath,0)!=0){
//		#ifdef _CMCC_OMP_SEC_TIME
//		start_unzip_cmcc_noah = clock();
//		#endif
//		extractFile_For_OMP(srcpath, dstpath, "assets/cmcc_noah");
//		#ifdef _CMCC_OMP_SEC_TIME
//		end_unzip_cmcc_noah = clock();
//		t_unzip = (double)(end_unzip_cmcc_noah - start_unzip_cmcc_noah) / CLOCKS_PER_SEC;
//		LOGW("5.3. unzip cmcc_noah consume time is %lf",t_unzip);
//		#endif
//		}
//		if(access(cmcc_zoepath,0)!=0){
//		#ifdef _CMCC_OMP_SEC_TIME
//		start_unzip_cmcc_zoeso = clock();
//		#endif
//		extractFile_For_OMP(srcpath, dstpath, "assets/cmcc_zoe.so");
//		#ifdef _CMCC_OMP_SEC_TIME
//		end_unzip_cmcc_zoeso = clock();
//		t_unzip = (double)(end_unzip_cmcc_zoeso - start_unzip_cmcc_zoeso) / CLOCKS_PER_SEC;
//		LOGW("5.4. unzip cmcc_zoe.so consume time is %lf",t_unzip);
//		#endif
//		}
//		if(access(cmcc_noah_v7path,0)!=0){
//		#ifdef _CMCC_OMP_SEC_TIME
//		start_unzip_cmcc_noah_v7 = clock();
//		#endif
//		extractFile_For_OMP(srcpath, dstpath, "assets/cmcc_noah_v7");
//		#ifdef _CMCC_OMP_SEC_TIME
//		end_unzip_cmcc_noah_v7 = clock();
//		t_unzip = (double)(end_unzip_cmcc_noah_v7 - start_unzip_cmcc_noah_v7) / CLOCKS_PER_SEC;
//		LOGW("5.5. unzip cmcc_noah_v7 consume time is %lf",t_unzip);
//		#endif
//		}
//		if(access(cmcc_zoe_v7path,0)!=0){
//		#ifdef _CMCC_OMP_SEC_TIME
//		start_unzip_cmcc_zoe_v7so = clock();
//		#endif
//		extractFile_For_OMP(srcpath, dstpath, "assets/cmcc_zoe_v7.so");
//		#ifdef _CMCC_OMP_SEC_TIME
//		end_unzip_cmcc_zoe_v7so = clock();
//		t_unzip = (double)(end_unzip_cmcc_zoe_v7so - start_unzip_cmcc_zoe_v7so) / CLOCKS_PER_SEC;
//		LOGW("5.6. unzip cmcc_zoe_v7.so consume time is %lf",t_unzip);
//		#endif
//		}
//		if(access(cmcc_noah_mipspath,0)!=0){
//		#ifdef _CMCC_OMP_SEC_TIME
//		start_unzip_cmcc_noah_mips = clock();
//		#endif
//		extractFile_For_OMP(srcpath, dstpath, "assets/cmcc_noah_mips");
//		#ifdef _CMCC_OMP_SEC_TIME
//		end_unzip_cmcc_noah_mips = clock();
//		t_unzip = (double)(end_unzip_cmcc_noah_mips - start_unzip_cmcc_noah_mips) / CLOCKS_PER_SEC;
//		LOGW("5.7. unzip cmcc_noah_mips consume time is %lf",t_unzip);
//		#endif
//		}
//		if(access(cmcc_zoe_mipspath,0)!=0){
//		#ifdef _CMCC_OMP_SEC_TIME
//		start_unzip_cmcc_zoe_mipsso = clock();
//		#endif
//		extractFile_For_OMP(srcpath, dstpath, "assets/cmcc_zoe_mips.so");
//		#ifdef _CMCC_OMP_SEC_TIME
//		end_unzip_cmcc_zoe_mipsso = clock();
//		t_unzip = (double)(end_unzip_cmcc_zoe_mipsso - start_unzip_cmcc_zoe_mipsso) / CLOCKS_PER_SEC;
//		LOGW("5.8. unzip cmcc_zoe_mips.so consume time is %lf",t_unzip);
//		#endif
//		}
//		if(access(cmcc_noah_x86path,0)!=0){
//		#ifdef _CMCC_OMP_SEC_TIME
//		start_unzip_cmcc_noah_x86 = clock();
//		#endif
//		extractFile_For_OMP(srcpath, dstpath, "assets/cmcc_noah_x86");
//		#ifdef _CMCC_OMP_SEC_TIME
//		end_unzip_cmcc_noah_x86 = clock();
//		t_unzip = (double)(end_unzip_cmcc_noah_x86 - start_unzip_cmcc_noah_x86) / CLOCKS_PER_SEC;
//		LOGW("5.9. unzip cmcc_noah_x86 consume time is %lf",t_unzip);
//		#endif
//		}
//		if(access(cmcc_zoe_x86path,0)!=0){
//		#ifdef _CMCC_OMP_SEC_TIME
//		start_unzip_cmcc_zoe_x86so = clock();
//		#endif
//		extractFile_For_OMP(srcpath, dstpath, "assets/cmcc_zoe_x86.so");
//		#ifdef _CMCC_OMP_SEC_TIME
//		end_unzip_cmcc_zoe_x86so = clock();
//		t_unzip = (double)(end_unzip_cmcc_zoe_x86so - start_unzip_cmcc_zoe_x86so) / CLOCKS_PER_SEC;
//		LOGW("5.9. unzip cmcc_noah_x86.so consume time is %lf",t_unzip);
//		#endif
//		}

	LOGI("after extractFile");

#ifdef _CMCC_OMP_SEC_TIME
	end_unzip = clock();
	t_unzip = (double) (end_unzip - start_unzip) / CLOCKS_PER_SEC;
	LOGW("5. unzip consume time is %lf", t_unzip);
#endif

	return 0;
}
