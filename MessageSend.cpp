#include "MessageSend.h"

int MessageSend_btpd(int KeyIn , const void *MessageToSend )
{
	int qid;
	key_t key;
	struct message msg;
	
	if((key=ftok(".", KeyIn ))==-1)
	{
		perror("ftok");
		return 0;
	}

	if((qid=msgget(key,IPC_CREAT|0666))==-1)
	{
		perror("msgget");
		return 0;
	}

	printf("Open queue %d\n",qid);	
	
	memcpy( msg.msg_text , MessageToSend , BUFFER_SIZE_SEND );
	
	msg.msg_type=getpid();//can enum your own message_type
	
	if((msgsnd(qid,&msg,strlen(msg.msg_text),0))<0)
	{
		perror("message posted");
		return 0;;
	}
	printf("\nmsg send: id  %s  compelete\n ",msg.msg_text);
	
	return -1;
}
