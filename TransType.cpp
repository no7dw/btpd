#include "TransType.h"

/*
void CourseInfo2CourseItem(const CourseInfo &CIC , CourseItem &CIS)
{    
    CIS.Grade = CIC.Grade;
    CIS.Level = CIC.Level;
    (CIS.ID).assign( CIC.ID, strlen(CIC.ID) );
    (CIS.Name).assign( CIC.Name, strlen(CIC.Name) );
    (CIS.Path).assign( CIC.Path, strlen(CIC.Path) );
    strcpy(CIS.Hash, CIC.Hash);
}

void CourseItem2CourseInfo(const CourseItem *pCIS , CourseInfo *pCIC)
{
    pCIC->Grade = pCIS->Grade;
    pCIC->Level = pCIS->Level;
    strcpy(pCIC->Hash , pCIS->Hash);
    
    strcpy(pCIC->ID , pCIS->ID.c_str());
    strcpy(pCIC->Name , pCIS->Name.c_str());
    strcpy(pCIC->Path , pCIS->Path.c_str());
}
*/

//a function that change JobInfo to JobInfoC
void JobInfo2JobInfoC(const JobInfo *pJI, JobInfoC *pJIC )
{
	UTIL_Debug_Print(__FILE__,__LINE__,__func__);
	if(pJI!=NULL && pJIC!=NULL)
	{
		//strcpy( pJIC->ID ,pJI->ID.c_str());    //pJIC->ID is already CourseID
		strcpy( pJIC->Name ,pJI->Name.c_str());    
		strcpy( pJIC->Percent ,pJI->Percent.c_str()); 
		printf("\nin JobInfo2JobInfoC pJIC->ID =%s\t pJI->percent= %d \n",pJIC->ID,pJI->Percent.c_str());
		strcpy( pJIC->TimeLeft ,pJI->TimeLeft.c_str());
		pJIC->Downloaded = pJI->Downloaded;
		pJIC->DwRate = pJI->DwRate;
		pJIC->UpRate = pJI->UpRate;    
		UTIL_Debug_Print(__FILE__,__LINE__,__func__);
	}
	else
	{
		if(pJI==NULL)
			UTIL_Debug_Print(__FILE__,__LINE__,__func__,"(pJI==NULL )");
		else//(pJIC==NULL)
			UTIL_Debug_Print(__FILE__,__LINE__,__func__,"(pJIC==NULL )");
	}	
	UTIL_Debug_Print(__FILE__,__LINE__,__func__);
}
