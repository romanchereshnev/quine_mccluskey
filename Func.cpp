/*
 * Func.cpp
 *
 *  Created on: 28.10.2012
 *      Author: roma
 */

#include "Func.h"
#include "Basic.h"
#include <cmath>
#include <cstdlib>
#include <new>

#include <iostream>

using namespace std;

void ShowArr(int * arr, int len)
{
	for (int i = 0; i < len; i++)
		cout << arr[i] << " ";
	cout << endl;
}

void ShowArr(bool * arr, int len)
{
	for (int i = 0; i < len; i++)
		cout << arr[i] << " ";
	cout << endl;
}


int variables;  // Сколько всего переменных(входов) в функции
int * num_mm;	// Указатель на массив из номеров минтермов
int len;        // Длиння массива минтермов.
bool form;  	// Показывает то, чему равна функция при таких минтермах
int * not_use;	// Указатель на массив из из номеров неопределенных минтермов
int size;       // Размер массива

void ShowEqtn(Equation * eqtn)
{
	cout << "Адресс функции: " << eqtn << endl;
	cout << "variables = " << eqtn->variables << endl;
	cout << "num_mm = " << eqtn->num_mm << endl;
	ShowArr(eqtn->num_mm, eqtn->len);
	cout << "len = " << eqtn->len << endl;
	cout << "form = " << eqtn->form << endl;
	cout << "not_use = " << eqtn->not_use << endl;
	ShowArr(eqtn->not_use, eqtn->size);
	cout << "size = " << eqtn->size << endl << endl;
}

/*
 * ****************************************************************************************************************************************************
 *                                                                    Helpers
 * ****************************************************************************************************************************************************
 */




static int compare(const void * ptr1, const void * ptr2)
{
	int answ;
	if ( ((Polynorm *)ptr1)->Weight() < ((Polynorm *)ptr2)->Weight() )
	{
		answ = -1;
	}
	else if ( ((Polynorm *)ptr1)->Weight() > ((Polynorm *)ptr2)->Weight() )
		answ = 1;
	else
		answ = 0;
	return answ;
}

static bool FullCompare(Polynorm * arr, int len, Polynorm * slag)
{
	int i, j;
	bool was;
	for (i = 0; i < len; i++)
	{
		was = true;
		for (j = 0; j < slag->Elements(); j++)
		{


			if((*slag)[j] != arr[i][j])
			{
				was = false;
				break;
			}
		}
		if (was)
			return false;
	}
	return true;

}

static bool Cut(Polynorm * slag1, Polynorm * slag2, Polynorm * tmp)
{
	bool res = false;
	for (int i = 0; i < slag1->Elements(); i++)
	{


		if (((*slag1)[i] + (*slag2)[i]) == 0 && (*slag1)[i] && (*slag2)[i])                       // Если они различны.
		{
			if (res)                                            // Если мы нашли второй различный элемент
			{
				res = false;									// Результатом будет false
				break;											// Преравать цикл
			}
			*tmp = *slag1;
			(*tmp)[i] = 0;
			tmp->FindWeight();
			res = true;                                         // Первые различные
		}
		else if ((!(*slag1)[i] && (*slag2)[i]) || ((*slag1)[i] && !(*slag2)[i])) // Если отличаются по ранее сокращенным
		{
			res = false;										// Результатом будет false
			break;												// Преравать цикл
		}
	}
	if (res)
	{
		tmp->NotUse();
		slag1->Use();
		slag2->Use();
	}

	return res;
}

static bool ImplictProbe(Polynorm * orig, Polynorm * cuted)
{
	for (int i = 0; i < orig->Elements(); i++)
	{
		if((*cuted)[i] && ((*orig)[i] != (*cuted)[i]))
			return false;
	}
	return true;
}

static int * ImlictMatrix(bool ** tbl, int col, int row, int * gr, int * vert)
{
	int i, j;
	delete [] vert;
	int * vrt = new int[col];
	for (i = 0; i < col; i++)
		vrt[i] = 0;
	for(i = 0; i < col; i++)
	{
		if (gr[i] == 0)
			continue;
		for(j = 0; j < row; j++)
		{
			if (tbl[i][j])
			{
				vrt[j]++;
			}
		}
	}
	return vrt;
}

static bool ArrNotNull(int * arr, int len)
{
	for (int i = 0; i < len; i++)
		if (arr[i] != 0)
			return true;
	return false;
}

/*
 * ****************************************************************************************************************************************************
 *                                                                     Line
 * ****************************************************************************************************************************************************
 */

Line::Line()
{
	dots = NULL;
	pos_count = 0;
	first = NULL;
	second = NULL;
}


Line::Line(Pos * start, Pos * end,Point * frst, Point * scnd, int hight)
{
	pos_count = 4;
	dots = new Pos[4];
	result = false;
	first = frst;
	second = scnd;


	dots[0].x = start->x;
	dots[0].y = start->y;

	dots[3].x = end->x;
	dots[3].y = end->y;

	float percent = float((dots[3].y - dots[0].y) / float(0.8 * hight));
	if (percent < 0)
			percent = -percent;
	if (percent > 1)
			percent = 1;
	dots[1].x = dots[0].x + (dots[3].x - dots[0].x) * percent;
	dots[1].y = dots[0].y;

	dots[2].x = dots[0].x + (dots[3].x - dots[0].x) * percent;;
	dots[2].y = dots[3].y;

}

Line::Line(Pos * start, Pos * end,Point * frst, Point * scnd, int brk, int count)
{
	pos_count = 4;
	dots = new Pos[4];
	result = false;
	first = frst;
	second = scnd;

	dots[0].x = start->x;
	dots[0].y = start->y;

	dots[3].x = end->x;
	dots[3].y = end->y;

	int piece = (dots[3].x - dots[0].x) / (count + 2);
	dots[1].x = dots[3].x - brk * piece;
	dots[1].y = dots[0].y;

	dots[2].x = dots[3].x - brk * piece;
	dots[2].y = dots[3].y;

}

Line::~Line()
{
	delete [] dots;
}

void Line::ChangePos(int dot_num, Pos * pos)
{
	if(dot_num > 0 && dot_num < pos_count - 1)
	{
		dots[dot_num].x = pos->x;
		dots[dot_num].y = pos->y;
	}
}

Line & Line::operator=(Line & ln)
{
	delete [] dots;
	pos_count = ln.pos_count;
	dots = new Pos[pos_count];
	first = ln.first;
	second = ln.second;
	for (int i = 0; i < pos_count; i++)
		dots[i] = ln.dots[i];

	return *this;
}

Line::Line(Line & ln)
{
	delete [] dots;
	pos_count = ln.pos_count;
	dots = new Pos[pos_count];
	first = ln.first;
	second = ln.second;
	for (int i = 0; i < pos_count; i++)
		dots[i] = ln.dots[i];

}

void Line::DeleteDot(int num_dot)
{
    Pos * tmp = new Pos[pos_count - 1];
    int i, j;
    for(i = 0, j = 0; i < pos_count; i++)
    {
          if(j != num_dot)
          {
                  tmp[i] = dots[j];
                  j++;
           }
     }
     delete [] dots;
     dots = tmp;   
}
void Line::Show () const
{
	cout << "First point = " << first << endl;
	cout << "Second point = " << second << endl;
	cout << "value = " << result << endl;
	cout << endl;
}

void Line::ChangePos(Pos * pos, bool kind, int hight)
{
	if (kind)
	{
		dots[3].x = pos->x;
		dots[3].y = pos->y;
	}
	else
	{

		dots[0].x = pos->x;
		dots[0].y = pos->y;
	}

	float percent = float((dots[3].y - dots[0].y) / float(0.8 * hight));
	if (percent < 0)
			percent = -percent;
	if (percent > 1)
			percent = 1;
	dots[1].x = dots[0].x + (dots[3].x - dots[0].x) * percent;
	dots[1].y = dots[0].y;

	dots[2].x = dots[0].x + (dots[3].x - dots[0].x) * percent;;
	dots[2].y = dots[3].y;
}


/*
 * ****************************************************************************************************************************************************
 *                                                                    Func
 * ****************************************************************************************************************************************************
 */


Func::Func(int ent, int ext, Pos * ps, Func * prnt)
{
	parent = prnt;          // Родитель
	exits_count = ext; 		// Количесвто выходов
	enters_count = ent;       // Количество входов
	enters = new Point[ent];       // Указатель на массив из точек входа в функцию
	for (int i = 0; i < enters_count; i++)
		enters[i].SetElement(NULL);
	exits = new Point[ext];   		// Указатель на точки выхода
	for (int i = 0; i < exits_count; i++)
		exits[i].SetElement(NULL);

	elements = NULL;   		// Указатель на массив из базисных элементов в функции
 	element_count = 0;      // Счетчик базисиных элементов в функции
	functions = NULL;       // Указатель на массив из функций, которые находятся в функции.
	func_count = 0;         // Счеткик вложенных функций.
	lines = NULL;
	lines_count = 0;
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


Func::Func(int ent, int ext, Pos * ps, Equation ** eqtns, Func * prnt, int size, int width, int hight)
{
	element_count = 0;
	elements = NULL;
	exits_count = ext; 		// Количесвто выходов
	enters_count = ent;       // Количество входов

	enters = new Point[ent];       // Указатель на массив из точек входа в функцию
	exits = new Point[ext];   		// Указатель на точки выхода
	functions = NULL;       // Указатель на массив из функций, которые находятся в функции.
	func_count = 0;         // Счеткик вложенных функций.
	int i, j, count;
	parent = prnt;

	pos = *ps;

	lines = NULL;
	lines_count = 0;
	Line * ln;
	Pos tmp_pos;
	Pos tmp_pos2;
	int piece = hight / (enters_count + 1);

	for(i = 0; i < enters_count; i++)
	{
		tmp_pos.x = width / 4;
		tmp_pos.y = piece * (i + 1);
		Create(basic_not, 1, &tmp_pos);
		elements[i]->SetEnter(&enters[i]);

		FindPointPos(NULL,  NULL, size, width, hight, true, point, &tmp_pos, i);
		FindPointPos(NULL,  elements[i], size, width, hight, true, basic_not, &tmp_pos2, i);
		ln = new Line(&tmp_pos, &tmp_pos2, &enters[i], &(*elements[i])[0], hight);
		NewLine(ln);
	}


	int len, num, tmp;


	int sz, brk, counter, enter;

	counter = 0;
	//cout << "here?\n";
	for (i = 0; i < ext; i++)
	{
		Polynorm * min_form;
		min_form = MacKlascy(eqtns[i], &len);

		piece = hight / (exits_count + 1);
		tmp_pos.x = width * 0.75;
		tmp_pos.y = piece * (i + 1);

		//cout << "Create mention x = " << tmp_pos.x << " y = " << tmp_pos.y << endl;
		if (eqtns[i]->form)
		{
			tmp = Create(basic_or, len, &tmp_pos);
		}
		else
		{
			tmp = Create(basic_and, len, &tmp_pos);
		}

		tmp_pos.x = width * 0.5;
		piece = size * 1.5;

		sz = len / 2;
		for (j = 0; j < len; j++, counter++)
		{
			tmp_pos.x = width * 0.5;
			tmp_pos.y = size * 4 * counter + size * 1.5;
			num = Create(min_form[j].Form(), min_form[j].Count(), &tmp_pos);
			for (count = 0, enter = 0; count < min_form[i].Elements(); count++)
			{
				if (min_form[j][count] > 0)
				{
					elements[num]->SetEnter(&enters[min_form[j][count] - 1]);    // Задаем вход созданного элемента на точку входа, на которую указывает
														// выражение
					FindPointPos(NULL,  elements[num], size, width, hight, true, basic_and, &tmp_pos, enter);
					FindPointPos(NULL,  elements[num], size, width, hight, true, point, &tmp_pos2, min_form[j][count] - 1);

					ln = new Line(&tmp_pos2, &tmp_pos, &enters[min_form[j][count] - 1], &(*elements[num])[enter], hight);

					NewLine(ln);
					enter++;
				}
				else if((min_form[j][count] < 0))
				{
					elements[num]->SetEnter( elements[-min_form[j][count] - 1]->TakeExit());    // Задаем вход созданного элемента на элемент НЕ входа

					FindPointPos(NULL,  elements[num], size, width, hight, true, basic_and, &tmp_pos, enter);
					FindPointPos(NULL,  elements[-min_form[j][count] - 1], size, width, hight, false, basic_not, &tmp_pos2, min_form[j][count] - 1);

					ln = new Line(&tmp_pos2, &tmp_pos, elements[-min_form[j][count] - 1]->TakeExit(), &(*elements[num])[enter],hight);

					NewLine(ln);
					enter++;
				}

			}

			FindPointPos(NULL,  elements[num], size, width, hight, false, basic_and, &tmp_pos, i);
			FindPointPos(NULL,  elements[tmp], size, width, hight, true, basic_and, &tmp_pos2, j);


			brk = std::abs(sz - (j + 1));
			ln = new Line(&tmp_pos, &tmp_pos2, elements[num]->TakeExit(), &(*elements[tmp])[j],brk, sz);
			NewLine(ln);
			elements[tmp]->SetEnter(elements[num]->TakeExit());
			//cout << "First point x = " << tmp_pos.x << " y = " << tmp_pos.y << endl;
			//cout << "Second x = " << tmp_pos2.x << " y = " << tmp_pos2.y << endl << endl;;

		}
		exits[i].SetElement(NULL);
		exits[i].SetEnter(elements[tmp]->TakeExit());
		FindPointPos(NULL,  elements[tmp], size, width, hight, false, basic_and, &tmp_pos, count);
		FindPointPos(NULL,  NULL, size, width, hight, false, point, &tmp_pos2, i);

		ln = new Line(&tmp_pos, &tmp_pos2, elements[tmp]->TakeExit(), &exits[i], hight);
		NewLine(ln);

		delete [] min_form;


	}
	//Show();
	for (i = 0, j = 0; i < eqtns[0]->variables; i++, j++)
		if (!elements[j]->Ready())
		{
			DeleteElement(j);
			j--;
		}
}

Func::~Func()
{
	int i;

	for (i = 0; i < element_count; i++ )
	{
		DeleteElement(i);
	}


	for (i = 0; i < func_count; i++ )
		DeleteFunction(i);

	for (i = 0; i < lines_count; i++ )
		DeleteLine(i);

	delete [] elements;
	delete [] functions;
	delete [] lines;
	delete [] enters;

	delete [] exits;

}

void Func::NewLine(Line * ln)
{
	int i;
	Line ** tmp = new Line*[++lines_count];
	for(i = 0; i < lines_count - 1; i++)
		tmp[i] = lines[i];

	delete [] lines;
	lines = tmp;
	lines[lines_count - 1] = ln;
}

int Func::CreateMK(int ent, int ext, Equation ** eqtns, Pos * ps,int size, int width, int hight, Func * func)
{
	Func ** ftmp;
	int num;
	ftmp = functions;
	func_count++;
	functions = new Func*[func_count];
	for (int i = 0; i < func_count - 1; i++)
	{
		functions[i] = ftmp[i];
	}
	Func * fnc;
	//cout << this << endl;
	//cout << func << endl;
	//for (int i = 0; i < ext; i++)
	//{
		//cout << "Адресс масива " << eqtns << endl;
		//ShowEqtn(eqtns[i]);
	//}

	fnc = new Func(ent, ext, ps, eqtns, func, size, width, hight);
	functions[func_count - 1] = fnc;
	num = func_count - 1;

	return num;
}

int Func::Create(Detals choice, int ent, Pos * pos, int end)
{
	Basic ** tmp;
	Func ** ftmp;
	int num;
	switch(choice)
	{
	case basic_not:
		tmp = elements;
		element_count++;
		elements = new Basic*[element_count];
		for (int i = 0; i < element_count - 1; i++)
		{
			elements[i] = tmp[i];
		}
		delete [] tmp;

		Not * new_not;
		new_not = new Not(pos);
		elements[element_count - 1] = new_not;
		num = element_count - 1;
		break;
	case basic_or:
		tmp = elements;
		element_count++;
		elements = new Basic*[element_count];
		for (int i = 0; i < element_count - 1; i++)
		{
			elements[i] = tmp[i];
		}
		delete [] tmp;
		Or * new_or;
		new_or = new Or(ent, pos);
		elements[element_count - 1] = new_or;
		num = element_count - 1;
		break;
	case basic_and:
		tmp = elements;
		element_count++;
		elements = new Basic*[element_count];
		for (int i = 0; i < element_count - 1; i++)
		{
			elements[i] = tmp[i];
		}
		And * new_and;
		new_and = new And(ent, pos);
		elements[element_count - 1] = new_and;
		num = element_count - 1 ;
		delete [] tmp;
		break;
	case function:
		ftmp = functions;
		func_count++;
		functions = new Func*[func_count];
		for (int i = 0; i < func_count - 1; i++)
		{
			functions[i] = ftmp[i];
		}
		Func * fnc;
		fnc = new Func(ent, end, pos, this);
		functions[func_count - 1] = fnc;
		num = func_count - 1;
	default:
		return -1;
	}
	return num;
}


bool Func::ChangeEnterNum(int ent) 		// Изменяет количество выходов из функции
{
	for (int i = 0; i < enters_count; i++)
	{
		if (!enters[i].CutEn())
			return false;
		for (int j = 0; j < enters[i].Amount(); j++)
		{
			if (!(enters[i].CutEx(enters[i][j])))
				return false;
		}
	}
	delete [] enters;
	enters_count = ent;
	enters = new Point[enters_count];
	return true;
}


bool Func::ChangeExitNum(int ext) 	// Изменяет количество входов в функции
{
	for (int i = 0; i < exits_count; i++)
	{
		if (!exits[i].CutEn())
			return false;
		for (int j = 0; j < exits[i].Amount(); j++)
		{
			if (!exits[i].CutEx(exits[i][j]))
				return false;
		}
	}

	delete []  exits;

	exits_count = ext;
	exits = new Point[exits_count];

	return true;
}

bool Func::DeleteElement(int i) 	// Удаляет элемент под номером i  TODO
{
	if (i > element_count)
		return false;
	if (elements[i] == NULL)
		return true;
     int j;
	for (j = 0; j < elements[i]->EnterCount(); j++)
	{

		if ((*elements[i])[j].Ready() )
		{
			(*elements[i])[j].CutEn();
		}
	}
	for (j = 0; j < elements[i]->TakeExit()->Amount(); j++)
	{
		if ( !elements[i]->TakeExit()->CutEx((*elements[i]->TakeExit())[j]))
		{
			return false;
		}
	}


    Basic ** tmp = new Basic*[--element_count];
    int k;
    for (j = 0, k = 0; j < element_count + 1; j++)
    {

    	if(j != i)
        {
    		tmp[k] = elements[j];
            k++;
        }
    }
    elements[i]->~Basic();
      
    delete [] elements;
    elements  = tmp;

	return true;
}

bool Func::DeleteFunction(int i)
{
	if (i > func_count)
		return false;

	int j, count, k;

	Func ** tmp = new Func*[--func_count];

	for (j = 0, k = 0; j < func_count + 1; j++)
	{

		if(j != i)
		{

			tmp[k] = functions[j];
		    k++;
		}
	}

	for (j = 0; j < functions[i]->EnterCount(); j++)
	{
		if (functions[i]->TakeEnter(j)->HasEnter())
			{
				functions[i]->TakeEnter(j)->CutEn();
			}
	}

	for (j = 0; j < functions[i]->ExitCount(); j++)
	{
		for (k = 0; k < functions[i]->TakeExit(j)->Amount(); k++)
		{
			if ( !functions[i]->TakeExit(j)->CutEx( (*functions[i]->TakeExit(j))[k] ))
			{
				return false;
			}
		}
	}

	functions[i]->~Func();
	delete [] functions;
	functions = tmp;


	return true;
}

void Func::FindPointPos(Func * func, Basic * el, int size, int width, int hight, bool st_fn, Detals det, Pos * pos, int num)
{
	int i, piece;

	switch (det)
	{
	case basic_or:
	case basic_and:
		if (st_fn)
		{
			piece = (size * 3) / (el->EnterCount() + 1);
			pos->y = el->Position()->y - 1.5 * size + piece * (num + 1);
			pos->x = el->Position()->x - 1.5 * size;
		}
		else
		{
			pos->y = el->Position()->y ;
			pos->x = el->Position()->x + 1.5 * size;
		}
		break;
	case basic_not:
		pos->y = el->Position()->y;
		pos->x = st_fn ? pos->x = el->Position()->x - 1.5 * size : pos->x = el->Position()->x + 1.5 * size;
		break;
	case function:
		if (st_fn)
		{
			piece = (size * 6) / (func->EnterCount() + 1);
			pos->y = func->Position()->y + piece * (num + 1);
			pos->x = func->Position()->x - 2.5 * size;
		}
		else
		{
			piece = (size * 6) / (func->ExitCount() + 1);
			pos->y = func->Position()->y + piece * (num + 1);
			pos->x = func->Position()->x + 2.5 * size;
		}

		break;
	case point:
		if (st_fn)
		{
			piece = (hight) / (enters_count + 1);
			pos->y = piece * (num + 1);
			pos->x = 0.5 * size;
		}
		else
		{
			piece = (hight) / (exits_count + 1);
			pos->y = piece * (num + 1);
			pos->x =  width - 0.5 * size;
		}
		break;
	}
}

bool Func::DeleteLine(int i)
{
     int j, k;
     Line ** tmp = new Line*[--lines_count];

     for(j = 0, k = 0; j < lines_count + 1; j++)
     {
          if (j != i)
          {
                tmp[k] = lines[j];
                k++;
          }             
     }
	
     lines[i]->~Line();
     delete [] lines;
     lines = tmp;
   
     return true;
}

bool Func::Ready() const
{
	int i;
	for (i = 0; i < enters_count; i++)
		if ( !enters[i].Ready())
			return false;
	for (i = 0; i < exits_count; i++)
		if ( !exits[i].Ready())
			return false;
	for (i = 0; i < element_count; i++)
		if (elements[i] != NULL && !elements[i]->Ready())
			return false;
	for (i = 0; i < func_count; i++)
		if (functions[i] != NULL && !functions[i]->Ready())
			return false;
	return true;
}


/*
void Func::Tree(Point * root, bool snch)
{

	Point * tmp = root->TakeEnter();
	int i;
	bool flag;
	cout << "before while\n";
	while(root->Sinch() != snch)
	{
		tmp = root->TakeEnter();  //
		while (tmp->Sinch() != snch)
		{

			flag = false;
			for(i = 0; i < tmp->Element()->EnterCount(); i++)
			{
				if ( (*tmp->Element())[i].Sinch() != snch)
				{
					tmp = (*tmp->Element())[i].TakeEnter();
					flag = true;
					break;
				}
			}
			if (!flag)
			{
				tmp->Element()->Result();
			}
		}
	}
}
*/

bool Func::SetSignal(bool * imp)
{
//	bool sinch = imp[0]->sinch;
	int i;
	for (i = 0; i < enters_count; i++)
	{
		enters[i].Set(imp[i]);
	}
	for (i = 0; i < element_count; i++)
		elements[i]->Result();
	for (i = 0; i < func_count; i++)
		functions[i]->Result();
	/*
	for (i = 0; i < exits_count; i++)
	{
		Tree(&exits[i], sinch);
	}
	*/
	return true;
}

bool Func::Result()
{
	int i;
	for (i = 0; i < element_count; i++)
		elements[i]->Result();
	for (i = 0; i < func_count; i++)
			functions[i]->Result();
	return true;
}


Polynorm * Func::MacKlascy(Equation * eqtn, int * len)
{
	int variables = eqtn->variables + 1;                    // Длинна таблиц весовых элементов.
	Polynorm * implict_table = new Polynorm[eqtn->len];     // Массив из элеметов в которые должны входить обрезанные многочлены

	int table_len = eqtn->len + eqtn->size;                 // Длинна всех минтермов
	Polynorm * funk = new Polynorm[table_len];  						// Создаем массив из лог многочленов, которых столько же сколько и номеров минтермов
	int * lens = new int[variables + 1];      					// Создать массив из количества многочленов, у которых вес равен индексу массива
	int i, j;
	for (i = 0; i < variables; i++)
		lens[i] = 0;


	for (i = 0; i < eqtn->len; i++)
	{
		Polynorm temp = Polynorm(eqtn->num_mm[i], eqtn->variables);
		funk[i] = temp;    										// Прировнять многочлен, к созданному по номеру многочлену.
		implict_table[i] = funk[i];
		lens[funk[i].Weight()]++;                               // Увеличить счетчик, показывающий, сколько элементов с этим весом у нас есть.
	}


	for( j = 0; j < eqtn->size; j++)
	{
		Polynorm temp = Polynorm(eqtn->not_use[j], eqtn->variables);
		funk[i + j] = temp;
		lens[funk[i + j].Weight()]++;                            // Увеличить счетчик, показывающий, сколько элементов с этим весом у нас есть.
	}

	std::qsort(funk, eqtn->len, sizeof (Polynorm), compare);     // Сортировка в порядке возростания.
	for (i = 1; i < variables; i++)
		lens[i] = lens[i] + lens[i - 1];                         // Сладывает следующий элемент массива с предидущим.
																 // Индекс массива это вес многочленов

	lens[variables] = lens[variables - 1];

																 // Значения внутри массива, показывает, на каком начинается следующие многочлены,
	Polynorm * answ = new Polynorm[eqtn->len + eqtn->size];                   // Здесь будут записанны окончательные результаты
	int answ_len = 0;

	int stop, start;





    Polynorm * tmp = new Polynorm[variables * variables];                  // Массив из указателей на временые члены алгоритма МакКласки
    int tmp_len = 0;											 // Длинна массива временной таблицы сокращений
    Polynorm temp_poly;                                        // Временный элемент,  в который будет записан результат сокращения.
    int * tmp_weight = new int[variables];                   // Здесь будут записанны веса сокращенных элементов
    for (i = 0; i < variables; i++)
    	tmp_weight[i] = 0;
    bool exit = true;


    Polynorm * t_ptr;
    while (exit)
    {
    	for (i = 0; i < table_len; i++)                          // Для всех минтремов
    	{
    		for (start = lens[funk[i].Weight()], stop = lens[funk[i].Weight() + 1]; start < stop; start++)
    		{													// Начиная с индекса следующего по весу минтрема и заканчвая индексом
																// вес которого на два больше.
                 if( Cut(&funk[i], &funk[start], &temp_poly) )   // Если можно сократить
                 {
                  	 if (FullCompare(tmp, tmp_len, &temp_poly)) // Если такого элемента нет
                	 {

                		tmp[tmp_len] = temp_poly;                  // Добавить элемент в новую таблицу
                		tmp_weight[tmp[tmp_len].Weight()]++;       // Увеличить количество многочленов в таким весом
                		tmp_len++;									// Увеличеть его длинну
                	 }
                 }

    		}

    	}

    	for (i = 1; i < variables; i++)
    		tmp_weight[i] = tmp_weight[i] + tmp_weight[i - 1];
    	exit = false;
    	for(i = 0; i < table_len; i++)
    	{
    		if (!funk[i].WasUse())
    		{
    			answ[answ_len] = funk[i];
    			answ_len++;

    		}
    		else
    			exit = true;
    	}

    	t_ptr = funk;
    	funk = tmp;
    	tmp = t_ptr;
    	table_len = tmp_len;
    	for (i = 0; i < variables; i++)
    	{
    		lens[i] = tmp_weight[i];
    		tmp_weight[i] = 0;
    	}
    	tmp_len = 0;
    }


   delete [] tmp;
   delete [] tmp_weight;
   delete [] lens;

   //////////////////


   table_len = 0;
   bool ** table = new bool*[eqtn->len];
   for (i = 0; i < eqtn->len; i++)                                          // Создание импликатной партии
 	   table[i] = new bool[answ_len];


   int * goriz = new int[eqtn->len];         								// Подсчет того, сколько элементов входит в минтерм
   for (i = 0; i < eqtn->len; i++)
      	goriz[i] = 0;


   int * vert = new int[answ_len];											// Подсчет того, во сколько минтермов входит элемент.
   for (i = 0; i < answ_len; i++)
      	vert[i] = 0;


   for (i = 0; i < eqtn->len; i++)
	   for (j = 0; j < answ_len; j++)
		   if(table[i][j] = ImplictProbe(&implict_table[i], &answ[j]))         // Заполнение импликаинтной матрицы
		   {
			   goriz[i]++;
			   vert[j]++;
		   }
   // Находим те многочлены, в которых входит только один многочлен
   for(i = 0; i < eqtn->len; i++)                                         	// Для всех членов импликантных многочленов
	   if(goriz[i] == 1)                                                    // Для тех, многочленов в которых входят только один обрезанный
		   for(j = 0; j < answ_len; j++)                                    // Поиск этого многочлена
			   if (table[i][j] == true)
			   {
				   funk[table_len] = answ[j];
				   table_len++;
				   vert[j] = 0;
				   for(int i = 0; i < eqtn->len; i++)						// Находим все многочлены, в которые входит обрезанный
					   if (table[i][j] == true)
						   goriz[i] = 0;                                     // Помечаем все многочлены,
 																			// в которые вхоядт обрезанные как уже использованые
			   }
   exit = ArrNotNull(goriz, eqtn->len);
   vert = ImlictMatrix(table, eqtn->len, answ_len, goriz, vert);

   int max = 0;
   while(exit)
   {
	   max = 0;
	   for(j = 0; j < answ_len; j++)
		   if(vert[max] < vert[j])
			   max = j;
	   funk[table_len] = answ[max];
	   table_len++;
	   vert[max] = 0;
	   for(i = 0; i < eqtn->len; i++)						// Находим все многочлены, в которые входит обрезанный
		  if (table[i][max] == true)
		  {
			  goriz[i] = 0;                                     // Помечаем все многочлены,
		  }
	   vert =  ImlictMatrix(table, eqtn->len, answ_len, goriz, vert);
	   exit = ArrNotNull(goriz, eqtn->len);

   }


   if (!eqtn->form)
	   for (i = 0; i < table_len; i++)
		   funk[i].Revers();
   delete [] answ;
   delete [] goriz;
   delete [] vert;
   *len = table_len;
   return funk;
}


void Func::Show() const
{
    cout << "elemets = " << element_count << endl;
    cout << "functions= " << func_count << endl;
    cout << "lines = " << lines_count << endl;
    cout << "exits = " << exits_count << endl;
    cout << "enters = " << enters_count << endl;
    /*
	for (int i = 0; i < element_count; i++)
	{
		cout << "elements #" << i + 1 << " ";
		cout << elements[i] << endl;
	}
    for (int i = 0; i < lines_count; i++)
	{
    	cout << "lines #" << i + 1 << " ";
		cout << lines[i] << endl;
	} */
	for (int i = 0; i < func_count; i++)
	{
         cout << "func " << "x = "  << functions[i]->Position()->x << " y = " << functions[i]->Position()->y;
         cout << endl;
	}

}

Func & Func::operator=(Func & f)
{
	int i;
	delete [] enters;       // Указатель на массив из точек входа в функцию
	enters_count = f.enters_count;       // Количество входов
	enters = new Point[enters_count];
	for (i = 0; i < enters_count; i++)
		enters[i] = f.enters[i];

	delete [] exits;       // Указатель на массив из точек выходов из функции
	exits_count = f.exits_count;       // Количество выходов
	exits = new Point[exits_count];
	for (i = 0; i < exits_count; i++)
		exits[i] = f.exits[i];

	for (i = 0; i < element_count; i++)
		elements[i]->~Basic();
	delete [] elements;
	element_count = f.element_count;       // Количество выходов
	elements = new Basic*[element_count];
	for (i = 0; i < element_count; i++)
		elements[i] = f.elements[i];

	for (i = 0; i < func_count; i++)
			functions[i]->~Func();
	delete [] functions;
	func_count = f.func_count;       // Количество выходов
	functions = new Func*[func_count];
	for (i = 0; i < func_count; i++)
		functions[i] = f.functions[i];

	for (i = 0; i < lines_count; i++)
		lines[i]->~Line();
	delete [] lines;
	lines_count = f.	lines_count;       // Количество выходов
	lines = new Line*[lines_count];
	for (i = 0; i < lines_count; i++)
		lines[i] = f.lines[i];


	parent = f.parent;         // Функция, в которой находится данная функция
	pos = f.pos;

	return *this;
}

Func::Func(Func & f)
{
	int i;
	delete [] enters;       // Указатель на массив из точек входа в функцию
	enters_count = f.enters_count;       // Количество входов
	enters = new Point[enters_count];
	for (i = 0; i < enters_count; i++)
		enters[i] = f.enters[i];

	delete [] exits;       // Указатель на массив из точек выходов из функции
	exits_count = f.exits_count;       // Количество выходов
	exits = new Point[exits_count];
	for (i = 0; i < exits_count; i++)
		exits[i] = f.exits[i];

	for (i = 0; i < element_count; i++)
		elements[i]->~Basic();
	delete [] elements;
	element_count = f.element_count;       // Количество выходов
	elements = new Basic*[element_count];
	for (i = 0; i < element_count; i++)
		elements[i] = f.elements[i];

	for (i = 0; i < func_count; i++)
			functions[i]->~Func();
	delete [] functions;
	func_count = f.func_count;       // Количество выходов
	functions = new Func*[func_count];
	for (i = 0; i < func_count; i++)
		functions[i] = f.functions[i];

	for (i = 0; i < lines_count; i++)
		lines[i]->~Line();
	delete [] lines;
	lines_count = f.	lines_count;       // Количество выходов
	lines = new Line*[lines_count];
	for (i = 0; i < lines_count; i++)
		lines[i] = f.lines[i];


	parent = f.parent;         // Функция, в которой находится данная функция
	pos = f.pos;
}
/*
 * ****************************************************************************************************************************************************
 *                                                                   Polynorm
 * ****************************************************************************************************************************************************
 */


Polynorm::Polynorm()
{
    polynorm = NULL;
    use = false;
}

Polynorm::Polynorm(int num_mma, int len)
{
    int mask = 1;
    mask <<= (len - 1);
    polynorm = new signed short[len];
    elements = len;
    use = false;
    all_not = false;
    form = true;
    norm = 0;
    for(int i = 0; i < len; i++)
    {
         if( (num_mma & mask) != 0)
         {
               polynorm[i] = i + 1;
               norm++;
          }
          else
               polynorm[i] = - (i + 1);
         mask >>= 1;
    }
}

Polynorm::~Polynorm()
{
    delete [] polynorm;
}

Polynorm & Polynorm::operator=(Polynorm & plnm)
{
	elements = plnm.elements;
	form = plnm.form;
	use = plnm.use;
	all_not = plnm.all_not;
	norm = plnm.norm;
	delete [] polynorm;
	polynorm = new signed short[elements];
	for (unsigned int i = 0; i < plnm.elements; i++)
	     	 polynorm[i] = plnm.polynorm[i];
	return *this;
}

Polynorm::Polynorm( const Polynorm & plnm)
{
	elements = plnm.elements;
	form = plnm.form;
	use = plnm.use;
	all_not = plnm.all_not;
	norm = plnm.norm;
	delete [] polynorm;
	polynorm = new signed short[elements];
	for (unsigned int i = 0; i < plnm.elements; i++)
	     	 polynorm[i] = plnm.polynorm[i];
}

void Polynorm::Change(signed short * m, unsigned int num, bool f, bool a_n, bool u)
{

     elements = num;
     form = f;
     use = u;
     all_not = a_n;
     delete [] polynorm;
     polynorm = new signed short[elements];
     for (unsigned int i = 0; i < num; i++)
         	 polynorm[i] = m[i];
}

void Polynorm::Revers()
{
	for (unsigned int i = 0; i < elements; i++)
		polynorm[i] = -polynorm[i];
	form ^= true;
}
void Polynorm::FindWeight()
{
	norm = 0;
	for(unsigned int i = 0; i < elements; i++)
	{
		if(polynorm[i] > 0)
			norm++;
	}
}

int Polynorm::Count() const
{
	int len = 0;
	for (unsigned int i = 0; i < elements; i++ )
		if (polynorm[i] != 0)
			len++;
	return len;
}

