#include "JICIXml.h"
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

	JICIXml *pJICIXml= new JICIXml();
	pJICIXml->LoadXml(argv[1]);
/*
	JobInfo_CourseInfo *pJICI;
	pJICI = pJICIXml->GetJobInfo_CourseInfo("3");
	
	if (pJICI != NULL)
	{
	  cout<< pJICI->JI.ID << endl;
	  cout<< pJICI->JI.Priority << endl;
	  cout<< pJICI->JI.Partition << endl;
	  cout<< pJICI->JI.Space << endl;
	  cout<< pJICI->JI.Name << endl;
	  cout<< pJICI->JI.Status << endl;
	  
	  cout<< pJICI->JI.Size << endl;
	  cout<< pJICI->JI.Downloaded << endl;
	  cout<< pJICI->JI.TimeLeft << endl;
	  cout<< pJICI->JI.Percent << endl;
	  cout<< pJICI->JI.DwRate << " kB/s"<< endl;

	  cout<< pJICI->JI.UpRate << " kB/s"<< endl;
	  cout<< pJICI->JI.Npeers << endl;		  
	}
	else
	{
	  cout << "null" << endl;
	  return 0;
	}	
	
	vector<JobInfo_CourseInfo> VJI,*pVJI;
	pVJI = pJICIXml->GetJobInfo_CourseInfo();

	vector<JobInfo_CourseInfo>::iterator iter;
	int index = 0;
	for( iter = pVJI->begin(); iter != pVJI->end();index++, iter++)
	{
	    cout << (*iter).CI.Name << " CI.ID= " << (*iter).CI.ID << " JI.ID= " << (*iter).JI.ID << " JI.Name= " << (*iter).JI.Name << " Size= "  << (*iter).JI.Size << " Percent= " << (*iter).JI.Percent << endl;
	}
	*/

	CgiXML *pCgiXml = new CgiXML();
	pCgiXml->LoadXml(argv[2]);
	
	vector<JobInfo> VJI2;
	VJI2 = *pCgiXml->GetJobInfo();	
	
	pJICIXml->UpdateJobInfo(VJI2);	
	
	delete pCgiXml;
	pCgiXml = NULL;

	delete pJICIXml;
	pJICIXml = NULL;
	
	return 0;
	
}
