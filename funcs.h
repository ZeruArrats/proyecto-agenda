#pragma once
#include <windows.h>
#include <fstream>
#include <CommDlg.h>
#include <ctime>
#include "resource.h"
#include "struct.h"

using namespace std;

// Guarda el contnido de la lista en archivo binario 
void cleanMem(contact *head){
	contact *aux=NULL;
	aux=head;
	if(head!=NULL)
		while(aux->next!=NULL){
			aux=aux->next;
			delete aux->last;
		}
		delete aux;
}
void guardar(contact *head){
	ofstream save; 
	contact *aux=NULL;
	aux=head;
	save.open("save/file.bin", ios::binary|ios::trunc);
		if(save.is_open()){
			if(head!=NULL) 
			while(aux!=NULL){
				save.write((char*)aux->name,sizeof(aux->name));
				save.write((char*)aux->dir,sizeof(aux->dir));
				save.write((char*)aux->email,sizeof(aux->email));
				save.write((char*)aux->tel,sizeof(aux->tel));
				save.write((char*)aux->cel,sizeof(aux->cel));
				save.write((char*)aux->img,sizeof(aux->img));
				save.write((char*)aux->cat,sizeof(aux->cat));
				save.write((char*)aux->song,sizeof(aux->song));
				aux=aux->next;
			}
		}
		save.close();
		cleanMem(head);

}
//Crea una lista a partir de los datos de un archivo
contact* cargar(){
	ifstream load; 
	contact *head=NULL,*aux=NULL,*last=NULL, *nw=NULL;
	load.open("save/file.bin", ios::binary|ios::_Nocreate);
	if(load.is_open()){
		load.peek();
		aux=head;
		while(!load.eof()){
			nw=new contact;
			ZeroMemory(nw,sizeof(contact));
			load.read((char*)nw->name,sizeof(nw->name));
			load.read((char*)nw->dir,sizeof(nw->dir));
			load.read((char*)nw->email,sizeof(nw->email));
			load.read((char*)nw->tel,sizeof(nw->tel));
			load.read((char*)nw->cel,sizeof(nw->cel));
			load.read((char*)nw->img,sizeof(nw->img));
			load.read((char*)nw->cat,sizeof(nw->cat));
			load.read((char*)nw->song,sizeof(nw->song));
			if(head!=NULL){
				aux = nw;
				aux->last=last;								//puntero al anterior
				last->next=aux;								//puntero del anterior al siguiente
				last=aux;									//el anterior ahora es el actual
				aux=aux->next;								//el actual ahora es el siguiente
			}else{
				head = nw;
				head->next=NULL;
				head->last=NULL;
				last=head;
			}
			load.peek();
		}
	}
	load.close();
	return head;
}
//Regresa el puntero a el nodo creado
contact * add_cont(HWND hwnd,contact *aux){
	if(aux==NULL) {
		aux=new contact;
		ZeroMemory(aux,sizeof(contact));
		aux->next=NULL;
		aux->last=NULL;
	}
	GetDlgItemText(hwnd,IDC_NAME,aux->name,MAX_PATH);
	GetDlgItemText(hwnd,IDC_DIR,aux->dir,MAX_PATH);
	GetDlgItemText(hwnd,IDC_SONG,aux->song,MAX_PATH);
	GetDlgItemText(hwnd,IDC_IMG,aux->img,MAX_PATH);
	GetDlgItemText(hwnd,IDC_TEL,aux->tel,MAX_PATH);
	GetDlgItemText(hwnd,IDC_CEL,aux->cel,MAX_PATH);
	GetDlgItemText(hwnd,IDC_EMAIL,aux->email,MAX_PATH);
	GetDlgItemText(hwnd,IDC_CAT,aux->cat,MAX_PATH);
	//Retornar puntero al nodo creado
	return aux;
}
contact *add_cont(contact *aux){
	contact *nw;
	if(aux!=NULL){
		nw=new contact;
		*nw=*aux;
		nw->next=NULL;
		nw->last=NULL;
	}
	return nw;
}
contact *del_cont(contact *aux,contact *head){
	if(aux==head){
		if(head->next!=NULL){
			head=head->next;
			head->last=NULL;
			delete aux;
		} else {
			delete head;
			head=NULL;
		}
	}else if(aux->next!=NULL){
		aux->last->next=aux->next;
		aux->next->last=aux->last;
		delete aux;
	}else{
		aux->last->next=NULL;
		delete aux;
	}
	return head;
}
//concatena los datos principales de un nodo en una sola cadena
char * concat_data(contact *aux){
	char str[MAX_PATH];
	ZeroMemory(str,sizeof(str));
	strcat(str,"Categoría: ");
	strcat(str,aux->cat);
	strcat(str,"\r\n");
	strcat(str,"Dirección: ");
	strcat(str,aux->dir);
	strcat(str,"\r\n");
	strcat(str,"Telefono: ");
	strcat(str,aux->tel);
	strcat(str,"\r\n");
	strcat(str,"Celular: ");
	strcat(str,aux->cel);
	strcat(str,"\r\n");
	strcat(str,"Email: ");
	strcat(str,aux->email);
	strcat(str,"\0");
	//retornar puntero a cadena
	return str;
}
//Busca el contacto a partir de su nombre
contact *find(char *name,contact *head){
	contact *aux;
	aux=head;
	while(aux!=NULL){
		if(!strcmp(name,aux->name))
			return aux;
		aux=aux->next;
	}
	return NULL;
}

//Actualiza el listbox con los datos de la lista
void upd_LB(HWND hList,contact *head){
	contact *aux;
	char *pstr,caux[200];
	SendMessage(hList,LB_RESETCONTENT,0,0);
	aux=head;
	while(aux!=NULL){
		//pstr=concat_data(aux);
		pstr=aux->name;
		strcpy(caux,pstr);
		SendMessage(hList,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)caux);
		aux=aux->next;
	}
}

//Dibuja el calendario
void Draw_Cal(HDC hDC,HWND hwnd){
	HRGN hCal[6][7];
	HWND hAux;
	/*HBRUSH Brsh;
	Brsh=CreateSolidBrush((COLORREF)WHITE_BRUSH);*/
	int x1=20,x2=70,y1=60,y2=80;
	for(int j=0;j<6;j++){
		for(int i=0; i<7;i++){
			hCal[j][i]=CreateRectRgn(x1, y1, x2, y2);
			Rectangle(hDC,x1, y1, x2, y2);
			TextOut(hDC,x1+3,y1+3,"1",1);
			x1+=55; x2+=55;
		}
		x1=20; x2=70; y1+=25; y2+=25;
	}
}

//initDate
tm *getDtp(char *dtp){
	time(&actual_time);
	timeinfo = localtime(&actual_time);
	string day,month,year;
	string aux; int pos1,pos2;
	tm aux_tm; double diff;

	ZeroMemory(&aux_tm,sizeof(aux_tm));
	aux=dtp;
	pos1=aux.find_first_of('/');
	day=aux.substr(0,pos1);
	pos2=aux.find_first_of('/',pos1+1);
	month=aux.substr(pos1+1,pos2-2);
	year=aux.substr(pos2+1);
	aux_tm.tm_mday=atoi(day.c_str());
	aux_tm.tm_mon=atoi(month.c_str());
	aux_tm.tm_year=atoi(year.c_str())-1900;

	diff=difftime(actual_time,mktime(&aux_tm));
	diff/=86400;
	return &aux_tm;
}

LPSTR CommDlg(HWND hdlg,int type){
	//Objeto OpenFileName
	OPENFILENAME ofn;
	char filename[MAX_PATH];
	HANDLE hFile;
	ZeroMemory(&ofn,sizeof(ofn));
	ZeroMemory(&filename,MAX_PATH);
	ofn.lStructSize=sizeof(ofn);
	ofn.hwndOwner=hdlg;
	ofn.lpstrFile=filename;
	ofn.nMaxFile=sizeof(filename);
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	switch(type){
	case 0:
		ofn.lpstrFilter="Archivos MP3\0*.mp3\0"; 
		break;
	case 1:
		ofn.lpstrFilter="Mapa de bits\0*.bmp\0";
		break;
	}
	if(GetOpenFileName(&ofn))
		return ofn.lpstrFile;
	else{
		MessageBox(0,"No eligio archivo", "Aviso", MB_OK|MB_ICONINFORMATION);
		return NULL;
	}
}
//Pendiente
void loadSong(char *comm,char *file){
	char caux[MAX_PATH];
	strcpy(caux,"");
	strcat(caux,"open \"");
	strcat(caux,file);
	strcat(caux,"\" alias mp3");
	mciSendString(caux, NULL, 0, 0);
	strcpy(caux,comm);
	strcat(caux," mp3");
	mciSendString(caux, NULL, 0, 0);

}
void loadImg(HWND hwnd,char *file){
	HBITMAP bmp1,bmp2;
	//bmp1= (HBITMAP)SendDlgItemMessage(hdlg, IDC_BMP, STM_GETIMAGE, IMAGE_BITMAP, 0);
	bmp2=(HBITMAP)LoadImage(NULL,file,IMAGE_BITMAP,115,115,LR_LOADFROMFILE);
	SendDlgItemMessage(hwnd, IDC_BMP, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);
}