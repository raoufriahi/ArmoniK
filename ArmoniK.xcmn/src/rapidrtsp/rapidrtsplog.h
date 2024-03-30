/*
 * Copyright (c) 2017-2024 UbVideo
 *
 * The computer program contained herein contains proprietary
 * information which is the property of UbVideo.
 * The program may be used and/or copied only with the written
 * permission of UbVideo or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#ifndef __RRLOG_H__
#define __RRLOG_H__

#include <stdio.h>
#include <stdarg.h>
//#include <stdint.h>

    /* Enable this to get full debugging output */
    /* #define _DEBUG */

#ifdef _DEBUG
#undef NODEBUG
#endif

typedef enum
{ 
	RRLOG_CRIT=0,
	RRLOG_ERROR,
	RRLOG_WARNING,
	RRLOG_INFO,
    RRLOG_DEBUG,
    RRLOG_DEBUGHIGH,
    RRLOG_ALL
} RRLOG_Level_t;

extern RRLOG_Level_t debuglevel;

typedef void (fRRLOG_Callback)(int level, const char *fmt, va_list);
void RRLOG_SetCallback(fRRLOG_Callback *cb);
void RRLOG_SetOutput(FILE *file);

#ifdef __GNUC__
void RRLOG_Printf(const char *format, ...) __attribute__ ((__format__ (__printf__, 1, 2)));
void RRLOG_Status(const char *format, ...) __attribute__ ((__format__ (__printf__, 1, 2)));
void RRLOG(int level, const char *format, ...) __attribute__ ((__format__ (__printf__, 2, 3)));
#else
void RRLOG_Printf(const char *format, ...);
void RRLOG_Status(const char *format, ...);
void RRLOG(int level, const char *format, ...);
#endif
void RRLOG_Hex(int level, const unsigned char *data, unsigned long len);
void RRLOG_HexString(int level, const unsigned char *data, unsigned long len);
void RRLOG_SetLevel(RRLOG_Level_t lvl);
RRLOG_Level_t RRLOG_GetLevel(void);


#endif
