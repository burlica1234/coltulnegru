#include <iostream>
#include "winbgim.h"
#include <string.h>
#include "graphics.h"

#define MAX 20

using namespace std;
int player = 0;
int win = 0;
int TablaDeJoc[MAX][MAX];
// 1 +  2 \\ 3 //
int n;
int piesaAleasa;
int lv, cv, la, ca, ln, cn;

void initTabla()
{
    n=3;
    int i,j;
    for (i=1; i<=n; i++)
        for (j=1; j<=n; j++)
            TablaDeJoc[i][j]=0;
    TablaDeJoc[1][1]=3; // +
    TablaDeJoc[1][0]=3;
    TablaDeJoc[n][n] = 4;

    la=1;
    ca=1;
    lv=1;
    cv=0;
}

void desPiesa(int p, int l, int c)
{
    char numeFisier[10];
    switch (p)
    {
    case 0:
        strcpy(numeFisier,"0.jpg");
        break;
    case 1:
        strcpy(numeFisier,"1.jpg");
        break;
    case 2:
        strcpy(numeFisier,"2.jpg");
        break;
    case 3:
        strcpy(numeFisier,"3.jpg");
        break;
    case 4:
        strcpy(numeFisier, "4.jpg");
        break;
    }
    readimagefile(numeFisier, 50*c, 100+50*l, 50*(c+1)-2,100+50*(l+1)-2);
}

void desTabla()
{
    int i,j,p;
    for (i=1; i<=n; i++)
    {
        for (j=1; j<=n; j++)
        {
            p=TablaDeJoc[i][j];
            desPiesa(p,i,j);
        }
    }
}

void alegePiesa(int &p)
{
    int x,y;
    p=0;
    if(ismouseclick(WM_LBUTTONDOWN))
    {
        clearmouseclick(WM_LBUTTONDOWN);
        x=mousex();
        y=mousey();
        if (x>=50 && x<=100 && y>=25 && y<=75) p=1;
        if (x>=150 && x<=200 && y>=25 && y<=75) p=2;
        if (x>=250 && x<=300 && y>=25 && y<=75) p=3;
        if (x>=350 && x<=400 && y>=25 && y<=75) p=4;
        Beep(1000,200);
    }
}

bool vinDinStanga()
{
    return lv==la && cv==ca-1;
}

bool vinDeSus()
{
    return lv==la-1 && cv==ca;
}

bool vinDinDreapta()
{
    return lv==la && cv==ca+1;
}

bool vinDeJos()
{
    return lv==la+1 && cv==ca;
}

void mergJos()
{
    cn=ca;
    ln=la+1;

}

void mergSus()
{
    cn=ca;
    ln=la-1;

}

void mergStanga()
{
    cn=ca-1;
    ln=la;

}

void mergDreapta()
{
    cn=ca+1;
    ln=la;

}

void calculeazaLoculUndeTrebuiePusaPiesa()
{
    do
    {

        switch(TablaDeJoc[la][ca])
        {
        /* piesa 1 = \\ */
        case 1:
            if (vinDinStanga()) mergJos();
            else if (vinDinDreapta()) mergSus();
            else if (vinDeJos()) mergStanga();
            else mergDreapta();
            break;
        /* piesa 2 = // */
        case 2:
            if (vinDinStanga()) mergSus();
            else if (vinDinDreapta()) mergJos();
            else if (vinDeJos()) mergDreapta();
            else mergStanga();
            break;
        /* piesa 3 = + */
        case 3:
            if (vinDinStanga()) mergDreapta();
            else if (vinDinDreapta()) mergStanga();
            else if (vinDeJos()) mergSus();
            else mergJos();
            break;
        }
        if (ln == n && cn == n)
            break;

        // cout<<"     lv="<<lv<<" cv="<<cv<<", la="<<la<<" ca="<<ca<<" -> ln="<<ln<<" cn="<<cn<<endl;
        lv=la; cv=ca;
        la=ln; ca=cn;
    }
    while (TablaDeJoc[ln][cn]!=0);
}

bool punerePiesa()
{
    int linia,coloana,x,y;
    int stanga=50, sus=150;
    int dreapta=50+50*n, jos=150+50*n;
    bool ok=false;
    bool alegere_corecta;
    int lv1=lv, cv1=cv, la1=la, ca1=ca;
    do
    {
        if ((cn >= n-1 && ln > n-1) || (cn > n-1 && ln >= n-1)) // verifica daca ai castigat 
        {
            calculeazaLoculUndeTrebuiePusaPiesa(); 
            if (cn == n && ln == n)
            {
                win = 1;
                break;
            }
        }
        alegere_corecta=false;
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            x=mousex();
            y=mousey();
            if (x>=stanga && x<=dreapta && y>=sus&&y<=jos)
            {
                linia=(y-sus)/50+1;
                coloana=(x-stanga)/50+1;
                calculeazaLoculUndeTrebuiePusaPiesa(); // determina ln,cn
                alegere_corecta=TablaDeJoc[linia][coloana]==0 && linia==ln && coloana==cn;
                // cout<<"lv="<<lv<<" cv="<<cv<<", la="<<la<<" ca="<<ca<<" -> ln="<<ln<<" cn="<<cn<<endl;
                if (alegere_corecta)
                {
                    Beep(1500,200);
                    TablaDeJoc[linia][coloana]=piesaAleasa;
                    desPiesa(piesaAleasa,linia,coloana);

                }
                else 
                {
                    Beep(200,200);
                    lv=lv1; cv=cv1; la=la1; ca=ca1;
                }
            }
        }
    }
    while (!alegere_corecta);
    return ok;
}

void afiseazaMeniul()
{
    readimagefile("1.jpg",50,25,100,75);
    readimagefile("2.jpg",150,25,200,75);
    readimagefile("3.jpg",250,25,300,75);
    readimagefile("0.jpg",350,25,400,75);
}

bool jocFinal()
{
    if (TablaDeJoc[ln][cn] == 4)
        return 1;
    else
        return 0;
}

int main()
{
    initwindow(1000,800);
    setbkcolor(3);
    cleardevice();
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 5);
    outtextxy(250, 700, "COLTUL NEGRU");
    afiseazaMeniul();
    initTabla();
    desTabla();
    int i = 0;
    int continua;

    do
    {
        i++;
        player = i % 2; // 2 jucatori: nr 1 si nr 0;
        piesaAleasa=0;
        do
        {
            alegePiesa(piesaAleasa);
            if (piesaAleasa==4)
            {
                closegraph();
                return 0;
            }
        }
        while (piesaAleasa==0);
        punerePiesa();
        
    }
    while (!win);
    cout << "Player " << player << "win";
    cleardevice();
    if (player == 0)
    {
        outtextxy(250, 700, "PLAYER 0 WIN");
    }
    else
        outtextxy(250, 700, "PLAYER 1 WIN");
    outtextxy(0, 0, "PRESS LEFT TO RESTART");
    outtextxy(0, 100, "PRESS any key TO QUIT");
    char c;
    c = (char)getch();
    if (c == KEY_F1)
        main();
    closegraph();

    return 0;
}
