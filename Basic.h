/*
 * Basic.h
 *
 *  Created on: 24.10.2012
 *      Author: roma
 */

#ifndef BASIC_H_
#define BASIC_H_

// TODO Сделать что-нибудь с зациклиностью.

#include <cstdlib>
#include <iostream>

using namespace std;

enum Detals { basic_not, basic_and , basic_or , point, function, delt};

/*
struct Impuls
{
	bool value; // Сингнал, который находится в точке.
	bool sinch; // Синхронизация всех элементов
};
*/

struct Pos
{
	int x;
	int y;
};

/*
 * ****************************************************************************************************************************************************
 *                                                                     POINT
 * ****************************************************************************************************************************************************
 */
class Basic;



class Point
{
private:
	bool impuls;
	Basic * element;
	Point * enter;      //Указатель на точку, которыя подает сигнал на эту точку.
	Point ** exits;     //Указатель на массив из указательей, на точки на которую эта точка подает сигнал.
	int amount;         // Количество выходных из точки.
	bool ent_use;     	// Подключен ли вход.
public:
	Point();
	virtual ~Point();
    bool NewExit(Point * p);  					// Создать связь с точкой p, вернуть true если все удачно, в противном случае false
	bool SetExit(Point * p, int num_ext = 0);  	// Задает связь с точкой, на которую будут подваться сигналы из точки.
	bool SetEnter(Point * p); 					// Создать связь с точкой p, которая будет подавать сигнал в эту точку
	bool Value() const;       					// Возврящает сигнал, который течет в точке
	bool CutEx(Point * p);      				// Разорвать выходную связь с элементом p, вернуть true если все удачно, в противном случае false
	bool CutEn();                               // Разорвать связь, с элементов который подет сигнал в точку.
	void Set(bool imp);         	        // Установить значение в точке
	bool Ready() const;       					// Проверка на то, готов ли элемент к работе. То есть подлючены ли выходи и входы.

	bool HasEnter() const { return ent_use; };
	int Amount() const { return amount; }
	Basic * Element() const { return element;}
	void SetElement( Basic * elem) { element = elem; }
	Point * TakeEnter() { return enter; }
	Point * operator[](int i) { return exits[i]; }
	Point & operator=(Point & p);



	void Show() const
	{
		using namespace std;
		cout << "this: " << this << endl;
		cout << "impuls: " << impuls << endl;
//		cout << "sinch: " << impuls.sinch << endl;
		if (ent_use)
			cout << "enter: " << enter << endl;
		for (int i = 0; i < amount; i ++)
			cout << "exits: " << exits[i] << endl;
		cout << "amount: " << amount << endl;
		cout << "ent_use: " << ent_use << endl;
		cout << "element: " << element << endl;
	}

};




/*
 * ****************************************************************************************************************************************************
 *                                                                     BASIC
 * ****************************************************************************************************************************************************
 */

class Basic
{
protected:
	enum { Max_Enters = 8};
	int enters;        	// Количество входов
	Point * input;    	// Указатель, на массив точек, на которые подается сигнал
	Point output;     	// Точка, на которую подается выходной сигна
	Pos pos;
	Detals det;



    //bool AlEntSinch() const; // Проверка на то, все ли вы

public:
	Basic(int en);    // Конструктор, который устанавливет количество входов
	virtual ~Basic();
	//void SetSinch(bool shc)   { sinch = shc; };             // Устанавливает sinch
	//bool TakeSinch() const { return sinch; }                // Возвращает sinch
	virtual void Result() = 0;                              //

	bool SetExit(Point * p);                                // Соединяет выход элемента с точкой
	bool SetEnter(Point * p, int num = -1);                 // Соединяет точку с одной из точек входа.
	Point * TakeExit() { return &output;};                  // Возваращает адресс выхода элемента
   	bool HasInput() const;                                        // Проверка на то, все ли входы имеют соединение.
   	Point & operator[](int i) { return input[i]; }          // Возвращает один из входных точек.
   	Point operator[](int i) const { return input[i]; }
   	int EnterCount() const { return enters; }
   	void ChangePos(Pos * ps) { pos.x = ps->x; pos.y = ps->y; }
   	Pos * Position() { return &pos; }
   	bool IsSinch(bool snch) const;
   	Detals KindOfElement() const { return det; }

   	bool Ready() const;
   	Basic & operator=(Basic & b);
   	Basic(Basic & b);

   	void Show() const
   	{
   		using namespace std;
   		cout << "Enters = " << enters << endl;
   		for (int i = 0; i < enters; i++)
   		{
   			cout << "INPUT #:" << i << "\n ";
   			input[i].Show();
   		}
   		cout << "OUTPUT :\n";
   		output.Show();
   	}
};

/*
 * ****************************************************************************************************************************************************
 *                                                                     AND
 * ****************************************************************************************************************************************************
 */


class And : public Basic
{
public:
	And(int en = 2, Pos * ps = NULL);           //Конструктор
	virtual ~And();            // Деконструктор
	virtual void Result();             // Помещает результат вычесления логического И от входов в exit
	void SetEnt(int max);      // Обрезает все соединения и узменяет количества входов в функцию
	And & operator=(And & a);
};

/*
 * ****************************************************************************************************************************************************
 *                                                                     OR
 * ****************************************************************************************************************************************************
 */


class Or :  public Basic
{
public:
	Or(int en = 2, Pos * ps = NULL);      //
	virtual ~Or();
	virtual void Result() ;          // Помещает результат вычесления логического ИЛИ от входов в exit
	void SetEnt(int max);     // Обрезает все соединения и узменяет количества входов в функцию
	Or & operator=(Or & o);
};

/*
 * ****************************************************************************************************************************************************
 *                                                                    NOT
 * ****************************************************************************************************************************************************
 */

class Not :  public Basic
{
public:
	Not(Pos * ps = NULL);      //
	virtual ~Not();
	virtual void Result();         // Помещает результат вычесления логического НЕ от входа в exit
	Not & operator=(Not & n);
};



#endif /* BASIC_H_ */
