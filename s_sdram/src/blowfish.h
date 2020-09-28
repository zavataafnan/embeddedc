/*********************************************************************
* Filename:   blowfish.h
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding Blowfish implementation.
*********************************************************************/

#ifndef BLOWFISH_H
#define BLOWFISH_H

/*************************** HEADER FILES ***************************/
#include <stddef.h>

/****************************** MACROS ******************************/
#define BLOWFISH_BLOCK_SIZE 8           // Blowfish operates on 8 bytes at a time

#define uchar unsigned char // 8-bit
#define uint unsigned long // 32-bit

#define F(x,t) t = keystruct->s[0][(x) >> 24]; \
               t += keystruct->s[1][((x) >> 16) & 0xff]; \
               t ^= keystruct->s[2][((x) >> 8) & 0xff]; \
               t += keystruct->s[3][(x) & 0xff]; 
#define swap(r,l,t) t = l; l = r; r = t;
#define ITERATION(l,r,t,pval) l ^= keystruct->p[pval]; F(l,t); r^= t; swap(r,l,t);



typedef struct {
	uint p[18];
	uint s[4][256];
} BLOWFISH_KEY;


/*********************** FUNCTION DECLARATIONS **********************/


void blowfish_encrypt(uchar in[], uchar out[], BLOWFISH_KEY *keystruct);
void blowfish_decrypt(uchar in[], uchar out[], BLOWFISH_KEY *keystruct);
void key_schedule(uchar user_key[], BLOWFISH_KEY *keystruct, int len);

#endif   // BLOWFISH_H
