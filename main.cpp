#include <iostream.h>
#include <stdio.h>
#include <string.h>

void fnEncode( char *pszSrc, char *pszDest, int nSrcLen, int nDestLen)
{
	int				nDestPos	= 0;
	int				nRestCount	= 0;
	unsigned char	chMade = 0, chRest = 0;
	
	for (int i = 0; i < nSrcLen; i++)
	{
		if (nDestPos >= nDestLen) break;
		
		chMade = ((chRest | (pszSrc[i] >> (2 + nRestCount))) & 0x3f);
		chRest = (((pszSrc[i] << (8 - (2 + nRestCount))) >> 2) & 0x3f);
		
		nRestCount += 2;
		
		if (nRestCount < 6)
			pszDest[nDestPos++] = chMade + 0x3c;
		else
		{
			if (nDestPos < nDestLen - 1)
			{
				pszDest[nDestPos++]	= chMade + 0x3c;
				pszDest[nDestPos++]	= chRest + 0x3c;
			}
			else
				pszDest[nDestPos++] = chMade + 0x3c;
			
			nRestCount	= 0;
			chRest		= 0;
		}
	}
	
	if (nRestCount > 0)
		pszDest[nDestPos++] = chRest + 0x3c;
	
	pszDest[nDestPos] = '\0';
	
	return;
}

void fnDecode(char *pwszSrc, char *pszDest, int nDestLen){
	unsigned char Decode6BitMask[5] = { 0xfc, 0xf8, 0xf0, 0xe0, 0xc0 };
	int				nLen = strlen(pwszSrc);
	int				nDestPos = 0, nBitPos = 2;
	int				nMadeBit = 0;
	char	ch, chCode, tmp;
	
	//char			*pszSrc = new char[nLen + 1];
	char *pszSrc;
	
	//WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszSrc, sizeof(pszSrc), NULL, NULL);
	pszSrc = pwszSrc;
	
	for (int i = 0; i < nLen; i++)
	{
		if ((pszSrc[i] - 0x3c) >= 0)
			ch = pszSrc[i] - 0x3c;
		else
		{
			nDestPos = 0;
			break;
		}
		
		if (nDestPos >= nDestLen) break;
		
		if ((nMadeBit + 6) >= 8)
		{
			chCode = (tmp | ((ch & 0x3f) >> (6 - nBitPos)));
			pszDest[nDestPos++] = chCode;
			
			nMadeBit = 0;
			
			if (nBitPos < 6) 
				nBitPos += 2;
			else
			{
				nBitPos = 2;
				continue;
			}
		}
		
		tmp = ((ch << nBitPos) & Decode6BitMask[nBitPos - 2]);
		
		nMadeBit += (8 - nBitPos);
	}
	
	pszDest[nDestPos] = '\0';
	
	//delete [] pszSrc;
	
	return;
}

void main(){
	//cout<<sizeof(char)<<endl;
	
	char src[20] = "gzcb";
	
	char dest[20] = "nnn";

	char decode[] = "aaa";
	
	fnEncode(src,dest,sizeof(src),15);
	
	cout<<dest<<endl;

	fnDecode(dest,decode,15);

	cout<<decode<<endl;
}