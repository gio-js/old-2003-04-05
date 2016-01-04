#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <alloc.h>
#include <ctype.h>

#define MOUSE 0x33

int xi,yi,xf,yf,bo,col;

char VisMenu();
void PrimaSchermata();
void modifica_tasto(int dx,int dy);
void Tasto(int dx,int dy);
void PTastoGioca (int col1,int col2);
void PTastoIstruzioni (int col1,int col2);
void PTastoScores (int col1,int col2);
void PTastoCredit (int col1,int col2);
void PTastoEsci (int col1,int col2);
void AttivaMouse (int*posx,int *posy);
void Mouse(int stato,int xmin,int ymin,int xmax,int ymax);
union REGS BottoniMouse(int bot);
void MuoviMouse(int m);
void VertMouse(int ymin,int ymax);
void HoriMouse(int xmin,int xmax);
union REGS PrendiXYMouse(void);

void AttivaMouse(int *posx,int *posy)
{
     union REGS rel,xy;
     Mouse(1,5,5,getmaxx(),getmaxy());
     for (;;)
     {
	bo=0;
	rel=BottoniMouse(0);
	xi=xf=rel.x.cx;
	yi=yf=rel.x.dx;

	while (rel.x.ax!=1)
	{
		xy=PrendiXYMouse();
		if (xy.x.cx!=xf || xy.x.dx!=yf)
		{
		 xy=PrendiXYMouse();
		 xf=xy.x.cx;
		 yf=xy.x.dx;
		 bo=1;
		}
		rel=BottoniMouse(0);
	}
	if (bo==1)
	 {
	  *posx = xy.x.cx;
	  *posy = xy.x.dx;
	  break;
	 }
     }
}

void Mouse(int stato,int xmin,int ymin,int xmax,int ymax)
{                               /* inizializza il mouse */
	VertMouse(ymin,ymax);	/* vuole in ingresso il modo di vedere il mouse */
	HoriMouse(xmin,xmax);	/* le coordinate limiti per il mouse */
	MuoviMouse(stato);
}

void MuoviMouse(int m)
{                                       /* vuole in ingresso il modo */
	union REGS regs;                /* di vedere il mouse */
	regs.x.ax = m;			/* 1= si vede la freccia */
	int86(MOUSE, &regs, &regs);     /* 2= non si vede niente */
}

void HoriMouse(int xmin,int xmax)
{                                	/* inserisci i valori di x minimi */
	union REGS regs;		/* e massimi in cui vuoi il mouse */
	regs.x.ax=7;
	regs.x.cx=xmin;
	regs.x.dx=xmax;
	int86(MOUSE, &regs, &regs);
}

void VertMouse(int ymin,int ymax)
{					/* inserisci i valori di y minimi */
	union REGS regs;		/* e massini in cui vuoi il mouse */
	regs.x.ax=8;
	regs.x.cx=ymin;
	regs.x.dx=ymax;
	int86(MOUSE,&regs,&regs);
}

union REGS BottoniMouse(int bot)
{                                       /* inserisci il numero del bottone */
	union REGS regs;                /* 0=sinistra 1=destra 2=centro */
	regs.x.ax=5;                    /* e ritorna un union REGS con */
	regs.x.bx=bot;                  /* in AX 1 se Љ premuto il tasto */
	int86(MOUSE,&regs,&regs);	/* in CX X in cui Љ stato premuto */
	return(regs);			/* in DX Y in cui Љ stato premuto ‚ */
}

union REGS PrendiXYMouse(void)
{                                       /* tiene sempre aggiornate le  */
	union REGS regs;		/* coordinate del mouse */
	regs.x.ax=3;
	int86(MOUSE,&regs,&regs);
	return(regs);
}

void PrimaSchermata()
{
 char carattere;
 int j,i,colore;
 char tetris [20][30] = {"ЫЫЫ ЫЫЫ ЫЫЫ ЫЫЫ  ЫЫЫ ЫЫЫ",
			 " Ы  Ы    Ы  Ы  Ы  Ы  Ы  ",
			 " Ы  ЫЫ   Ы  ЫЫЫ   Ы  ЫЫЫ",
			 " Ы  Ы    Ы  Ы  Ы  Ы    Ы",
			 " Ы  ЫЫЫ  Ы  Ы  Ы ЫЫЫ ЫЫЫ",};
 int colori [20] = {RED,GREEN,YELLOW,MAGENTA,LIGHTGREEN,LIGHTBLUE,CYAN,BLUE,LIGHTMAGENTA,LIGHTRED,LIGHTCYAN};
 int mat = 20;
 int posx [100],posy[100];
 int x,y;
 int ins=0;
 x = 90;
 y = 150;

/*PRIMA SCHERMATA*/

 for(i=0;i<5;i++)       //guardo quanti mattoni ci sono nella schermata
    for(j=0;j<24;j++)
       {
	if (tetris[i][j]=='Ы')
	  {
	   posy[ins]=i;
	   posx[ins]=j;
	   ins++;
	  }
       }
 setcolor(RED);
 for (i=0;i<ins;i++)
  {
   colore = i%11;
   setfillstyle (6,colori[colore]);
   rectangle ((posx[i]*mat)+x+5,(posy[i]*mat)+y+8,(posx[i]*mat)+x+5+mat,(posy[i]*mat)+8+mat+y);
   bar ((posx[i]*mat)+x+7,(posy[i]*mat)+10+y,(posx[i]*mat)+x+3+mat,(posy[i]*mat)+6+mat+y);
  }
 setcolor (GREEN);
 x=80;
 y=300;
 moveto (x,y);
 settextstyle (TRIPLEX_FONT,0,3);
 outtext ("Una produzione esclusiva di Sorgente Giovanni");//fine schermata principale
 delay (2000);
 cleardevice();
}

char VisMenu()
{
 char carattere;
 int j,i,colore;
 int colori [20] = {RED,GREEN,YELLOW,MAGENTA,LIGHTGREEN,LIGHTBLUE,CYAN,BLUE,LIGHTMAGENTA,LIGHTRED,LIGHTCYAN};
 int mat = 20;
 int posx [100],posy[100];
 int x,y;
 int ins=0;

/*SECONDA SCHERMATA: SELEZIONE*/

 mat=20;
 setcolor(RED);
 i=0;
 for (y=0;y<(getmaxy()/mat);y++)
    for(x=0;x<(getmaxx()/mat);x++,i++)
      {
       colore = i%11;            //disegno i mattoni piu piccoloni
       setfillstyle (6,colori[colore]);
       rectangle (x*mat,y*mat,(x*mat)+mat,(y*mat)+mat);
       bar ((x*mat)+2,(y*mat)+2,(x*mat)+mat-2,(y*mat)+mat-2);
      }
 setcolor(RED);
 mat = 25;
 y = 170;
 x = 7;
 for (i=0;i<ins;i++)
  {
   setfillstyle (SOLID_FILL,BLACK);
   rectangle ((posx[i]*mat)+x+5,(posy[i]*mat)+y+8,(posx[i]*mat)+x+5+mat,(posy[i]*mat)+8+mat+y);
   bar ((posx[i]*mat)+x+7,(posy[i]*mat)+10+y,(posx[i]*mat)+x+3+mat,(posy[i]*mat)+6+mat+y);
  }
 Tasto (50,-150);  //alto sx   Gioca       //G
 Tasto (350,-150); //alto dx   Istruzioni  //T
 Tasto (200,-50);  //centro    Scores      //R
 Tasto (50,50);    //basso sx  Credit      //D
 Tasto (350,50);   //basso dx  Esci        //E
 PTastoGioca (RED,BLACK);
 PTastoIstruzioni (RED,BLACK);
 PTastoScores (RED,BLACK);
 PTastoCredit (RED,BLACK);
 PTastoEsci (RED,BLACK);
 carattere = 'Z';
 while (carattere != 'G' && carattere != 'T' && carattere != 'R' && carattere != 'D' && carattere != 'E')
   {
    carattere = getch();
    carattere = toupper(carattere);
   }
 switch(carattere)
   {
    case 'G': modifica_tasto (50,-150);
	      PTastoGioca (12,8);
	      break;
    case 'T': modifica_tasto (350,-150);
	      PTastoIstruzioni (12,8);
	      break;
    case 'R': modifica_tasto (200,-50);
	      PTastoScores (12,8);
	      break;
    case 'D': modifica_tasto (50,50);
	      PTastoCredit (12,8);
	      break;
    case 'E': modifica_tasto (350,50);
	      PTastoEsci (12,8);
	      break;
   }
 return carattere;
}


void Tasto(int dx,int dy) //tasto dentro largo 80 alto 30
{
	setcolor(15);
	rectangle(90+dx,270+dy,170+dx,300+dy);
	setfillstyle(1,7);
	floodfill(95+dx,275+dy,15);
	moveto(80+dx,310+dy);
	lineto(90+dx,300+dy);
	moveto(170+dx,270+dy);
	lineto(180+dx,260+dy);
	rectangle(80+dx,260+dy,180+dx,310+dy);
	setfillstyle(1,15);
	floodfill(85+dx,265+dy,15);
	setfillstyle(1,8);
	floodfill(90+dx,305+dy,15);
	setcolor(8);
	moveto(80+dx,260+dy);
	lineto(180+dx,260+dy);
	moveto(80+dx,260+dy);
	lineto(80+dx,310+dy);
	moveto(90+dx,270+dy);
	lineto(170+dx,270+dy);
	moveto(90+dx,270+dy);
	lineto(90+dx,300+dy);

}

void modifica_tasto(int dx,int dy)
{
	setcolor(1);
	rectangle(80+dx,260+dy,180+dx,310+dy);
	setfillstyle(1,0);
	floodfill(85+dx,265+dy,1);
	setcolor(15);
	rectangle(80+dx,260+dy,180+dx,310+dy);
	rectangle(90+dx,270+dy,170+dx,300+dy);
	setfillstyle(1,7);
	floodfill(95+dx,275+dy,15);
	moveto(80+dx,310+dy);
	lineto(90+dx,300+dy);
	moveto(170+dx,270+dy);
	lineto(180+dx,260+dy);
	setfillstyle(1,8);
	floodfill(85+dx,265+dy,15);
	setfillstyle(1,15);
	floodfill(90+dx,305+dy,15);
	setcolor(8);
	moveto(80+dx,310+dy);
	lineto(180+dx,310+dy);
	moveto(180+dx,260+dy);
	lineto(180+dx,310+dy);
	moveto(90+dx,300+dy);
	lineto(170+dx,300+dy);
	moveto(170+dx,270+dy);
	lineto(170+dx,300+dy);

}

void PTastoGioca (int col1,int col2)
{
 int w;
 setcolor (col1);                       //alto sx Gioca
 settextstyle (SANS_SERIF_FONT,0,1);
 w = textwidth ("G");
 outtextxy (157,122,"G");
 setcolor (col2);
 w = textwidth ("G");
 outtextxy (157+w,122,"ioca");
}

void PTastoIstruzioni (int col1,int col2)
{
 int w;
 setcolor (col1);                   //alto dx isTruzioni
 w = textwidth ("t")-1;
 outtextxy (445+(w*2),122,"t");
 setcolor (col2);
 outtextxy (445,122,"Is");
 outtextxy (445+(w*3),122,"ruzioni");
}

void PTastoScores (int col1,int col2)
{
 int w;
 setcolor (col1);                   //centro scoRes
 w = textwidth ("S");
 outtextxy (300+(w*3),222,"r");
 setcolor (col2);
 outtextxy (300,222,"Sco");
 outtextxy (300+(w*4)-2,222,"es");
}

void PTastoCredit (int col1,int col2)
{
 int w;
 setcolor (col1);                   //basso sx creDit
 w = textwidth ("D");
 outtextxy (157+(w*3)-1,322,"d");
 setcolor (col2);
 outtextxy (157,322,"Cre");
 outtextxy (157+(w*4)-1,322,"it");
}

void PTastoEsci (int col1,int col2)
{
 int w;
 setcolor (col1);                    //basso dx Esci
 w = textwidth ("E");
 outtextxy (462,322,"E");
 setcolor (col2);
 outtextxy (462+w,322,"sci");
}