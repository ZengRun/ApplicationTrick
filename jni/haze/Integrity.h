/*
 * Integrity.h
 *
 *  Created on: 2012-11-17
 *      Author: sushuai
 */

#ifndef INTEGRITY_H_
#define INTEGRITY_H_

int ompReadFile(const char *filepath, const int len, unsigned char *out);
int verifyHmacDecrypt(char *filePath);

#endif /* INTEGRITY_H_ */
