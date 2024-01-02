#include <iostream>
#include "winbgim.h"
#include <string.h>
#include "graphics.h"

#define MAX 20

using namespace std;
int player = 0;
int win = 0;
int margine = 0;
int TablaDeJoc[MAX][MAX];
// 1 +  2 \\ 3 //
int n;
int piesaAleasa;
int lv, cv, la, ca, ln, cn;
int lnV, cnV;
int main();
void initTabla()
{
    n = 8;
    int i, j;
    for (i = 1; i <= n; i++)
        for (j = 1; j <= n; j++)
            TablaDeJoc[i][j] = 0;
    TablaDeJoc[1][1] = 3; // +
    TablaDeJoc[1][0] = 3;
    TablaDeJoc[n][n] = 4;

    la = 1;
    ca = 1;
    lv = 1;
    cv = 0;
}

void desPiesa(int p, int l, int c)
{
    char numeFisier[10];
    switch (p)
    {
    case 0:
        strcpy(numeFisier, "0.jpg");
        break;
    case 1:
        strcpy(numeFisier, "1.jpg");
        break;
    case 2:
        strcpy(numeFisier, "2.jpg");
        break;
    case 3:
        strcpy(numeFisier, "3.jpg");
        break;
    case 4:
        strcpy(numeFisier, "4.jpg");
        break;
    }
    readimagefile(numeFisier, 50 * c, 100 + 50 * l, 50 * (c + 1) - 2, 100 + 50 * (l + 1) - 2);
}

void desTabla()
{
    int i, j, p;
    for (i = 1; i <= n; i++)
    {
        for (j = 1; j <= n; j++)
        {
            p = TablaDeJoc[i][j];
            desPiesa(p, i, j);
        }
    }
}

void alegePiesa(int& p)
{
    int x, y;
    p = 0;
    if (ismouseclick(WM_LBUTTONDOWN))
    {
        clearmouseclick(WM_LBUTTONDOWN);
        x = mousex();
        y = mousey();
        if (x >= 50 && x <= 100 && y >= 25 && y <= 75) p = 1;
        if (x >= 150 && x <= 200 && y >= 25 && y <= 75) p = 2;
        if (x >= 250 && x <= 300 && y >= 25 && y <= 75) p = 3;
        if (x >= 350 && x <= 400 && y >= 25 && y <= 75) p = 4;
        Beep(1000, 200);
    }
}

bool vinDinStanga()
{
    return lv == la && cv == ca - 1;
}

bool vinDeSus()
{
    return lv == la - 1 && cv == ca;
}

bool vinDinDreapta()
{
    return lv == la && cv == ca + 1;
}

bool vinDeJos()
{
    return lv == la + 1 && cv == ca;
}

void mergJos()
{
    cn = ca;
    ln = la + 1;

}

void mergSus()
{
    cn = ca;
    ln = la - 1;

}

void mergStanga()
{
    cn = ca - 1;
    ln = la;

}

void mergDreapta()
{
    cn = ca + 1;
    ln = la;

}

void calculeazaLoculUndeTrebuiePusaPiesa()
{
    do
    {

        switch (TablaDeJoc[la][ca])
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
        lv = la; cv = ca;
        la = ln; ca = cn;
    } while (TablaDeJoc[ln][cn] != 0);
}

void verificaJoc()
{
    switch (TablaDeJoc[la][ca])
    {
        /* piesa 1 = \\ */
    case 1:
        if (vinDinStanga())
        {
            cnV = ca;
            lnV = la + 1;
        }
        else if (vinDinDreapta())
        {
            cnV = ca;
            lnV = la - 1;
        }
        else if (vinDeJos())
        {
            cnV = ca - 1;
            lnV = la;
        }
        else
        {
            cnV = ca + 1;
            lnV = la;
        }
        break;
        /* piesa 2 = // */
    case 2:
        if (vinDinStanga())
        {
            cnV = ca;
            lnV = la - 1;
        }
        else if (vinDinDreapta())
        {
            cnV = ca;
            lnV = la + 1;
        }
        else if (vinDeJos())
        {
            cnV = ca + 1;
            lnV = la;
        }
        else
        {
            cnV = ca - 1;
            lnV = la;
        }
        break;
        /* piesa 3 = + */
    case 3:
        if (vinDinStanga())
        {
            cnV = ca + 1;
            lnV = la;
        }
        else if (vinDinDreapta())
        {
            cnV = ca - 1;
            lnV = la;
        }
        else if (vinDeJos())
        {
            cnV = ca;
            lnV = la - 1;
        }
        else
        {
            cnV = ca;
            lnV = la + 1;
        }
        break;
    }
}

bool punerePiesa()
{
    int linia, coloana, x, y;
    int stanga = 50, sus = 150;
    int dreapta = 50 + 50 * n, jos = 150 + 50 * n;
    bool ok = false;
    bool alegere_corecta;
    int lv1 = lv, cv1 = cv, la1 = la, ca1 = ca;
    do
    {

        alegere_corecta = false;
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            x = mousex();
            y = mousey();
            if (x >= stanga && x <= dreapta && y >= sus && y <= jos)
            {
                linia = (y - sus) / 50 + 1;
                coloana = (x - stanga) / 50 + 1;
                calculeazaLoculUndeTrebuiePusaPiesa(); // determina ln,cn
                alegere_corecta = TablaDeJoc[linia][coloana] == 0 && linia == ln && coloana == cn;
                // cout<<"lv="<<lv<<" cv="<<cv<<", la="<<la<<" ca="<<ca<<" -> ln="<<ln<<" cn="<<cn<<endl;
                if (alegere_corecta)
                {
                    Beep(1500, 200);
                    TablaDeJoc[linia][coloana] = piesaAleasa;
                    desPiesa(piesaAleasa, linia, coloana);

                }
                else
                {
                    Beep(200, 200);
                    lv = lv1; cv = cv1; la = la1; ca = ca1;
                }
            }
        }
    } while (!alegere_corecta);
    return ok;
}

void afiseazaMeniul()
{
    readimagefile("1.jpg", 50, 25, 100, 75);
    readimagefile("2.jpg", 150, 25, 200, 75);
    readimagefile("3.jpg", 250, 25, 300, 75);
    readimagefile("0.jpg", 350, 25, 400, 75);
}

void jocFinal()
{

    if (cn == n || ln == n || cn == 1 || ln == 1)
    {
        verificaJoc();
        if (cnV > n || cnV < 1 || lnV > n || lnV < 1) // verifica daca ai ajuns la margine 
        {
            margine = 1;
            
        }
    }
    if ((cn >= n - 1 && ln > n - 1) || (cn > n - 1 && ln >= n - 1)) // verifica daca ai castigat 
    {
        verificaJoc();
        if (cnV == n && lnV == n)
        {
            win = 1;
        }
    }
}

bool verificaButonMeniu(int left, int top, int right, int bottom) {
    if (ismouseclick(WM_LBUTTONDOWN)) {
        int x = mousex();
        int y = mousey();
        clearmouseclick(WM_LBUTTONDOWN);


        if (x > left && x < right && y > top && y < bottom) {
            return true;
        }
    }
    return false;
}
bool ButonActivare(int left, int top, int right, int bottom) {
    if (ismouseclick(WM_LBUTTONDOWN)) {
        int x = mousex();
        int y = mousey();
        clearmouseclick(WM_LBUTTONDOWN);


        if (x > left && x < right && y > top && y < bottom) {
            return false;
        }
    }
    return true;
}
void deseneazaMeniu() {
    cleardevice();


    setbkcolor(COLOR(0, 100, 100));
    cleardevice();

    // culore meniu plus titlu
    setcolor(WHITE);
    settextstyle(BOLD_FONT, HORIZ_DIR, 5);
    outtextxy(300, 200, "Coltul Negru");

    // desenare buton 3d pt meniu
    int btnLeft = 400, btnTop = 350, btnRight = 600, btnBottom = 400;
    int depth = 5; // adancime buton 3d(cat de bine se vede paralelipipedul)
    // stg si top 
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar3d(btnLeft, btnTop, btnRight, btnBottom, depth, 1);

    // fata butonului
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(btnLeft + depth, btnTop + depth, btnRight - depth, btnBottom - depth);

    // scris buton 
    setcolor(BLACK);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    outtextxy(btnLeft + depth + 50, btnTop + depth + 10, "Start Game");

    // infrumusetare meniu,adaugare cercuri
    setcolor(WHITE);
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
    circle(100, 100, 50);
    setfillstyle(SLASH_FILL, LIGHTBLUE);
    floodfill(100, 100, WHITE);

    // cerc dreapta jos
    setcolor(WHITE);
    circle(900, 700, 50);
    setfillstyle(SLASH_FILL, LIGHTBLUE);
    floodfill(900, 700, WHITE);



    while (!verificaButonMeniu(btnLeft, btnTop, btnRight, btnBottom)) {
        delay(100);
    }
}

void sfarsit()
{
    

    if (margine)
    {
        setcolor(BLACK);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
        outtextxy(400, 600, "Ai ajuns la margine! ");
        if(player)
            outtextxy(410, 700, "PLAYER 2 WIN");
        else
            outtextxy(410, 700, "PLAYER 1 WIN");

    }
    else
    {
        if (player)
        {
            setcolor(BLACK);
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
            outtextxy(410, 700, "PLAYER 1 WIN");
        }
        else
        {
            setcolor(BLACK);
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
            outtextxy(410, 700, "PLAYER 2 WIN");
        }
    }
    // desenare buton 3d pt meniu
    int btnLeft = 400, btnTop = 350, btnRight = 550, btnBottom = 400;
    int depth = 5; // adancime buton 3d(cat de bine se vede paralelipipedul)
    // stg si top 
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar3d(btnLeft, btnTop, btnRight, btnBottom, depth, 1);

    // fata butonului
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(btnLeft + depth, btnTop + depth, btnRight - depth, btnBottom - depth);

    // scris buton 
    setcolor(BLACK);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    outtextxy(btnLeft + depth + 50, btnTop + depth + 10, "Exit");

    while (!verificaButonMeniu(btnLeft, btnTop, btnRight, btnBottom)) {
        delay(100);
    }


}

int main()
{
    initwindow(1000, 800);
    deseneazaMeniu();

    setbkcolor(3);
    cleardevice();
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 5);
    outtextxy(250, 700, "COLTUL NEGRU");
    afiseazaMeniul();
    initTabla();
    desTabla();
    int i = 0;


    do
    {
        i++;
        player = i % 2; // 2 jucatori: nr 1 si nr 0;
        piesaAleasa = 0;
        do
        {
            alegePiesa(piesaAleasa);
            if (piesaAleasa == 4)
            {
                closegraph();
                return 0;
            }
        } while (piesaAleasa == 0);
        
        punerePiesa();
        jocFinal();

    } while (!win && !margine);
    
    cleardevice();

  
    sfarsit();

    closegraph();

    return 0;
}
