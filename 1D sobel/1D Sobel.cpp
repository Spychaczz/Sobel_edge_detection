#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
using namespace std;

struct bfh {
	char bfType[3];
	unsigned int bfSize;
	unsigned short int bfReserved1;
	unsigned short int bfReserved2;
	unsigned long bfOffbits;
};
bfh BFH;

struct bih {
	unsigned int biSize;
	long biWidth;
	long biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	unsigned long biXpelsPerMeter;
	unsigned long biYpelsPerMeter;
	unsigned long biCrlUses;
	unsigned long biCrlImportant;
};

struct kolor
{
	unsigned char RED;
	unsigned char GREEN;
	unsigned char BLUE;
};
unsigned char zerowyBajt = 0;

void WczytajNaglowek(ifstream& bitmapa, ofstream& nowabitmapa, string& nazwaBMP, string& nowa_nazwa, bih& BIH, bfh& BFH, char calyNaglowek[54]){
	bool wczytanieBitmapy = false;
	while (!wczytanieBitmapy)
	{
		cout << "Podaj nazwe pliku bmp (wraz z koncowka '.bmp'): " << endl;
		cin >> nazwaBMP;
		cout << "Podaj nazwe nowego pliku do ktorego zapisac obraz: " << endl;
		cin >> nowa_nazwa;
		bitmapa.open(nazwaBMP.c_str(), ios::binary);
		nowabitmapa.open(nowa_nazwa.c_str(), ios::binary);
		if (!bitmapa)
		{
			system("cls");
			cout << "Nie udalo sie wczytac pliku!" << endl;
		}
		else {
			wczytanieBitmapy = true;

			system("cls");
			// odczyt bfh
			bitmapa.read(reinterpret_cast<char*>(&BFH.bfType), 2);
			bitmapa.read(reinterpret_cast<char*>(&BFH.bfSize), 4);
			bitmapa.read(reinterpret_cast<char*>(&BFH.bfReserved1), 2);
			bitmapa.read(reinterpret_cast<char*>(&BFH.bfReserved2), 2);
			bitmapa.read(reinterpret_cast<char*>(&BFH.bfOffbits), 4);
			cout << "BITMAPFILEHEADER" << endl;
			cout << "bfType: \t" << BFH.bfType[0] << BFH.bfType[1] << endl;
			cout << "bfSize: \t" << BFH.bfSize << endl;
			cout << "bfReserved1: \t" << BFH.bfReserved1 << endl;
			cout << "bfReserved2: \t" << BFH.bfReserved2 << endl;
			cout << "bfOffbits: \t" << BFH.bfOffbits << endl;

			// odczyt bih
			bitmapa.read(reinterpret_cast<char*>(&BIH.biSize), 4);
			bitmapa.read(reinterpret_cast<char*>(&BIH.biWidth), 4);
			bitmapa.read(reinterpret_cast<char*>(&BIH.biHeight), 4);
			bitmapa.read(reinterpret_cast<char*>(&BIH.biPlanes), 2);
			bitmapa.read(reinterpret_cast<char*>(&BIH.biBitCount), 2);
			bitmapa.read(reinterpret_cast<char*>(&BIH.biCompression), 4);
			bitmapa.read(reinterpret_cast<char*>(&BIH.biSizeImage), 4);
			bitmapa.read(reinterpret_cast<char*>(&BIH.biXpelsPerMeter), 4);
			bitmapa.read(reinterpret_cast<char*>(&BIH.biYpelsPerMeter), 4);
			bitmapa.read(reinterpret_cast<char*>(&BIH.biCrlUses), 4);
			bitmapa.read(reinterpret_cast<char*>(&BIH.biCrlImportant), 4);
			bitmapa.seekg(0);
			bitmapa.read(reinterpret_cast<char*>(&calyNaglowek[0]), 54);
			cout << endl << "BITMAPINFOHEADER" << endl;
			cout << "biSize: \t" << BIH.biSize << endl;
			cout << "biWidth: \t" << BIH.biWidth << endl;
			cout << "biHeight: \t" << BIH.biHeight << endl;
			cout << "biPlanes: \t" << BIH.biPlanes << endl;
			cout << "biBitCount: \t" << BIH.biBitCount << endl;
			cout << "biCompression: \t" << BIH.biCompression << endl;
			cout << "biSizeImage: \t" << BIH.biSizeImage << endl;
			cout << "biXPixPerMeter: " << BIH.biXpelsPerMeter << endl;
			cout << "biYPixPerMeter: " << BIH.biYpelsPerMeter << endl;
			cout << "biCrlUsed: \t" << BIH.biCrlUses << endl;
			cout << "biCrlImportant: " << BIH.biCrlImportant << endl;
		}
	}
}

void WczytajObraz(bih BIH, kolor* &starypixel, kolor* &nowypixel, int IloscPixeli, int width, ifstream& bitmapa){
	
	int licznik = 0;
		//odczyt obrazu
		for (unsigned int i = 0; i < IloscPixeli; i++)
		{
			if ((4 - (BIH.biWidth * 3 % 4)) != 4 and width % (i + 1) == 0 and licznik < (4 - (BIH.biWidth * 3 % 4)))//nie wczytywanie bajtow zerowych
			{
				licznik++;
				continue;
			}
			bitmapa.read(reinterpret_cast<char*>(&starypixel[i].BLUE), 1);  //wczytanie obrazu
			bitmapa.read(reinterpret_cast<char*>(&starypixel[i].GREEN), 1); //
			bitmapa.read(reinterpret_cast<char*>(&starypixel[i].RED), 1);   //

			nowypixel[i].BLUE = starypixel[i].BLUE;
			nowypixel[i].GREEN = starypixel[i].GREEN;
			nowypixel[i].RED = starypixel[i].RED;
			licznik = 0;
		}

}

void ZrobDetekcje(int width, int IloscPixeli, kolor* &starypixel, kolor * &nowypixel){
	
	int mask1[3][3] =
	{
		{-1,0,1},
		{-2,0,2},
		{-1,0,1}
	};
		
	int mask3[3][3] =
	{
		{1,2,1},
		{0,0,0},
		{-1,-2,-1}
	};
	
	int gxValBlue = 0;
	int gyValBlue = 0;

	int gxValGreen = 0;
	int gyValGreen = 0;

	int gxValRed = 0;
	int gyValRed = 0;

	int squaredBlue = 0;	
	int squaredGreen = 0;
	int squaredRed = 0;

	for (unsigned int i = width + 1; i < (IloscPixeli - width - 1); i++)
	{
		//zaczynam od i=width +1 (drugi wiersz, druga kolumna) czyli ;

		//{-1,0,1}		
		//{-2,0,2}	 	
		//{-1,0,1}		

		gxValBlue = (starypixel[i - width - 1].BLUE * mask1[0][0] + starypixel[i - width].BLUE * mask1[0][1] + starypixel[i - width + 1].BLUE * mask1[0][2]
				+ starypixel[i - 1].BLUE * mask1[1][0] + starypixel[i].BLUE * mask1[1][1] + starypixel[i + 1].BLUE * mask1[1][2] +
				starypixel[i + width - 1].BLUE * mask1[2][0] + starypixel[i + width].BLUE * mask1[2][1] + starypixel[i + width + 1].BLUE * mask1[2][2]);

		gyValBlue = (starypixel[i - width - 1].BLUE * mask3[0][0] + starypixel[i - width].BLUE * mask3[0][1] + starypixel[i - width + 1].BLUE * mask3[0][2]
				+ starypixel[i - 1].BLUE * mask3[1][0] + starypixel[i].BLUE * mask3[1][1] + starypixel[i + 1].BLUE * mask3[1][2] +
				starypixel[i + width - 1].BLUE * mask3[2][0] + starypixel[i + width].BLUE * mask3[2][1] + starypixel[i + width + 1].BLUE * mask3[2][2]);

		squaredBlue = (int)sqrt(gxValBlue * gxValBlue + gyValBlue * gyValBlue);

		gxValGreen = (starypixel[i - width - 1].GREEN * mask1[0][0] + starypixel[i - width].GREEN * mask1[0][1] + starypixel[i - width + 1].GREEN * mask1[0][2]
				+ starypixel[i - 1].GREEN * mask1[1][0] + starypixel[i].GREEN * mask1[1][1] + starypixel[i + 1].GREEN * mask1[1][2] +
				starypixel[i + width - 1].GREEN * mask1[2][0] + starypixel[i + width].GREEN * mask1[2][1] + starypixel[i + width + 1].GREEN * mask1[2][2]);

		gyValGreen = (starypixel[i - width - 1].GREEN * mask3[0][0] + starypixel[i - width].GREEN * mask3[0][1] + starypixel[i - width + 1].GREEN * mask3[0][2]
				+ starypixel[i - 1].GREEN * mask3[1][0] + starypixel[i].GREEN * mask3[1][1] + starypixel[i + 1].GREEN * mask3[1][2] +
				starypixel[i + width - 1].GREEN * mask3[2][0] + starypixel[i + width].GREEN * mask3[2][1] + starypixel[i + width + 1].GREEN * mask3[2][2]);

		squaredGreen = (int)sqrt(gxValGreen * gxValGreen + gyValGreen * gyValGreen);

		gxValRed = (starypixel[i - width - 1].RED * mask1[0][0] + starypixel[i - width].RED * mask1[0][1] + starypixel[i - width + 1].RED * mask1[0][2]
				+ starypixel[i - 1].RED * mask1[1][0] + starypixel[i].RED * mask1[1][1] + starypixel[i + 1].RED * mask1[1][2] +
				starypixel[i + width - 1].RED * mask1[2][0] + starypixel[i + width].RED * mask1[2][1] + starypixel[i + width + 1].RED * mask1[2][2]);

		gyValRed = (starypixel[i - width - 1].RED * mask3[0][0] + starypixel[i - width].RED * mask3[0][1] + starypixel[i - width + 1].RED * mask3[0][2]
				+ starypixel[i - 1].RED * mask3[1][0] + starypixel[i].RED * mask3[1][1] + starypixel[i + 1].RED * mask3[1][2] +
				starypixel[i + width - 1].RED * mask3[2][0] + starypixel[i + width].RED * mask3[2][1] + starypixel[i + width + 1].RED * mask3[2][2]);

		squaredRed = (int)sqrt(gxValRed * gxValRed + gyValRed * gyValRed);

		if (squaredBlue > 255)
		{
			nowypixel[i].BLUE = 255;
		}
		else if (squaredBlue < 0)
		{
			nowypixel[i].BLUE = 0;
		}
		else
		{
			nowypixel[i].BLUE = squaredBlue;
		}

		if (squaredGreen > 255)
		{
			nowypixel[i].GREEN = 255;
		}
		else if (squaredGreen < 0)
		{
			nowypixel[i].GREEN = 0;
		}
		else
		{
			nowypixel[i].GREEN = squaredGreen;
		}

		if (squaredRed > 255)
		{
			nowypixel[i].RED = 255;
		}
		else if (squaredRed < 0)
		{
			nowypixel[i].RED = 0;
		}
		else
		{
			nowypixel[i].RED = squaredRed;
		}
	}
}

void ZapiszNowyObraz(ofstream &nowabitmapa, char calyNaglowek[54], kolor * &nowypixel, bfh BFH, bih BIH ,int IloscPixeli, int width, unsigned char zerowyBajt){
	//wpisanie obrazu do nowego pliku
	nowabitmapa.write(reinterpret_cast<char*>(&calyNaglowek[0]), 54);	// wpisanie naglowka;

	for (unsigned int i = 0; i < IloscPixeli; i++)
	{
		nowabitmapa.write(reinterpret_cast<char*>(&nowypixel[i].BLUE), 1);
		nowabitmapa.write(reinterpret_cast<char*>(&nowypixel[i].GREEN), 1);
		nowabitmapa.write(reinterpret_cast<char*>(&nowypixel[i].RED), 1);

		if (BFH.bfSize - BFH.bfOffbits > 3 * IloscPixeli and width % (i + 1) == 0 ) //  dopisuj bajty zerowe na koncu wiersza jesli trzeba
		{
			for (int j = 0; j < (4 - ((BIH.biWidth * 3 % 4))); j++)
			{
				nowabitmapa.write(reinterpret_cast<char*>(&zerowyBajt), 1);
			}		
		}
	}
}

int main()
{

	string nazwaBMP, nowa_nazwa;
	ifstream bitmapa;
	ofstream nowabitmapa;
	char calyNaglowek[54];
	bfh BFH;
	bih BIH;

		

	int mask2[3][3] =
	{
		{0,1,2},
		{-1,0,1},
		{-2,-1,0}
	};
	
	int mask4[3][3] =
	{
		{2,1,0},
		{1,0,-1},
		{0,-1,-2}
	};

	int mask5[3][3] =
	{
		{1,0,-1},
		{2,0,-2},
		{1,0,-1}
	};

	int mask6[3][3] =
	{
		{0,-1,-2},
		{1,0,-1},
		{2,1,0}
	};

	int mask7[3][3] =
	{
		{-1,-2,-1},
		{0,0,0},
		{1,2,1}
	};

	int mask8[3][3] =
	{
		{-2,-1,0},
		{-1,0,1},
		{0,1,2}
	};

	WczytajNaglowek(bitmapa, nowabitmapa, nazwaBMP, nowa_nazwa, BIH, BFH, calyNaglowek);

	unsigned int IloscPixeli = BIH.biWidth * BIH.biHeight;
	const unsigned int width = BIH.biWidth;
	const unsigned int height = BIH.biHeight;
	cout << "ilosc pikseli: " << IloscPixeli << endl;

	kolor* nowypixel = new kolor[IloscPixeli];

	kolor* starypixel = new kolor[IloscPixeli];


	WczytajObraz(BIH, starypixel, nowypixel, IloscPixeli, width, bitmapa);

	ZrobDetekcje(width, IloscPixeli, starypixel, nowypixel);

	ZapiszNowyObraz(nowabitmapa, calyNaglowek, nowypixel, BFH, BIH, IloscPixeli, width, zerowyBajt);

	if (BFH.bfSize - BFH.bfOffbits > 3 * IloscPixeli) cout << "ilosc zerowych bajtow: " << 4 - (BIH.biWidth * 3 % 4) << endl;
	cout << "WIDTH: " << BIH.biWidth << endl;

	return 0;
	}

