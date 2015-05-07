#include "GUI.h"

GUI_ItemDef_t windowItems[15];
int numberOfItems;

void windowRedraw(void){
	int i;
	for(i=0; i<numberOfItems; i++){
		windowItems[i].redrawFunc(windowItems[i].data);
	}
}

#define GUI_ITEM_EVENT_NOEVENT 0

int posX, posY;

void callbackManager(int systemEvent){
	int i;
	int tempItemEvent;
	
	for(i=0;i<numberOfItems;i++){
		tempItemEvent=windowItems[i].eventDecoder(posX,posY,systemEvent,windowItems[i].data);
	  if(tempItemEvent!=GUI_ITEM_EVENT_NOEVENT){
			windowItems[i].callBack(tempItemEvent,windowItems[i].data);
		}	
	}
	
}


void GUI_Button_RedrawFunc(void* item_data){
	int i;
	i++;
}
int GUI_Button_ItemEventDecoder(int x,int y,int systemEvent,void *item_data){
	return 0;
}


