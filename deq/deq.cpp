#include "deq.h"
#include "../Util.h"
//initialize static





Deq::Deq(int grade)//private
{		
	m_DeqGrade = grade;	
	sprintf(m_queue_path , "/tmp/hdd/volumes/HDD1/queue_%d.xml", m_DeqGrade);	//QUEUEFILEPATH_1234	
	//printf("\nm_queue_path = %s \n", m_queue_path );	
	pBookListXmlObj= new BookListXml();
	pBookListXmlObj->LoadXml(m_queue_path);	
	QueueSize = pBookListXmlObj->GetListSize();
	QCI.clear();
	QCI = *(pBookListXmlObj->GetBookListQueue());
	//Deq::QueueAllSize the value should be initialize here after reading the 4 xml files
}

Deq::~Deq()//private
{
	delete pBookListXmlObj;
	pBookListXmlObj = NULL;
}


//should pervent the repeat Item
int Deq::AddItem(CourseInfo CI)
{
	DQCI::iterator pdeque;	
	for(pdeque = QCI.begin(); pdeque != QCI.end(); pdeque++ )
	{
		if( strcmp( (*pdeque).Hash , CI.Hash)==0)
		{
			return -1;//已有
		}		
	}
	UTIL_Debug_Print(__FILE__,__LINE__,__func__,"AddItem to Dequeue");
	
	QCI.push_back(CI);		
	
//	QueueAllSize = pBookListXmlObj->GetListSize() + 1;//static
	//BUG:: need to know wheteher the torrent is right 
	//then if succeed +1
	//else do nothing
	
//	cout << "QueueAllSize "<< QueueAllSize << endl;
	return pBookListXmlObj->AddItem(CI);
}

CourseInfo* Deq::GetFirstCourseInfo()
{
    //printcontents();
	if(QCI.size()>0)
	{
		printf("QCI[0]: %s\t%s\t%s\t\n",QCI[0].ID,QCI[0].Name,QCI[0].Hash);
		return &QCI[0];
	}
	else	
		return NULL;
}

DQCI* Deq::GetDequeCourseInfo()
{
	return &QCI;
}

void Deq::RemoveItem()
{
	QCI.pop_front();
//	QueueAllSize = pBookListXmlObj->GetListSize() - 1;//static
}

int Deq::RemoveItem(const string &CourseID)
{
	printf("\n\t @@@ \t[%s::%s() %d ] CourseID : %s !!\n",__FILE__,__func__,__LINE__,CourseID.c_str());
	
	DQCI::iterator pdeque;
	for(pdeque = QCI.begin(); pdeque != QCI.end(); pdeque++ )
	{
		if (strcmp((*pdeque).ID , CourseID.c_str())==0) 
		{
			QCI.erase(pdeque);			
			pBookListXmlObj->RemoveItem(CourseID);	
//			QueueAllSize = pBookListXmlObj->GetListSize() - 1;//static
//			cout << "QueueAllSize "<< QueueAllSize << endl;
			return 0;
		}
	}
	return -1;
}
		
void Deq::printcontents()
{
	DQCI::iterator pdeque;
	cout <<  endl << "printcontents" << endl << "Deq Grade = " << m_DeqGrade << " size = " << QCI.size()<< endl;
	cout << "The output is:"<< endl;
	for(pdeque = QCI.begin(); pdeque != QCI.end(); pdeque++ )
	{
		cout << (*pdeque).ID << " "<< (*pdeque).Name << " "<< (*pdeque).Level << " "<< (*pdeque).Path << endl;
	}
	cout << endl;
}
