#ifndef _BLOCK_DEBUG_H_
#define _BLOCK_DEBUG_H_

#include <windows.h>

#if defined(_DEBUG) || defined(DEBUG)
#define TRACE(...) _OutputDebugString_("debug: " __VA_ARGS__)
#else // Release
#define TRACE(...)
#endif

void _OutputDebugString_(LPCSTR pszFormat, ...);

#endif // _BLOCK_DEBUG_H_
