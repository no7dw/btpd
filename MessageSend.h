#ifndef __MESSAGE_SEND_H
#define __MESSAGE_SEND_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "client.h"

#define BUFFER_SIZE_SEND 100 
#define BTPD_MESSAGE_KEY 333	//AVTrainning
#define BTPD_MESSAGE_KEY_ViewPicture BTPD_MESSAGE_KEY+ViewPicture	//ViewPicture
#define BTPD_MESSAGE_KEY_LyricSpeak BTPD_MESSAGE_KEY+LyricSpeak       //LyricSpeak	
#define BTPD_MESSAGE_KEY_SongSing BTPD_MESSAGE_KEY+SongSing		//SongSing

struct message{
	long msg_type;
	char msg_text[BUFFER_SIZE_SEND];
};

int MessageSend_btpd(int KeyIn , const void *MessageToSend );


#endif
