#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#if defined _WIN32 || defined _WIN64
#include <io.h>
#endif /* platform */

#ifdef BZLIB_SUPPORT
#include <bzlib.h>
#endif

#ifdef ZLIB_SUPPORT
#include <zlib.h>
#endif

#include "dat.h"

void
preread_datafile(const char* filename_in)
{
  int fd = 0;
  int len;

#ifdef ZLIB_SUPPORT
  char* gzfilename;
  gzFile gzf;
#endif

#ifdef BZLIB_SUPPORT
  char* bzfilename;
  BZFILE* bzf;
#endif

#ifdef BZLIB_SUPPORT
  bzfilename = malloc(strlen(filename_in) + 5);
  strcpy(bzfilename, filename_in);
  strcat(bzfilename, ".bz2");
  bzf = BZ2_bzopen(bzfilename, "rb");
  free(bzfilename);
  bzfilename = NULL;

  if (bzf != NULL)
  {
    int bufsize = 0;
    int bufpos = 0;
    int br;
    unsigned char* ptr;
    do
    {
      if (bufpos >= bufsize)
      {
        bufsize += 1024 * 1024;
        datafile_buffer = (unsigned char *) realloc(datafile_buffer, bufsize);
        if (datafile_buffer == NULL)
        {
          perror("realloc()");
          exit(42);
        }
      }

      br = BZ2_bzread(bzf, datafile_buffer + bufpos, bufsize - bufpos);
      if (br == -1)
      {
        fprintf(stderr, "gzread failed.\n");
        exit(42);
      }

      bufpos += br;
    } while (br>0);

    /* try to shrink buffer... */
    ptr = (unsigned char *) realloc(datafile_buffer, bufpos);
    if (ptr != NULL) datafile_buffer = ptr;

    BZ2_bzclose(bzf);
    return;
  }

  /* drop through and try for an gzip compressed or uncompressed datafile... */
#endif

#ifdef ZLIB_SUPPORT
  gzfilename = malloc(strlen(filename_in) + 4);
  strcpy(gzfilename, filename_in);
  strcat(gzfilename, ".gz");
  gzf = gzopen(gzfilename, "rb");
  free(gzfilename);
  gzfilename = NULL;

  if (gzf != NULL)
  {
    int bufsize = 0;
    int bufpos = 0;
    unsigned char* ptr;
    do
    {
      int br;
      if (bufpos >= bufsize)
      {
        bufsize += 1024 * 1024;
        datafile_buffer = (unsigned char*)realloc(datafile_buffer, bufsize);
        if (datafile_buffer == NULL)
        {
          perror("realloc()");
          exit(42);
        }
      }

      br = gzread(gzf, datafile_buffer + bufpos, bufsize - bufpos);
      if (br == -1)
      {
        fprintf(stderr, "gzread failed.\n");
        exit(42);
      }

      bufpos += br;
    } while (!gzeof(gzf));

    /* try to shrink buffer... */
    ptr = (unsigned char*)realloc(datafile_buffer, bufpos);
    if (ptr != NULL) datafile_buffer = ptr;

    gzclose(gzf);
    return;
  }

  /* drop through and try for an uncompressed datafile... */
#endif

  fd = open(filename_in, O_RDONLY | O_BINARY);
//  fd = open(filename_in, O_RDONLY);
  if (fd == -1)
  {
    fprintf(stderr, "can't open %s:", filename_in);
    perror("");
    exit(42);
  }

  len = filelength(fd);
  datafile_buffer = (unsigned char*)malloc(len);
  if (datafile_buffer == NULL)
  {
    perror("malloc()");
    close(fd);
    exit(42);
  }

  if (read(fd, datafile_buffer, len) != len)
  {
    perror("read()");
    close(fd);
    exit(42);
  }

  close(fd);
}

unsigned char*
dat_open(const char* filename_in)
{
  int num;
	int c1;
	char name[21];
	int ofs;
	unsigned char* ptr = NULL;

	if (datafile_buffer == NULL) return NULL;

	memset(name, 0, sizeof(name));

	num = ( (datafile_buffer[0] <<  0) +
	        (datafile_buffer[1] <<  8) +
	        (datafile_buffer[2] << 16) +
	        (datafile_buffer[3] << 24) );

	ptr = datafile_buffer + 4;

	for (c1 = 0; c1 < num; c1++)
  {
		memcpy(name, ptr, 12);
		ptr += 12;

		if (strnicmp(name, filename_in, strlen(filename_in)) == 0)
    {
			ofs = ( (ptr[0] <<  0) +
				      (ptr[1] <<  8) +
				      (ptr[2] << 16) +
				      (ptr[3] << 24) );

			return (datafile_buffer + ofs);
		}

		ptr += 8;
	}

	return NULL;
}

unsigned int
dat_filelen(const char* filename_in)
{
	unsigned char* ptr;
	int num;
	int c1;
	char name[21];
	int len;

	memset(name, 0, sizeof(name));

	num = ( (datafile_buffer[0] <<  0) +
	        (datafile_buffer[1] <<  8) +
	        (datafile_buffer[2] << 16) +
	        (datafile_buffer[3] << 24) );

	ptr = datafile_buffer + 4;

	for (c1 = 0; c1 < num; c1++)
  {
    memcpy(name, ptr, 12);
		ptr += 12;

		if (strnicmp(name, filename_in, strlen(filename_in)) == 0)
    {
			ptr += 4;
			len = ( (ptr[0] <<  0) +
				      (ptr[1] <<  8) +
				      (ptr[2] << 16) +
				      (ptr[3] << 24) );

			return len;
		}

		ptr += 8;
	}

	return 0;
}
