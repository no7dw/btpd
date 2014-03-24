#include "CgiSettingXml.h"
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc , char** argv)
{
	if(argc<2)
	{
	    cout<< "Usage: CgiXML filename" << endl;
	    return -1;
	}
	CgiSettingXML *pCgiSettingXMLObj= new CgiSettingXML();
	
	pCgiSettingXMLObj->LoadXml(argv[1]);
	SettingInfo *pSI;
	pSI = pCgiSettingXMLObj->GetSettingInfo();
 	char result[50] = {0};
 	if (pSI != NULL)
 	{
 	    sprintf( result , " %d  | %d | %d | %d | %d | %d " ,(int)pSI->upload , pSI->download ,pSI->maxseedtime ,pSI->maxactivertr, pSI->maxinactivetm ,pSI->autodelfinish );
	    cout << result << endl;
 	}
	return 1;

	
}
