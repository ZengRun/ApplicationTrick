

#include "inc/sha256.h"
#include "inc/aes_cbc.h"
#include "inc/base64_enc.h"
#include "inc/base64_dec.h"
#include "inc/hmac_sha256.h"
#include "../logcat.h"
#include "Key.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include "inc/unzip.h"
#include "../libcmcc_haze.h"

#define		OMP_IMSI_LEN			16
#define		OMP_TIME_STAMP_LEN		12
#define		OMP_APPKEY_LEN			32
#define		OMP_DEVICEID_LEN		32
#define		OMP_SEED_LEN			44
#define		OMP_PID_LEN				32
#define		OMP_ENCRYPTED_IMSI_LEN	44

#define ENCRYPT_MODE		0
#define DECRYPT_MODE		1

#define CRYPT_BLOCK_LEN		8
#define OPM_RANDOM_LEN		16


#define OMP_SUCCESS						0
#define OMP_NULL_PTR_ERR				1
#define OMP_HASH_TYPE_NOT_SUPPORT		2
#define OMP_HMAC_ERR					3
#define OMP_MEMORY_NOT_ENOUGH_ERR		4
#define OMP_IMSI_INVALID_ERR			5
#define OMP_DES_KEY_LEN_ERR				6
#define OMP_PARAM_LEN_ERR				7
#define OMP_HEX_STR_DATA_ERR			8
#define OMP_TIME_STAMP_NOT_EQUALS_ERR	9
#define OMP_INIT_RAND_FAILED_ERR		10
#define OMP_GET_PID_ERR					11

