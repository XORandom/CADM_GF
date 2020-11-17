#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <locale.h>
#include <cmath>
#include <string>
using namespace std;

class GaluaField // GF(p^n)
{
private:
	int p = 2;
	int n = 2;
	unsigned char polynomial;
	unsigned char* table;
public:
	void change_n() {
		int ch;
		cout << "Для построения GF(2^n) выберите степень n в диапазоне от 2 до 4: ";
		cin >> ch;
		if (ch >= 2 && ch <= 4)
			n = ch;
		else
			cout << "Введено недопустимое значение. По умолчанию n = 2." << endl;
	};
	void show_poly(unsigned char poly) {
		int flag = 0;
		for (int i = 7; i > 1; i--)
		{
			if ((poly&(1 << i)) == (1 << i))
			{
				if (flag == 0)
					cout << "x^" << i;
				else
					cout << "+x^" << i;
				flag = 1;
			}
		}
		if ((poly & 2) == 2)
		{
			if (flag == 0)
				cout << "x";
			else
				cout << "+x";
			flag = 1;
		}
		if (poly % 2 == 1)
		{
			if (flag == 0)
				cout << "1";
			else
				cout << "+1";
			flag = 1;
		}
		if (flag == 0)
			cout << "0";
	};
	void show_bin(unsigned char poly) {
		for (int i = (n - 1); i >= 0; i--)
		{
			if ((poly&(1 << i)) == (1 << i))
				cout << "1";
			else
				cout << "0";
		}
	};
	void create_polynomial() {
		unsigned char polynomials[5] = { 7, // 0000 0111    для n = 2: x^2+x+1
			11,// 0000 1011    для n = 3: x^3+x+1
			13,// 0000 1101    для n = 3: x^3+x^2+1
			19,// 0001 0011    для n = 4: x^4+x+1
			25,// 0001 1001    для n = 4: x^4+x^3+1
		};
		if (n == 2)
			polynomial = polynomials[0];
		else if (n == 3)
		{
			int ch;
			cout << "Выберите порождающий полином: " << endl << "1: ";
			show_poly(polynomials[1]);
			cout << endl << "2: ";
			show_poly(polynomials[2]);
			cout << endl;
			cin >> ch;
			if (ch == 1)
				polynomial = polynomials[1];
			else if (ch == 2)
				polynomial = polynomials[2];
			else
			{
				cout << "Введено недопустимое значение. По умолчанию будет использован полином 1." << endl;
				polynomial = polynomials[1];
			}
		}
		else
		{
			int ch;
			cout << "Выберите порождающий полином: " << endl << "1: ";
			show_poly(polynomials[3]);
			cout << endl << "2: ";
			show_poly(polynomials[4]);
			cout << endl;
			cin >> ch;
			if (ch == 1)
				polynomial = polynomials[3];
			else if (ch == 2)
				polynomial = polynomials[4];
			else
			{
				cout << "Введено недопустимое значение. По умолчанию будет использован полином 1." << endl;
				polynomial = polynomials[3];
			}
		}
	};
	void create_table() {
		unsigned char temp, sub;
		int t_size = pow(p, n);
		table = new unsigned char[t_size];
		table[0] = 0;
		for (int i = 1; i <= n; i++)
			table[i] = 1 << (i - 1);
		sub = polynomial;
		sub = sub & (~(1 << n));
		table[n + 1] = sub;
		temp = sub;
		for (int i = n + 2; i < pow(p, n); i++)
		{
			temp = temp << 1;
			if ((temp&(1 << n)) == (1 << n))
			{
				temp &= ~(1 << n);
				temp = temp ^ sub;
			}
			table[i] = temp;
		}
	};
	void show_table() {
		cout << "\t\tGF(2^" << n << ")\t\t";
		show_poly(polynomial);
		cout << "=0" << endl << endl << "\tСтепень\t| Элемент\t| Элемент в bin\t| Многочлен" << endl;
		cout << "\t--------------------------------------------------" << endl;
		cout << "\t-inf\t| a^-inf\t| ";
		show_bin(table[0]);
		cout << "\t\t| ";
		show_poly(table[0]);
		for (int i = 0; i < (pow(p, n) - 1); i++)
		{
			cout << endl << "\t" << i << "\t| a^" << i << "\t\t| ";
			show_bin(table[i + 1]);
			cout << "\t\t| ";
			show_poly(table[i + 1]);
		}
		cout << endl;
	};
	void addition()
	{
		unsigned char a = 0, b = 0, res = 0;
		char* str;
		cout << "Вводить числа с учетом впередистоящих нулей (количество знаков дожно быть равно n), знак '+' в пробелы не обрамлять:" << endl;
		str = new char[n * 2 + 1];
		cin >> str;
		for (int i = 0; i < n; i++)
		{
			if (str[i] == '1')
				a += pow(p, (n - 1 - i));
		}
		for (int i = n + 1; i < n * 2 + 1; i++)
		{
			if (str[i] == '1')
				b += pow(p, (n * 2 - i));
		}
		res = a ^ b;
		cout << "Результат: ";
		show_bin(res);
	};
	void multiplication()
	{
		unsigned char a = 0, b = 0, res = 0;
		int a_ind, b_ind, res_ind;
		char* str;
		cout << "Вводить числа с учетом впередистоящих нулей (количество знаков дожно быть равно n), знак '*' в пробелы не обрамлять:" << endl;
		str = new char[n * 2 + 1];
		cin >> str;
		for (int i = 0; i < n; i++)
		{
			if (str[i] == '1')
				a += pow(p, (n - 1 - i));
		}
		for (int i = n + 1; i < n * 2 + 1; i++)
		{
			if (str[i] == '1')
				b += pow(p, (n * 2 - i));
		}
		for (int i = 0; i < pow(p, n); i++)
		{
			if (a == table[i])
				a_ind = i - 1;
			if (b == table[i])
				b_ind = i - 1;
		}
		if (a_ind == -1 || b_ind == -1)
			res_ind = -1;
		else
		{
			int temp = pow(p, n);
			res_ind = (a_ind + b_ind) % (temp - 1);
		}
		res = table[res_ind + 1];
		cout << "Результат: ";
		show_bin(res);
	};
	void loga1()
	{
		cout << "Необходимо ввести элемент в бинарном виде" << endl;
		unsigned char a = 0;
		int a_ind;
		char* str;
		cout << "Вводить число с учетом впередистоящих нулей (количество знаков дожно быть равно " << n << "):" << endl;
		str = new char[n];
		cin >> str;
		for (int i = 0; i < n; i++) {
			if (str[i] == '1') {
				a += pow(p, (n - 1 - i));
			}

		}

		for (int i = 0; i < pow(p, n); i++) {
			if (a == table[i])
				a_ind = i - 1;
		}
		if (a_ind == -1) {
			cout << "Результат: -inf";//обозначение первогот элемента, нулевой 
		}
		else {
			cout << "Результат: " << a_ind;
		}

	}
	void loga2()
	{
		cout << "Необходимо ввести степень элемента в поле" << endl;
		unsigned char a = 0, res = 0;
		int a_ind, res_ind;
		cout << "Вводить степень в целочисленном промежутке [-1;" << pow(2, n) - 2 << "] (-1 обозначает -inf степень)" << endl;
		int stepen;
		cin >> stepen;
		if (stepen >= -1 && stepen <= pow(2, n) - 2) {
			if (stepen == -1) {
				cout << "Результат: ";
				show_bin(table[0]);
			}
			else {
				cout << "Результат: "; show_bin(table[stepen + 1]);
			}
		}
		else {
			cout << "Неверный ввод данных";
		}
	};

};


int main()
{
	setlocale(LC_ALL, "RUS");
	int ch;
	GaluaField GF2;
	do {
		GF2.change_n();
		GF2.create_polynomial();
		GF2.create_table();
		GF2.show_table();
		while (1)
		{
			cout << endl << "1. Сложение двух элементов GF\n2. Умножение двух элементов GF\n3. Логарифм GF\n4. Обратный логарифм GF\n5. Построить новое поле GF\n6. Выход :)" << endl;
			cin >> ch;
			if (ch == 1)
				GF2.addition();
			else if (ch == 2)
				GF2.multiplication();
			else if (ch == 3)
				GF2.loga1();
			else if (ch == 4)
				GF2.loga2();
			/*else if (ch == 5)
				GF2.loga21();*/
			else
				break;
		}
	} while (ch == 5);
	return 0;
}
