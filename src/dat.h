#ifndef __DAT_H
#define __DAT_H

#include <stdlib.h>
#include <stdio.h>
//#include <cstdlib>
//#include <cstdio>


#ifdef __cplusplus
extern "C" {
#endif

static unsigned char* datafile_buffer = NULL;

void preread_datafile(const char*);    // filename
unsigned char* dat_open(const char*);  // filename
unsigned int dat_filelen(const char*); // filename

#ifdef __cplusplus
}
#endif

#endif