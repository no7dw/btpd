#include "CgiXml.h"
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

	CgiXML *pCgiXMLObj= new CgiXML();
	pCgiXMLObj->LoadXml(argv[1]);
/*	
	CourseInfo CI1;				
	strcpy( CI1.Hash, "d1f2fadcdf453222caf60cd1f61d09cc36949757");
	CI1.Grade = 1;				
	strcpy( CI1.Path, "/tmp/hdd/volumes/HDD/12/unit3");
	strcpy( CI1.ID, "123456789");
	strcpy( CI1.Name, "C++ Primer锛堢");
	CI1.Level = 12 ;	
	int ID = pCgiXMLObj->WhatsMyID(CI1);
	return 0;
*/	
	
	JobInfo *pJI;
	pJI = pCgiXMLObj->GetJobInfo("3");
	//pJI = pCgiXMLObj->GetInfo("1");
	if (pJI != NULL)
	{
	  cout<< pJI->ID << endl;
	  cout<< pJI->Priority << endl;
	  cout<< pJI->Partition << endl;
	  cout<< pJI->Space << endl;
	  cout<< pJI->Name << endl;
	  cout<< pJI->Status << endl;
	  
	  cout<< pJI->Size << endl;
	  cout<< pJI->Downloaded << endl;
	  cout<< pJI->TimeLeft << endl;
	  cout<< pJI->Percent << endl;
	  cout<< pJI->DwRate << " kB/s"<< endl;

	  cout<< pJI->UpRate << " kB/s"<< endl;
	  cout<< pJI->Npeers << endl;
	 
	  
	}
	else
	{
	  cout << "null" << endl;
	  return 0;
	}
	
	vector<JobInfo> VJI,*pVJI;
	pVJI = pCgiXMLObj->GetJobInfo();
	//memcpy(&VJI, pVJI, sizeof(JobInfo)*MaxJob);
	//cout << "VJI.size()" << VJI.size() << endl;
	vector<JobInfo>::iterator iter;
	int index = 0;
	for( iter = pVJI->begin(); iter != pVJI->end();index++, iter++)
	{
	    cout << (*iter).Name << " " << (*iter).ID  << " " << (*iter).Percent << endl;
	}
	delete pCgiXMLObj;
	pCgiXMLObj = NULL;
	return 1;
	//BUG::Segmentation fault (core dumped) gdb will find vector<JobInfo> VJI,*pVJI after the function end;
}
