#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "HD44780.h"

#ifndef _BV
#define _BV(bit)				(1<<(bit))
#endif

#ifndef sbi
#define sbi(reg,bit)		reg |= (_BV(bit))
#endif

#ifndef cbi
#define cbi(reg,bit)		reg &= ~(_BV(bit))
#endif

int8_t pressed_button () {
		int column_index = 7; // Numer portu kolumny
		int row_index = 0; // Numer portu wiersza
		int button_idex = 1; // Indeks przycisku klawiatury
		while (column_index > 3) {
			cbi(PORTD, column_index); // Ustawiam port kolumny na stan niski
			while (row_index < 4) {
				if (bit_is_clear(PIND, row_index)) {
					return button_idex;
				} else {
					button_idex++;
				}
				row_index++;
			}
			row_index = 0;
			sbi(PORTD, column_index); // Ustawiam port kolumny na stan wysoki
			column_index--;
		}
		if (button_idex == 17) { // Jesli zaden przycisk nie jest wcisniety
			return -1;
		}
}

int main() {
	LCD_Initalize(); // Uruchamia wyswietlacz, konfiguruje porty, wysyla konfiguracje
	LCD_Home(); //  Ustawia poczatkowe wspolrzedne wyswietlacza
	char text[20]; // Max 2 wiersze po 16 znakow

	/*
	// Zadanie 1
	uint16_t s = 0; // Sekundy
	uint16_t m = 0;	// Minuty
	uint16_t h = 12; // Godziny

	while (1) {
		LCD_Clear(); // Wyczyszczenie wyswietlacza
		LCD_GoTo(0, 0); // Przejscie do pierwszego wiersza wyswietlacza
		if (h == 24)	// Instrukcja, gdy godziny przekrocza 24 h
		{
			h = 0;
			m = 0;
			s = 0;
		}
		if (s == 60)	//Instrukcja, gdy sekundy przekrocza 60 s
		{
			m++;
			s = 0;
		}
		if (m == 60)	//Instrukcja gdy minuty przekrocza 60 min
		{
			h++;
			m = 0;
			s = 0;
		}
		if (s < 10 && m < 10 && h < 10) {
			sprintf(text, "0%d:0%d:0%d", h, m, s++);
		} else if (s < 10 && m < 10) {
			sprintf(text, "%d:0%d:0%d", h, m, s++);
		} else if (s < 10 && h < 10) {
			sprintf(text, "0%d:%d:0%d", h, m, s++);
		} else if (m < 10 && h < 10) {
			sprintf(text, "0%d:0%d:%d", h, m, s++);
		} else if (s < 10) {
			sprintf(text, "%d:%d:0%d", h, m, s++);
		} else if (m < 10) {
			sprintf(text, "%d:0%d:%d", h, m, s++);
		} else if (h < 10) {
			sprintf(text, "0%d:%d:%d", h, m, s++);
		}
		LCD_WriteText(text); // Wyswietlanie czasu
		LCD_GoTo(0, 1); // Przejscie do drugiego wiersza wyswietlacza
		sprintf(text, "252891, 252864");
		LCD_WriteText(text); // Wyswietlanie indeksow
		_delay_ms(1000);
	}
	*/

	/*
	// Zadanie 2
	// Ustawiam porty wierszy jako wejœcia
	cbi(DDRD, PD0);
	cbi(DDRD, PD1);
	cbi(DDRD, PD2);
	cbi(DDRD, PD3);

	// Ustawiam porty wierszy na stan wysoki
	sbi(PORTD, PD0);
	sbi(PORTD, PD1);
	sbi(PORTD, PD2);
	sbi(PORTD, PD3);

	// Ustawiam porty kolumn jako wyjœcia
	sbi(DDRD, PD4);
	sbi(DDRD, PD5);
	sbi(DDRD, PD6);
	sbi(DDRD, PD7);

	// Ustawiam porty kolumn na stan wysoki
	sbi(PORTD, PD4);
	sbi(PORTD, PD5);
	sbi(PORTD, PD6);
	sbi(PORTD, PD7);

	while (1) {
			LCD_GoTo(0, 1); // Przejscie do drugiego wiersza wyswietlacza
			sprintf(text, "Wcisnieto: %d", pressed_button());
			LCD_WriteText(text); // Wyswietlenie wcisnietego przycisku
			_delay_ms(500);
			while (bit_is_clear(PIND, PD0) || bit_is_clear(PIND, PD1) || bit_is_clear(PIND, PD2) || bit_is_clear(PIND, PD3)) { // Czekanie az nie pusci sie przycisku
				_delay_ms(500);
			}
			LCD_Clear(); // Wyczyszczenie wyswietlacza
	}
	*/


	// Zadanie 3
	// Ustawiam porty wierszy jako wejœcia
	cbi(DDRD, PD0);
	cbi(DDRD, PD1);
	cbi(DDRD, PD2);
	cbi(DDRD, PD3);

	// Ustawiam porty wierszy na stan wysoki
	sbi(PORTD, PD0);
	sbi(PORTD, PD1);
	sbi(PORTD, PD2);
	sbi(PORTD, PD3);

	// Ustawiam porty kolumn jako wyjœcia
	sbi(DDRD, PD4);
	sbi(DDRD, PD5);
	sbi(DDRD, PD6);
	sbi(DDRD, PD7);

	// Ustawiam porty kolumn na stan wysoki
	sbi(PORTD, PD4);
	sbi(PORTD, PD5);
	sbi(PORTD, PD6);
	sbi(PORTD, PD7);

	int button; // Przechowuje numer wcisnietego przycisku
	int result; // Przechowuje wynik obliczen
	int operation = 0; // Jesli odejmowanie to = 1, jesli dodawanie to = 2
	int index1 = 0, index2 = 0; // Sluza do indeksowania tablic do przechowywania liczb
	int number_flag = 1; // Jesli wczytujemy 1. liczbe to = 1, jesli 2. to = 2
	int result_flag = 0; // Jesli uzyto znaku '=' 1 raz to = 1, jesli 2 razy to = 2
	char tab1[6] = {'x', 'x', 'x', 'x', 'x', '\0'}, tab2[6] = {'x', 'x', 'x', 'x', 'x', '\0'}; // Tablice do przechowywania liczb

	while (1) {
		button = pressed_button();
		switch (button)
		{
		case -1: // Przypadek, gdy zaden przycisk nie jest wcisniety
			break;
		case 1:
			if (number_flag == 1) { // Wczytanie cyfry do tablicy 1. liczby
				tab1[index1] = '7';
				index1++;
			} else {				// Wczytanie cyfry do tablicy 2. liczby
				tab2[index2] = '7';
				index2++;
			}
			break;
		case 2:
			if (number_flag == 1) {
				tab1[index1] = '4';
				index1++;
			} else {
				tab2[index2] = '4';
				index2++;
			}
			break;
		case 3:
			if (number_flag == 1) {
				tab1[index1] = '1';
				index1++;
			} else {
				tab2[index2] = '1';
				index2++;
			}
			break;
		case 5:
			if (number_flag == 1) {
				tab1[index1] = '8';
				index1++;
			} else {
				tab2[index2] = '8';
				index2++;
			}
			break;
		case 6:
			if (number_flag == 1) {
				tab1[index1] = '5';
				index1++;
			} else {
				tab2[index2] = '5';
				index2++;
			}
			break;
		case 7:
			if (number_flag == 1) {
				tab1[index1] = '2';
				index1++;
			} else {
				tab2[index2] = '2';
				index2++;
			}
			break;
		case 8:
			if (number_flag == 1) {
				tab1[index1] = '0';
				index1++;
			} else {
				tab2[index2] = '0';
				index2++;
			}
			break;
		case 9:
			if (number_flag == 1) {
				tab1[index1] = '9';
				index1++;
			} else {
				tab2[index2] = '9';
				index2++;
			}
			break;
		case 10:
			if (number_flag == 1) {
				tab1[index1] = '6';
				index1++;
			} else {
				tab2[index2] = '6';
				index2++;
			}
			break;
		case 11:
			if (number_flag == 1) {
				tab1[index1] = '3';
				index1++;
			} else {
				tab2[index2] = '3';
				index2++;
			}
			break;
		case 13:
			number_flag = 2; // Zmiana wczytywanej liczby na 2.
			operation = 1; // Oznacza odejmowanie
			break;
		case 14:
			number_flag = 2; // Zmiana wczytywanej liczby na 2.
			operation = 2; // Oznacza dodawanie
			break;
		case 16:
			result_flag++; // Zwieksza flage, ktora informuje ile razy uzyto znaku '='
			if (result_flag == 1) { // Jesli uzyto znaku '=' raz
				if (operation == 1) { // Jesli odejmowanie
					result = atoi(tab1) - atoi(tab2);
				} else if (operation == 2) { // Jesli dodawanie
					result = atoi(tab1) + atoi(tab2);
				}
			} else if (result_flag == 2) { // Jesli uzyto znaku '=' 2 razy
				for (int i = 0; i < 5; i++) { // Czyszczenie tablic do przechowywania liczb
					tab1[i] = 'x';
					tab2[i] = 'x';
				}
				number_flag = 1; // Zmiana wczytywanej liczby na 1.
				result_flag = 0; // Wyzerowanie uzyc znaku '='
				operation = 0; // Zresetowanie operacji matematycznej
				index1 = 0; // Wyzerowanie indeksow tablic do przechowywania liczb
				index2 = 0;
			}
			break;
		}
		LCD_GoTo(0, 0); // Przejscie do pierwszego wiersza wyswietlacza
		if (operation == 0) { // Jesli nie wcisnieto znaku '-' ani '+'
			if (tab1[0] == 'x') { // Jesli nie ma liczby w tablicy
				sprintf(text, " ");
			} else {
				sprintf(text, "%d", atoi(tab1));
			}
		} else if (operation == 1 && result_flag == 0) { // Jesli odejmowanie i nie wcisnieto znaku '='
			if (tab2[0] == 'x') { // Jesli nie wpisano jeszcze 2. liczby
				sprintf(text, "%d-", atoi(tab1));
			} else {
				sprintf(text, "%d-%d", atoi(tab1), atoi(tab2));
			}
		} else if (operation == 2 && result_flag == 0) { // Jesli dodawanie i nie wcisnieto znaku '='
			if (tab2[0] == 'x') { // Jesli nie wpisano jeszcze 2. liczby
				sprintf(text, "%d+", atoi(tab1));
			} else {
				sprintf(text, "%d+%d", atoi(tab1), atoi(tab2));
			}
		} else if (operation == 1 && result_flag == 1) { // Jesli odejmowanie i wcisnieto znak '='
			sprintf(text, "%d-%d=%d", atoi(tab1), atoi(tab2), result);
		} else if (operation == 2 && result_flag == 1) { // Jesli dodawanie i wcisnieto znak '='
			sprintf(text, "%d+%d=%d", atoi(tab1), atoi(tab2), result);
		}
		LCD_WriteText(text); // Wyswietlenie wcisnietego przycisku
		_delay_ms(100);
		while (bit_is_clear(PIND, PD0) || bit_is_clear(PIND, PD1) || bit_is_clear(PIND, PD2) || bit_is_clear(PIND, PD3)) { // Czekanie az nie pusci sie przycisku
			_delay_ms(100);
		}
		LCD_Clear(); // Wyczyszczenie wyswietlacza
	}

}
