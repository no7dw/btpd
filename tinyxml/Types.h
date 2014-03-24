#ifndef __TYPES_H
#define __TYPES_H


//#define MAXARRAYSIZE 1000
#define ExamNum 10 
#define MAXQCONTENT 500
#define MAXPATH 200
#define MAXID 10 

typedef enum {
	CHOICE_STYLE_OTHER,
	CHOICE_STYLE_TEXT,
	CHOICE_STYLE_IMG

}ChoiceStyle;


typedef struct {
	char CIID;
	ChoiceStyle EnumChoiceStyle;		
	char ChoicePath[MAXPATH];	
	char ChoiceContent[MAXQCONTENT];

}ChoiceItem;

typedef struct {
	char QID[MAXID];	 
	char QContent[MAXQCONTENT];
	char ContentAudio[MAXPATH];
	ChoiceItem ChoiceItemA;			
	ChoiceItem ChoiceItemB;			
	ChoiceItem ChoiceItemC;			
	ChoiceItem ChoiceItemD;		
	char Answer[1];

}QuestionItem;

#endif