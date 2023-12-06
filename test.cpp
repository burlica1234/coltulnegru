#include <iostream>
#include "winbgim.h"
#include <string.h>
#include "graphics.h"


#define MAX 20
using namespace std;

int tabla[MAX][MAX];
int n; 
int PiesaAleasa;


void initTabla()
{
	n = 8;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			tabla[i][j] = 0;
		}
	}
	tabla[1][1] = 3;
	tabla[8][8] = 4;
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
	case 2 :
		strcpy(numeFisier, "2.jpg");
		break;
	case 3:
		strcpy(numeFisier, "3.jpg");
		break;
	case 4:
		strcpy(numeFisier, "4.jpg");

	}
	readimagefile(numeFisier, 70 * c, 55 + 70 * l, 70 * (c + 1) - 2, 55 + 70 * (l + 1) - 2);
	// left, top, right, bottom;

	/*readimagefile("1.jpg", 100, 100, 50, 50);
	readimagefile("1.jpg", 150, 100, 100, 50);*/
}


void desTabla()
{
	int i, j, p;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			p = tabla[i][j];
			desPiesa(p, i, j);
		}
	}
}

int main()
{

	initwindow(1000, 800);
	setbkcolor(3);
	cleardevice();
	settextstyle(EUROPEAN_FONT, HORIZ_DIR, 5);
	outtextxy(250, 700, "COLTUL NEGRU");
	initTabla();
	desTabla();
	getch();
	closegraph();
	return 0;
}

