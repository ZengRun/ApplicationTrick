/*
 ============================================================================
 Name        : interface.c
 Author      : sushuai
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include  "dlfcn.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <memory.h>
//#include<sys/ptrace.h>

#include "Integrity.h"
#include "Reinforce.h"
#include "Key.h"
#include "logcat.h"
#include "time.h"
//#include "../encrypt.h"
#include "../libcmcc_haze.h"

int (*So)();
int ZIPMARK = 1;
void *Handle = NULL;
#define JNIREG_CLASS "com/cmcc/omss/NativeC"
static JNINativeMethod gMethods[4];
#define LOG_TAG "cmccomp_haze_so"

typedef uint32_t u4;
typedef uint64_t u8;

struct Object {
	void* clazz;
	u4 lock;
};

struct ArrayObject {
	struct Object ok;
	u4 length;
	u8 content[1];

};

void (*openDexFile)(const u4* args, jvalue *pResult);
JNINativeMethod *dvm_dalvik_system_DexFile;

int get_name_by_pid(pid_t pid, char* name) {
	int tmp;
	char buf[1024] = { 0 };

	//"/proc/%d/cmdline"
	snprintf(buf, 1024, "/proc/%d/cmdline", pid);
	if ((tmp = open(buf, O_RDONLY)) == -1)
		return -1;
	read(tmp, buf, 1024);
	strncpy(name, buf, 1023);
	return 0;
}

int lookup(JNINativeMethod *table, const char *name, const char *sig,
		void (**fnPtrout)(u4 const *, union jvalue *)) {
	int i = 0;
	//LOGI("in lookup");
	while (table[i].name != NULL) {
		if ((strcmp(name, table[i].name) == 0)
				&& (strcmp(sig, table[i].signature) == 0)) {
			//LOGI("in in if lookup name is %s", name);
			*fnPtrout = table[i].fnPtr;
			return 1;
		}
		i++;
	}
	return 0;
}

static jint XXXXXXXXXXXXXXXXXXXXX(JNIEnv* env, jobject thiz, jstring pkname,
		jstring apkpath) {
	LOGI("in haze rustezedd");
	int ret = 1;
	int fd,length;
	char dataPth[128]={0};
	struct stat filestat;
	jvalue pResult;
	jint result;
	struct ArrayObject *test12;
#ifdef _CMCC_OMP_SEC_TIME
	double t = 0.00;
	clock_t start = 0, end = 0;
	start = clock();
#endif

	char *strpkname = (char *) (*env)->GetStringUTFChars(env, pkname,
			JNI_FALSE);
	char *strapkpath = (char *) (*env)->GetStringUTFChars(env, apkpath,
			JNI_FALSE);
	char corepath[128] = { 0 };

	if (strlen(strpkname) <= 0 || strlen(strapkpath) <= 0) {
		//LOGI("strlen(strpkname)<=0||strlen(strapkpath)<=0");
		return (jint) ret;
	}
	if (ZIPMARK != 0) {
		ZIPMARK = unzip(strpkname, strapkpath);
		if (ZIPMARK != 0) {
			return ZIPMARK;
		}
	}

	// "/data/data/%s"
	snprintf(corepath, 128, "/data/data/%s", strpkname);
	//"/data/data/%s/cmcc_march"
	snprintf(dataPth, 128,"/data/data/%s/cmcc_march" , strpkname);

	//LOGI("dataPth is %s", dataPth);
	if ((fd = open(dataPth, O_RDONLY)) < 0)
		exit(-1);
	if ((fstat(fd, &filestat)) < 0)
		exit(-1);
	length = filestat.st_size;
	close(fd);
	char *inntertest = calloc(length - 151552 + 1, sizeof(char));
	if (Handle == NULL) {
		Handle = (void*) getSoLidPath(corepath);
	}
	So = dlsym(Handle, _STUB_DECRYPTDEX_);

	ret = (*So)(strpkname, strapkpath,inntertest);
	//LOGI("in haze DecryptDex ret is %d", ret);
	//LOGI("in rusteze DecryptDex ret is %d", ret);
	if (ret != 0) {
		exit(-1);
	}
	//LOGI("inntertest is %s", inntertest);

	//"/system/lib/libdvm.so"
	void *ldvm = (void*) dlopen("/system/lib/libdvm.so", RTLD_LAZY);

	//"dvm_dalvik_system_DexFile"
	dvm_dalvik_system_DexFile = (JNINativeMethod*) dlsym(ldvm,"dvm_dalvik_system_DexFile");

	//"openDexFile"  "([B)I"
	lookup(dvm_dalvik_system_DexFile, "openDexFile", "([B)I", &openDexFile);

	test12 = (struct ArrayObject*) calloc(
			sizeof(struct Object) + sizeof(u4) + length - 151552 + 1,
			sizeof(char));
	test12->length = length - 151552;
	memcpy(&(test12->content), inntertest, length - 151552);
	//LOGI("before openDexFile result is %d", result);
	openDexFile((u4*) &test12, &pResult);
	result = (jint) pResult.l;
	//LOGI("after openDexFile result is %d", result);
	
	free(inntertest);
	free(test12);
	
	if (Handle != NULL) {
		dlclose(Handle);
		Handle = NULL;
	}

	(*env)->ReleaseStringUTFChars(env, apkpath, strapkpath);
	(*env)->ReleaseStringUTFChars(env, pkname, strpkname);

#ifdef _CMCC_OMP_SEC_TIME
	end = clock();
	t = (double) (end - start) / CLOCKS_PER_SEC;
	LOGW("6. interface consume time is %lf", t);
#endif


	return (jint) result;
}

static jint XXXXXXXXXXXXXXXXXXXXXXXX(JNIEnv* env, jobject thiz, jstring pkname,
		jstring apkpath) {
	//LOGI("in haze rustezedd_Lollipop");
	int ret = 1;
	int fd,length;
	struct stat filestat;

#ifdef _CMCC_OMP_SEC_TIME
	double t = 0.00;
	clock_t start = 0, end = 0;
	start = clock();
#endif

	char *strpkname = (char *) (*env)->GetStringUTFChars(env, pkname,
			JNI_FALSE);
	char *strapkpath = (char *) (*env)->GetStringUTFChars(env, apkpath,
			JNI_FALSE);
	char corepath[128] = { 0 };

	//LOGI("strpkname is %s", strpkname);
	//LOGI("strapkpath is %s", strapkpath);

	if (strlen(strpkname) <= 0 || strlen(strapkpath) <= 0) {
		//LOGI("strlen(strpkname)<=0||strlen(strapkpath)<=0");
		return (jint) ret;
	}
	if (ZIPMARK != 0) {
		ZIPMARK = unzip(strpkname, strapkpath);
		if (ZIPMARK != 0) {
			return ZIPMARK;
		}
	}

	snprintf(corepath, 128, "/data/data/%s", strpkname);

	if (Handle == NULL) {
		Handle = (void*) getSoLidPath(corepath);
	}
	//error here
	So = dlsym(Handle, _STUB_DECRYPTDEX_FILE);

	ret = (*So)(strpkname, strapkpath);
	LOGI("in haze DecryptDex ret is %d", ret);
	LOGI("in rusteze DecryptDex ret is %d", ret);
	if (ret != 0) {
		exit(-1);
	}

	if (Handle != NULL) {
		dlclose(Handle);
		Handle = NULL;
	}
	(*env)->ReleaseStringUTFChars(env, apkpath, strapkpath);
	(*env)->ReleaseStringUTFChars(env, pkname, strpkname);

#ifdef _CMCC_OMP_SEC_TIME
	end = clock();
	t = (double) (end - start) / CLOCKS_PER_SEC;
	LOGW("6. interface consume time is %lf", t);
#endif

	return 0;

}

static void XXXXXXXXXXXXXXXXXXXXXXXXXXXX(JNIEnv* env, jobject thiz, jstring pkname){

	LOGI("in rustezehide");
	char *strpkname = (char *) (*env)->GetStringUTFChars(env, pkname,
				JNI_FALSE);
	char *dexname=NULL;
	dexname=(char*)malloc(sizeof(char)*(strlen(strpkname)+25));//   /data/data/%s/cmcc_march
	sprintf(dexname,"/data/data/%s/cmcc_march",strpkname);
	char* classesdxZip=NULL;
	classesdxZip=(char*)malloc(sizeof(char)*(strlen(strpkname)+25));//   /data/data/%s/classes.zip
	sprintf(classesdxZip,"/data/data/%s/classes.zip",strpkname);
	char* classesdx=NULL;
	classesdx=(char*)malloc(sizeof(char)*(strlen(strpkname)+25));//   /data/data/%s/classes.dex
	sprintf(classesdx,"/data/data/%s/classes.dex",strpkname);
	char* hideddex=NULL;
	hideddex=(char*)malloc(sizeof(char)*(strlen(strpkname)+25));//   /data/data/%s/.cmcc_march
	sprintf(hideddex,"/data/data/%s/.cmcc_march",strpkname);
	char* oatname=NULL;
	oatname=(char*)malloc(sizeof(char)*(strlen(strpkname)+28));//   /data/data/%s/softsec_march
	sprintf(oatname,"/data/data/%s/softsec_march",strpkname);
	(*env)->ReleaseStringUTFChars(env, pkname, strpkname);

	remove(classesdx);
	free(classesdx);
	remove(classesdxZip);
	free(classesdxZip);
	remove(dexname);
	rename(hideddex,dexname);
	free(dexname);
	free(hideddex);
	remove(oatname);
	//加密softsec_march, 删除明文，密文命名为softsec_march
//	encrypt_oat(oatname);
	free(oatname);
}

static int XXXXXXXXXXXXXXXXXXXXXXXXXXX(JNIEnv* env, jobject thiz, jstring pkname){
	LOGI("in rustezedecoat");
	char *strpkname = (char *) (*env)->GetStringUTFChars(env, pkname,JNI_FALSE);

	char *dexname=NULL;
	dexname=(char*)malloc(sizeof(char)*(strlen(strpkname)+25));//   /data/data/%s/cmcc_march
	sprintf(dexname,"/data/data/%s/cmcc_march",strpkname);

	char* classdx=NULL;
	classdx=(char*)malloc(sizeof(char)*(strlen(strpkname)+25));//   /data/data/%s/classes.dex
	sprintf(classdx,"/data/data/%s/classes.dex",strpkname);

	rename(dexname,classdx);
	free(dexname);
	free(classdx);
//
//	char* oatPath=NULL;
//	oatPath=(char*)malloc(sizeof(char)*(strlen(strpkname)+28));//   /data/data/%s/softsec_march
//	sprintf(oatPath,"/data/data/%s/softsec_march",strpkname);
//
//	char* oathide=NULL;
//	oathide=(char*)malloc(sizeof(char)*(strlen(strpkname)+28));//   /data/data/%s/.softsec_march
//	sprintf(oathide,"/data/data/%s/.softsec_march",strpkname);
//	(*env)->ReleaseStringUTFChars(env, pkname, strpkname);
//
//	int fd, length, ret;
//	size_t retlen = 0;
//	struct stat stat;
//	char enckey[33] = "\0";
//	int i,n;
//	if ((fd = open(oatPath, O_RDONLY)) < 0)
//		return -2;
//	if ((fstat(fd, &stat)) < 0)
//		return -3;
//	length = stat.st_size;
//	close(fd);
//	char *endata = (char*) calloc(length, sizeof(char));
//	ret = ompReadFile(oatPath, length-32, endata);
//	if (ret != 0) {
//		free(endata);
//		return -4;
//	}
//
//	FILE *f=NULL;
//	f=fopen(oatPath,"rb");
//	if(f==NULL)
//		return -5;
//	fseek(f,-32L,SEEK_END);
//	retlen=fread(enckey,sizeof(unsigned char),32,f);
//	if(retlen==0||retlen<32)
//		return -6;
//	fclose(f);
//	f=NULL;
//	LOGI("in dec oat,get ikey:%s",enckey);
//
//	rename(oatPath,oathide);//重命名 softsec_march-->.softsec_march
//	descryptFile(endata,length-32,enckey,oatPath);//解密softsec_march,明文命名为softsec_march
//	free(endata);
//	free(oatPath);
//	free(oathide);
	return 0;
}

/*
 * Register several native methods for one class.
 */
 static int registerNativeMethods(JNIEnv* env, const char* className,
		JNINativeMethod* gMethods, int numMethods) {
	jclass clazz;
	clazz = (*env)->FindClass(env, className);
	if (clazz == NULL) {
		return JNI_FALSE;
	}
	if ((*env)->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
		return JNI_FALSE;
	}

	return JNI_TRUE;
}
/*
 * Register native methods for all classes we know about.
 */
static int registerNatives(JNIEnv* env) {

	//LOGI("in registerNatives");


	if (!registerNativeMethods(env, JNIREG_CLASS, gMethods,
			sizeof(gMethods) / sizeof(gMethods[0])))
		return JNI_FALSE;

	return JNI_TRUE;
}
/*
 * Set some test stuff up.
 *
 * Returns the JNI version on success, -1 on failure.
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {

	//ptrace(PTRACE_TRACEME,0,0,0);//anti debug error here
	JNIEnv* env = NULL;
	jint result = -1;
	//LOGI("in JNI_OnLoad");
	gMethods[0].name = "rustezedd";
	gMethods[0].signature = "(Ljava/lang/String;Ljava/lang/String;)I";
	gMethods[0].fnPtr = XXXXXXXXXXXXXXXXXXXXX;

	gMethods[1].name = "rustezedd_Lollipop";
	gMethods[1].signature = "(Ljava/lang/String;Ljava/lang/String;)I";
	gMethods[1].fnPtr = XXXXXXXXXXXXXXXXXXXXXXXX;

	gMethods[2].name = "rustezehide";
	gMethods[2].signature = "(Ljava/lang/String;)V";
	gMethods[2].fnPtr = XXXXXXXXXXXXXXXXXXXXXXXXXXXX;

	gMethods[3].name = "rustezedecoat";
	gMethods[3].signature = "(Ljava/lang/String;)I";
	gMethods[3].fnPtr = XXXXXXXXXXXXXXXXXXXXXXXXXXX;


	if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return -1;
	}
	//assert(env != NULL);

	if (!registerNatives(env)) { //注册
		return -1;
	}
	/* success -- return valid version number */
	result = JNI_VERSION_1_4;

	return result;
}
