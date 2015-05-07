#ifndef __GUI_H__
	#define __GUI_H__
		
	typedef void (*GUI_Callback)(int event_code,void* item_data);	
	typedef void (*GUI_RedrawFunc)(void* item_data);		
	typedef int (*GUI_ItemEventDecoder)(int x,int y,int systemEvent,void *item_data);
	
	typedef struct gui_item_def	{
		void* data;
		GUI_Callback callBack;
		GUI_RedrawFunc redrawFunc;
		GUI_ItemEventDecoder eventDecoder;
	} GUI_ItemDef_t;

	typedef struct gui_item_button_data{
		int estado;
		int posX;
		int posY;
		char * text;
	}	GUI_Item_Button_Data_t;
	
	void GUI_Button_RedrawFunc(void* item_data);		
  int GUI_Button_ItemEventDecoder(int x,int y,int systemEvent,void *item_data);
	
	
	#define GUI_Button_Def(Name,Callback,posX,posY,Text)  GUI_Item_Button_Data_t Name##_Data={0,posX,posY,Text}; \
					GUI_ItemDef_t Name ={(void*)&Name##_Data,Callback,&GUI_Button_RedrawFunc,&GUI_Button_ItemEventDecoder};  
	
#endif

