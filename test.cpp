#include "graphics.h"
#include <iostream>
#include <math.h>
#include <stdlib.h> // Provides exit
#include <ctype.h>

#define MAX 20
using namespace std;

int TablaDeJoc[MAX][MAX];
int n;
int lv, cv, la, ca, ln, cn;
void patrate()
{
    for (int i = 0; i <= 3; i++)
        for (int j = 0; j <= 3; j++)
            rectangle(100 + j * 200, 100 + i * 200, 300 + j * 200, 300 + i * 200);
}
void patrat1()
{
    for (int i = 0; i < 3; i++)
        rectangle(125 + i * 25, 125 + i * 25, 275 - i * 25, 275 - i * 25);
    rectangle(195, 195, 205, 205);
}

void patrat4()
{
    for (int i = 0; i < 6; i++)
    {
        line(300, 500, 500 - i * 33, 300);
        line(500, 500, 300 + i * 33, 300);
    }
}
void poligon(int xc, int yc, int n, float alfa, int r)
{
    float beta = 2 * 3.14159265 / n;
    int x[100], y[100];
    for (int i = 0; i <= n; i++)
    {
        x[i] = xc + r * cos(alfa + beta * i);
        y[i] = yc + r * sin(alfa + beta * i);
        
    }
    for (int i = 0; i < n; i++)
        line(x[i], y[i], x[i + 1], y[i + 1]);
}
void romb(int xc, int yc, int r)
{
    line(xc, yc - r, xc + r, yc);
    line(xc + r, yc, xc, yc + r);
    line(xc, yc + r, xc - r, yc);
    line(xc - r, yc, xc, yc - r);
}
void diamant(int xc, int yc, int r)
{
    if (r > 10)
    {
        romb(xc, yc, r);
        diamant(xc, yc - r, r / 2);
        diamant(xc + r, yc, r / 2);
        diamant(xc, yc + r, r / 2);
        diamant(xc - r, yc, r / 2);
    }
}

void afiseazaMeniul()
{
    readimagefile("1.jpg", 50, 25, 100, 75);
    readimagefile("2.jpg", 150, 25, 200, 75);
    readimagefile("3.jpg", 250, 25, 300, 75);
    readimagefile("0.jpg", 350, 25, 400, 75);
}
void initTabla()
{
    n = 8;
    int i, j;
    for (i = 1; i <= n; i++)
        for (j = 1; j <= n; j++)
            TablaDeJoc[i][j] = 0;
    TablaDeJoc[1][1] = 3; // +
    TablaDeJoc[1][0] = 3; // +
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
int main()
{
    initwindow(800, 1000);
    //patrate();
    afiseazaMeniul();
    initTabla();
    desTabla();
    //patrate();
    //patrat1();
    //patrat4();
    //poligon(500,300,10,30,90);
    //diamant(100, 150, 50);
    getch();
    closegraph();
    return 0;
}
