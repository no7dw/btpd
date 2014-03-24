#ifndef __CODESET_CONVERT_H
#define __CODESET_CONVERT_H

#ifdef CODESET_CONVERT

#include "iconv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


class CodesetConvert
{
private:
	//代码转换:从一种编码转为另一种编码
	static int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen);
public:	
	//UNICODE码转为GB2312码
	static int u2g(char *inbuf,int inlen,char *outbuf,int outlen);
	//GB2312码转为UNICODE码
	static int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen);
};

#endif
#endif