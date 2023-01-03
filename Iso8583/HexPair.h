// HexPair.h
//#include "Macro.h"
#pragma once

BOOL	HexPairToString(uint8_t* pData, uint uDataSize, uint8_t* pString, uint* puStringSize);
int     HexPairToStringEx(char* dest, char* src, int src_len);
BOOL	StringToHexPair(uint8_t* pString, uint uLength, uint8_t* pData, uint* puSize);
BOOL	StringToHexPair1(uint8_t* pString, uint uLength);

//BOOL	HexPairToWideString(LPBYTE pData, UINT uDataSize, WCHAR* pString, UINT* puStringSize);
//BOOL	WideStringToHexPair(LPCWSTR pString, UINT uLength, LPBYTE pData, UINT* puSize);
