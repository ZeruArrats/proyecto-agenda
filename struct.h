#pragma once
#include <windows.h>
#include <ctime>


char *Month[12]={ "ENERO", "FEBRERO", "MARZO", "ABRIL", "MAYO", "JUNIO", "JULIO", "AGOSTO","SEPTIEMBRE", "OCTUBRE", "NOVIEMBRE", "DICIEMBRE" };
int DPM[12]={31,28,31,30,31,30,31,31,30,31,30,31};
char *Weekday[7]={"Domingo","Lunes","Martes","Miercoles","Jueves","Viernes","Sabado"};
char *Cat[3]={"Amigos","Familia","Trabajo"};

time_t actual_time;
tm *timeinfo;

//Estructura contacto 
struct contact{
	char name[51];
	char dir[51];
	char email[51];
	char tel[11];
	char cel[11];
	char img[MAX_PATH];
	char cat[31];
	char song[MAX_PATH];
	contact *next, *last;
};
//Estructura fecha
struct datepick{
	char day[3];
	char mon[3];
	char year[5];
	char hour[3];
	char min[3];
};
//Estructura Evento
struct evnt{
	contact *cont;
	datepick time;
	char desc[MAX_PATH];
	int alarm;
	char alrmName[MAX_PATH];
	bool seen;
	evnt *next;
};
//Estructura imagenes
struct img{
	char file[MAX_PATH];
	img *next;
};