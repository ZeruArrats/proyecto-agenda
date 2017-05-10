#pragma once
#include <windows.h>
#include <CommCtrl.h>
#include "resource.h"
#include "procs.h"
#include "funcs.h"

BOOL CALLBACK AddProc(HWND, UINT,WPARAM, LPARAM);
BOOL CALLBACK EventProc(HWND, UINT,WPARAM, LPARAM);
BOOL CALLBACK NewEvProc(HWND, UINT,WPARAM, LPARAM);

contact *head=NULL;//*aux=NULL;  

BOOL CALLBACK WinProc(HWND hwnd, UINT msg,WPARAM wParam, LPARAM lParam){
	HWND hList,hEdit,hErase;
	HANDLE hPlay,hPause,hStop,hSearch;
	HINSTANCE hInstance;
	contact *aux=NULL, *srch=NULL,*srch_head=NULL;
	int count;
	hInstance = (HINSTANCE)lParam;
	hList=GetDlgItem(hwnd,IDC_LIST);
	hEdit=GetDlgItem(hwnd,IDC_EDIT);
	hErase=GetDlgItem(hwnd,IDC_ERASE);
	hPlay=LoadImage(hInstance,"play.bmp",IMAGE_BITMAP,15,15,LR_LOADFROMFILE);
	hPause=LoadImage(hInstance,"pause.bmp",IMAGE_BITMAP,15,15,LR_LOADFROMFILE);
	hStop=LoadImage(hInstance,"stop.bmp",IMAGE_BITMAP,15,15,LR_LOADFROMFILE);
	hSearch=LoadImage(hInstance,"search.bmp",IMAGE_BITMAP,15,15,LR_LOADFROMFILE);
	count=SendMessage(hList,LB_GETCOUNT,0,0);
	mciSendString("open c:\\sonido.mp3 alias alarma", NULL, 0, 0);
	if(count==0){
		EnableWindow(hEdit,false);
		EnableWindow(hErase,false);
	}
	int sel,len; char caux[MAX_PATH],str[MAX_PATH],*pstr=NULL;
	
	switch(msg){
	case WM_INITDIALOG:
		EndDialog((HWND)lParam,0);				//Cerrar la ventana login
		head=cargar();							//cargar regresa el puntero al inicio de la lista
		//Inicializar el ListBox con los datos de la lista
		SendMessage(hList,LB_SETCOLUMNWIDTH,200,0);
		upd_LB(hList,head);						//Actualizar ListBox
		SendDlgItemMessage(hwnd,IDC_CAT,CB_ADDSTRING,0,(LPARAM)"Todos");
		for(int i=0;i<3;i++) SendDlgItemMessage(hwnd,IDC_CAT,CB_ADDSTRING,0,(LPARAM)Cat[i]);
		SendDlgItemMessage(hwnd,IDC_CAT,CB_SETCURSEL,(WPARAM)0,0);
		SendDlgItemMessage(hwnd,IDC_PLAY,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)(HANDLE)hPlay);
		SendDlgItemMessage(hwnd,IDC_PAUSE,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)(HANDLE)hPause);
		SendDlgItemMessage(hwnd,IDC_STOP,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)(HANDLE)hStop);
		SendDlgItemMessage(hwnd,IDC_BT_SRCH,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)(HANDLE)hSearch);
		return true;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case ID_ARCHIVO_AGREGAR:
			DialogBoxParam(0,MAKEINTRESOURCE(IDD_ADD),hwnd,(DLGPROC)AddProc,(LPARAM)0);
			upd_LB(hList,head);					//Actualizar ListBox
			return true;
		case ID_ARCHIVO_EVENTOS:
			DialogBox(0,MAKEINTRESOURCE(IDD_EVENT),hwnd,(DLGPROC)EventProc);
			return true;
		case ID_ARCHIVO_SALIR:
			EndDialog(hwnd,0);
			return true;
		case IDC_ADD:
			DialogBoxParam(0,MAKEINTRESOURCE(IDD_ADD),hwnd,(DLGPROC)AddProc,(LPARAM)0);
			upd_LB(hList,head);					//Actualizar ListBox
			return true;
		case IDC_EDIT:
			sel=SendMessage(hList,LB_GETCURSEL,0,0);
			SendMessage(hList,LB_GETTEXT,(WPARAM)sel,(LPARAM)(LPCTSTR)caux);
			aux=find(caux,head);
			DialogBoxParam(0,MAKEINTRESOURCE(IDD_ADD),hwnd,(DLGPROC)AddProc,(LPARAM)aux);
			return true;
		case IDC_ERASE:
			sel=SendMessage(hList,LB_GETCURSEL,0,0);
			SendMessage(hList,LB_GETTEXT,(WPARAM)sel,(LPARAM)(LPCTSTR)caux);
			aux=find(caux,head);
			head=del_cont(aux,head);
			upd_LB(hList,head);
			return true;
		case IDC_ED_SRCH:
			switch(HIWORD(wParam)){
			case EN_CHANGE:				
				len=SendDlgItemMessage(hwnd,IDC_ED_SRCH,EM_LINELENGTH,(WPARAM)-1,0);
				count=SendMessage(hList,LB_GETCOUNT,0,0);
				if(len>0&&count!=0){
					GetDlgItemText(hwnd,IDC_ED_SRCH,str,50);
					aux=head;
					while(aux!=NULL){
						int i=0;
						for(i=0;i<len;i++)	caux[i]=aux->name[i];
						caux[i]='\0';
						if(!strcmp(caux,str)) {
							if(srch_head==NULL) srch_head=add_cont(aux);
							else{
								srch=srch_head;
								while(srch->next!=NULL) srch=srch->next;
								srch->next=add_cont(aux);
								srch->next->last=srch;
							}
						}
						aux=aux->next;
					}
					upd_LB(hList,srch_head);
					cleanMem(srch_head);
				}else upd_LB(hList,head);
				return true;
			}
			return true;
		case IDC_CAT:
			switch(HIWORD(wParam)){
			case CBN_SELCHANGE:
				sel=SendDlgItemMessage(hwnd,IDC_CAT,CB_GETCURSEL,0,0);
				if(sel!=0){
					SendDlgItemMessage(hwnd,IDC_CAT,CB_GETLBTEXT,sel,(LPARAM)caux);
					aux=head;
					while(aux!=NULL){
						if(!strcmp(caux,aux->cat)){
							if(srch_head==NULL) srch_head=add_cont(aux);
							else{
								srch=srch_head;
								while(srch->next!=NULL) srch=srch->next;
								srch->next=add_cont(aux);
								srch->next->last=srch;
							}
						}
						aux=aux->next;
					}
					upd_LB(hList,srch_head);
					cleanMem(srch_head);
				}else upd_LB(hList,head);
				return true;
			}
			return true;
		case IDC_LIST:
			switch(HIWORD(wParam)){
			case LBN_SELCHANGE:
				sel=SendMessage(hList,LB_GETCURSEL,0,0);
				SendMessage(hList,LB_GETTEXT,(WPARAM)sel,(LPARAM)(LPCTSTR)caux);
				aux=find(caux,head);
				if(aux!=NULL){
					loadImg(hwnd,aux->img);
					pstr=concat_data(aux);
					strcpy(caux,pstr);
					SendDlgItemMessage(hwnd,IDC_DATA,WM_SETTEXT,0,(LPARAM)caux);
					SetDlgItemText(hwnd,IDC_SONG,aux->song);
				}
				EnableWindow(hEdit,true);
				EnableWindow(hErase,true);
				return true;
			}
			return true;
		case IDC_PLAY:
			count=SendDlgItemMessage(hwnd,IDC_SONG,EM_LINELENGTH,-1,0);
			GetDlgItemText(hwnd,IDC_SONG,caux,MAX_PATH);
			if(count>0) loadSong("play",caux);
			return true;
		case IDC_PAUSE:
			count=SendDlgItemMessage(hwnd,IDC_SONG,EM_LINELENGTH,-1,0);
			GetDlgItemText(hwnd,IDC_SONG,caux,MAX_PATH);
			if(count>0) loadSong("pause",caux);
			return true;
		case IDC_STOP:
			count=SendDlgItemMessage(hwnd,IDC_SONG,EM_LINELENGTH,-1,0);
			GetDlgItemText(hwnd,IDC_SONG,caux,MAX_PATH);
			if(count>0) loadSong("close",caux);
			return true;
		}
		return true;
	case WM_CLOSE:
		EndDialog(hwnd,0);
		return true;
	case WM_DESTROY:
		guardar(head);
		PostQuitMessage(0);
		
		return true;
	}
	return false;
}

BOOL CALLBACK LogProc(HWND hwnd, UINT msg,WPARAM wParam, LPARAM lParam){
	switch(msg){
	case WM_INITDIALOG:
		return true;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDC_SUBMIT:
			DialogBoxParam(0,MAKEINTRESOURCE(IDD_MAIN),NULL,(DLGPROC)WinProc,(LPARAM)hwnd);
			return true;
		}
		return true;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return true;
	case WM_DESTROY:
		
		PostQuitMessage(0);
		return true;

	}
	return false;
}

BOOL CALLBACK AddProc(HWND hwnd, UINT msg,WPARAM wParam, LPARAM lParam){
	LPSTR pstr; char caux[MAX_PATH];
	static bool edit=false;
	static contact *aux;
	switch(msg){
	case WM_INITDIALOG:
		SendDlgItemMessage(hwnd,IDC_NAME,EM_SETLIMITTEXT,(WPARAM)50,0);
		SendDlgItemMessage(hwnd,IDC_DIR,EM_SETLIMITTEXT,(WPARAM)50,0);
		SendDlgItemMessage(hwnd,IDC_TEL,EM_SETLIMITTEXT,(WPARAM)10,0);
		SendDlgItemMessage(hwnd,IDC_CEL,EM_SETLIMITTEXT,(WPARAM)10,0);
		SendDlgItemMessage(hwnd,IDC_EMAIL,EM_SETLIMITTEXT,(WPARAM)50,0);
		SendDlgItemMessage(hwnd,IDC_IMG,EM_SETLIMITTEXT,(WPARAM)MAX_PATH,0);
		SendDlgItemMessage(hwnd,IDC_SONG,EM_SETLIMITTEXT,(WPARAM)MAX_PATH,0);
		for(int i=0;i<3;i++) SendDlgItemMessage(hwnd,IDC_CAT,CB_ADDSTRING,0,(LPARAM)Cat[i]);
		if(lParam!=0){
			edit=true;
			aux=(contact*)lParam;
			SetDlgItemText(hwnd,IDC_NAME,aux->name);
			SetDlgItemText(hwnd,IDC_DIR,aux->dir);
			SetDlgItemText(hwnd,IDC_TEL,aux->tel);
			SetDlgItemText(hwnd,IDC_CEL,aux->cel);
			SetDlgItemText(hwnd,IDC_EMAIL,aux->email);
			SetDlgItemText(hwnd,IDC_IMG,aux->img);
			SetDlgItemText(hwnd,IDC_SONG,aux->song);
			//
		}else edit=false;
		return true;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDC_ADD:
			if(SendDlgItemMessage(hwnd,IDC_NAME,EM_LINELENGTH,(WPARAM)-1,0)>0){
				if(!edit){										//Si es la opcion agregar
					if(head==NULL){								//Si la lista esta vacia
						head=add_cont(hwnd,NULL);				//Crear nodo
						
					}else{										//Si la lista tiene elementos
						aux=head;
						while(aux->next!=NULL) aux=aux->next;	//Ir al ultimo nodo 
						aux->next=add_cont(hwnd,NULL);			//Crear nodo
						aux->next->last=aux;						//"El anterior del siguiente es el actual"
					}
				} else aux=add_cont(hwnd,aux);
				EndDialog(hwnd,0);							//Cierra el dialogo
			}else MessageBox(hwnd,"Campo nombre es obligatorio","Advertencia",IDOK);
			return true;
		case IDC_BROWSE1:
			pstr=CommDlg(hwnd,0);
			if(pstr!=NULL){
				strcpy(caux,pstr);
				SetDlgItemText(hwnd,IDC_SONG,caux);
			}
			return true;
		case IDC_BROWSE2:
			pstr=CommDlg(hwnd,1);
			if(pstr!=NULL){
				strcpy(caux,pstr);
				SetDlgItemText(hwnd,IDC_IMG,caux);
				loadImg(hwnd,caux);
			}
			return true;
		}
		return true;
	case WM_CLOSE:
		EndDialog(hwnd,0);
		return true;
	case WM_DESTROY:
		EndDialog(hwnd,0);
		return true;

	}
	return false;
}

BOOL CALLBACK EventProc(HWND hwnd, UINT msg,WPARAM wParam, LPARAM lParam){
	HWND hDTP;
	HDC hDC;
	PAINTSTRUCT Ps;
	char pstr[11], aux[MAX_PATH];
	hDTP=GetDlgItem(hwnd,IDC_DATE);
	switch(msg){
	case WM_INITDIALOG:
		GetWindowText(hDTP,pstr,10);
		getDtp(pstr);
		return true;
	case WM_PAINT:
		hDC=BeginPaint(hwnd,&Ps);
		Draw_Cal(hDC,hwnd);
		EndPaint(hwnd,&Ps);
		return true;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDC_ADD:
			DialogBox(0,MAKEINTRESOURCE(IDD_NW_EV),hwnd,(DLGPROC)NewEvProc);
			return true;
		case IDC_EXIT:
			EndDialog(hwnd,0);
			return true;
		}
		return true;
	case WM_CLOSE:
		EndDialog(hwnd,0);

		return true;
	case WM_DESTROY:
		EndDialog(hwnd,0);
		return true;

	}
	return false;
}

BOOL CALLBACK NewEvProc(HWND hwnd, UINT msg,WPARAM wParam, LPARAM lParam){
	HWND hList;
	hList=GetDlgItem(hwnd,IDC_CONT);
	switch(msg){
	case WM_INITDIALOG:
		upd_LB(hList,head);
		return true;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDC_CANCEL:
			EndDialog(hwnd,0);
			return true;
		}
		return true;
	case WM_CLOSE:
		EndDialog(hwnd,0);

		return true;
	case WM_DESTROY:
		EndDialog(hwnd,0);
		return true;

	}
	return false;
}