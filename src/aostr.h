/* Copyright (C) 2023 James W M Barford-Evans
 * <jamesbarfordevans at gmail dot com>
 * Copyright (C) 2026 Reuben Percival
 * <reubenpercival@tutanota.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * See the COPYING file for more information. */
#ifndef AOSTR_H
#define AOSTR_H

#include <stdarg.h>
#include <stddef.h>
#include <sys/types.h>

#include "types.h"

typedef struct AoStr {
    char *data;
    u64 len;
    u64 capacity;
} AoStr;

AoStr *aoStrAlloc(u64 capacity);
AoStr *aoStrNew(void);
void aoStrRelease(AoStr *buf);

void aoStrTmpBufInit(void);
void aoStrPoolReset(void);
void aoStrPoolPrintStats(void);

int aoStrExtendBuffer(AoStr *buf, u64 additional);
void aoStrToLowerCase(AoStr *buf);
void aoStrToUpperCase(AoStr *buf);
void aoStrPutChar(AoStr *buf, char ch);
void aoStrRepeatChar(AoStr *buf, char ch, int times);
int aoStrCmp(AoStr *b1, AoStr *b2);
AoStr *aoStrDupRaw(char *s, u64 len);
AoStr *aoStrDup(AoStr *buf);
void aoStrRemovePreviousChar(AoStr *s, char ch);

char *aoStrMove(AoStr *buf);

void aoStrCatLen(AoStr *buf, const void *d, u64 len);
void aoStrCatAoStr(AoStr *buf, AoStr *s2);
void aoStrCat(AoStr *buf, const void *d);
void aoStrCatRepeat(AoStr *buf, char *str, int times);
void aoStrCatPrintf(AoStr *b, const char *fmt, ...);
void aoStrCatFmt(AoStr *buf, const char *fmt, ...);

void aoStrCatColoured(AoStr *buf, const char *color, const char *str);
void aoStrCatColouredFmt(AoStr *buf, const char *color, const char *fmt, ...);

AoStr *aoStrPrintf(const char *fmt, ...);
AoStr *aoStrEscapeString(AoStr *buf);
AoStr *aoStrEncode(AoStr *buf);

void aoStrArrayRelease(AoStr **arr, int count);
AoStr **aoStrSplit(char *to_split, char delimiter, int *count);
/* Returns a temporary string. The buffer is a fixed size so should only be used
 * for short strings, useful for strings that have a short lifetime. DO NOT
 * FREE the buffer. Copy it if you need it to stay around */
char *tprintf(const char *fmt, ...);
/* Returns a heap allocated string, thus can handle long strings */
char *mprintf(const char *fmt, ...);
char *mprintFmt(const char *fmt, ...);
char *mprintVa(const char *fmt, va_list ap, s64 *_len);
AoStr *aoStrError(void);
AoStr *aoStrIntToHumanReadableBytes(s64 bytes);

u64 aoStrHashFunction(AoStr *buf);
u64 cstringMurmur(char *data, s64 len);
u64 aoStrGetLen(AoStr *buf);
AoStr *aoStrIdentity(AoStr *buf);
int aoStrEq(AoStr *b1, AoStr *b2);

#endif
