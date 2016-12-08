/*
 * Integrity.h
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "inc/hmac_sha256.h"
#include "Key.h"
#include <unistd.h>
#include "../logcat.h"
#define HMACFILE "hmac.reinforce"

//int verifyClassesHmac(char *pkname);

