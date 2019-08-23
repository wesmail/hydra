/*********************************************************************
 * Copyright:
 *   GSI, Gesellschaft fuer Schwerionenforschung mbH
 *   Planckstr. 1
 *   D-64291 Darmstadt
 *   Germany
 * created  1.12.2000 by Horst Goeringer
 *********************************************************************
 * rawapi.h
 *   entry definitions for client api
 **********************************************************************
 *  7. 2.2001, H.G.: rfio_gsi_query added
 * 26. 2.2001, H.G.: rfio_fstat, rfio_perror, rfio_serror added
 *                   for ROOT version 300-05
 **********************************************************************
 */
#ifndef RAWAPI_H
#define RAWAPI_H

#define USER_API

#include "rawcomm.h"
#include "rawcli.h"
#define RFILE srawAPIFile


extern "C" RFILE *rfio_fopen( char *, char *);
   /* open file */

extern "C" int rfio_fread( char *, int, int, RFILE *);
   /* read file buffer */

extern "C" int rfio_fwrite( char *, int, int, RFILE *);
   /* write file buffer */

extern "C" int rfio_fclose( RFILE *);
   /* close file */

extern "C" int rfio_open( char *, int, int);
   /* open file */

extern "C" int rfio_read(int, char *, int);
   /* read file buffer */

extern "C" int rfio_write(int, char *, int);
   /* write file buffer */

extern "C" int rfio_lseek(int, int, int);
   /* move read/write file mark */

extern "C" int rfio_close(int);
   /* close file */

extern "C" int rfio_access(char *, int);
   /* get access status */

extern "C" int rfio_fstat(int, struct stat *);
   /* get file status */

extern "C" int rfio_parse(char *, char **, char **);
   /* parse name to host and path */

extern "C" void rfio_perror(const char *);
   /* print error message */

extern "C" char *rfio_serror();
   /* get error message */

extern "C" int rfio_unlink(char *);
   /* */

extern "C" int rfio_gsi_query(int, int, char *);
   /* get query info for remote file already opened */

#endif
