# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)
# haze
include $(CLEAR_VARS)
LOCAL_CFLAGS += -O3 -fvisibility=hidden
LOCAL_MODULE    := cmcc_haze
LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_SRC_FILES := \
				haze/interface.c	\
				haze/decrypt.c \
				haze/Reinforce.c \
				haze/Integrity.c \
				opensource/rebuild.c \
				opensource/KeyIOApi.c \
				opensource/rijndeal.c \
				opensource/aes_cbc.c \
				opensource/aes_core.c \
				opensource/hmac_sha256.c \
				opensource/base64_dec.c \
				opensource/base64_enc.c \
				opensource/sha256.c \
				opensource/ioapia.c \
				opensource/miniunz.c \
        		opensource/unzip.c \
       			opensource/zip.c \
        		opensource/mztools.c \
				haze/unzip.c \

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -lz -llog 
cmd-strip = $(TOOLCHAIN_PREFIX)strip $1
include $(BUILD_SHARED_LIBRARY)

#noah
include $(CLEAR_VARS)
LOCAL_CFLAGS += -O3 -fvisibility=hidden
LOCAL_MODULE    := cmcc_noah
LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_SRC_FILES :=  \
					noah/Stub_Function.c \
					noah/Alg.c \
					noah/descrypt.c \
					noah/Decryption.c \
					noah/Integrity.c \
					noah/Reinforce.c \

LOCAL_SHARED_LIBRARIES := cmcc_haze
cmd-strip = $(TOOLCHAIN_PREFIX)strip $1
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -lz -llog 
include $(BUILD_SHARED_LIBRARY)
