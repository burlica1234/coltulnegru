#include <iostream>
#include "winbgim.h"
#include <string.h>
#include "graphics.h"


#define MAX 20
using namespace std;

int tabla[MAX][MAX];
int n;
int PiesaAleasa;
int lv, cv, la, ca, ln, cn;
int piesaAleasa;

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
	case 2:
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
		switch (tabla[la][ca])
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
		// cout<<"     lv="<<lv<<" cv="<<cv<<", la="<<la<<" ca="<<ca<<" -> ln="<<ln<<" cn="<<cn<<endl;
		lv = la; cv = ca;
		la = ln; ca = cn;
	} while (tabla[ln][cn] != 0);
}

int punerePiesa()
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
					alegere_corecta = tabla[linia][coloana] == 0 && linia == ln && coloana == cn;
					// cout<<"lv="<<lv<<" cv="<<cv<<", la="<<la<<" ca="<<ca<<" -> ln="<<ln<<" cn="<<cn<<endl;
					if (alegere_corecta)
					{
						Beep(1500, 200);
						tabla[linia][coloana] = piesaAleasa;
						desPiesa(piesaAleasa, linia, coloana);
					}
					else {
						Beep(200, 200);
						lv = lv1; cv = cv1; la = la1; ca = ca1;
					}
				}
			}
		} while (!alegere_corecta);
		return ok;
}
	bool jocFinal()
	{
		return ln == 0 || cn == 0;
	}
void afiseazaMeniul()
{
		readimagefile("1.jpg", 50, 25, 100, 75);
		readimagefile("2.jpg", 150, 25, 200, 75);
		readimagefile("3.jpg", 250, 25, 300, 75);
		readimagefile("0.jpg", 350, 25, 400, 75);
}
int main()
{

	initwindow(1000, 800);
	setbkcolor(3);
	cleardevice();
	settextstyle(EUROPEAN_FONT, HORIZ_DIR, 5);
	outtextxy(250, 700, "COLTUL NEGRU");
	afiseazaMeniul();
	initTabla();
	desTabla();
	do
	{
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
	} while (!jocFinal());
	getch();
	closegraph();
	return 0;
}
