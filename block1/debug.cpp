#include <windows.h>
#include "debug.h"
#include <stdio.h>

void _OutputDebugString_(LPCSTR pszFormat, ...)
{
    va_list argp;
    char pszBuf[1024];
    va_start(argp, pszFormat);
    vsprintf(pszBuf, pszFormat, argp);
    va_end(argp);
    OutputDebugStringA(pszBuf);
}