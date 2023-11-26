#include <windows.h>
#include "resource.h"
#include <fstream>
#include <iostream>
#include <CommDlg.h>
#pragma comment(lib, "Comctl32.lib")
#include <CommCtrl.h>
#include <unordered_map>
#include <sstream>
#include <limits>
#include <iomanip>
using namespace std;

struct vets
{

	char nombre[50], usuario[10], contra[10], cedula[20], foto[300];
	vets* sig, * ant;
};

struct cita
{
	int estatus;
	char year[5], month[3], day[3], hour[3], minute[3];
	char dueño[50], especiepet[20], mascota[50], motivo[200], tel[12], cost[10];
	cita* sigcit, * antcit;
};


struct EspMas
{
	char especie[20];
	EspMas* sigesp, * antesp;
};

vets* inicio, * aux, * auxant, * who = nullptr;
cita* iniciocit, * auxcit, * auxantcit, * modificarInfo;
EspMas* inicioesp, * auxesp, * auxantesp;


char zFile[MAX_PATH]; //256 caracteres
char zFileTitle[MAX_PATH]; //256 caracteres
int howmuch, presEx, nameint = 0;
char usu[10] = { 0 };
char pasw[10] = { 0 };
char Meses[12][3] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"};
char Dias[31][3] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31"};
char Años[5][5] = {"2023", "2024"/*bi*/, "2025", "2026", "2027"};
const char* yearcomp;
const char* monthcomp;
const char* daycomp;
const char* hourcomp;
const char* minutecomp;
const char* dueñocomp;

LRESULT CALLBACK Log_in(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK main_screen(HWND hwndmain_screen, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK VetUsu(HWND hwndVetUsu, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK VetAdd(HWND hwndVetAdd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ManCit(HWND hwndManCit, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Especie(HWND hwndEspecie, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Agenda(HWND hwndAgenda, UINT msg, WPARAM wParam, LPARAM lParam);



void leerlista();
void leerCitas();
void leerEspecies();
void escribirEspecies();
void escribirCitas();
void ModificarVet(vets* infoMod, HWND hwnd);
void AddVet(vets* nuevo);
bool contieneNumeros(const char* cadena);
bool contieneLetras(const char* cadena);
void AddCita(cita* nuevo);
bool noSeRepite(char caracter, const char* cadena);
void AddEsp(EspMas* nuevo);
void guardarlista();
bool diaEnRango(const char* dayStr, int rangoInicio, int rangoFin);
int charToInt(char c);
bool esBisiesto(const char* yearStr);
void obtenerFechaYNombre(const std::string& cadena, int& year, int& month, int& day, std::string& nombre);
std::string convertirEnteroACadena(int numero);
void eliminar();
void eliminarPorFecha(cita*& iniciocit, const char* year, const char* month, const char* day, const char* dueño);
void obtenerHoraMinuto(const std::string& cadena, int& hour, int& minute);
void ModificarCit();
void escribirEnArchivo(const char* nombreArchivo, cita* iniciocit);

SYSTEMTIME st;
HWND hwnd = NULL;
HWND hwndmain_screen = NULL;
HWND hwndVetUsu = NULL;
HWND hwndVetAdd = NULL;
HWND hwndManCit = NULL;
HWND hwndEspecie = NULL;
HWND hwndAgenda = NULL;
HWND hwndFilt = NULL;



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmdLine, int cShow)
{
	leerEspecies();
	leerCitas();
	leerlista();
	GetLocalTime(&st);
	SYSTEMTIME st;


	hwnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Log_in);
	ShowWindow(hwnd, SW_SHOW);
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));


	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	escribirEspecies();
	escribirCitas();
	guardarlista();
	escribirEnArchivo("citas_tabla.txt", iniciocit);
	return msg.wParam;

}

LRESULT CALLBACK Log_in(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	case WM_INITDIALOG:
	{
		return TRUE;

	}break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{

		case IDC_BUTTON1:
		{

			GetDlgItemText(hwnd, IDC_EDIT1, usu, 10);
			GetDlgItemText(hwnd, IDC_EDIT2, pasw, 10);

			if (inicio == NULL) {
				MessageBox(hwnd, "No hay usuarios registrados. Registrese.", "AVISO", MB_OK | MB_ICONINFORMATION);
				hwndVetAdd = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG4), hwndVetAdd, VetAdd);
				ShowWindow(hwnd, SW_HIDE);
				ShowWindow(hwndVetAdd, SW_SHOWDEFAULT);
			}
			else {
				aux = inicio;
				while (aux != NULL && strcmp(usu, aux->usuario) != 0) {
					aux = aux->sig;
				}

				if (aux == NULL) {
					MessageBox(hwnd, "Matricula no encontrada", "AVISO", MB_OK | MB_ICONINFORMATION);
				}
				else {
					if (strcmp(pasw, aux->contra) == 0) {
						EndDialog(hwnd, 0);
						who = aux;
						hwndmain_screen = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hwndmain_screen, main_screen);
						ShowWindow(hwnd, SW_HIDE);
						ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);
					}
					else {

						MessageBox(hwnd, "Contraseña incorrecta", "AVISO", MB_OK | MB_ICONERROR);
					}
				}
			}

		}break;


		}break;
	}break;

	case WM_CLOSE:
	{
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc)
		{
		case IDYES:
		{
			DestroyWindow(hwnd);

		}break;
		}break;
	}break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	}
	return 0;
}

LRESULT CALLBACK main_screen(HWND hwndmain_screen, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		aux = inicio;
		while (aux->sig != nullptr && strcmp(usu, aux->usuario))
		{
			aux = aux->sig;
		}

		HBITMAP bmp; //1

		bmp = (HBITMAP)LoadImage(NULL, aux->foto, IMAGE_BITMAP, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE); //2
		HWND hPControl = GetDlgItem(hwndmain_screen, IDC_Picture);
		SendDlgItemMessage(hwndmain_screen, IDC_Picture, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); //3 
		SendMessage(hPControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

		SetDlgItemText(hwndmain_screen, IDC_STATICtext1, aux->nombre);
		return TRUE;
		

	}break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{

		case ID_DOCTORES_USUARIO:
		{
			hwndVetUsu = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG3), hwndVetUsu, VetUsu);
			ShowWindow(hwndmain_screen, SW_HIDE);
			ShowWindow(hwndVetUsu, SW_SHOWDEFAULT);

		}break;

		case ID_DOCTORES_AGREGAR:
		{
			hwndVetAdd = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG4), hwndVetAdd, VetAdd);
			ShowWindow(hwndmain_screen, SW_HIDE);
			ShowWindow(hwndVetAdd, SW_SHOWDEFAULT);

		}break;

		case ID_DOCTORES_CERRARSESION:
		{

			ShowWindow(hwndmain_screen, SW_HIDE);
			ShowWindow(hwnd, SW_SHOWDEFAULT);

		}break;

		case ID_NUEVACITA:
		{
			hwndManCit = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG5), hwndManCit, ManCit);
			ShowWindow(hwndmain_screen, SW_HIDE);
			ShowWindow(hwndManCit, SW_SHOWDEFAULT);

		}break;

		case ID_AGENDA:
		{
			hwndAgenda = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG6), hwndAgenda, Agenda);
			ShowWindow(hwndmain_screen, SW_HIDE);
			ShowWindow(hwndAgenda, SW_SHOWDEFAULT);

		}break;

		case ID_SALIR:
		{
			int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (opc)
			{
			case IDYES:
			{
				DestroyWindow(hwnd);					
			}break;

			}break;

		}break;

		}break;

	}break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);

	}break;

	}
	return 0;
}

LRESULT CALLBACK VetUsu(HWND hwndVetUsu, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		SYSTEMTIME st;
		EnableWindow(GetDlgItem(hwndVetUsu, IDC_EDIT2), false);
		EnableWindow(GetDlgItem(hwndVetUsu, IDC_EDIT5), false);
		EnableWindow(GetDlgItem(hwndVetUsu, IDC_EDIT3), false);

		aux = inicio;
		while (aux->sig != nullptr && strcmp(usu, aux->usuario))
		{
			aux = aux->sig;
		}

		SetDlgItemText(hwndVetUsu, IDC_EDIT2, aux->nombre);
		SetDlgItemText(hwndVetUsu, IDC_EDIT5, aux->cedula);
		SetDlgItemText(hwndVetUsu, IDC_EDIT3, aux->usuario);

		HBITMAP bmp; //1

		bmp = (HBITMAP)LoadImage(NULL, aux->foto, IMAGE_BITMAP, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE); //2
		HWND hPControl = GetDlgItem(hwndVetUsu, IDC_Picture);
		SendDlgItemMessage(hwndVetUsu, IDC_Picture, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); //3 
		SendMessage(hPControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

		return TRUE;

	}break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{

		case IDC_BUTTON_RETURN1:
		{
			ShowWindow(hwndVetUsu, SW_HIDE);
			ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);

		}break;

		case IDC_BUTTON2:
		{
			ShowWindow(GetDlgItem(hwndVetUsu, IDC_BUTTON3), SW_SHOW);
			ShowWindow(GetDlgItem(hwndVetUsu, IDC_BUTTON2), SW_HIDE);
			ShowWindow(GetDlgItem(hwndVetUsu, IDC_BUTTON4), SW_SHOW);
			ShowWindow(GetDlgItem(hwndVetUsu, IDC_STATICtext1), SW_SHOW);
			ShowWindow(GetDlgItem(hwndVetUsu, IDC_EDIT9), SW_SHOW);
			EnableWindow(GetDlgItem(hwndVetUsu, IDC_EDIT2), true);
			EnableWindow(GetDlgItem(hwndVetUsu, IDC_EDIT5), true);
			EnableWindow(GetDlgItem(hwndVetUsu, IDC_EDIT3), true);
			UpdateWindow(hwndVetUsu);
			InvalidateRect(hwndVetUsu, NULL, TRUE);

		}break;

		case IDC_BUTTON4:
		{
			vets* modificarInfo = new vets;

			GetDlgItemText(hwndVetUsu, IDC_EDIT2, modificarInfo->nombre, sizeof(modificarInfo->nombre));
			GetDlgItemText(hwndVetUsu, IDC_EDIT5, modificarInfo->cedula, sizeof(modificarInfo->cedula));
			GetDlgItemText(hwndVetUsu, IDC_EDIT3, modificarInfo->usuario, sizeof(modificarInfo->usuario));
			GetDlgItemText(hwndVetUsu, IDC_EDIT9, modificarInfo->contra, sizeof(modificarInfo->contra));
			strcpy_s(modificarInfo->foto, aux->foto);

			ModificarVet(modificarInfo, hwndVetUsu);

			ShowWindow(hwndVetUsu, SW_HIDE);
			ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);
		}

		case IDC_BUTTON3:
		{
			if (LOWORD(wParam) == IDC_BUTTON3 && HIWORD(wParam) == BN_CLICKED)
			{
				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hwndVetAdd;
				ofn.lpstrFilter = "Todos\0 * .*\0ImagenBMP\0 * .bmp\0";
				ofn.lpstrFile = zFile;
				ofn.lpstrFile[0] = '\0';
				ofn.lpstrFileTitle = zFileTitle;
				ofn.nMaxFileTitle = sizeof(zFileTitle);
				ofn.nMaxFile = sizeof(zFile);
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				ofn.nFilterIndex = 2;

				if (GetOpenFileName(&ofn) == TRUE)
				{
					presEx = 1;
					HBITMAP bmp; //1

					bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE); //2
					HWND hPControl = GetDlgItem(hwndVetAdd, IDC_Picture);
					SendDlgItemMessage(hwndVetAdd, IDC_Picture, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); //3 
					SendMessage(hPControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);


				}

			}

		}break;

		}break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);

	}break;

	}
	return 0;
}

LRESULT CALLBACK VetAdd(HWND hwndVetAdd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		return TRUE;

	}break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{

		case IDC_BUTTON_RETURN2:
		{
			if (howmuch == 0)
			{
				ShowWindow(hwndVetAdd, SW_HIDE);
				ShowWindow(hwnd, SW_SHOWDEFAULT);
			}
			else
			{
				ShowWindow(hwndVetAdd, SW_HIDE);
				ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);
			}

		}break;

		case IDC_BUTTON_RETURN1://guardar
		{

			int lengthNom = GetWindowTextLength(GetDlgItem(hwndVetAdd, IDC_EDIT2));
			int lengthced = GetWindowTextLength(GetDlgItem(hwndVetAdd, IDC_EDIT5));
			int lengthUsu = GetWindowTextLength(GetDlgItem(hwndVetAdd, IDC_EDIT3));
			int lengthcont = GetWindowTextLength(GetDlgItem(hwndVetAdd, IDC_EDIT4));

			if (lengthNom > 0 && lengthced > 0 && lengthUsu > 0 && lengthcont > 0 && presEx == 1)
			{
				vets* temp = new vets;
				GetDlgItemText(hwndVetAdd, IDC_EDIT2, temp->nombre, 50);
				GetDlgItemText(hwndVetAdd, IDC_EDIT5, temp->cedula, 20);
				GetDlgItemText(hwndVetAdd, IDC_EDIT3, temp->usuario, 10);
				GetDlgItemText(hwndVetAdd, IDC_EDIT4, temp->contra, 10);
				strcpy_s(temp->foto, zFile);

				if (contieneNumeros(temp->nombre) == false)
				{
					AddVet(temp);
					nameint = 1;
				}
				else
				{
					MessageBox(hwndVetAdd, "El nombre solo aceptara letras", "AVISO", MB_OK | MB_ICONERROR);
				}

			}
			else
			{
				MessageBox(hwndVetAdd, "Favor de no dejar informacion en blanco", "AVISO", MB_OK | MB_ICONERROR);
			}

			if (lengthNom > 0 && lengthced > 0 && lengthUsu > 0 && lengthcont > 0 && presEx == 1 && nameint == 1)
			{
				presEx = 0;
				if (howmuch == 0)
				{
					howmuch = 1;
					ShowWindow(hwndVetAdd, SW_HIDE);
					ShowWindow(hwnd, SW_SHOWDEFAULT);
				}
				else
				{
					ShowWindow(hwndVetAdd, SW_HIDE);
					ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);
				}
			}

		}break;

		case IDC_BUTTON3examinar:
		{
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwndVetAdd;
			ofn.lpstrFilter = "Todos\0 * .*\0ImagenBMP\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.lpstrFileTitle = zFileTitle;
			ofn.nMaxFileTitle = sizeof(zFileTitle);
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE)
			{
				presEx = 1;
				HBITMAP bmp; //1

				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE); //2
				HWND hPControl = GetDlgItem(hwnd, IDC_Picture);
				SendDlgItemMessage(hwndVetAdd, IDC_Picture, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); //3 
				SendMessage(hPControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);


			}

		}break;

		}break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);

	}break;

	}
	return 0;
}

LRESULT CALLBACK Especie(HWND hwndEspecie, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{

		return TRUE;

	}break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{

		case IDC_BUTTON2:
		{
			int lengthEsp = GetWindowTextLength(GetDlgItem(hwndEspecie, IDC_EDIT1));

			if (lengthEsp > 0)
			{
				EspMas* temp = new EspMas;
				GetDlgItemText(hwndEspecie, IDC_EDIT1, temp->especie, 20);
				AddEsp(temp);
				SetDlgItemText(hwndEspecie, IDC_EDIT1, "");
				ShowWindow(hwndEspecie, SW_HIDE);
				ShowWindow(hwndManCit, SW_HIDE);
				ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);
				UpdateWindow(hwndManCit);
			}
			else
			{
				MessageBox(hwndEspecie, "Favor de no dejar informacion en blanco", "AVISO", MB_OK | MB_ICONERROR);
			}

		}break;

		case IDC_BUTTON1:
		{
			ShowWindow(hwndEspecie, SW_HIDE);			

		}break;

		}break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);

	}break;

	}
	return 0;

}

LRESULT CALLBACK ManCit(HWND hwndManCit, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
		{
			aux = inicio;
			while (aux->sig != nullptr && strcmp(usu, aux->usuario))
			{
				aux = aux->sig;
			}

			HBITMAP bmp; //1

			bmp = (HBITMAP)LoadImage(NULL, aux->foto, IMAGE_BITMAP, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE); //2
			HWND hPControl = GetDlgItem(hwndManCit, IDC_Picture);
			SendDlgItemMessage(hwndManCit, IDC_Picture, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); //3 
			SendMessage(hPControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

			SetDlgItemText(hwndManCit, IDC_EDIT13, aux->nombre);
			GetLocalTime(&st);
			UpdateWindow(hwndManCit);
			hwndEspecie = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG7), hwndEspecie, Especie);

			EspMas* temp = inicioesp;
			
			while (temp != nullptr)
			{
				SendMessage(GetDlgItem(hwndManCit, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)temp->especie);
				temp = temp->sigesp;
			}

		}break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{

			case IDC_BUTTON_RETURN2:
			{
				ShowWindow(hwndManCit, SW_HIDE);
				ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);

			}break;

			case IDC_BUTTON_RETURN1:
			{
				char caracterAEvaluar = '.';
				int lengthNomCit = GetWindowTextLength(GetDlgItem(hwndManCit, IDC_EDIT2));
				int lengthTel = GetWindowTextLength(GetDlgItem(hwndManCit, IDC_EDIT3));
				int lengthMot = GetWindowTextLength(GetDlgItem(hwndManCit, IDC_EDIT4));
				int lengthCost = GetWindowTextLength(GetDlgItem(hwndManCit, IDC_EDIT6));
				int lengthMasNam = GetWindowTextLength(GetDlgItem(hwndManCit, IDC_EDIT5));				
				int lengthyear = GetWindowTextLength(GetDlgItem(hwndManCit, IDC_EDIT10));//
				int lengthEsp = GetWindowTextLength(GetDlgItem(hwndManCit, IDC_COMBO1));
				int lengthmont = GetWindowTextLength(GetDlgItem(hwndManCit, IDC_EDIT11));
				int lengthday = GetWindowTextLength(GetDlgItem(hwndManCit, IDC_EDIT12));
				int lengthHrs = GetWindowTextLength(GetDlgItem(hwndManCit, IDC_EDIT1));
				int lengthMin = GetWindowTextLength(GetDlgItem(hwndManCit, IDC_EDIT7));
				// Extraer la fecha y hora en variables separadas
				int yearSis = st.wYear;
				int monthSis = st.wMonth;
				int daySis = st.wDay;
				int hourSis = st.wHour;
				int minuteSis = st.wMinute;
				
				if (lengthNomCit > 0 && lengthTel > 0 && lengthMot > 0 && lengthCost > 0 && lengthMasNam > 0 && lengthyear > 0 && lengthEsp > 0 && lengthmont > 0 && lengthday > 0 && lengthMin > 0 && lengthHrs > 0 && lengthEsp > 0)
				{
					cita* temp = new cita;
					GetDlgItemText(hwndManCit, IDC_EDIT2, temp->dueño, 50);
					GetDlgItemText(hwndManCit, IDC_EDIT3, temp->tel, 12);
					GetDlgItemText(hwndManCit, IDC_EDIT4, temp->motivo, 200);
					GetDlgItemText(hwndManCit, IDC_EDIT6, temp->cost, 10);
					GetDlgItemText(hwndManCit, IDC_EDIT5, temp->mascota, 50);
					GetDlgItemText(hwndManCit, IDC_COMBO1, temp->especiepet, sizeof(temp->especiepet));
					GetDlgItemText(hwndManCit, IDC_EDIT10, temp->year, 5);
					GetDlgItemText(hwndManCit, IDC_EDIT11, temp->month, 3);
					GetDlgItemText(hwndManCit, IDC_EDIT12, temp->day, 3);
					GetDlgItemText(hwndManCit, IDC_EDIT1, temp->hour, 3);
					GetDlgItemText(hwndManCit, IDC_EDIT7, temp->minute, 3);
					


					int indiceEsp = SendMessage(GetDlgItem(hwndManCit, IDC_COMBO1), CB_GETCURSEL, 0, 0);
					
					int longTel = GetWindowTextLength(GetDlgItem(hwndManCit, IDC_EDIT3));
					int YearEnt = atoi(temp->year);
					int MonthEnt = atoi(temp->month);
					int DayEnt = atoi(temp->day);
					int HrsEnt = atoi(temp->hour);
					int MinEnt = atoi(temp->minute);

					if (YearEnt < yearSis ||
						(YearEnt == yearSis && MonthEnt < monthSis) ||
						(YearEnt == yearSis && MonthEnt == monthSis && DayEnt < daySis) ||
						(YearEnt == yearSis && MonthEnt == monthSis && DayEnt == daySis && HrsEnt < hourSis) ||
						(YearEnt == yearSis && MonthEnt == monthSis && DayEnt == daySis && HrsEnt == hourSis && MinEnt < minuteSis))
					{

						// La fecha ingresada es anterior a la fecha actual
						MessageBox(hwndManCit, "La fecha no puede ser anterior a la actual", "AVISO", MB_OK | MB_ICONERROR);
					}
					else
					{

						if (contieneNumeros(temp->dueño) == false && contieneNumeros(temp->mascota) == false)
						{
							if (contieneLetras(temp->cost))
							{
								MessageBox(hwndManCit, "El telefono, fechas y costo solo aceptaran numeros", "AVISO", MB_OK | MB_ICONERROR);
							}
							else
							{

								if (longTel == 8 || longTel == 10 || longTel == 12)
								{
									if (noSeRepite(caracterAEvaluar, temp->cost))
									{
										if (lengthyear == 4)
										{
											if (lengthmont <= 2 && lengthday <= 2 && lengthHrs <= 2 && lengthMin <= 2)
											{
												if (atoi(temp->hour) >= 0 && atoi(temp->hour) <= 24)
												{
													if (atoi(temp->minute) >= 0 && atoi(temp->minute) <= 59)
													{

														if (diaEnRango(temp->month, 1, 12))
														{

															if (isdigit(temp->year[0]) && esBisiesto(temp->year))
															{

																if (atoi(temp->month) == 2)
																{
																	if (diaEnRango(temp->day, 1, 29))
																	{

																		AddCita(temp);
																		ShowWindow(hwndManCit, SW_HIDE);
																		ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);

																	}
																	else
																	{
																		MessageBox(hwndManCit, "año bisiesto solo acepta dias del 1 - 29", "AVISO", MB_OK | MB_ICONERROR);
																	}
																}
																if (atoi(temp->month) == 1 || atoi(temp->month) == 3 || atoi(temp->month) == 5 || atoi(temp->month) == 7 || atoi(temp->month) == 8 || atoi(temp->month) == 10 || atoi(temp->month) == 12)
																{
																	if (diaEnRango(temp->day, 1, 31))
																	{

																		AddCita(temp);
																		ShowWindow(hwndManCit, SW_HIDE);
																		ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);

																	}
																	else
																	{
																		MessageBox(hwndManCit, "dicho mes hacepta de 1 - 31 dias", "AVISO", MB_OK | MB_ICONERROR);
																	}
																}
																else if (atoi(temp->month) == 4 || atoi(temp->month) == 6 || atoi(temp->month) == 9 || atoi(temp->month) == 11)
																{
																	if (diaEnRango(temp->day, 1, 30))
																	{

																		AddCita(temp);
																		ShowWindow(hwndManCit, SW_HIDE);
																		ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);

																	}
																	else
																	{
																		MessageBox(hwndManCit, "dicho mes hacepta de 1 - 30 dias", "AVISO", MB_OK | MB_ICONERROR);
																	}
																}
															}
															else
															{
																if (atoi(temp->month) == 2)
																{
																	if (diaEnRango(temp->day, 1, 28))
																	{

																		AddCita(temp);
																		ShowWindow(hwndManCit, SW_HIDE);
																		ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);

																	}
																	else
																	{
																		MessageBox(hwndManCit, "año no bisiesto solo acepta dias del 1 - 28", "AVISO", MB_OK | MB_ICONERROR);
																	}
																}
																else if (atoi(temp->month) == 1 || atoi(temp->month) == 3 || atoi(temp->month) == 5 || atoi(temp->month) == 7 || atoi(temp->month) == 8 || atoi(temp->month) == 10 || atoi(temp->month) == 12)
																{
																	if (diaEnRango(temp->day, 1, 31))
																	{

																		AddCita(temp);
																		ShowWindow(hwndManCit, SW_HIDE);
																		ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);

																	}
																	else
																	{
																		MessageBox(hwndManCit, "dicho mes hacepta de 1 - 31 dias", "AVISO", MB_OK | MB_ICONERROR);
																	}
																}
																else if (atoi(temp->month) == 4 || atoi(temp->month) == 6 || atoi(temp->month) == 9 || atoi(temp->month) == 11)
																{
																	if (diaEnRango(temp->day, 1, 30))
																	{

																		AddCita(temp);
																		ShowWindow(hwndManCit, SW_HIDE);
																		ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);

																	}
																	else
																	{
																		MessageBox(hwndManCit, "dicho mes hacepta de 1 - 30 dias", "AVISO", MB_OK | MB_ICONERROR);
																	}
																}
															}
														}
														else
														{
															MessageBox(hwndManCit, "solo hay 12 meses en el año", "AVISO", MB_OK | MB_ICONERROR);
														}





													}
													else
													{
														MessageBox(hwndManCit, "Los minutos solo aceptan de 0-59", "AVISO", MB_OK | MB_ICONERROR);
													}
												}
												else
												{
													MessageBox(hwndManCit, "la hora solo acepta de 0-24", "AVISO", MB_OK | MB_ICONERROR);
												}

											}
											else
											{
												MessageBox(hwndManCit, "El mes, dia, hora y minutos solo puede llevar hasta 2 numeros", "AVISO", MB_OK | MB_ICONERROR);
											}

										}
										else
										{
											MessageBox(hwndManCit, "El año solo puede contener 4 numeros", "AVISO", MB_OK | MB_ICONERROR);
										}

									}
									else
									{
										MessageBox(hwndManCit, "El precio solo acepara 1 solo punto para centavos", "AVISO", MB_OK | MB_ICONERROR);
									}
								}
								else
								{
									MessageBox(hwndManCit, "la longitud del telefono solo puede ser de 8, 10 o 12 numeros", "AVISO", MB_OK | MB_ICONERROR);
								}

							}
						}
						else
						{
							MessageBox(hwndManCit, "El nombre de la mascota y del dueño solo aceptara letras", "AVISO", MB_OK | MB_ICONERROR);
						}
					}
				}
				else
				{
					MessageBox(hwndManCit, "Favor de no dejar informacion en blanco", "AVISO", MB_OK | MB_ICONERROR);
				}

			}break;

			case IDC_BUTTON3:
			{

			ShowWindow(hwndEspecie, SW_SHOWDEFAULT);

			}break;


		}break;

	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);

	}break;

	}return 0;
	
	
}

LRESULT CALLBACK Agenda(HWND hwndAgenda, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{



		aux = inicio;
		while (aux->sig != nullptr && strcmp(usu, aux->usuario))
		{
			aux = aux->sig;
		}

		HBITMAP bmp; //1

		bmp = (HBITMAP)LoadImage(NULL, aux->foto, IMAGE_BITMAP, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE); //2
		HWND hPControl = GetDlgItem(hwndAgenda, IDC_Picture);
		SendDlgItemMessage(hwndAgenda, IDC_Picture, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); //3 
		SendMessage(hPControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

		SetDlgItemText(hwndAgenda, IDC_EDIT17, aux->nombre);

		int yearSis = st.wYear;
		int monthSis = st.wMonth;
		int daySis = st.wDay;
		int hourSis = st.wHour;
		int minuteSis = st.wMinute;

		auxcit = iniciocit;

		while (auxcit != nullptr)
		{
			std::string combinedString = std::string(auxcit->day) + "/" + std::string(auxcit->month) + "/" + std::string(auxcit->year) + " - " + std::string(auxcit->dueño);
			SendMessage(GetDlgItem(hwndAgenda, IDC_LIST1), LB_ADDSTRING, (WPARAM)0, (LPARAM)combinedString.c_str());
			auxcit = auxcit->sigcit;

		}

		EnableWindow(GetDlgItem(hwndAgenda, IDC_BUTTON_RETURN1), false);
		EnableWindow(GetDlgItem(hwndAgenda, IDC_BUTTON_RETURN3), false);
		EnableWindow(GetDlgItem(hwndAgenda, IDC_BUTTON2), false);
		EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT2), false);
		EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT3), false);
		EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT5), false);
		EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT8), false);
		EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT4), false);
		EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT6), false);
		EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT7), false);
		EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT15), false);//
		EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT13), false);
		EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT16), false);
		EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT14), false);
		HWND hEditControl = GetDlgItem(hwndAgenda, IDC_COMBO2);
		ShowWindow(hEditControl, SW_HIDE);
		HWND hEditControl1 = GetDlgItem(hwndAgenda, IDC_STATICfecha);
		ShowWindow(hEditControl1, SW_HIDE);
		HWND hEditControl2 = GetDlgItem(hwndAgenda, IDC_EDIT18);
		ShowWindow(hEditControl2, SW_HIDE);
		HWND hEditControl3 = GetDlgItem(hwndAgenda, IDC_EDIT19);
		ShowWindow(hEditControl3, SW_HIDE);
		HWND hEditControl4 = GetDlgItem(hwndAgenda, IDC_EDIT20);
		ShowWindow(hEditControl4, SW_HIDE);



		return TRUE;

	}break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{

		case IDC_BUTTON_RETURN2:
		{
			ShowWindow(hwndAgenda, SW_HIDE);
			ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);

		}break;

		case IDC_LIST1:
		{
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			{
				EnableWindow(GetDlgItem(hwndAgenda, IDC_BUTTON_RETURN1), true);
				EnableWindow(GetDlgItem(hwndAgenda, IDC_BUTTON_RETURN3), true);


				int indice = 0;
				char textoList[30] = { 0 };
				int yearlist, monthlist, daylist;
				std::string dueñocompstg;

				indice = SendDlgItemMessage(hwndAgenda, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwndAgenda, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)textoList);
				obtenerFechaYNombre(textoList, yearlist, monthlist, daylist, dueñocompstg);

				std::string yearcompstg = convertirEnteroACadena(yearlist);
				std::string monthcompstg = convertirEnteroACadena(monthlist);
				std::string daycompstg = convertirEnteroACadena(daylist);



				const char* yearcomp = yearcompstg.c_str();
				const char* monthcomp = monthcompstg.c_str();
				const char* daycomp = daycompstg.c_str();
				const char* dueñocomp = dueñocompstg.c_str();



				auxcit = iniciocit;
				while (auxcit->sigcit != nullptr && strcmp(yearcomp, auxcit->year) != 0 && strcmp(monthcomp, auxcit->month) != 0 && strcmp(daycomp, auxcit->day) != 0 && strcmp(dueñocomp, auxcit->dueño) != 0)
				{
					auxcit = auxcit->sigcit;

				}

				if (strcmp(yearcomp, auxcit->year) == 0 && strcmp(monthcomp, auxcit->month) == 0 && strcmp(daycomp, auxcit->day) == 0 && strcmp(dueñocomp, auxcit->dueño) == 0 && auxcit->sigcit != nullptr)
				{

					SetDlgItemText(hwndAgenda, IDC_EDIT2, auxcit->dueño);

					SetDlgItemText(hwndAgenda, IDC_EDIT3, auxcit->tel);
					SetDlgItemText(hwndAgenda, IDC_EDIT5, auxcit->mascota);
					SetDlgItemText(hwndAgenda, IDC_EDIT8, auxcit->especiepet);
					SetDlgItemText(hwndAgenda, IDC_EDIT4, auxcit->motivo);
					SetDlgItemText(hwndAgenda, IDC_EDIT6, auxcit->cost);
					SetDlgItemText(hwndAgenda, IDC_EDIT7, auxcit->day);
					SetDlgItemText(hwndAgenda, IDC_EDIT13, auxcit->month);
					SetDlgItemText(hwndAgenda, IDC_EDIT14, auxcit->year);
					SetDlgItemText(hwndAgenda, IDC_EDIT15, auxcit->hour);
					SetDlgItemText(hwndAgenda, IDC_EDIT16, auxcit->minute);

					if (auxcit->estatus == 0)
					{
						SendDlgItemMessage(hwndAgenda, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
						SendDlgItemMessage(hwndAgenda, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
						SendDlgItemMessage(hwndAgenda, IDC_RADIO3, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					}
					else if (auxcit->estatus == 1)
					{
						SendDlgItemMessage(hwndAgenda, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
						SendDlgItemMessage(hwndAgenda, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
						SendDlgItemMessage(hwndAgenda, IDC_RADIO3, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					}
					else if (auxcit->estatus == 2)
					{
						SendDlgItemMessage(hwndAgenda, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
						SendDlgItemMessage(hwndAgenda, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
						SendDlgItemMessage(hwndAgenda, IDC_RADIO3, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					}
				}
				else
				{
					MessageBox(NULL, NULL, "no se encontro", MB_OK | MB_ICONINFORMATION);
				}
			}break;

			}


		}break;

		case IDC_BUTTON_RETURN3:
		{
			int indice = 0;
			char textoList[30] = { 0 };
			char hourchar[3];
			char minutechar[3];
			int yearlist, monthlist, daylist;
			std::string dueñocompstg;

			indice = SendDlgItemMessage(hwndAgenda, IDC_LIST1, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwndAgenda, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)textoList);
			obtenerFechaYNombre(textoList, yearlist, monthlist, daylist, dueñocompstg);
			std::string yearcompstg = convertirEnteroACadena(yearlist);
			std::string monthcompstg = convertirEnteroACadena(monthlist);
			std::string daycompstg = convertirEnteroACadena(daylist);


			const char* yearcomp = yearcompstg.c_str();
			const char* monthcomp = monthcompstg.c_str();
			const char* daycomp = daycompstg.c_str();
			const char* dueñocomp = dueñocompstg.c_str();





			eliminarPorFecha(iniciocit, yearcomp, monthcomp, daycomp, dueñocomp);

			ShowWindow(hwndAgenda, SW_HIDE);
			ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);

		}break;

		case IDC_BUTTON_RETURN1:
		{
			EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT2), true);
			EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT3), true);
			EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT5), true);
			HWND hEditControlcomb = GetDlgItem(hwndAgenda, IDC_COMBO2);
			ShowWindow(hEditControlcomb, SW_SHOWDEFAULT);
			HWND hEditControl = GetDlgItem(hwndAgenda, IDC_EDIT8);
			ShowWindow(hEditControl, SW_HIDE);
			EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT4), true);
			EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT6), true);
			EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT7), true);
			EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT13), true);
			EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT14), true);
			EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT15), true);
			EnableWindow(GetDlgItem(hwndAgenda, IDC_EDIT16), true);
			EnableWindow(GetDlgItem(hwndAgenda, IDC_RADIO1), true);
			EnableWindow(GetDlgItem(hwndAgenda, IDC_RADIO2), true);
			EnableWindow(GetDlgItem(hwndAgenda, IDC_RADIO3), true);
			EnableWindow(GetDlgItem(hwndAgenda, IDC_BUTTON2), true);
			EspMas* temp = inicioesp;

			while (temp != nullptr)
			{
				SendMessage(GetDlgItem(hwndAgenda, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)temp->especie);
				temp = temp->sigesp;
			}

		}break;

		case IDC_BUTTON2:
		{
			int indice = 0;
			char textoList[30] = { 0 };
			char hourchar[3];
			char minutechar[3];
			int yearlist, monthlist, daylist;
			std::string dueñocompstg;

			indice = SendDlgItemMessage(hwndAgenda, IDC_LIST1, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwndAgenda, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)textoList);
			obtenerFechaYNombre(textoList, yearlist, monthlist, daylist, dueñocompstg);
			std::string yearcompstg = convertirEnteroACadena(yearlist);
			std::string monthcompstg = convertirEnteroACadena(monthlist);
			std::string daycompstg = convertirEnteroACadena(daylist);


			const char* yearcomp = yearcompstg.c_str();
			const char* monthcomp = monthcompstg.c_str();
			const char* daycomp = daycompstg.c_str();
			const char* dueñocomp = dueñocompstg.c_str();
			char caracterAEvaluar = '.';
			int lengthNomCit = GetWindowTextLength(GetDlgItem(hwndAgenda, IDC_EDIT2));
			int lengthTel = GetWindowTextLength(GetDlgItem(hwndAgenda, IDC_EDIT3));
			int lengthMot = GetWindowTextLength(GetDlgItem(hwndAgenda, IDC_EDIT4));
			int lengthCost = GetWindowTextLength(GetDlgItem(hwndAgenda, IDC_EDIT6));
			int lengthMasNam = GetWindowTextLength(GetDlgItem(hwndAgenda, IDC_EDIT5));
			int lengthyear = GetWindowTextLength(GetDlgItem(hwndAgenda, IDC_EDIT14));//
			int lengthEsp = GetWindowTextLength(GetDlgItem(hwndAgenda, IDC_COMBO2));
			int lengthmont = GetWindowTextLength(GetDlgItem(hwndAgenda, IDC_EDIT13));
			int lengthday = GetWindowTextLength(GetDlgItem(hwndAgenda, IDC_EDIT7));
			int lengthHrs = GetWindowTextLength(GetDlgItem(hwndAgenda, IDC_EDIT15));
			int lengthMin = GetWindowTextLength(GetDlgItem(hwndAgenda, IDC_EDIT16));
			// Extraer la fecha y hora en variables separadas
			int yearSis = st.wYear;
			int monthSis = st.wMonth;
			int daySis = st.wDay;
			int hourSis = st.wHour;
			int minuteSis = st.wMinute;

			if (lengthNomCit > 0 && lengthTel > 0 && lengthMot > 0 && lengthCost > 0 && lengthMasNam > 0 && lengthyear > 0 && lengthEsp > 0 && lengthmont > 0 && lengthday > 0 && lengthMin > 0 && lengthHrs > 0 && lengthEsp > 0)
			{
				cita* modificarInfo = new cita;
				GetDlgItemText(hwndAgenda, IDC_EDIT2, modificarInfo->dueño, sizeof(modificarInfo->dueño));
				GetDlgItemText(hwndAgenda, IDC_EDIT3, modificarInfo->tel, sizeof(modificarInfo->tel));
				GetDlgItemText(hwndAgenda, IDC_EDIT5, modificarInfo->mascota, sizeof(modificarInfo->mascota));
				GetDlgItemText(hwndAgenda, IDC_EDIT4, modificarInfo->motivo, sizeof(modificarInfo->motivo));
				GetDlgItemText(hwndAgenda, IDC_COMBO2, modificarInfo->especiepet, sizeof(modificarInfo->especiepet));
				GetDlgItemText(hwndAgenda, IDC_EDIT6, modificarInfo->cost, sizeof(modificarInfo->cost));//
				GetDlgItemText(hwndAgenda, IDC_EDIT7, modificarInfo->day, sizeof(modificarInfo->day));
				GetDlgItemText(hwndAgenda, IDC_EDIT13, modificarInfo->month, sizeof(modificarInfo->month));
				GetDlgItemText(hwndAgenda, IDC_EDIT14, modificarInfo->year, sizeof(modificarInfo->year));
				GetDlgItemText(hwndAgenda, IDC_EDIT15, modificarInfo->hour, sizeof(modificarInfo->hour));
				GetDlgItemText(hwndAgenda, IDC_EDIT16, modificarInfo->minute, sizeof(modificarInfo->minute));



				if (IsDlgButtonChecked(hwndAgenda, IDC_RADIO1) == BST_CHECKED) {
					//Seleccionó maestro
					modificarInfo->estatus = 0;
				}
				else if (IsDlgButtonChecked(hwndAgenda, IDC_RADIO2) == BST_CHECKED) {
					modificarInfo->estatus = 1;
				}
				else if (IsDlgButtonChecked(hwndAgenda, IDC_RADIO3) == BST_CHECKED) {
					modificarInfo->estatus = 2;
				}
				else
				{
					MessageBox(NULL, NULL, "Elija un estatus", MB_OK | MB_ICONINFORMATION);
				}

				int longTel = GetWindowTextLength(GetDlgItem(hwndAgenda, IDC_EDIT3));
				int YearEnt = atoi(modificarInfo->year);
				int MonthEnt = atoi(modificarInfo->month);
				int DayEnt = atoi(modificarInfo->day);
				int HrsEnt = atoi(modificarInfo->hour);
				int MinEnt = atoi(modificarInfo->minute);

				if (YearEnt < yearSis ||
					(YearEnt == yearSis && MonthEnt < monthSis) ||
					(YearEnt == yearSis && MonthEnt == monthSis && DayEnt < daySis) ||
					(YearEnt == yearSis && MonthEnt == monthSis && DayEnt == daySis && HrsEnt < hourSis) ||
					(YearEnt == yearSis && MonthEnt == monthSis && DayEnt == daySis && HrsEnt == hourSis && MinEnt < minuteSis))
				{

					// La fecha ingresada es anterior a la fecha actual
					MessageBox(hwndAgenda, "La fecha no puede ser anterior a la actual", "AVISO", MB_OK | MB_ICONERROR);
				}
				else
				{

					if (contieneNumeros(modificarInfo->dueño) == false && contieneNumeros(modificarInfo->mascota) == false)
					{
						if (contieneLetras(modificarInfo->cost))
						{
							MessageBox(hwndAgenda, "El telefono, fechas y costo solo aceptaran numeros", "AVISO", MB_OK | MB_ICONERROR);
						}
						else
						{

							if (longTel == 8 || longTel == 10 || longTel == 12)
							{
								if (noSeRepite(caracterAEvaluar, modificarInfo->cost))
								{
									if (lengthyear == 4)
									{
										if (lengthmont <= 2 && lengthday <= 2 && lengthHrs <= 2 && lengthMin <= 2)
										{
											if (atoi(modificarInfo->hour) >= 0 && atoi(modificarInfo->hour) <= 24)
											{
												if (atoi(modificarInfo->minute) >= 0 && atoi(modificarInfo->minute) <= 59)
												{

													if (diaEnRango(modificarInfo->month, 1, 12))
													{

														if (isdigit(modificarInfo->year[0]) && esBisiesto(modificarInfo->year))
														{

															if (atoi(modificarInfo->month) == 2)
															{
																if (diaEnRango(modificarInfo->day, 1, 29))
																{

																	ModificarCit();
																	ShowWindow(hwndAgenda, SW_HIDE);
																	ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);

																}
																else
																{
																	MessageBox(hwndAgenda, "año bisiesto solo acepta dias del 1 - 29", "AVISO", MB_OK | MB_ICONERROR);
																}
															}
															if (atoi(modificarInfo->month) == 1 || atoi(modificarInfo->month) == 3 || atoi(modificarInfo->month) == 5 || atoi(modificarInfo->month) == 7 || atoi(modificarInfo->month) == 8 || atoi(modificarInfo->month) == 10 || atoi(modificarInfo->month) == 12)
															{
																if (diaEnRango(modificarInfo->day, 1, 31))
																{

																	ModificarCit();
																	ShowWindow(hwndAgenda, SW_HIDE);
																	ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);

																}
																else
																{
																	MessageBox(hwndAgenda, "dicho mes hacepta de 1 - 31 dias", "AVISO", MB_OK | MB_ICONERROR);
																}
															}
															else if (atoi(modificarInfo->month) == 4 || atoi(modificarInfo->month) == 6 || atoi(modificarInfo->month) == 9 || atoi(modificarInfo->month) == 11)
															{
																if (diaEnRango(modificarInfo->day, 1, 30))
																{

																	ModificarCit();
																	ShowWindow(hwndAgenda, SW_HIDE);
																	ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);

																}
																else
																{
																	MessageBox(hwndAgenda, "dicho mes hacepta de 1 - 30 dias", "AVISO", MB_OK | MB_ICONERROR);
																}
															}
														}
														else
														{
															if (atoi(modificarInfo->month) == 2)
															{
																if (diaEnRango(modificarInfo->day, 1, 28))
																{

																	ModificarCit();
																	ShowWindow(hwndAgenda, SW_HIDE);
																	ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);

																}
																else
																{
																	MessageBox(hwndAgenda, "año no bisiesto solo acepta dias del 1 - 28", "AVISO", MB_OK | MB_ICONERROR);
																}
															}
															else if (atoi(modificarInfo->month) == 1 || atoi(modificarInfo->month) == 3 || atoi(modificarInfo->month) == 5 || atoi(modificarInfo->month) == 7 || atoi(modificarInfo->month) == 8 || atoi(modificarInfo->month) == 10 || atoi(modificarInfo->month) == 12)
															{
																if (diaEnRango(modificarInfo->day, 1, 31))
																{

																	ModificarCit();
																	ShowWindow(hwndAgenda, SW_HIDE);
																	ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);

																}
																else
																{
																	MessageBox(hwndAgenda, "dicho mes hacepta de 1 - 31 dias", "AVISO", MB_OK | MB_ICONERROR);
																}
															}
															else if (atoi(modificarInfo->month) == 4 || atoi(modificarInfo->month) == 6 || atoi(modificarInfo->month) == 9 || atoi(modificarInfo->month) == 11)
															{
																if (diaEnRango(modificarInfo->day, 1, 30))
																{

																	ModificarCit();
																	ShowWindow(hwndAgenda, SW_HIDE);
																	ShowWindow(hwndmain_screen, SW_SHOWDEFAULT);

																}
																else
																{
																	MessageBox(hwndAgenda, "dicho mes hacepta de 1 - 30 dias", "AVISO", MB_OK | MB_ICONERROR);
																}
															}
														}
													}
													else
													{
														MessageBox(hwndAgenda, "solo hay 12 meses en el año", "AVISO", MB_OK | MB_ICONERROR);
													}





												}
												else
												{
													MessageBox(hwndAgenda, "Los minutos solo aceptan de 0-59", "AVISO", MB_OK | MB_ICONERROR);
												}
											}
											else
											{
												MessageBox(hwndAgenda, "la hora solo acepta de 0-24", "AVISO", MB_OK | MB_ICONERROR);
											}

										}
										else
										{
											MessageBox(hwndAgenda, "El mes, dia, hora y minutos solo puede llevar hasta 2 numeros", "AVISO", MB_OK | MB_ICONERROR);
										}

									}
									else
									{
										MessageBox(hwndAgenda, "El año solo puede contener 4 numeros", "AVISO", MB_OK | MB_ICONERROR);
									}

								}
								else
								{
									MessageBox(hwndAgenda, "El precio solo acepara 1 solo punto para centavos", "AVISO", MB_OK | MB_ICONERROR);
								}
							}
							else
							{
								MessageBox(hwndAgenda, "la longitud del telefono solo puede ser de 8, 10 o 12 numeros", "AVISO", MB_OK | MB_ICONERROR);
							}

						}
					}
					else
					{
						MessageBox(hwndAgenda, "El nombre de la mascota y del dueño solo aceptara letras", "AVISO", MB_OK | MB_ICONERROR);
					}
				}
			}
			else
			{
				MessageBox(hwndAgenda, "Favor de no dejar informacion en blanco", "AVISO", MB_OK | MB_ICONERROR);
			}


		}break;

		case IDC_BUTTON_RETURN4:
		{

			HWND hEditControl1 = GetDlgItem(hwndAgenda, IDC_STATICfecha);
			HWND hEditControl2 = GetDlgItem(hwndAgenda, IDC_EDIT18);
			HWND hEditControl3 = GetDlgItem(hwndAgenda, IDC_EDIT19);
			HWND hEditControl4 = GetDlgItem(hwndAgenda, IDC_EDIT20);
			ShowWindow(hEditControl1, SW_SHOWDEFAULT);
			ShowWindow(hEditControl2, SW_SHOWDEFAULT);
			ShowWindow(hEditControl3, SW_SHOWDEFAULT);
			ShowWindow(hEditControl4, SW_SHOWDEFAULT);

		}break;

		case IDC_BUTTON5:
		{
			SendMessage(GetDlgItem(hwndAgenda, IDC_LIST1), LB_RESETCONTENT, 0, 0);
			int yearSis = st.wYear;
			int monthSis = st.wMonth;
			int daySis = st.wDay;
			int hourSis = st.wHour;
			int minuteSis = st.wMinute;

			cita* FiltInfo = new cita;
			GetDlgItemText(hwndAgenda, IDC_EDIT20, FiltInfo->year, sizeof(FiltInfo->year));
			GetDlgItemText(hwndAgenda, IDC_EDIT19, FiltInfo->month, sizeof(FiltInfo->month));
			GetDlgItemText(hwndAgenda, IDC_EDIT18, FiltInfo->day, sizeof(FiltInfo->day));

			int YearEnt = atoi(FiltInfo->year);
			int MonthEnt = atoi(FiltInfo->month);
			int DayEnt = atoi(FiltInfo->day);

			if (YearEnt < yearSis ||
				(YearEnt == yearSis && MonthEnt < monthSis) ||
				(YearEnt == yearSis && MonthEnt == monthSis && DayEnt < daySis))
			{

				// La fecha ingresada es anterior a la fecha actual
				MessageBox(hwndAgenda, "La fecha no puede ser anterior a la actual", "AVISO", MB_OK | MB_ICONERROR);
			}
			else
			{
				auxcit = iniciocit;
				while (auxcit->sigcit != nullptr)
				{
					if (YearEnt >= yearSis && MonthEnt >= monthSis && DayEnt >= daySis)
					{
						
						std::string combinedString = std::string(auxcit->day) + "/" + std::string(auxcit->month) + "/" + std::string(auxcit->year) + " - " + std::string(auxcit->dueño);
						SendMessage(GetDlgItem(hwndAgenda, IDC_LIST1), LB_ADDSTRING, (WPARAM)0, (LPARAM)combinedString.c_str());

					}
					auxcit = auxcit->sigcit;

				}

			}
		}break;

		}

	}break;
	
		

	
	
		
	

	

		case WM_DESTROY:
		{
			PostQuitMessage(0);

		}break;
	
	}
	return 0;
}


bool contieneNumeros(const char* cadena) {
	while (*cadena) {
		if (isdigit(*cadena)) {
			return true; // Se encontró un número
		}
		++cadena;
	}
	return false; // No se encontraron números
}

void AddVet(vets* nuevo) 
{

	if (inicio == nullptr) { //Si 'inicio es igual a nullptr, o sea, apunta a nada, la lista esta vacia
		inicio = new vets;
		aux = inicio;
		nuevo->ant = NULL;

		aux->sig = nullptr;

		strcpy_s(aux->nombre, nuevo->nombre);
		strcpy_s(aux->cedula, nuevo->cedula);
		strcpy_s(aux->contra, nuevo->contra);
		strcpy_s(aux->usuario, nuevo->usuario);
		strcpy_s(aux->foto, nuevo->foto);

		howmuch = 0;

	}
	else {
		howmuch = 1;
		aux = inicio;

		while (aux->sig != nullptr)
		{
			auxant = aux;
			aux = aux->sig;
		}

		aux->sig = new vets;

		aux->sig->sig = nullptr;

		nuevo->ant = auxant;

		aux = aux->sig;

		strcpy_s(aux->nombre, nuevo->nombre);
		strcpy_s(aux->cedula, nuevo->cedula);
		strcpy_s(aux->contra, nuevo->contra);
		strcpy_s(aux->usuario, nuevo->usuario);
		strcpy_s(aux->foto, nuevo->foto);


	}
}

void ModificarVet(vets* infoMod, HWND hwnd)
{
	aux = inicio;

	while (aux != nullptr && strcmp(usu, aux->usuario) != 0)
	{
		aux = aux->sig;
	}

	if (aux == nullptr)
	{
		MessageBox(hwndVetUsu, "Usuario no en contrado.", "AVISO", MB_OK | MB_ICONQUESTION);
	}
	else
	{
		strcpy_s(aux->nombre, infoMod->nombre);
		strcpy_s(aux->contra, infoMod->contra);
		strcpy_s(aux->cedula, infoMod->cedula);
		strcpy_s(aux->usuario, infoMod->usuario);
		strcpy_s(aux->foto, infoMod->foto);
	}
}

bool contieneLetras(const char* cadena) {
	while (*cadena) {
		if (std::isalpha(*cadena)) {
			return true;
		}
		++cadena;
	}
	return false;
}

void AddCita(cita* nuevo)
{
	
	if (iniciocit == nullptr) 
	{ //Si 'inicio es igual a nullptr, o sea, apunta a nada, la lista esta vacia
		iniciocit = new cita;
		auxcit = iniciocit;
		nuevo->antcit = NULL;

		auxcit->sigcit = nullptr;
	
		strcpy_s(auxcit->dueño, nuevo->dueño);
		strcpy_s(auxcit->mascota, nuevo->mascota);
		strcpy_s(auxcit->motivo, nuevo->motivo);
		strcpy_s(auxcit->tel, nuevo->tel);
		strcpy_s(auxcit->cost, nuevo->cost);
		strcpy_s(auxcit->especiepet, nuevo->especiepet);//
		strcpy_s(auxcit->year, nuevo->year);
		strcpy_s(auxcit->month, nuevo->month);
		strcpy_s(auxcit->day, nuevo->day);
		strcpy_s(auxcit->hour, nuevo->hour);
		strcpy_s(auxcit->minute, nuevo->minute);
		nuevo->estatus = 0;
	}
	
	else
	{
		auxcit = iniciocit;

		while (auxcit->sigcit != nullptr)
		{
			auxantcit = auxcit;
			auxcit = auxcit->sigcit;
		}

		auxcit->sigcit = new cita;

		auxcit->sigcit->sigcit = nullptr;

		nuevo->antcit = auxantcit;

		auxcit = auxcit->sigcit;

		strcpy_s(auxcit->dueño, nuevo->dueño);
		strcpy_s(auxcit->mascota, nuevo->mascota);
		strcpy_s(auxcit->motivo, nuevo->motivo);
		strcpy_s(auxcit->tel, nuevo->tel);
		strcpy_s(auxcit->cost, nuevo->cost);
		strcpy_s(auxcit->especiepet, nuevo->especiepet);//
		strcpy_s(auxcit->year, nuevo->year);
		strcpy_s(auxcit->month, nuevo->month);
		strcpy_s(auxcit->day, nuevo->day);
		strcpy_s(auxcit->hour, nuevo->hour);
		strcpy_s(auxcit->minute, nuevo->minute);
		nuevo->estatus = 0;

	}
	
}

bool noSeRepite(char caracter, const char* cadena) {
	std::unordered_map<char, int> contador;

	// Contar la ocurrencia de cada carácter en la cadena
	for (const char* ptr = cadena; *ptr != '\0'; ++ptr) {
		contador[*ptr]++;
	}

	// Verificar si el carácter no se repite más de una vez
	return contador[caracter] <= 1;
}

void AddEsp(EspMas* nuevo)
{
	if (inicioesp == nullptr) { //Si 'inicio es igual a nullptr, o sea, apunta a nada, la lista esta vacia
		inicioesp = new EspMas;
		auxesp = inicioesp;
		nuevo->antesp = NULL;

		auxesp->sigesp = nullptr;

		strcpy_s(auxesp->especie, nuevo->especie);

	}
	else
	{
		auxesp = inicioesp;

		while (auxesp->sigesp != nullptr)
		{
			auxantesp = auxesp;
			auxesp = auxesp->sigesp;
		}

		auxesp->sigesp = new EspMas;

		auxesp->sigesp->sigesp = nullptr;

		nuevo->antesp = auxantesp;

		auxesp = auxesp->sigesp;

		strcpy_s(auxesp->especie, nuevo->especie);

	}
}

void guardarlista()//vet
{
	ofstream escribir;
	escribir.open("C:\\Users\\medin\\OneDrive\\Escritorio\\ejemplo codigo progra\\test progra\\Test_PrograAv_PIA\\Test_PrograAv_PIA\\vets.bin", ios::out | ios::binary);
	aux = inicio;

	if (escribir.is_open())
	{
		while (aux != nullptr)
		{
			escribir.write((char*)aux, sizeof(vets));
			aux = aux->sig;
		}

		escribir.close();
	}

}

void leerlista()//vet
{

	ifstream leer;
	leer.open("C:\\Users\\medin\\OneDrive\\Escritorio\\ejemplo codigo progra\\test progra\\Test_PrograAv_PIA\\Test_PrograAv_PIA\\vets.bin", ios::in | ios::binary);

	aux = inicio;
	if (leer.is_open())
	{
		vets* usuLeido = new vets;

		while (!leer.read((char*)usuLeido, sizeof(vets)).eof())
		{

			while (aux != nullptr && aux->sig != nullptr)
			{
				auxant = aux;
				aux = aux->sig;
			}
			if (aux == nullptr)
			{
				inicio = usuLeido;
				inicio->sig = nullptr;
				aux = inicio;
				inicio->ant = nullptr;
			}
			else
			{
				aux->sig = usuLeido;
				aux = aux->sig;
				aux->sig = nullptr;
				aux->ant = auxant;
			}

			usuLeido = new vets;
		}
		leer.close();
	}
}

void leerCitas()
{
	ifstream leer;
	leer.open("C:\\Users\\medin\\OneDrive\\Escritorio\\ejemplo codigo progra\\test progra\\Test_PrograAv_PIA\\Test_PrograAv_PIA\\citas.bin", ios::in | ios::binary);

	auxcit = iniciocit;
	if (leer.is_open())
	{
		while (true)
		{
			cita* usuLeido = new cita;
			if (!leer.read((char*)usuLeido, sizeof(cita)))
			{
				// La lectura no tuvo éxito, probablemente llegamos al final del archivo
				delete usuLeido;
				break;
			}

			if (auxcit == nullptr)
			{
				iniciocit = usuLeido;
				iniciocit->sigcit = nullptr;
				auxcit = iniciocit;
				iniciocit->antcit = nullptr;
			}
			else
			{
				auxcit->sigcit = usuLeido;
				auxcit = auxcit->sigcit;
				auxcit->sigcit = nullptr;
				auxcit->antcit = auxantcit;
			}
			auxantcit = auxcit;
		}
		leer.close();
	}
}

void escribirCitas()
{
	ofstream escribir;
	escribir.open("C:\\Users\\medin\\OneDrive\\Escritorio\\ejemplo codigo progra\\test progra\\Test_PrograAv_PIA\\Test_PrograAv_PIA\\citas.bin", ios::out | ios::binary);


	auxcit = iniciocit;
	if (escribir.is_open())
	{
		while (auxcit != nullptr)
		{
			escribir.write((char*)auxcit, sizeof(cita));
			auxcit = auxcit->sigcit;
		}

		escribir.close();
	}
}

void leerEspecies()
{
	ifstream leer;
	leer.open("C:\\Users\\medin\\OneDrive\\Escritorio\\ejemplo codigo progra\\test progra\\Test_PrograAv_PIA\\Test_PrograAv_PIA\\Especies.bin", ios::in | ios::binary);
	auxesp = inicioesp;
	if (leer.is_open())
	{
		EspMas* usuLeido = new EspMas;

		while (!leer.read((char*)usuLeido, sizeof(EspMas)).eof())
		{
			while (auxesp != nullptr && auxesp->sigesp != nullptr)
			{
				auxantesp = auxesp;
				auxesp = auxesp->sigesp;
			}
			if (auxesp == nullptr)
			{
				inicioesp = usuLeido;
				inicioesp->sigesp = nullptr;
				auxesp = inicioesp;
				inicioesp->antesp = nullptr;
			}
			else
			{
				auxesp->sigesp = usuLeido;
				auxesp = auxesp->sigesp;
				auxesp->sigesp = nullptr;
				auxesp->antesp = auxantesp;
			}
			usuLeido = new EspMas;
		}
		leer.close();
	}

}

void escribirEspecies()
{
	ofstream escribir;
	escribir.open("C:\\Users\\medin\\OneDrive\\Escritorio\\ejemplo codigo progra\\test progra\\Test_PrograAv_PIA\\Test_PrograAv_PIA\\Especies.bin", ios::out | ios::binary);
	/*
	ofstream escribir("especies.dat", ios::out | ios::binary);*/
	auxesp = inicioesp;
	if (escribir.is_open())
	{
		while (auxesp != nullptr)
		{
			escribir.write((char*)auxesp, sizeof(EspMas));
			auxesp = auxesp->sigesp;
		}
		escribir.close();
	}
}

bool esBisiesto(const char* yearStr)
{
	// Convertir la cadena a un número entero
	int year = atoi(yearStr);

	return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

bool diaEnRango(const char* dayStr, int rangoInicio, int rangoFin)
{
	
	// Convertir el carácter a un entero
	int dia = atoi(dayStr);

	// Verificar si el día está en el rango
	return (dia >= rangoInicio && dia <= rangoFin);
}

int charToInt(char c) {
	return c - '0';
}

void obtenerFechaYNombre(const std::string& cadena, int& day, int& month, int& year, std::string& nombre)
{
	std::istringstream stream(cadena);
	char discard; // Para descartar caracteres no necesarios

	stream >> day;
	stream.ignore();  // Ignorar la barra "/"
	stream >> month;
	stream.ignore();  // Ignorar la barra "/"
	stream >> year;

	stream.ignore(3); // Ignorar " - "
	std::getline(stream >> std::ws, nombre); // Leer el nombre restante en la línea

	// Agregar mensajes de depuración
	std::ostringstream debugMessage;
	debugMessage << "Year: " << year << "\nMonth: " << month << "\nDay: " << day << "\nName: " << nombre;

	// Mostrar el cuadro de diálogo
	MessageBox(NULL, debugMessage.str().c_str(), "Valores de Fecha y Nombre", MB_OK | MB_ICONINFORMATION);
}

std::string convertirEnteroACadena(int numero)
{
	std::ostringstream stream;
	stream << numero;
	return stream.str();
}

void eliminarPorFecha(cita*& iniciocit, const char* year, const char* month, const char* day, const char* dueño) {
	auxcit = iniciocit;

	while (auxcit != nullptr) 
	{
		// Compara la fecha actual con la fecha proporcionada
		if (strcmp(auxcit->year, year) == 0 &&
			strcmp(auxcit->month, month) == 0 &&
			strcmp(auxcit->day, day) == 0 &&
			strcmp(auxcit->dueño, dueño) == 0)
		{

			MessageBox(NULL, "Eliminado", "Depuración", MB_OK);
			// Guarda el nodo anterior y siguiente
			cita* anterior = auxcit->antcit;
			cita* siguiente = auxcit->sigcit;

			// Libera la memoria del nodo actual
			delete auxcit;

			// Actualiza los punteros en los nodos circundantes
			
			if (anterior != nullptr) {
				anterior->sigcit = siguiente;
			}
			else {
				// Si no hay anterior, el nodo eliminado era el primero
				iniciocit = siguiente;
			}
			
			if (siguiente != nullptr) {
				siguiente->antcit = anterior;
			}

			// Sale del bucle después de eliminar el primer nodo con la fecha dada
			return;
		}
		else 
		{
			MessageBox(NULL, "No se encontro fecha", "Depuración", MB_OK);
		}
		
		// Mueve al siguiente nodo
		auxcit = auxcit->sigcit;
	}

	
}

void ModificarCit()
{
	auxcit = iniciocit;

	while (auxcit != nullptr && strcmp(yearcomp, auxcit->year) != 0 && strcmp(monthcomp, auxcit->month) != 0 && strcmp(daycomp, auxcit->day) != 0 && strcmp(dueñocomp, auxcit->dueño) != 0)
	{
		auxcit = auxcit->sigcit;
	}

	if (auxcit == nullptr)
	{
		MessageBox(hwndVetUsu, "Usuario no en contrado.", "AVISO", MB_OK | MB_ICONQUESTION);
	}
	else
	{
		strcpy_s(auxcit->dueño, modificarInfo->dueño);
		strcpy_s(auxcit->mascota, modificarInfo->mascota);
		strcpy_s(auxcit->motivo, modificarInfo->motivo);
		strcpy_s(auxcit->tel, modificarInfo->tel);
		strcpy_s(auxcit->cost, modificarInfo->cost);
		strcpy_s(auxcit->especiepet, modificarInfo->especiepet);//
		strcpy_s(auxcit->year, modificarInfo->year);
		strcpy_s(auxcit->month, modificarInfo->month);
		strcpy_s(auxcit->day, modificarInfo->day);
		strcpy_s(auxcit->hour, modificarInfo->hour);
		strcpy_s(auxcit->minute, modificarInfo->minute);
		auxcit->estatus = modificarInfo->estatus;

	}
}

void obtenerHoraMinuto(const std::string& cadena, int& hour, int& minute)
{
	std::istringstream stream(cadena);
	char discard; // Para descartar caracteres no necesarios

	// Ignorar cualquier cosa antes de " a las "
	stream.ignore(INT_MAX, 'a');
	stream.ignore(INT_MAX, ' ');

	// Leer la hora y los minutos
	stream >> hour >> discard >> minute;

	// Crear un mensaje de depuración
	std::ostringstream debugMessage;
	debugMessage << "Hour: " << hour << "\nMinute: " << minute;

	// Mostrar el cuadro de diálogo
	MessageBox(NULL, debugMessage.str().c_str(), "Valores de Hora y Minuto", MB_OK | MB_ICONINFORMATION);
}

void escribirEnArchivo(const char* nombreArchivo, cita* iniciocit) 
{
	std::ofstream archivo(nombreArchivo);

	if (!archivo.is_open()) {
		
		return;
	}

	// Escribir encabezado de la tabla
	archivo << std::setw(10) << "Estatus" << std::setw(6) << "Year" << std::setw(6) << "Month" << std::setw(6) << "Day"
		<< std::setw(6) << "Hour" << std::setw(8) << "Minute" << std::setw(20) << "Dueño" << std::setw(20) << "Especie/Pet"
		<< std::setw(20) << "Mascota" << std::setw(40) << "Motivo" << std::setw(15) << "Teléfono" << std::setw(10) << "Costo" << std::endl;

	// Iterar sobre la lista y escribir cada cita en una fila de la tabla
	cita* actual = iniciocit;
	while (actual != nullptr) {
		archivo << std::setw(10) << actual->estatus << std::setw(6) << actual->year << std::setw(6) << actual->month
			<< std::setw(6) << actual->day << std::setw(6) << actual->hour << std::setw(8) << actual->minute
			<< std::setw(20) << actual->dueño << std::setw(20) << actual->especiepet << std::setw(20) << actual->mascota
			<< std::setw(40) << actual->motivo << std::setw(15) << actual->tel << std::setw(10) << actual->cost << std::endl;

		actual = actual->sigcit;
	}

	

	archivo.close();
}