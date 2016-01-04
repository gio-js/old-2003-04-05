#include <graphics.h>
#include <tetris.h>
#include <effetti.h>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <alloc.h>
#include <credits.h>

/*//////////////////////
//DICHIARAZIONE MACRO///
//////////////////////*/

#define GIU 1080
#define DESTRA 1077
#define SINISTRA 1075
#define RUOTADX 1100
#define RUOTADXM 1068
#define RUOTASX 1115
#define RUOTASXM 1083
#define ESC 1027

/*/////////////////////////
//DICHIARAZIONE FUNZIONI///
/////////////////////////*/
void Gioca();
void DefinisciPezzi(); //inizializza tutti i pezzi
void ScegliPezzo();    //sceglie il pezzo da utilizzare e altro
void MemorizzaPezzo(char struttura[][5],int misx,int misy); //memorizza il pezzo nel buffer
void Scrivi_in_array (int x,int y); //scrive le lettere del pezzo nella scermata del gioco
int Sposta_in_Basso(int ritardo); //sposta se possibile il pezzo in basso
int ContrSpostBasso (int x,int y); //controlla che il pezzo si possa spostare verso il basso
void CancelladalloSchermo (int x,int y); //cancella dallo schermo il pezzo disegnato
void SpostaPezzo_in_array (int xi,int yi,int xf,int yf); //sposta il pezzo nell'array
void Trasforma_in_f (int x,int y);//trasforma in f il pezzo che ha finito if scendere
void Cancella_da_Array(int x,int y); //cancella il pezzo nell'array
void Sposta_a_Destra(); //sposta il pezzo a destra se possibile!!
int ContrSpostDx (int x,int y); //controlla se e' possibile spostare il pezzo a destra
void Sposta_a_Sinistra(); //sposta il pezzo a sinistra se possibile!!
int ContrSpostSx (int x,int y);  //controlla che il pezzo si possa spostare a destra
void Ruota_a_Destra(); //la funzione vede se e' possibile ruotare il pezzo
		       //e se e' possibile lo fa ruotare
int ContrRotazione(int x,int y);//controlla se il pezzo puo' ruotare in quella posizione
void RuotaDxA(); //ruota il pezzo a destra PASSAGGIO DA PARI A DISPARI
void RuotaDxB(); //ruota il pezzo a destra PASSAGGIO DA DISPARI A PARI
void RuotaSxA(); //ruota il pezzo a sinistra PASSAGGIO DA PARI A DISPARI
void RuotaSxB(); //ruota il pezzo a sinistra PASSAGGIO DA DISPARI A PARI
void Ruota_a_Sinistra(); //stessa cosa della rotazione a destra solo che ruota a sinistra
void Scambio(int *a,int *b); //scambia semplicemente il valore delle var
int ControllaGioco(); //controlla che la partita non sia finita
int ControllaPunti(); //controlla se e' stata completata una striscia di mattoni
		      //nel caso che sia vero la cancella
void CancellaStriscia(int y[],int p); //cancella la striscia di mattoni completata
void Vedi_se_Migliore(); //sottoprogramma in cui guardo se il giocatore ha totalizzato un punteggio migliore
void Salva();  //salva i migliori punteggi
void Carica(); //carica i migliori punteggi


/*/////////////////////////
//DICHIARAZIONE STRUTTURA///
/////////////////////////*/

typedef struct pezzo
   {
    char forma [5][5];
    int lx;
    int ly;
   }FPezzo;

/*/////////////////////////
//DICHIARAZIONE VARIABILI//
/////////////////////////*/
FILE *Migliori; //file in cui salvero' i punteggi
char inizializza [20]= {"\\borlandc\\bgi"};
char livello [20];
char giocatore[20],punti[10];
char scelta;
int msx,msy;
int xp,yp; //coordinate principali, con cui mi riferiro' allo schermo
int xa,ya; //coordinate principali, con cui mi riferiro' all' array
int colori [20] = {RED,GREEN,YELLOW,MAGENTA,LIGHTGREEN,LIGHTBLUE,CYAN,BLUE,LIGHTMAGENTA,LIGHTRED,LIGHTCYAN};
int colmat [4];
void *dispezzo; //buffer di dove memorizzero' il pezzo
char schermata [31][20]; //array che rappresenta la schermata principale del gioco
int quale; //memorizza la posizione del pezzo che sto utilizzando nell'array
int ruotato; //indica quante volte e' stato ruotato il pezzo
long int puntii;
const int mat_su_riga = 20;
const int mat_su_col = 31;
const int grandezza = 15;  //grandezza singolo mattone 15pix
char nmigliori[5][10];                 /*                                       */
char pmigliori[5][10];                 /* Dichiarazioni per i migliori punteggi */
unsigned int puntimig[5]={0,0,0,0,0};  /*                                       */
FPezzo pezzi[7];
FPezzo pezzoaus;

void main()
{
 Carica();
 randomize();
 Iniz_Grafica(inizializza);
 PrimaSchermata ();
 scelta = 'Z';
 while (scelta != 'E')
   {
    cleardevice();
    scelta = VisMenu();               /*Gioca = G alto sx*/
    delay(1000);                      /*Istruzioni = T alto dx*/
    cleardevice();                    /*Scores = R centro*/
    switch (scelta)                   /*Esci = E basso dx*/
      {                               /*Credit = D basso sx*/
       case 'G': Gioca();
		 break;
       case 'T': VisIstr();
		 break;
       case 'R': VdPunteggi(nmigliori,pmigliori);
		 break;
       case 'D': closegraph();
		 Animazione();
		 Iniz_Grafica(inizializza);
		 break;
      }
   }
 closegraph();
}

void Gioca()
{
 /*dichiarazioni che servono solo per la partita effettiva*/
 int gameover,termina_discesa,fattibile,premuto;
 int compara,puntdisc;
 unsigned int riposo;
 int i,j; //i=riga; j=colonna;

 Sel_Liv (livello,giocatore);
 cleardevice();
 /*Inizializzo, con la funzione, tutte le struture dei pezzi*/
 DefinisciPezzi();
 /*inizializzo la schermata mettendo spazi in tutte le posizioni*/
 for (i=0;i<mat_su_riga;i++)
   for (j=0;j < mat_su_col;j++)
      schermata [j][i] = ' ';
 /*disegno la schermata iniziale*/
 Rett_Centrale(mat_su_riga,grandezza);
 ParteSinistra(livello);
 puntii=0;
 sprintf(punti,"%ld",puntii);
 ParteDestra(giocatore,punti);
 /*calcolo coordinata x ed y di dove potro' andare
 a mettere il pezzo nella pos 0,0*/
 xp = ((getmaxx() - (mat_su_riga*grandezza))/2);
 yp = 8;
 gameover = 0;
 compara = strcmp (livello,"FACILE");
 if (compara==0)
   riposo=500;
 else
  {
   compara = strcmp (livello,"MEDIO");
   if (compara == 0)
     riposo=200;
   else
     riposo=80;
  }
 while (gameover == 0) //ciclo della partita intera
   {
    xa=ya=0;
    ScegliPezzo();
    MemorizzaPezzo (pezzoaus.forma,pezzoaus.lx,pezzoaus.ly);
    Scrivi_in_array (xa,ya);
    putimage (xp+(ya*grandezza),yp+(xa*grandezza),dispezzo,OR_PUT); /*disegna l'immagine*/
    delay (riposo);
    termina_discesa = 0;
    ruotato = 0;
    while (termina_discesa == 0) //ciclo del singolo pezzo
       {
	premuto = 0;
	while (kbhit()==0) //ciclo che aspetta che venga premuto un pulsante
	 {                 //e' quello che fa scendere il pezzo
	  fattibile = Sposta_in_Basso(riposo);
	  if (fattibile == 0) {termina_discesa=1; break;}
	 }
       if (termina_discesa==0)
	 {
	  premuto = getch()+1000;
	  if (premuto==1000) premuto = getch()+1000;
	  switch (premuto)
	    {
	     case GIU:  fattibile = Sposta_in_Basso(80);
			if (fattibile == 0) termina_discesa=1;
			break;
	     case DESTRA: Sposta_a_Destra();
			  break;
	     case SINISTRA: Sposta_a_Sinistra();
			    break;
	     case RUOTADXM:
	     case RUOTADX: Ruota_a_Destra();
			   break;
	     case RUOTASXM:
	     case RUOTASX: Ruota_a_Sinistra();
			   break;
	     case ESC: termina_discesa=1;
		       gameover = 1;
		       break;
	    }
	 }
      }//fine della discesa del pezzo
     free (dispezzo);
     Trasforma_in_f(xa,ya);
     for (i=0;i<mat_su_riga;i++)   //faccio in modo che non ci siano imprevisti
       for (j=0;j < mat_su_col;j++)//sulla schermata, cioe' che non sia rimasto memoriz.
	 {                         //qualche carattere del mattone appena sceso
	  if (schermata [j][i] != 'f')
	     schermata [j][i] = ' ';
	 }
     puntdisc = ControllaPunti();
     puntii += puntdisc;
     setfillstyle (SOLID_FILL,BLACK);//cancello il vechhio punteggio
     bar (521,167,609,185);
     setcolor (CYAN);
     sprintf (punti,"%ld",puntii); //trasformo i punti da interi a stringa
     outtextxy (525,170,punti);    //scrivo il punteggio raggiunto
     if (gameover == 0) gameover = ControllaGioco();
     delay (100);
   }//fine della partita
 VisRisultati(giocatore,punti);
 Vedi_se_Migliore();
}  //fine sott.prog gioca

/*///////////////////////////////////////////////////////////////////
///////////////SOTTOPROGRAMMI DELLA FUNZIONE GIOCA///////////////////
///////////////////////////////////////////////////////////////////*/
void DefinisciPezzi()
{
 /*pezzo primo*/
 strcpy (pezzi[0].forma[0],"abcd");
 pezzi[0].lx = 4;
 pezzi[0].ly = 1;
 /*fine primo pezzo*/
 /*secondo pezzo*/
 pezzi[1].forma[0][0] = 'a';
 pezzi[1].forma[1][0] = 'b';
 pezzi[1].forma[1][1] = 'c';
 pezzi[1].forma[2][1] = 'd';
 pezzi[1].lx = 2;
 pezzi[1].ly = 3;
 /*fine secondo pezzo*/
 /*terzo pezzo*/
 pezzi[2].forma[0][1] = 'a';
 pezzi[2].forma[1][1] = 'b';
 pezzi[2].forma[1][0] = 'c';
 pezzi[2].forma[2][0] = 'd';
 pezzi[2].lx = 2;
 pezzi[2].ly = 3;
 /*fine terzo pezzo*/
 /*quarto pezzo*/
 strcpy (pezzi[3].forma[0],"ab");
 strcpy (pezzi[3].forma[1],"cd");
 pezzi[3].lx = pezzi[3].ly = 2;
 /*fine quarto pezzo*/
 /*quinto pezzo*/
 strcpy (pezzi[4].forma[0],"abc");
 pezzi[4].forma[1][0] = 'd';
 pezzi[4].lx = 3;
 pezzi[4].ly = 2;
 /*fine quinto pezzo*/
 /*sesto pezzo*/
 strcpy (pezzi[5].forma[0],"abc");
 pezzi[5].forma[1][2] = 'd';
 pezzi[5].lx = 3;
 pezzi[5].ly = 2;
 /*fine sesto pezzo*/
 /*settimo pezzo*/
 strcpy (pezzi[6].forma[0],"abc");
 pezzi[6].forma[1][1] = 'd';
 pezzi[6].lx = 3;
 pezzi[6].ly = 2;
 /*fine settimo pezzo*/
}

int Sposta_in_Basso(int ritardo)
{
 int possibile = 0;
 possibile = ContrSpostBasso (xa,ya);
 if (possibile == 1)
   {
    CancelladalloSchermo(xa,ya); //cancella l'immagine
    SpostaPezzo_in_array(xa,ya,xa+1,ya);
    putimage (xp+(ya*grandezza),yp+((xa+1)*grandezza),dispezzo,OR_PUT); /*disegna immagine*/
    xa++;
    delay (ritardo);
   }
 return possibile;
}

void ScegliPezzo()
{
 /*Scelgo il pezzo da utilizzare*/
 quale = rand()%7;
 pezzoaus = pezzi[quale];

 /*Scelgo i colori dei mattoni
 a,0  b,1  c,2  d,3*/
 int k=0,k1,no,col;
 while (k <= 4)
   {
    no = 0;
    col = rand()%11;
    colmat [k] = colori[col];
    for (k1=0;k1<k;k1++)
      {
       if (colmat[k1] == colmat[k])
	 {
	  no = 1;
	  break;
	 }
      }
    if (no == 0)
      k++;
   }
 /*Scelgo infine la posizione iniziale di dove far partire il pezzo*/
 ya = rand()%(mat_su_riga-pezzoaus.lx);
}

void MemorizzaPezzo(char struttura[][5],int misx,int misy)
{
 int x,y,col;
 unsigned int size;

 setcolor (RED);
 for (x=0;x<misx;x++)
   {
    for (y=0;y<misy;y++)
     {
      if (struttura [y][x] == 'a' || struttura [y][x] == 'b' || struttura [y][x] == 'c' || struttura [y][x] == 'd')
	{
	 switch (struttura [y][x])
	   {
	    case 'a' : setfillstyle (6,colmat[0]);
		       break;
	    case 'b' : setfillstyle (6,colmat[1]);
		       break;
	    case 'c' : setfillstyle (6,colmat[2]);
		       break;
	    case 'd' : setfillstyle (6,colmat[3]);
		       break;
	   }
	 rectangle (10+(x*grandezza),300+(y*grandezza),10+(x*grandezza)+grandezza,300+(y*grandezza)+grandezza);
	 bar (12+(x*grandezza),302+(y*grandezza),8+(x*grandezza)+grandezza,298+(y*grandezza)+grandezza);
	}
     }
   }//fine del disegno del pezzo
 /*MEMORIZZO IL DISEGNO NEL PUNTATORE E SUCCESSIVAMENTE LO CANCELLO*/
 size = imagesize (10,300,10+(pezzoaus.lx*grandezza),300+(pezzoaus.ly*grandezza));
 if ((dispezzo = malloc(size)) == NULL)
  {
   closegraph();
   printf("Errore: memoria insufficiente per continuare.\n");
   getch();
   exit(1);
  }
 getimage (10,300,10+(pezzoaus.lx*grandezza),300+(pezzoaus.ly*grandezza),dispezzo);
 setfillstyle (SOLID_FILL,BLACK); /*cancello il pezzo*/
 bar (10,300,100,400);
}

void Scrivi_in_array (int x,int y)
{
 int i,j;   //i = righe;  j=colonne
 for (i=0;i<pezzoaus.lx;i++)
   for (j=0;j<pezzoaus.ly;j++)
     {
      if (pezzoaus.forma [j][i] == 'a' || pezzoaus.forma [j][i] == 'b' || pezzoaus.forma [j][i] == 'c' || pezzoaus.forma [j][i] == 'd')
	 schermata [x+j][y+i] = pezzoaus.forma[j][i];
     }
}

int ContrSpostBasso (int x,int y)
{
 int i,j; //i=righe; j=colonne
 int fattibile;
 for (i=0;i<pezzoaus.lx;i++)
   for (j=0;j<pezzoaus.ly;j++)
     {
      if (pezzoaus.forma [j][i] == 'a' || pezzoaus.forma [j][i] == 'b' || pezzoaus.forma [j][i] == 'c' || pezzoaus.forma [j][i] == 'd')
	if (schermata [x+j+1][y+i] != 'f')
	    fattibile = 1;
	else {fattibile = 0; goto exit;}
     }
 exit:
 if (fattibile == 1)
   {
    if (x < (mat_su_col-pezzoaus.ly)) fattibile = 1;
    else fattibile = 0;
   }
 return fattibile;
}

void CancelladalloSchermo (int x,int y)
{
 int j,i,dx,sx,sop,sot; //i=righe; j=colonne
 setfillstyle (SOLID_FILL,BLACK);
 for (i=0;i<pezzoaus.lx;i++)
    for (j=0;j<pezzoaus.ly;j++)
      {
       dx=sx=sop=sot=0;
       if (schermata [x+j][y+i] == 'a' || schermata [x+j][y+i] == 'b' || schermata [x+j][y+i] == 'c' || schermata [x+j][y+i] == 'd')
	 {
	  if (schermata[x+j][y+i-1] == 'f' && y>0) sx = 1;
	  if (schermata[x+j][y+i+1] == 'f' && y<(mat_su_riga-pezzoaus.lx)) dx = 1;
	  if (schermata[x+j+1][y+i] == 'f' && x<(mat_su_col-pezzoaus.ly)) sot = 1;
	  if (schermata[x+j-1][y+i] == 'f' && x>0) sop = 1;
	  bar (xp+((y+i)*grandezza)+sx,yp+((x+j)*grandezza)+sop,xp+((y+i)*grandezza)+15-dx,yp+((x+j)*grandezza)+15-sot);
	 }
      }
}

void SpostaPezzo_in_array (int xi,int yi,int xf,int yf)
{
 int i,j; //i=righe;   j=colonne
 Cancella_da_Array(xi,yi);
 for (i=0;i<pezzoaus.lx;i++)
    for (j=0;j<pezzoaus.ly;j++)   /*il ciclo cancella il carattere del mattone*/
       {                          /*e lo riscrive nella nuova posizione*/
	if (pezzoaus.forma [j][i] == 'a' || pezzoaus.forma [j][i] == 'b' || pezzoaus.forma [j][i] == 'c' || pezzoaus.forma [j][i] == 'd')
	   schermata [j+xf][i+yf] = pezzoaus.forma[j][i];
       }
}

void Cancella_da_Array(int x,int y)
{
 int i,j; //i=righe;   j=colonne;
 for (i=0;i<pezzoaus.lx;i++)
    for (j=0;j<pezzoaus.ly;j++)   /*il ciclo cancella i caratteri del pezzo*/
       {
	if (pezzoaus.forma [j][i] == 'a' || pezzoaus.forma [j][i] == 'b' || pezzoaus.forma [j][i] == 'c' || pezzoaus.forma [j][i] == 'd')
	  {
	   schermata [j+x][i+y] = ' ';
	  }
       }
}

void Trasforma_in_f (int x,int y)
{
 int i,j; //i=righe;  j=colonne;
 for (i=0;i<pezzoaus.lx;i++)
   for (j=0;j<pezzoaus.ly;j++)
     {
      if (schermata [x+j][y+i] == 'a' || schermata [x+j][y+i] == 'b' || schermata [x+j][y+i] == 'c' || schermata [x+j][y+i] == 'd')
	 schermata[x+j][y+i] = 'f';
     }
}

void Sposta_a_Destra()//sott.prog. per spostare il pezzo a destra
{
 int possibile;
 possibile = ContrSpostDx(xa,ya);
 if (possibile == 1)
   {
    CancelladalloSchermo (xa,ya);
    SpostaPezzo_in_array (xa,ya,xa,ya+1);
    putimage (xp+((ya+1)*grandezza),yp+(xa*grandezza),dispezzo,OR_PUT);
    ya++;
    delay (80);
   }
}

int ContrSpostDx(int x,int y)
{
 int i,j; //i=righe;   j=colonne;
 int fattibile=1;
 for (i=0;i<pezzoaus.lx;i++)
   for (j=0;j<pezzoaus.ly;j++)
      {
       if (schermata [x+j][y+i] == 'a' || schermata [x+j][y+i] == 'b' || schermata [x+j][y+i] == 'c' || schermata [x+j][y+i] == 'd')
	  if (schermata [x+j][y+i+1] == 'f') {fattibile = 0; goto exit;}
      }
 exit:
 if (fattibile == 1)
  {
   if (y >= (mat_su_riga-pezzoaus.lx))
     fattibile = 0;
  }
 return fattibile;
}

void Sposta_a_Sinistra()  //sott.prog. per spostare il pezzo a destra
{
 int possibile;
 possibile = ContrSpostSx(xa,ya);
  if (possibile == 1)
   {
    CancelladalloSchermo (xa,ya);
    SpostaPezzo_in_array (xa,ya,xa,ya-1);
    putimage (xp+((ya-1)*grandezza),yp+(xa*grandezza),dispezzo,OR_PUT);
    ya--;
    delay (80);
   }
}

int ContrSpostSx (int x,int y) //sott.prog per contr lo spost del pezzo a sx
{
 int i,j; //i=righe;   j=colonne;
 int fattibile=1;
 for (i=0;i<pezzoaus.lx;i++)
   for (j=0;j<pezzoaus.ly;j++)
      {
       if (schermata [x+j][y+i] == 'a' || schermata [x+j][y+i] == 'b' || schermata [x+j][y+i] == 'c' || schermata [x+j][y+i] == 'd')
	  if (schermata [x+j][y+i-1] == 'f') {fattibile = 0; goto exit;}
      }
 exit:
 if (fattibile == 1)
  {
   if (y <= 0)
     fattibile = 0;
  }
 return fattibile;
}

void Ruota_a_Destra()
{
 int possibile;
 if (pezzoaus.lx != pezzoaus.ly)
   {
    if ((ruotato%2) == 0)
      RuotaDxA();
    else
      RuotaDxB();
    ruotato ++;
    possibile = ContrRotazione(xa,ya);
    if (possibile == 1)
      {
       free (dispezzo);
       Scambio (&pezzoaus.lx,&pezzoaus.ly);
       CancelladalloSchermo(xa,ya);
       Cancella_da_Array(xa,ya);
       Scambio (&pezzoaus.lx,&pezzoaus.ly);
       MemorizzaPezzo(pezzoaus.forma,pezzoaus.lx,pezzoaus.ly);
       Scrivi_in_array(xa,ya);
       putimage (xp+(ya*grandezza),yp+(xa*grandezza),dispezzo,OR_PUT);
      }
    else
      {
       if ((ruotato%2)==0)
	  RuotaSxB();
       else
	  RuotaSxA();
       ruotato--;
      }
   }
}

void Ruota_a_Sinistra()
{
 int possibile;
 if (pezzoaus.lx != pezzoaus.ly)
   {
   if ((ruotato%2)==0)
      RuotaSxB();
   else
      RuotaSxA();
   ruotato++;
   possibile = ContrRotazione(xa,ya);
   if (possibile == 1)
     {
      free (dispezzo);
      Scambio (&pezzoaus.lx,&pezzoaus.ly);
      CancelladalloSchermo(xa,ya);
      Cancella_da_Array(xa,ya);
      Scambio (&pezzoaus.lx,&pezzoaus.ly);
      MemorizzaPezzo(pezzoaus.forma,pezzoaus.lx,pezzoaus.ly);
      Scrivi_in_array(xa,ya);
      putimage (xp+(ya*grandezza),yp+(xa*grandezza),dispezzo,OR_PUT);
     }
   else
     {
      if ((ruotato%2)==0)
	 RuotaDxA();
      else
	 RuotaDxB();
      ruotato--;
     }
  }
}

void RuotaDxA()
{
 int i,j,cont; //i=righe;  j=colonne
 FPezzo pezrot;
 pezrot = pezzoaus;
 /*Cancello cio' che c'e scritto nel pezzo*/
 for (i=0;i<pezzoaus.lx;i++)
    for(j=0;j<pezzoaus.ly;j++)
       pezzoaus.forma[j][i] = ' ';
 //end
 for (i=0;i<pezzoaus.lx;i++)
    for (j=0,cont = pezzoaus.ly-1;j<pezzoaus.ly;j++,cont--)
	pezzoaus.forma[i][cont] = pezrot.forma[j][i];
 Scambio (&pezzoaus.lx,&pezzoaus.ly);
}

void RuotaDxB()
{
 int i,j,cont; //i=righe;  j=colonne
 FPezzo pezrot;
 pezrot = pezzoaus;
 /*Cancello cio' che c'e scritto nel pezzo*/
 for (i=0;i<pezzoaus.lx;i++)
    for(j=0;j<pezzoaus.ly;j++)
       pezzoaus.forma[j][i] = ' ';
 //end
 for (j=0;j<pezzoaus.lx;j++)
   for (i=0,cont=pezzoaus.ly-1;i<pezzoaus.ly;i++,cont--)
	pezzoaus.forma[j][cont] = pezrot.forma[i][j];
 Scambio (&pezzoaus.lx,&pezzoaus.ly);
}

void RuotaSxA()
{
 int i,j,cont; //i=righe;  j=colonne
 FPezzo pezrot;
 pezrot = pezzoaus;
 /*Cancello cio' che c'e scritto nel pezzo*/
 for (i=0;i<pezzoaus.lx;i++)
    for(j=0;j<pezzoaus.ly;j++)
       pezzoaus.forma[j][i] = ' ';
 //end
 for (j=0;j<pezzoaus.ly;j++)
   for (i=0,cont=pezzoaus.lx-1;i<pezzoaus.lx;i++,cont--)
       pezzoaus.forma[cont][j] = pezrot.forma[j][i];
 Scambio (&pezzoaus.lx,&pezzoaus.ly);
}

void RuotaSxB()
{
 int i,j,cont; //i=righe;  j=colonne
 FPezzo pezrot;
 pezrot = pezzoaus;
 /*Cancello cio' che c'e scritto nel pezzo*/
 for (i=0;i<pezzoaus.lx;i++)
    for(j=0;j<pezzoaus.ly;j++)
       pezzoaus.forma[j][i] = ' ';
 //end
 for (j=0;j<pezzoaus.ly;j++)
   for (i=0,cont=pezzoaus.lx-1;i<pezzoaus.lx;i++,cont--)
      pezzoaus.forma[cont][j] = pezrot.forma[j][i];
 Scambio (&pezzoaus.lx,&pezzoaus.ly);
}

void Scambio (int *a,int *b)
{
 int aus;
 aus = *a;
 *a = *b;
 *b = aus;
}

int ContrRotazione (int x,int y)
{
 int i,j; //i=righe;  j=colonne;
 int fattibile=1;
 for (i=0;i<pezzoaus.lx;i++)
    for (j=0;j<pezzoaus.ly;j++)
      {
       if (pezzoaus.forma [j][i] == 'a' || pezzoaus.forma [j][i] == 'b' || pezzoaus.forma [j][i] == 'c' || pezzoaus.forma [j][i] == 'd')
	   if (schermata[j+x][i+y] == 'f') {fattibile = 0; goto exit;}
      }
 exit:
 if (fattibile == 1)
   {
    if (y >= (mat_su_riga-pezzoaus.lx) || x >= (mat_su_col-pezzoaus.ly))
      fattibile = 0;
   }
 return fattibile;
}

int ControllaGioco()
{
 int esito,i,finepartita=0;
 for (i=0;i<mat_su_riga;i++)
   {
    if (schermata [0][i] == 'f') {esito = 1; break;}
   }
 if (esito == 1) finepartita = 1;

 return finepartita;
}

int ControllaPunti()
{
 int i,j,cont,punti=0,completati=0,compara; //i=righe;   j=colonne;
 int posy[5];
 /*    Guardo se e' stata completata una */
 /*    striscia di mattoni               */
 for (j=0;j<mat_su_col;j++)
   {
    cont = 0;
    for (i=0;i<mat_su_riga;i++)
      {
       if (schermata[j][i] == 'f') cont++;
       else break;
      }
    if (cont == mat_su_riga)
      {
       posy [completati]=j;
       completati++;
      }
   }
 compara = strcmp (livello,"FACILE");
 if (compara==0)
   punti = completati * 50;
 else
  {
   compara = strcmp (livello,"MEDIO");
   if (compara == 0)
      punti = completati * 200;
   else
      punti = completati * 500;
  }
 if (completati != 0)
   CancellaStriscia(posy,completati);
 return punti;
}

void CancellaStriscia(int y[],int p)
{
 int k,i,j,x,sop,sot;
 unsigned int size; //i=righe;   j=colonne;
 void *schermrest;
 for (i=0;i<p;i++)
   {
    /*Animazione per cancellare la striscia di mattoni*/
    setcolor (BLACK);
    x=0;
    for (x=0;x<mat_su_riga;x++)
      {
       for (j=x*grandezza;j<=(x*grandezza)+grandezza;j++)
	 {
	  sop=sot=0;
	  if (schermata[y[i]+1][x] == 'f' && y[i]<mat_su_col) sot = 1;
	  if (schermata[y[i]-1][x] == 'f') sop = 1;
	  line (xp+j,yp+((y[i])*grandezza)+sop,xp+j,yp+(y[i]*grandezza)+grandezza-sot);
	  delay(20);
	 }
      }
    /*sposto tutte le strisce di mattoni restanti in basso*/
    for (j=y[i];j>0;j--)
      {
       /*la memorizzo*/
       size = imagesize (xp,yp+(j*grandezza),xp+(mat_su_riga*grandezza),yp+(j*grandezza)-grandezza);
       if ((schermrest = malloc(size)) == NULL)
	 {
	  closegraph();
	  printf("Errore: memoria insufficiente per continuare.\n");
	  getch();
	  exit(1);
	 }
       getimage (xp,yp+(j*grandezza),xp+(mat_su_riga*grandezza),yp+(j*grandezza)-grandezza,schermrest);
       /*cancello la striscia di mattoni*/
       setfillstyle (SOLID_FILL,BLACK);
       bar (xp,yp+(j*grandezza),xp+(mat_su_riga*grandezza),yp+(j*grandezza)-grandezza);
       /*la porto in basso*/
       putimage (xp,yp+(j*grandezza),schermrest,OR_PUT);
       free (schermrest);
      }
    /*sposto gli elementi nell'array in basso*/
    for (j=y[i];j>0;j--)
      for (k=0;k<mat_su_riga;k++)
	  schermata[j][k] = schermata [j-1][k];
   }
}

void Vedi_se_Migliore()
{
 int i,j;
 for (i=0;i<5;i++)
   if (puntii>=puntimig[i]) break;
 if (i<5)
 {
  /*Sposto i nomi in basso*/
  for (j=4;j>i;j--)
    strcpy (nmigliori[j],nmigliori[j-1]);
  strcpy(nmigliori[i],giocatore);
  /*Sposto i punti numero in basso*/
  for (j=4;j>i;j--)
    puntimig[j]=puntimig[j-1];
  puntimig[i]=puntii;
  /*Sposto i punti lettera in basso*/
  for (j=4;j>i;j--)
    strcpy(pmigliori[j],pmigliori[j-1]);
  strcpy(pmigliori[i],punti);
  Salva();
 }
}

void Salva()
{
 int i;
 if((Migliori=fopen("migliori.$$$", "w+b")) != NULL)
   {
    fseek (Migliori,ftell(Migliori),SEEK_SET);
    fwrite(pmigliori,sizeof(char[5][10]),5,Migliori);
    fwrite(nmigliori,sizeof(char[5][10]),5,Migliori);
    fwrite(puntimig,sizeof(unsigned int),5,Migliori);
   }
 fclose(Migliori);
}

void Carica()
{
 int i;
 if((Migliori=fopen("migliori.$$$", "r+b")) != NULL)
   {
    fseek (Migliori,ftell(Migliori),SEEK_SET);
    fread(pmigliori,sizeof(char[5][10]),5,Migliori);
    fread(nmigliori,sizeof(char[5][10]),5,Migliori);
    fread(puntimig,sizeof(unsigned int),5,Migliori);
   }
 fclose(Migliori);
}
/*///////////////////////////////////////////////////////////////////
////////////FINE SOTTOPROGRAMMI DELLA FUNZIONE GIOCA/////////////////
///////////////////////////////////////////////////////////////////*/