#include<iostream>
#include<string>
#include<stdint.h>
#include<cstdio>
#include<fstream>
using namespace std;
int main()
{
	char rawLen[4];
	fread(rawLen,4,sizeof(char),stdin);
	uint32_t msgLen=*((uint32_t *)(rawLen));
	if(!msgLen)
		return 0;
	char msg[msgLen+1];
	fread(msg,msgLen,sizeof(char),stdin);
	msg[msgLen]='\0';
	string msg1=msg;
	string pMsg="\"abcd\"";
	uint32_t pMsgLen=6;
	char *rLen=reinterpret_cast<char *>(&pMsgLen);
	fwrite(rLen,4,sizeof(char),stdout);
	fwrite(pMsg.c_str(),pMsgLen,sizeof(char),stdout);
return 0;
}
