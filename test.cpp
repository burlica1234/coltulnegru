#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "winbgim.h"
#include <string.h>
#include "graphics.h"
#include <fstream>
#include <ctime>
using namespace std;
ifstream fin("fis.txt");
ifstream finn("fis_out.txt");
ifstream re("fis_progres.txt");
#define MAX_STATES 100
#define MAX 100
int i = 0;
int ver = 0;
int alegere = 1;
int player = 0;
int win = 0;
int margine = 0;
int TablaDeJoc[MAX][MAX];
// 1 +  2 \\ 3 //
int n;
int piesaAleasa;
int lv, cv, la, ca, ln, cn;
int lnV, cnV;
int depth = 5;
struct GameState {
    int TablaDeJoc[MAX][MAX];
    int player;
    int la, ca, lv, cv; // ultimele poz active/vizitate
};
int joccurent = -1;

int lr, cr;

GameState gameStates[MAX_STATES];
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

void initTablaFisier()
{
    n = 8;

    int camp;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
        {
            fin >> camp;
            TablaDeJoc[i][j] = camp;
        }
    la = 1;
    ca = 1;
    lv = 1;
    cv = 0;
}

void initProgres()
{
    n = 8;
    int camp;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
        {
            finn >> camp;
            TablaDeJoc[i][j] = camp;
        }
    finn >> la >> ca >> lv >> cv;

}

void salvareProgres()
{
    ofstream fout("fis_out.txt", ios::trunc);
    ofstream wr("fis_progres.txt", ios::trunc);
    wr << ver << " ";
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            fout << TablaDeJoc[i][j] << " ";
        }
        fout << '\n';
    }
    fout << la << " " << ca << " " << lv << " " << cv << "\n";
    fout.close();

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
        if (x >= 600 && x <= 750 && y >= 350 && y <= 400)p = 5;
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
void saveCurrentState() {
    if (joccurent < MAX_STATES - 1) {
        joccurent++;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                gameStates[joccurent].TablaDeJoc[i][j] = TablaDeJoc[i][j];
            }
        }
        gameStates[joccurent].player = player;
        gameStates[joccurent].la = la;
        gameStates[joccurent].ca = ca;
        gameStates[joccurent].lv = lv;
        gameStates[joccurent].cv = cv;
    }

}
bool punerePiesa()
{
    saveCurrentState();
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
                    lr = linia;
                    cr = coloana;
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

bool verificaButonMeniu(int left, int top, int right, int bottom,int &ref) {

    if (ismouseclick(WM_LBUTTONDOWN)) {
        int x = mousex();
        int y = mousey();
        clearmouseclick(WM_LBUTTONDOWN);


        if (x > left && x < right && y > top && y < bottom) {
            ref = 1;
            return true;
        }
    }
    return false;
}
bool verificaButonMeniu2(int left, int top, int right, int bottom, int& ref) {

    if (ismouseclick(WM_LBUTTONDOWN)) {
        int x = mousex();
        int y = mousey();
        


        if (x > left && x < right && y > top && y < bottom) {
            ref = 1;
            return true;
        }
    }
    return false;
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

    
    int ref1 = 0;
    
   
        while (!verificaButonMeniu(btnLeft, btnTop, btnRight, btnBottom,ref1)) {
            delay(100);
        }
       
}
void showRules() {

    cleardevice();
    setbkcolor(COLOR(0, 100, 100));
    cleardevice();
    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    outtextxy(100, 50, "Reguli:");


    outtextxy(100, 100, "1. Jocul incepe de la primul patrat al tablei, unde este deja plasata piesa in forma de cruce.");
    outtextxy(100, 150, "2. Linia neagra nu trebuie sa ajunga la margine, in caz contrar, celalalt jucator castiga.");
    outtextxy(100, 200, "3. Fiecare jucator are dreptul la cate o mutare, pe rand.");
    outtextxy(100, 250, "4. Este strict interzisa folosirea inteligentei artificiale!");



    int btnBackLeft = 200, btnBackTop = 700, btnBackRight = 400, btnBackBottom = 750;
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar3d(btnBackLeft, btnBackTop, btnBackRight, btnBackBottom, depth, 1);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(btnBackLeft + depth, btnBackTop + depth, btnBackRight - depth, btnBackBottom - depth);
    setcolor(BLACK);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    outtextxy(btnBackLeft + depth + 18, btnBackTop + depth + 10, "Play against a friend");

    int btnBackLeft2 = 600, btnBackTop2 = 700, btnBackRight2 = 800, btnBackBottom2 = 750;
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar3d(btnBackLeft2, btnBackTop2, btnBackRight2, btnBackBottom2, depth, 1);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(btnBackLeft2 + depth, btnBackTop2 + depth, btnBackRight2 - depth, btnBackBottom2 - depth);
    setcolor(BLACK);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    outtextxy(btnBackLeft2 + depth + 6, btnBackTop2 + depth + 10, "Play against computer");


    int alg1 = 0;
    int alg2 = 0;
    
    while (true) {
        
        if (verificaButonMeniu2(btnBackLeft2, btnBackTop2, btnBackRight2, btnBackBottom2, alg2)) { clearmouseclick(WM_LBUTTONDOWN); break; }
        if (verificaButonMeniu2(btnBackLeft, btnBackTop, btnBackRight, btnBackBottom, alg1)) { clearmouseclick(WM_LBUTTONDOWN); break;  }
            clearmouseclick(WM_LBUTTONDOWN);
        delay(100);
    }
    if (alg1)
        alegere = 1;
    if (alg2)
        alegere = 2;
    //cleardevice();
    //deseneazaMeniu();

}
void sfarsit()
{


    if (margine)
    {
        setcolor(BLACK);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
        outtextxy(400, 600, "Ai ajuns la margine! ");
        if (player)
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
            outtextxy(410, 700, "PLAYER 2 / COMPUTER WIN");
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

    int asas = 0;

    while (!verificaButonMeniu(btnLeft, btnTop, btnRight, btnBottom,asas)) {
        delay(100);
    }


}
void drawUndoButton() {
    // desenare buton 3d pt meniu
    int btnLeft = 600, btnTop = 350, btnRight = 750, btnBottom = 400;
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
    outtextxy(btnLeft + depth + 50, btnTop + depth + 10, "Undo");
}


void undoMove() {
    saveCurrentState();
    if (joccurent > 0) {
        joccurent--;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                TablaDeJoc[i][j] = gameStates[joccurent].TablaDeJoc[i][j];
            }
        }
        player = gameStates[joccurent].player;
        la = gameStates[joccurent].la;
        ca = gameStates[joccurent].ca;
        lv = gameStates[joccurent].lv;
        cv = gameStates[joccurent].cv;
        desPiesa(0, lr, cr);



    }

}
bool isInsideUndoButton(int x, int y) {

    int left = 600, top = 350, right = 750, bottom = 400;
    return (x > left && x < right && y > top && y < bottom);
}

void checkForUndoClick() {
    if (ismouseclick(WM_LBUTTONDOWN)) {
        int x = mousex();
        int y = mousey();
        clearmouseclick(WM_LBUTTONDOWN);
        if (isInsideUndoButton(x, y)) {
            undoMove();
        }
    }
}

void Calculator()
{
    int random;
    srand((unsigned)time(NULL));
    random = 1 + rand() % 3;
    piesaAleasa = random;
    calculeazaLoculUndeTrebuiePusaPiesa();
    Beep(1500, 200);
    int linia = ln;
    int coloana = cn;
    TablaDeJoc[linia][coloana] = piesaAleasa;
    desPiesa(piesaAleasa, linia, coloana);
   

}

void jocCalculator()
{
    int aFostPusaPiesa = 0;
    
    do
    {
        i++;
        
        player = i % 2; // 2 jucatori: nr 1 si nr 0;
        
        piesaAleasa = 0;

        if (player == 0 && aFostPusaPiesa)
        {
            Calculator();
            aFostPusaPiesa = 0;
        }
        else
        {
            while (piesaAleasa != 1 && piesaAleasa != 2 && piesaAleasa != 3 && piesaAleasa != 4)
                alegePiesa(piesaAleasa);
        }


        if (piesaAleasa == 4) {
            // salveaza progresul in fisier;
            ver = 1;
            i--;
            ofstream wr("fis_progres.txt", ios::trunc);
            wr << ver << " " << alegere << " " << i;

            salvareProgres();
            saveCurrentState();
            return;
        }
        else if (piesaAleasa == 5) {
            undoMove();
            saveCurrentState();

            i--;
        }
        else if (piesaAleasa == 1 || piesaAleasa == 2 || piesaAleasa == 3) {
            if (player)
            {
                punerePiesa();
                aFostPusaPiesa = 1;
            }
            jocFinal();
        }
        else {
            i--;
        }




    } while (!win && !margine);


}


int main()
{
    initwindow(1000, 800);
    deseneazaMeniu();
    showRules();
    setbkcolor(3);
    cleardevice();
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 5);
    outtextxy(250, 700, "COLTUL NEGRU");
    afiseazaMeniul();
    //initTabla();
    re >> ver;
    if (ver)
    {
        initProgres();
    }
    else
        initTablaFisier();
    desTabla();
    setcolor(WHITE);
    circle(900, 700, 50);
    setfillstyle(SLASH_FILL, LIGHTBLUE);
    floodfill(900, 700, WHITE);
    drawUndoButton();
    checkForUndoClick();
    //re >> alegere >> i;
    //if (alegere != 1 && alegere != 2)
        //input de la utilizator: 1 - joc in doi; 2 - joc calculator;
    //alegere = 2;
    if (alegere == 1)
    {
        do
        {
            i++;

            player = i % 2; // 2 jucatori: nr 1 si nr 0;
            piesaAleasa = 0;
            alegePiesa(piesaAleasa);

            if (piesaAleasa == 4) {
                // salveaza progresul in fisier;
                ver = 1;
                i--;
                ofstream wr("fis_progres.txt", ios::trunc);
                wr << ver << " " << alegere << " " << i;

                salvareProgres();
                saveCurrentState();
                closegraph();
                return 0;
            }
            else if (piesaAleasa == 5) {
                undoMove();
                saveCurrentState();

                i--;
            }
            else if (piesaAleasa == 1 || piesaAleasa == 2 || piesaAleasa == 3) {
                punerePiesa();
                jocFinal();
            }
            else {
                i--;
            }

        } while (!win && !margine);
    }
    else if (alegere == 2)
    {
        jocCalculator();
    }
    


    cleardevice();
    sfarsit();
    ver = 0;
    alegere = -1;
    i = 0;
    ofstream wr("fis_progres.txt", ios::trunc);
    wr << ver << " " << alegere << " " << i;
    closegraph();
    return 0;
}
