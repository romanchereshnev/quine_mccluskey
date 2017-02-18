/*
 * Basic.cpp
 *
 *  Created on: 24.10.2012
 *      Author: roma
 */

#include "Basic.h"
#include <cstdlib>

/*
 * ****************************************************************************************************************************************************
 *                                                                     POINT
 * ****************************************************************************************************************************************************
 */

Point::Point()
{
	impuls = false;
//	impuls.sinch = false;
	amount = 0;
	exits = NULL;
	ent_use = false;
}

Point::~Point()
{
	delete [] exits;
}

bool Point::NewExit(Point * p)
{
	Point **tmp;
	amount++;
	tmp = new Point*[amount];
	if (exits != NULL)
		for (int i = 0; i < amount - 1; i++)
		{
			tmp[i] = exits[i];
			if (exits[i] == p)
			{
				delete [] tmp;
				amount--;
				return false;
			}
		}
	delete [] exits;
	tmp[amount - 1] = p;
	exits = tmp;
	if(p == NULL)
		return true;
	p->enter = this;
	p->ent_use = true;
	return true;
}

bool Point::Value() const
{
	return impuls;
}
/*
bool Point::Sinch() const
{
	return impuls.sinch;
}
*/
bool Point::CutEx(Point * p)
{
	bool res = false;
	Point **tmp;
	tmp = new Point*[amount - 1];
	int i, j;
	for (i = 0, j = 0; i < amount; i++)
	{
		if (exits[i] != p)
		{
			tmp[j] = exits[i];
			j++;
		}
		else
		{
			res = true;
		}
	}
	delete [] exits;
	amount--;
	exits = tmp;
	if (p != NULL)
		p->ent_use = false;
	return res;
}

bool Point::CutEn()
{
	bool res = true;
	if (ent_use && enter != NULL)
		res = enter->CutEx(this);
	return res;
}

void Point::Set(bool imp)
{
	impuls = imp; // Установить значение в точке
//	impuls.sinch = imp->sinch;// Установить синхронизацию в точке
	for (int i = 0; i < amount; i++)
	{
		if(exits[i] != NULL)
			exits[i]->Set(imp);
	}
}

bool Point::Ready() const
{
	return ent_use && (amount > 0);               // подключенных выходов
}

bool Point::SetEnter(Point * p)
{
	if (p == this)
		return false;
	if (p == NULL)
	{
		enter = p;
		ent_use = true;
		return true;
	}
	return p->NewExit(this);
}


bool Point::SetExit(Point * p, int num_ext)
{
	if (amount <= num_ext)
		return false;
	exits[num_ext] = p;
	if (p)
	{
		p->ent_use = true;
		p->enter = this;
	}
	return true;
}

Point & Point::operator=(Point & p)
{
	impuls = p.impuls;
	element = p.element ;
	enter = p.enter;      //Указатель на точку, которыя подает сигнал на эту точку.
	delete [] exits;     //Указатель на массив из указательей, на точки на которую эта точка подает сигнал.

	amount = p.amount;         // Количество выходных из точки.
	ent_use = p.ent_use;     	// Подключен ли вход.
	exits = new Point*[amount];
	for (int i = 0; i < amount; i++)
	{
		exits[i] = p.exits[i];
  	}
	return *this;
}
/*
 * ****************************************************************************************************************************************************
 *                                                                     BASIS
 * ****************************************************************************************************************************************************
 */


Basic::Basic(int en)  	// Конструктор, который устанавливет максимальное значение входов
{
	if (en > Max_Enters)
		en = Max_Enters;
	output.SetEnter(NULL);

	enters = en;   						    // Устанавливет количество входов
    input = new Point[enters];         	// Выделяет память под точки входа.
    for (int i = 0; i < enters; i++)
    {
    	input[i].NewExit(NULL);
    }
    for(int i = 0; i < enters; i++)
    {
    	input[i].SetElement(this);
    }

    output.SetElement(this);
    //ent_sinch = new bool[enters];
}

Basic::~Basic()
{
	//delete [] ent_sinch;
	delete [] input;
}

bool Basic::SetExit(Point * p)
{
	return output.NewExit(p);
}

bool Basic::HasInput() const
{
	for(int i = 0; i < enters; i++)
		if (!input[i].Ready())
			return false;
	return true;
}

bool Basic::SetEnter(Point * p, int num)
{
	if (num == -1)
	{
		for (int i = 0; i < enters; i++)
		{
			if(!input[i].Ready())
			{

				return input[i].SetEnter(p);
			}
		}
		return false;
	}
	else
		return input[num].SetEnter(p);
}

/*
bool Basic::IsSinch(bool snch) const
{
	for (int i = 0; i < enters; i++)
		if (input[i].Sinch() != snch)
			return false;
	return true;
}
*/

bool Basic::Ready() const
{
	return (this->HasInput() && output.Amount() > 0);
}

Basic & Basic::operator=(Basic & b)
{
	enters = b.enters;        	// Количество входов
	delete [] input;

	for (int i = 0; i < enters; i++)
		input[i] = b.input[i];    	// Указатель, на массив точек, на которые подается сигнал
	output = b.output;     			// Точка, на которую подается выходной сигнал
	return * this;
}

Basic::Basic(Basic & b)
{
	enters = b.enters;        	// Количество входов
	delete [] input;

	for (int i = 0; i < enters; i++)
		input[i] = b.input[i];    	// Указатель, на массив точек, на которые подается сигнал
	output = b.output;     	// Точка, на которую подается выходной сигнал
}
/*
 * ****************************************************************************************************************************************************
 *                                                                     AND
 * ****************************************************************************************************************************************************
 */


And::And(int en, Pos * ps) : Basic(en)
{
	det = basic_and;
	if (ps)
	{
		pos.x = ps->x;
		pos.y = ps->y;
	}
	else
	{
		pos.x = -1000;
		pos.y = -1000;
	}
}

And::~And()
{
	//
}

void And::Result()
{
	bool tmp =  true;
	for (int i = 0; i < enters; i++)
		if ( !input[i].Value())
		{
			tmp = false;
			break;
		}
	output.Set(tmp);
}

void And::SetEnt(int max)
{
	if (max > Max_Enters)
		max = Max_Enters;
	else if (max < 0)
			max = 2;
	for (int i = 0; i < enters; i++)
		if (input[i].Ready())
		{
			input[i].CutEn();
		}
	enters = max;                        // Устанавливет количество входов
	delete [] input;
	input = new Point[enters];         // Выделяет память под указатели на входы.
}
And & And::operator=(And & a)
{
	delete [] input;
	enters = a.enters;
	input = new Point[a.enters];
	for (int i = 0; i < enters; i++)
		input[i] = a.input[i];
	output = a.output;
	return *this;
}
/*
 * ****************************************************************************************************************************************************
 *                                                                     OR
 * ****************************************************************************************************************************************************
 */

Or::Or(int en, Pos * ps) : Basic(en)
{
	det = basic_or;
	if (ps)
	{
		pos.x = ps->x;
		pos.y = ps->y;
	}
	else
	{
		pos.x = -1000;
		pos.y = -1000;
	}
}

Or::~Or()
{
	//
}

void Or::Result()
{
	bool tmp = false;
	for (int i = 0; i < enters; i++)
		if (input[i].Value())
		{
			tmp = true;
			break;
		}
	output.Set(tmp);
}

void Or::SetEnt(int max)
{
	if (max > Max_Enters)
		max = Max_Enters;
	else if (max < 0)
			max = 2;
	for (int i = 0; i < enters; i++)
		if (input[i].Ready())
		{
			input[i].CutEn();
		}
	enters = max;                        // Устанавливет количество входов
	delete [] input;
	input = new Point[enters];         // Выделяет память под указатели на входы.
}

Or & Or::operator=(Or & o)
{
	delete [] input;
	enters = o.enters;
	input = new Point[o.enters];
	for (int i = 0; i < enters; i++)
		input[i] = o.input[i];
	output = o.output;
	return *this;
}
/*
 * ****************************************************************************************************************************************************
 *                                                                    NOT
 * ****************************************************************************************************************************************************
 */

Not::Not(Pos * ps) : Basic(1)
{
	det = basic_not;
	if (ps)
	{
		pos.x = ps->x;
		pos.y = ps->y;
	}
	else
	{
		pos.x = -1000;
		pos.y = -1000;
	}
}


Not::~Not()
{
	//
}

void Not::Result()
{
	bool tmp = !input[0].Value();
	output.Set(tmp);
}

Not & Not::operator=(Not & n)
{
	delete [] input;
	enters = n.enters;
	input = new Point[n.enters];
	for (int i = 0; i < enters; i++)
		input[i] = n.input[i];
	output = n.output;
	return *this;
}
