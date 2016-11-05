/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.28                          *
*        Compiled Jan 30 2015, 16:41:06                              *
*        (c) 2015 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "counter.h"
#include "clock.h"
#include "menu.h"
#include "heading.h"
#include "notepad.h"
#include "global_inc.h"


char buffer[100];//__attribute((section(".ExRam")));

extern volatile int sec;
extern volatile int sec2;

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "notepad", ID_WINDOW_0, 0, 0, 320, 240, 0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect, "Multiedit", ID_MULTIEDIT_0, 15, 33, 287, 194, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 14, 8, 80, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "0:log.txt", ID_TEXT_1, 75, 8, 227, 20, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId)
  {
//  case WM_PAINT:
//  {
//
//
//  }
  case WM_INIT_DIALOG:
    //
    // Initialization of 'notepad'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, 0x008A7B7B);
    //
    // Initialization of 'Multiedit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
    MULTIEDIT_SetText(hItem, "ERROR");
    MULTIEDIT_SetReadOnly(hItem,0);
    MULTIEDIT_SetTextColor(hItem, MULTIEDIT_CI_EDIT, 0x000EF801);
    MULTIEDIT_SetBkColor(hItem, MULTIEDIT_CI_EDIT, GUI_BLACK);
//    MULTIEDIT_SetBufferSize(hItem, 150);
//    MULTIEDIT_SetMaxSize(hItem, 150);
    MULTIEDIT_SetFont(hItem, GUI_FONT_8X10_ASCII);
    MULTIEDIT_EnableBlink(hItem,1,0);
//    MULTIEDIT_SetAutoScrollV(hItem,1);
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetText(hItem, "Filename");
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id)
    {
    case ID_MULTIEDIT_0: // Notifications sent by 'Multiedit'
      switch(NCode)
      {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       Createnotepad
*/

WM_HWIN Createnotepad(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}
void Notepad( void * pvParameters)
{
	WM_HWIN hItem;
	WM_HWIN hWinnotepad;
	hWinnotepad = Createnotepad();
	int toread=0;
	int k=0;
	FIL fsrc;
	FRESULT f=0;
	taskENTER_CRITICAL();
	f=f_open(&fsrc,"0:log.txt",FA_READ|FA_OPEN_EXISTING);
	toread=f_size(&fsrc);
	if(Menu_Handle!=NULL)vTaskDelete(Menu_Handle);
	if(Heading_Handle!=NULL)vTaskDelete(Heading_Handle);
	taskEXIT_CRITICAL();

	while(1)
	{
		vTaskDelay(100);

		if(GUI_PID_IsPressed())
		{
			if(sec2>1000)
			{
//				xTaskCreate(Menu,(char const*)"Menu",512,NULL,6,&Menu_Handle);
				xTaskCreate(Heading_Task,(char const*)"Heading",512,NULL, 6, &Heading_Handle);
			}
		}else
		{
			sec2=0;
		}

		  if(toread>0)
		  {
			  taskENTER_CRITICAL();
			  f_read(&fsrc,buffer,100,&k);
			  hItem = WM_GetDialogItem(hWinnotepad, ID_MULTIEDIT_0);
			  MULTIEDIT_AddText(hItem,buffer);
			  toread-=k;
			  taskEXIT_CRITICAL();
		  }
		  else
		  {
			  f_close(&fsrc);
		  }

	}
}
