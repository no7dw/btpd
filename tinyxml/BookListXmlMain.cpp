#include "BookListXml.h"
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
	BookListXml *pBookListXmlObj= new BookListXml();
	pBookListXmlObj->LoadXml(argv[1]);
/*	
	CourseInfo CI;
	strcpy(CI.ID , "0001224");
	strcpy(CI.Name , "How to learn math");
	strcpy(CI.Path , "/tmp/how/");
	strcpy(CI.Hash , "ad34e35");
	CI.Level = 4;
	CI.Grade = 2;
	
	pBookListXmlObj->AddItem(CI);
	
	cout <<" result RemoveItem: "<< pBookListXmlObj->RemoveItem("0001225") << endl;
	*/
	return 1;

	
}
 
