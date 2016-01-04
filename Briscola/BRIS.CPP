#include<graphics.h>
#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<stdlib.h>
#include<string.h>

void Mazzo();
void Estrai();
short Gioca_Carta();
short Computer_Carta();
void Iniz_Grafica();
void Rettangolo (int x,int y);
void CopriCarta (int x,int y);
void ScriviCarta ();
void CartaPlayer();
void CartaComp();
//sottoprogrammi per il mouse
void PrendiCoordinate (int *posx,int *posy);
void Mouse(int stato,int xmin,int ymin,int xmax,int ymax);
union REGS BottoniMouse(int bot);
void MuoviMouse(int m);
void VertMouse(int ymin,int ymax);
void HoriMouse(int xmin,int xmax);
union REGS PrendiXYMouse(void);


#define MOUSE 0x33

typedef struct carta1
{
 int valore;
 char num[10];
 int punteggio;
 char seme[10];
}TCarta;

short turno, j, k;
int scoreP, scoreC,estr[50], briscola, carta, cartaC, q, h, w;
TCarta mazzo[50], player[3], computer[3];
int punteggi[]={11,0,10,0,0,0,0,2,3,4};
int valori[]={12,2,11,4,5,6,7,8,9,10};
char Semi[4][10]={"D","C","B","S"};
char semo[4][10]={"DENARI", "COPPE","BASTONI","SPADE"};
char numi[10][10]={"A","2","3","4","5","6","7","F","C","R"};
int playx []= {222,322,422};
int playy []= {330,330,330};
int compx []= {200,300,400,0};
int compy []= {20,20,20,0};
int mem   []= {3,3,3};
int xm,ym;
int xi,yi,xf,yf,bo;
union REGS rel,xy;
char *vuoto="";
int m;


void main()
{
 Iniz_Grafica();
/*carte del computer*/
 setcolor (GREEN);
 setfillstyle (6,GREEN);
 Rettangolo (200,20);
 CopriCarta (200,20);
 Rettangolo (300,20);
 CopriCarta (300,20);
 Rettangolo (400,20);
 CopriCarta (400,20);
/*Carte dell'utente*/
 Rettangolo (200,300);
 Rettangolo (300,300);
 Rettangolo (400,300);
/*Carte in mezzo giocate*/
 Rettangolo (250,150);
 Rettangolo (350,150);
/*Mazzo*/
 rectangle  (105,143,105+50,243);
 bar3d (20,150,20+75,150+100,10,10);
 CopriCarta (20,150);
/*Legenda*/
 outtextxy(50,450,"C=coppe D=denari B=bastoni S=spade");
 scoreP=scoreC=cartaC=j=k=0;
 int i, o, x, m, n;
 randomize();
 Mazzo(); //crea il mazzo
 Estrai(); //memorizza la sequenza delle carte
 i=0;
 while(i<3)  //distribuzione delle carte al giocatore
 {
  player[i]=mazzo[estr[i]];
  i++;
 }
 o=0;
 while(o<3)  //distribuzione delle carte al computer
 {
  computer[o]=mazzo[estr[i]];
  i++;
  o++;
 }
 q=1;
 while(q<40)  //inizio del gioco
 {
  ScriviCarta();
  setcolor (GREEN);
  setfillstyle (6,GREEN);
  Rettangolo (200,20);
  CopriCarta (200,20);
  Rettangolo (300,20);
  CopriCarta (300,20);
  Rettangolo (400,20);
  CopriCarta (400,20);
  if(q>35)
  {
   j=0;
   while(j<3)
   {
    setfillstyle (SOLID_FILL,BLACK);
    CopriCarta(compx[mem[j]],compy[mem[j]]);
    j++;
   }
   k++;
  }
  gotoxy(5,20);
  setcolor (RED);
  outtextxy (10,275,"Mancano");
  printf("%d ", 40-q);
  setcolor (RED);
  w = textwidth ("carte");
  outtextxy (10,310,"carte");
  setcolor (GREEN);
  if(turno==0)   //il 1o che gioca e` il giocatore
  {
   xm=ym=0;
   carta = 10;
   /*Finche' non ha preso un valore valido per la carta*/
   /*continua ad eseguire il ciclo*/
   while (carta == 10)
     {
      PrendiCoordinate(&xm,&ym);
      if (xm>=200 && xm<=275 && ym>=300 && ym<=400) carta = 1;
      if (xm>=300 && xm<=375 && ym>=300 && ym<=400) carta = 2;
      if (xm>=400 && xm<=475 && ym>=300 && ym<=400) carta = 3;
      m = strcmp (player[carta-1].num,vuoto);
      if (m==0) carta = 10;
     }
   Mouse (2,5,5,getmaxx(),getmaxy());
   CartaPlayer();
   j=cartaC=Computer_Carta();
   mem[k]=j;
   CartaComp();
   /*Aspetta in questo modo che si prema il tasto del mouse*/
   PrendiCoordinate(&xm,&ym);
   Mouse (2,5,5,getmaxx(),getmaxy());
   CopriCarta(350,150);
   CopriCarta(250,150);
   x=strcmp(player[carta-1].seme,computer[cartaC].seme);
   m=strcmp(mazzo[estr[39]].seme,player[carta-1].seme);
   n=strcmp(mazzo[estr[39]].seme,computer[cartaC].seme);
   if(x==0 || m==0 || n==0)
     {
      if(player[carta-1].valore>=computer[cartaC].valore)
	{
	 scoreP=scoreP+player[carta-1].punteggio+computer[cartaC].punteggio;
	 q++;
	 player[carta-1]=mazzo[estr[i]];
	 i++;
	 q++;
	 computer[cartaC]=mazzo[estr[i]];
	 i++;
	 turno=0;
	}
	else
	{
	 scoreC=scoreC+player[carta-1].punteggio+computer[cartaC].punteggio;
	 q++;
	 computer[cartaC]=mazzo[estr[i]];
	 i++;
	 q++;
	 player[carta-1]=mazzo[estr[i]];
	 i++;
	 turno=1;
	}
     }
   else
     {
      scoreP=scoreP+player[carta-1].punteggio+computer[cartaC].punteggio;
      q++;
      player[carta-1]=mazzo[estr[i]];
      i++;
      q++;
      computer[cartaC]=mazzo[estr[i]];
      i++;
      turno=0;
     }
  }
  else  //il 1o che gioca e` il computer
  {
   j=cartaC=Computer_Carta();
   CartaComp();
   mem[k]=j;
   xm=ym=0;
   carta = 10;
   /*Finche' non ha preso un valore valido per la carta*/
   /*continua ad eseguire il ciclo*/
   while (carta == 10)
     {
      PrendiCoordinate(&xm,&ym);
      if (xm>=200 && xm<=275 && ym>=300 && ym<=400) carta = 1;
      if (xm>=300 && xm<=375 && ym>=300 && ym<=400) carta = 2;
      if (xm>=400 && xm<=475 && ym>=300 && ym<=400) carta = 3;
      m = strcmp (player[carta-1].num,vuoto);
      if (m==0) carta = 10;
     }
   Mouse (2,5,5,getmaxx(),getmaxy());
   CartaPlayer();
   /*Aspetta in questo modo che si prema il tasto del mouse*/
   PrendiCoordinate(&xm,&ym);
   Mouse (2,5,5,getmaxx(),getmaxy());
   CopriCarta(350,150);
   CopriCarta(250,150);
   x=strcmp(player[carta-1].seme,computer[cartaC].seme);
   m=strcmp(mazzo[estr[39]].seme,player[carta-1].seme);
   n=strcmp(mazzo[estr[39]].seme,computer[cartaC].seme);
   if(x==0 || m==0 || n==0)
     {
      if(computer[cartaC].valore>=player[carta-1].valore)
	 {
	  scoreC=scoreC+player[carta-1].punteggio+computer[cartaC].punteggio;
	  q++;
	  computer[cartaC]=mazzo[estr[i]];
	  i++;
	  q++;
	  player[carta-1]=mazzo[estr[i]];
	 i++;
	  turno=1;
	 }
	else
	{
	 scoreP=scoreP+player[carta-1].punteggio+computer[cartaC].punteggio;
	 q++;
	 player[carta-1]=mazzo[estr[i]];
	 i++;
	 q++;
	 computer[cartaC]=mazzo[estr[i]];
	 i++;
	 turno=0;
	}
     }
   else
     {
      scoreC=scoreC+player[carta-1].punteggio+computer[cartaC].punteggio;
      q++;
      computer[cartaC]=mazzo[estr[i]];
      i++;
      q++;
      player[carta-1]=mazzo[estr[i]];
      i++;
      turno=1;
     }
  }
 }
 gotoxy(20,12);
 printf("Hai totalizzato %d punti, il computer %d.", scoreP, scoreC);
 gotoxy(35,13);
 if(scoreP>scoreC)
 {
  printf("HAI VINTO!!!");
 }
 if(scoreP<scoreC)
 {
  printf("HAI PERSO!!!");
 }
 if(scoreP==scoreC)
 {
  printf("HAI PAREGGIATO!!!");
 }
 getch();
}

/*////////////////////////////////////
//////SOTTOPROGRAMMI PER IL MOUSE/////
////////////////////////////////////*/

void PrendiCoordinate (int *posx,int *posy)
{
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
	regs.x.bx=bot;                  /* in AX 1 se Š premuto il tasto */
	int86(MOUSE,&regs,&regs);	/* in CX X in cui Š stato premuto */
	return(regs);			/* in DX Y in cui Š stato premuto ‚ */
}

union REGS PrendiXYMouse(void)
{                                       /* tiene sempre aggiornate le  */
	union REGS regs;		/* coordinate del mouse */
	regs.x.ax=3;
	int86(MOUSE,&regs,&regs);
	return(regs);
}

/*////////////////////////////////////
//FINE SOTTOPROGRAMMI PER IL MOUSE////
////////////////////////////////////*/

void Iniz_Grafica()
{
 int gdriver = DETECT;
 int gmode = DETECT;
 initgraph (&gdriver,&gmode,"E:\\BORLANDC\\BGI");
}

void Rettangolo(int x,int y)
{
 rectangle (x,y,x+75,y+100);
}

void CopriCarta (int x,int y)
{
 bar (x+5,y+5,x+70,y+95);
}

void Mazzo()
{
 int i, k, y;
 i=0;
 while(i<40)
 {
  k=0;
  while(k<10)
  {
	strcpy(mazzo[i].num,numi[k]);
	k++;
	i++;
  }
 }
 mazzo[43].num[0]='\0';
 i=k=0;
 while(i<40)
 {
  k=0;
  while(k<10)
  {
	mazzo[i].valore=valori[k];
	i++;
	k++;
  }
 }
 mazzo[43].valore=-1;
 i=0;
 while(i<40)
 {
  k=0;
  while(k<10)
  {
	mazzo[i].punteggio=punteggi[k];
	i++;
	k++;
  }
 }
 mazzo[43].punteggio=0;
 y=i=0;
 while(i<4)
 {
  k=0;
  while(k<10)
  {
	strcpy(mazzo[y].seme,Semi[i]);
	k++;
	y++;
  }
  i++;
 }
 mazzo[43].seme[0]='\0';
}

void Estrai()
{
 int kc, k, m, o;
 kc=0;
 while(kc<40)
 {
  m=random(40);
  k=0;
  while(k<kc)   // ricerca
  {
	if(estr[k]==m) break;
	k++;
  }
  if(k==kc)   // non esiste
  {
	estr[kc]=m;
	kc++;
  }
 }
 o=0;
 while(o<9)
 {
  estr[kc+o]=43;
  o++;
 }
 o=0;
 while(o<40)
 {
  m=strcmp(mazzo[estr[39]].seme,mazzo[o].seme);
  if(m==0)
	mazzo[o].valore=mazzo[o].valore+20;
  o++;
 }
}

short Computer_Carta()
{
 short cartaC, ptr, m, n, o, q;
 char *vuoto="";
 cartaC=0;
 ptr=0;
 if(turno==1)
 {
  while(ptr==0)
  {
	cartaC=random(3);
	ptr=strcmp(computer[cartaC].num,vuoto);
  }
 }
 else
 {
  o=0;
  while(o<3)
  {
	m=strcmp(player[carta-1].seme,computer[o].seme);
	if(m==0)
	{
	 ptr=0;
	 if(player[carta-1].valore<computer[o].valore)
	 {
	  cartaC=o;
	  o=3;
	 }
	 else
	 {
	  if(player[carta-1].valore>=10)
	  {
	   q=0;
	   while(q<3)
	   {
		m=strcmp(computer[q].seme,mazzo[estr[39]].seme);
		if(m==0)
		{
		 cartaC=q;
		 q=3;
		}
		else
		{
		 while(ptr==0)
		 {
		  cartaC=random(3);
		  ptr=strcmp(computer[cartaC].num,vuoto);
		 }
		}
		q++;
	   }
	   o=3;
	  }
	  else
	  {
	   while(ptr==0)
	   {
		cartaC=random(3);
		ptr=strcmp(computer[cartaC].num,vuoto);
	   }
	   o=3;
	  }
	 }
	}
	else
	{
	 if(player[carta-1].valore>=10)
	 {
	  q=0;
	  while(q<3)
	  {
	   m=strcmp(computer[q].seme,mazzo[estr[39]].seme);
	   if(m==0)
	   {
		cartaC=q;
		q=3;
	   }
	   else
	   {
		while(ptr==0)
		{
		 cartaC=random(3);
		 ptr=strcmp(computer[cartaC].num,vuoto);
		}
	   }
	   q++;
	  }
	  o=3;
	 }
	 else
	 {
	  while(ptr==0)
	  {
	   cartaC=random(3);
	   ptr=strcmp(computer[cartaC].num,vuoto);
	  }
	  o++;
	 }
	}
  }
 }
 return cartaC;
}

void ScriviCarta ()
{
 int x,y,h,i;
 x = 112;
 y = 170;
 /*scrivo briscola*/
 settextstyle (TRIPLEX_FONT,VERT_DIR,3);
 outtextxy (x,y,mazzo[estr[39]].seme);
 h = textwidth (mazzo[estr[39]].seme);
 outtextxy (x,y+h,mazzo[estr[39]].num);
 if(q>=34)
   bar (5,140,240,280);
 /*scrivo il numero delle carte dell'utente*/
 setfillstyle (SOLID_FILL,BLACK);
 CopriCarta (200,300);
 CopriCarta (300,300);
 CopriCarta (400,300);
 settextstyle (TRIPLEX_FONT,0,3);
 i = 0;
 while (i < 3)
  {
	outtextxy (playx[i],playy[i],player[i].num);
	h = textwidth (player[i].num);
	outtextxy (playx[i]+h,playy[i],player[i].seme);
	i++;
  }
}

void CartaPlayer ()
{
 int x [] = {200,300,400};
 int y [] = {300,300,300};
 int h;

 setfillstyle (SOLID_FILL,BLACK);
 CopriCarta (250,150);
 settextstyle (TRIPLEX_FONT,0,3);
 outtextxy (272,180,player[carta-1].num);
 h = textwidth (player[carta-1].num);
 outtextxy (272+h,180,player[carta-1].seme);
 CopriCarta (x[carta-1],y[carta-1]);
}

void CartaComp()
{
 int x [] = {200,300,400};
 int y [] = {20,20,20};
 int h;

 setfillstyle (SOLID_FILL,BLACK);
 CopriCarta (350,150);
 settextstyle (TRIPLEX_FONT,0,3);
 outtextxy (372,180,computer[cartaC].num);
 h = textwidth (computer[cartaC].num);
 outtextxy (372+h,180,computer[cartaC].seme);
 CopriCarta (x[cartaC],y[cartaC]);
}