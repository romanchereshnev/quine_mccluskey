/*
 * Func.h
 *
 *  Created on: 28.10.2012
 *      Author: roma
 */

#ifndef FUNC_H_
#define FUNC_H_

#include "Basic.h"
//#include <iostream>
//#include <cmath>sadsad


struct Equation  	// Структура, которая является уравнение функции
{
	int variables;  // Сколько всего переменных(входов) в функции
	int * num_mm;	// Указатель на массив из номеров минтермов
	int len;        // Длиння массива минтермов.

	int * not_use;	// Указатель на массив из из номеров неопределенных минтермов
	int size;       // Размер массива

	bool form;  	// Показывает то, чему равна функция при таких минтермах
};


void ShowEqtn(Equation * eqtn);

class Polynorm
{
private:

    signed short * polynorm;  	// Этот указаатель на массив содержит форму многочлена.
                                // Числа означают номер входа. Если число отрицательно, то вход будет инвертирован.
    unsigned int elements;      // Количество эл-тов массива, также количество переменных
    bool form;                  // Показывает какой формой, дизъюнктивной(false) или конъюнктивной(true), является данный многочлен.
                                // То есть складываются данные эл-ты или умножаются.
    bool all_not;               // Если true, то весь над всем многочленом производится отрицание, если false, то нет.
    bool use;                   // Если true, то значит был использован в упрощении функции.
    unsigned char norm;         // Сколько положительных эл-тов в многочлене. Используется в алгоритме Маккласки.


public:
    Polynorm();               // Конструктор по умолчанию.
    Polynorm(int num_mma, int ent); // Создается на основе номера минтерма и количества входов
									// Нужен для СДНФ или СКНФ
    virtual ~Polynorm();
    void Change(signed short * m, unsigned int num, bool f = true, bool a_n = false, bool u = false);

    bool WasUse() const { return use; }
    unsigned char Weight() const { return norm; }
    void FindWeight();
    int Elements() const { return elements; }
    signed short & operator[](int i) { return polynorm[i]; }
    signed short operator[](int i) const { return polynorm[i]; }
    Polynorm & operator=(Polynorm & plnm);
    Polynorm( const Polynorm & plnm);
    void Revers();                                      // Eсли элемент был А, то он становится не А и наобарот. Также он меняет конъюнкторы на дизюнкторы и наобарот.
    void Use() { use = true; }                     // Помеяает что многочлен был использован был использован
    void NotUse() { use = false; }
    int Count()  const;							//Возвращает количество не нулевых элементов
    Detals Form() const { return form ? basic_and : basic_or; }


    void Show() const
    {
    	using namespace std;
    	char ch = 'a' - 1;
    	for (unsigned int i = 0; i < elements; i++)
    	{
    		if (polynorm[i] < 0)
    			cout << "!";
    		if (!polynorm[i])
    			;
    		else
    			cout << char (ch + std::abs(polynorm[i]));
    	}
    	cout << " weight ";
    	cout << int (norm) << "\n";
    }
};

class Line
{
private:
	Pos * dots;
	int pos_count;
	bool result;
	Point * first;
	Point * second;
public:
	Line();
	Line(Pos * start, Pos * end,Point * frst, Point * scnd, int hight);
	Line(Pos * start, Pos * end,Point * frst, Point * scnd, int brk, int count);
	~Line();
	void ChangePos(int dot_num, Pos * pos);
	void ChangePos(Pos * pos, bool kind, int hight);  // kind = true - меняем конечную точку, false начальную
	int PosCount() { return pos_count; }
	void DeleteDot(int num_dot);
	Pos * operator[](int i) { return &dots[i]; }
	bool Value() const { return first->Value(); }
	Point * StartPoint() { return first; }
	Point * FinishPoint() { return second; }

	Line & operator=(Line & ln);
	Line(Line & ln);

	void Show () const;


};

class Func
{
private:
	Point * enters;       // Указатель на массив из точек входа в функцию
	int enters_count;       // Количество входов
	Point * exits;   		// Указатель на точки выхода
	int exits_count; 		// Количесвто выходов

	Basic ** elements;   	// Указатель на массив из указателей из базисных элементов в функции
 	int element_count;      // Счетчик базисиных элементов в функции
	Func ** functions;       // Указатель на массив из функций, которые находятся в функции.
	int func_count;         // Счеткик вложенных функций.
	//bool sinch;             // Синхронизация для элементов.

	Line ** lines;
	int lines_count;

	Func * parent;         // Функция, в которой находится данная функция
	Pos pos;



		//void Tree(Point * root, bool snch);
	Polynorm * MacKlascy(Equation * eqtn,int * len);
								// Передается уравнение в структуре eqtn. По нему строится СДНФ или СКНФ, в зависимости от того, что указанно в eqnt
								// Создает массив из логических элементов, приравнивет его к funk и возвращает кол-во членов.
public:
	Func(int ent, int ext, Pos * ps = NULL, Func * prnt = NULL);
	Func(int ent, int ext, Pos * ps, Equation ** eqtns, Func * prnt, int size, int width, int hight);// Получает количество входов и выходов из функции, и массив из указателей на уравнения,
												// По которым будет строится функция по алгоритму Маккласки. По одному на выход.
	virtual ~Func();
	int Create(Detals choice, int ent = 1, Pos * ps = NULL, int end = 1);  // Создает элемент в функции в зависимости от выбора. возвращает номер элемента в массиве
	int CreateMK(int ent, int ext, Equation ** eqtns, Pos * ps,int size, int width, int hight, Func * func);
	bool ChangeExitNum(int i); 		// Изменяет количество выходов из функции
	bool ChangeEnterNum(int i); 	// Изменяет количество входов в функции
	bool DeleteElement(int i);   	// Удаляет элемент под номером i
	bool DeleteFunction(int i);    // Удаляет функцию
	bool DeleteLine(int i);

	bool Result();
	Basic * Element(int i) const { return (i < element_count) ? elements[i] : NULL; } 	// Возвращает адресс на один из элементов базисов в этой функции
	Func * Function(int i) const { return (i < func_count) ? functions[i] : NULL; };    // Возвращает адресс на одну из функции из самой функции.
	Line * Lines(int i)  { return (i < lines_count) ? lines[i] : NULL; };
	void ChangePos(Pos * ps) { pos.x = ps->x; pos.y = ps->y; }
	Pos * Position()  { return &pos; }
	bool Ready() const;           		// Проверка на то, подключенны ли все выводы.

	bool SetSignal(bool * imp);  // Подает сигнал на точки и распостраняет их по всей функии.

	int EnterCount() const { return enters_count; }
	int ExitCount() const { return exits_count; }
	int ElementCount() const { return element_count; }
	int FuncCount() const { return func_count; }
	int LineCount() const { return lines_count; }
	void FindPointPos(Func * func, Basic * el, int size, int width, int hight, bool st_fn, Detals det, Pos * pos, int num); // false - вsход, true вход
	Func * Parent() { return parent; }
	void NewLine(Line * ln);
	Point * TakeEnter(int i) { return &enters[i]; }
	Point * TakeExit(int i) { return &exits[i]; }
	Point * operator[](int i) { return &exits[i]; }

	Func & operator=(Func & fnc);
	Func(Func & fnc);

	void Show() const;

};





#endif /* FUNC_H_ */
