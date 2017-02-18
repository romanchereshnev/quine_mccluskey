/*
 * Utils.cpp
 *
 *  Created on: 20.11.2012
 *      Author: roma
 */

#include "Utils.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

void delete_element(Func * func, int num_el, int size)
{
	Basic * el = func->Element(num_el);

	int i, j;


	for(i = 0; i < func->LineCount(); i++)
	{
		if (func->Lines(i)->StartPoint() == el->TakeExit())
		{
			func->DeleteLine(i);
			i--;
		}
	}
	for (i = 0; i < el->EnterCount(); i++)
	{

		for(j = 0; j < func->LineCount(); j++)
		{
			if (func->Lines(j)->FinishPoint() == &(*el)[i] )
			{
				func->DeleteLine(j);
				break;
			}
		}
	}
	func->DeleteElement(num_el);
}

void delete_func(Func * func, int num_func, int size)
{
	Func * function = func->Function(num_func);

	int i, j;


	for (j = 0; j < function->ExitCount(); j++)
	{
		for(i = 0; i < func->LineCount(); i++)
		{
			if (func->Lines(i)->StartPoint() == function->TakeExit(j))
					func->DeleteLine(i);
		}

	}



	for (i = 0; i < function->EnterCount(); i++)
	{

		for(j = 0; j < func->LineCount(); j++)
		{
			if (func->Lines(j)->FinishPoint() == function->TakeEnter(i) )
			{
				func->DeleteLine(j);
				break;
			}
		}
	}

	func->DeleteFunction(num_func);
}

gboolean delete_event(GtkWidget *  widget, GdkEvent *  event,  gpointer data)
{
	return FALSE;
}

void destroy(GtkWidget *  widget, gpointer data)
{
	gtk_main_quit();
}

gboolean delete_event_question(GtkWidget *  widget, GdkEvent *  event,  gpointer data)
{
	return FALSE;
}

void destroy_question(GtkWidget *  widget, gpointer data)
{
	//gtk_main_quit();
}

gboolean time_handler(GtkWidget* widget)
{
  if (widget->window == NULL)
	  return FALSE;
  gtk_widget_queue_draw(widget);
  return TRUE;
}

gboolean move(GtkWidget * widget, GdkEventMotion *  event, gpointer data)
{
	((Status *)data)->mouse_pos.x = (int)event->x;
	((Status *)data)->mouse_pos.y = (int)event->y;

	Func * func = ((Status *)data)->work_func;
	Pos pos = ((Status *)data)->mouse_pos;
	int i, j;
	int size = ((Status *)data)->size;

	if (((Status *)data)->elem != NULL )
	{
		((Status *)data)->elem->ChangePos(&((Status *)data)->mouse_pos);


		Basic * el = ((Status *)data)->elem;


		pos.x -= 1.5 * size;
		pos.y -= 1.5 * size;

		int piece = ( 3 * size) / (el->EnterCount() + 1);
		for (i = 0; i < el->EnterCount(); i++)
		{
			pos.y += piece;
			for(j = 0; j < func->LineCount(); j++)
				if (func->Lines(j)->FinishPoint() == &(*el)[i] )
						func->Lines(j)->ChangePos( &pos, true, ((Status *)data)->hight);
		}


		pos.x = ((Status *)data)->mouse_pos.x + 1.5 * size;
		pos.y = ((Status *)data)->mouse_pos.y;
		for(j = 0; j < func->LineCount(); j++)
			if (func->Lines(j)->StartPoint() == el->TakeExit())
				func->Lines(j)->ChangePos( &pos, false, ((Status *)data)->hight);


	}
	if (((Status *)data)->function != NULL )
	{
		//cout << "Yes, found\n";
		Func * function = ((Status *)data)->function;
		pos.x -= 2.5 * size;
		pos.y -= 3 * size;

		function->ChangePos(&((Status *)data)->mouse_pos);

		int piece = ( 6 * size) / (function->EnterCount() + 1);
		for (i = 0; i < function->EnterCount(); i++)
		{
			pos.y += piece;
			for(j = 0; j < func->LineCount(); j++)
				if (func->Lines(j)->FinishPoint() == function->TakeEnter(i) )
						func->Lines(j)->ChangePos( &pos, true, ((Status *)data)->hight);
		}

		pos.x =  ((Status *)data)->mouse_pos.x + 2.5 * size;
		pos.y =  ((Status *)data)->mouse_pos.y - 3 * size;
		piece = ( 6 * size) / (function->ExitCount() + 1);
		for (i = 0; i < function->ExitCount(); i++)
		{
			pos.y += piece;
			for(j = 0; j < func->LineCount(); j++)
				if (func->Lines(j)->StartPoint() == function->TakeExit(i) )
						func->Lines(j)->ChangePos( &pos, false, ((Status *)data)->hight);
		}
	}



	return TRUE;
}


bool IsFree(Func * func, Pos * pos, int size, Detals det)
{
	bool res = true;
	int width, hight, f_width, f_hight, el_width, el_hight;

	el_width = el_hight = 1.5 * size;
	f_width = 2.5 * size;
	f_hight = 3 * size;

	if (det == function)
	{
		width = f_width;
		hight = f_hight;
	}
	else
	{
		width = el_width;
		hight = el_hight;
	}

	for (int i = 0; i < func->ElementCount(); i++)
	{
		if ( std::abs(pos->x - func->Element(i)->Position()->x) < (width + el_width) && (std::abs(pos->y - func->Element(i)->Position()->y)) < (hight + el_hight  + 0.5 * size))
		{
			res = false;
			break;
		}
	}
	if (res)
	{
		for (int i = 0; i < func->FuncCount(); i++)
			if ( std::abs(pos->x - func->Function(i)->Position()->x) < (width + f_width) && (std::abs(pos->y - func->Function(i)->Position()->y)) < (hight + f_hight + 0.5 * size))
			{
				res = false;
				break;
			}
	}
	return res;
}


int FindEl(Func * func, Pos * pos, int size)
{
	int res = -1;
	for (int i = 0; i < func->ElementCount(); i++)
		if (std::abs(pos->x - func->Element(i)->Position()->x) < size && (std::abs(pos->y - func->Element(i)->Position()->y)) < (1.5 * size))
		{
			res = i;
			break;
		}
	return res;
}

int FindFunc(Func * func, Pos * pos, int size)
{
	int res = -1;
	for (int i = 0; i < func->FuncCount(); i++)
		if (std::abs(pos->x - func->Function(i)->Position()->x) < 2 * size && (std::abs(pos->y - func->Function(i)->Position()->y)) < (3 * size))
		{
			//cout << "pos.x = " << pos->x << endl;
			//cout << "func->Function(i)->Position()->x" << func->Function(i)->Position()->x << endl;
			//cout << "2 * size = " << 2 * size << endl << endl;

			//cout << "pos.y = " << pos->y << endl;
			//cout << "func->Function(i)->Position()->y" << func->Function(i)->Position()->y << endl;
			//cout << "2 * size = " << 3 * size << endl << endl;

			//cout << "res = " << i << endl << endl;
			res = i;
			break;
		}
	return res;
}

int FindStartPoint(Func * func, Pos * pos, int size, int width, int hight )
{
	if ( pos->x <= (size  * 1.2))
	{
		//cout << "Start\n";
		int piece = (hight / (func->EnterCount() + 1));
		int y = piece;
		for (int i = 0; i < func->EnterCount(); i++, y+= piece)
		{
			if (pos->y < (y + 0.5 * size) && pos->y > (y - 0.5 * size))
			{
				return i;
			}
		}
	}
	return -1;
}

Point * FindPoint(Func * func, Pos * pos, int size, Pos * pnt, int width, int hight, bool * side )
{
	int res = -1;
	int piece, y, i, j;
	if ( pos->x <= (size  * 1.2))
	{
		piece = (hight / (func->EnterCount() + 1));
		y = piece;
		for (i = 0; i < func->EnterCount(); i++, y+= piece)
		{
			if (pos->y < (y + 0.5 * size) && pos->y > (y - 0.5 * size))
			{
				pnt->x = size;
				pnt->y = y;
				*side = false;
				return func->TakeEnter(i);
			}
		}
	}

	else if ( pos->x >= width - 1.2 * size)
	{
		piece = (hight / (func->ExitCount() + 1));
		y = piece;
		for (i = 0; i < func->ExitCount(); i++, y+= piece)
		{
			if (pos->y < (y + 0.5 * size) && pos->y > (y - 0.5 * size) && !func->TakeExit(i)->Ready())
			{
				pnt->x = width - size;
				pnt->y = y;
				*side = true;
				return func->TakeExit(i);
			}
		}
	}
	for (i = 0; i < func->ElementCount(); i++)
	{
		if (std::abs(pos->x - func->Element(i)->Position()->x) < 1.5 * size && (std::abs(pos->y - func->Element(i)->Position()->y)) < (1.5 * size))
		{
			if (pos->x < func->Element(i)->Position()->x)
			{
				int count = func->Element(i)->EnterCount() + 2;
				int piece = ((3 * size) / count);
				int bourd = func->Element(i)->Position()->y - 1.5 * size +  piece;
				for(j = 0; j < count - 2; j++, bourd += piece)
				{
					if ( pos->y >= bourd && pos->y <= bourd + piece && !(*func->Element(i))[j].HasEnter())
					{
						pnt->x = func->Element(i)->Position()->x - 1.5 * size;
						pnt->y = func->Element(i)->Position()->y - (1.5 * size) +  ((3 * size) /(func->Element(i)->EnterCount() + 1)) * (1 + j);
						*side = true;
						return &(*func->Element(i))[j];
					}
				}
			}
			else
			{
				pnt->x = func->Element(i)->Position()->x + 1.5 * size;
				pnt->y = func->Element(i)->Position()->y;
				*side = false;
				return func->Element(i)->TakeExit();
			}
		}
	}
	for (i = 0; i < func->FuncCount(); i++)
	{

		if (std::abs(pos->x - func->Function(i)->Position()->x) < 2.5 * size && (std::abs(pos->y - func->Function(i)->Position()->y)) < (3 * size))
		{
			if (pos->x < func->Function(i)->Position()->x)
			{
				int count = func->Function(i)->EnterCount() + 2;
				piece = ((6 * size) / count);
				int bourd = func->Function(i)->Position()->y - 3 * size + piece;
				for(j = 0; j < count - 2; j++, bourd += piece)
				{

					if ( pos->y >= bourd && pos->y <= bourd + piece && !func->Function(i)->TakeEnter(j)->HasEnter())
					{
						pnt->x = func->Function(i)->Position()->x - 2.5 * size;
						pnt->y = func->Function(i)->Position()->y - (3 * size) +  ((6 * size) /(func->Function(i)->EnterCount() + 1)) * (1 + j);
						*side = true;
						return func->Function(i)->TakeEnter(j);
					}
				}

			}
			else
			{
				int count = func->Function(i)->ExitCount() + 2;
				piece = ((6 * size) / count);
				int bourd = func->Function(i)->Position()->y - 3 * size + piece;
				for(j = 0; j < count - 2; j++, bourd += piece)
				{
					if ( pos->y >= bourd && pos->y <= bourd + piece)
					{
						pnt->x = func->Function(i)->Position()->x + 2.5 * size;
						pnt->y = func->Function(i)->Position()->y - (3 * size) +  ((6 * size) /(func->Function(i)->ExitCount() + 1)) * (1 + j);
						*side = false;
						return func->Function(i)->TakeExit(j);
					}
				}
			}
		}
	}
	return NULL;
}


gboolean click(GtkWidget * widget, GdkEventButton *  event, gpointer data)
{
	Pos pos = { (int)event->x, (int)event->y};

	Point * tmp;
	int num_el = -1;

	bool side;
	int i;

	int size = ((Status *)data)->size;
	Func * func = ((Status *)data)->work_func;

	if (event->button == 3)
	{

		if ( (num_el =  FindEl(func, &pos, size)) != -1 )
		{
			func->Element(num_el)->Show();
			cout << endl;
		}
		else if ((num_el = FindFunc(func, &pos, size)) != -1)
		{

			((Status *)data)->work_func = func->Function(num_el);

		}
		else if (tmp = FindPoint( func, &pos, size, &((Status *)data)->start_pos, ((Status *)data)->width, ((Status *)data)->hight, &side))
		{
			//tmp->Show();
			//cout << endl;
		}
/*
        else
        {

        	cout << " work func show\n";
        	func->Show();
        }
*/
	}
	/*
	else if (event->button == 2)
	{
		//for (int i = 0; i < func->LineCount(); i++)
		//	func->Lines(i)->Show();

		//cout << "work func = " << &func << endl;
		cout << " work func show\n";
		func->Show();
	}
	*/



	else if (((Status *)data)->st_sp && event->button == 1)
	{
		int count;
		if  (((count = FindStartPoint(func, &pos, size, ((Status *)data)->width, ((Status *)data)->hight )) != -1) && func->Parent() == NULL)
		{
			((Status *)data)->imp[count] = !((Status *)data)->imp[count];
		}


	}
	else if (event->button == 1 && !((Status *)data)->st_sp)
	{

		switch(((Status *)data)->detal )
		{
		case basic_or:
		case basic_and:
		case basic_not:
			if (IsFree(func, &pos, size, ((Status *)data)->detal))
					 func->Create( ((Status *)data)->detal, ((Status *)data)->ent, &pos,1);
			break;

		case point:
			if ( (num_el =  FindEl(func, &pos, size)) != -1 )
			{
				((Status *)data)->elem = func->Element(num_el);
			}
			else if ((num_el =  FindFunc(func, &pos, size)) != -1 )
			{
				((Status *)data)->function = func->Function(num_el);
			}
			else if ((tmp = FindPoint( func, &pos, size, &((Status *)data)->start_pos,((Status *)data)->width, ((Status *)data)->hight, &side)) )
			{
				if(side)
				{
					((Status *)data)->finish_point = tmp;
				}
				else
				{
					((Status *)data)->start_point = tmp;
				}
			}

			break;
		case function:
			//cout << "x = " << pos.x << " y = " << pos.y << endl;
			if (IsFree(func, &pos, size, ((Status *)data)->detal))
			{

				if (!((Status *)data)->on_func)
					func->Create(((Status *)data)->detal, ((Status *)data)->ent, &pos, ((Status *)data)->ext);
				else
				{
					/*
					cout << "CreateMK\n" << endl;
					cout << "x = " << pos.x << " y = " << pos.y << endl;
					func->CreateMK(((Status *)data)->ent, ((Status *)data)->ext, ((Status *)data)->equations, &pos,
							((Status *)data)->size, ((Status *)data)->width, ((Status *)data)->hight, func); */

					((Status *)data)->start_pos = pos;
					for (i = 0; i < ((Status *)data)->ext; i++)
					{
						gtk_widget_show(((Status *)data)->entry[i]);
					}
					gtk_widget_show(((Status *)data)->wind);

				}
			}


			break;
		case delt:
			if ( (num_el =  FindEl(func, &pos, size)) != -1 )
			{
				((Status *)data)->elem = NULL;
				delete_element(func, num_el, size);
			}
			else if ((num_el = FindFunc(func, &pos, size)) != -1)
			{
				((Status *)data)->function = NULL;
				delete_func(func, num_el, size);
			}
		}
	}

	return TRUE;
}


gboolean click_release(GtkWidget * widget, GdkEventButton *  event, gpointer data)
{
	Pos pos = {event->x, event->y};
	((Status *)data)->elem = NULL;
	int size = ((Status *)data)->size;
	Func * func = ((Status *)data)->work_func;
	bool side;


	if(((Status *)data)->detal == point)
	{
		Point * tmp_point;
		Pos tmp_pos;
		if (((Status *)data)->start_point)
		{
			if ( ((tmp_point = FindPoint( func, &pos, size, &tmp_pos, ((Status *)data)->width, ((Status *)data)->hight, &side)) != NULL) && side && !tmp_point->HasEnter())
			{
				Line * line = new Line;
				Line tmp_line( &((Status *)data)->start_pos, &tmp_pos, ((Status *)data)->start_point, tmp_point, ((Status *)data)->hight);
				*line = tmp_line;
				((Status *)data)->work_func->NewLine(line);
				tmp_point->SetEnter(((Status *)data)->start_point);
			}
		}
		else if (((Status *)data)->finish_point)
		{
			if ( ((tmp_point = FindPoint(func, &pos, size, &tmp_pos, ((Status *)data)->width, ((Status *)data)->hight, &side)) != NULL) && !side)
			{
				Line * line = new Line;

				Line tmp_line(&tmp_pos, &((Status *)data)->start_pos, tmp_point, ((Status *)data)->finish_point,((Status *)data)->hight);
				*line = tmp_line;
				func->NewLine(line);

				((Status *)data)->finish_point->SetEnter(tmp_point);
			}
		}
		((Status *)data)->start_point = NULL;
		((Status *)data)->finish_point = NULL;
		((Status *)data)->function = NULL;
	}
	return TRUE;
}


void click_button_not(GtkWidget* widget, gpointer data)
{
	((Status *)data)->detal = basic_not;
}

void click_button_func(GtkWidget* widget, gpointer data)
{
	((Status *)data)->detal = function;
}

void click_button_and(GtkWidget* widget, gpointer data)
{
	((Status *)data)->detal =  basic_and;
}

void click_button_or(GtkWidget* widget, gpointer data)
{
	((Status *)data)->detal =  basic_or;
}

void click_button_line(GtkWidget* widget, gpointer data)
{
	((Status *)data)->detal =  point;
}

void click_button_del(GtkWidget* widget, gpointer data)
{
	((Status *)data)->detal =  delt;
}

void click_button_st_sp(GtkWidget* widget, gpointer data)
{
	if (((Status *)data)->work_func->Ready())
	{
		((Status *)data)->st_sp ^= true;
	}
	else
		((Status *)data)->st_sp = false;
}

void click_button_back(GtkWidget* widget, gpointer data)
{
	if (((Status *)data)->work_func->Parent() != NULL)
	{
		((Status *)data)->work_func = ((Status *)data)->work_func->Parent();

	}
}

void click_button_ent(GtkWidget* widget, gpointer data)
{
	const gchar *entry_text;
	bool res;
	for (int i = 0; i < ((Status *)data)->ext; i++)
	{
		//ShowEqtn(((Status *)data)->equations[i]);
		entry_text = gtk_entry_get_text (GTK_ENTRY (((Status *)data)->entry[i]));
		if (!(res = text_to_equation(entry_text,((Status *)data)->equations[i],((Status *)data)->ent )))
			break;
	}
	if (!res)
		cout << "NO!!!\n";
	else
	{

		((Status *)data)->work_func->CreateMK(((Status *)data)->ent, ((Status *)data)->ext, ((Status *)data)->equations, &((Status *)data)->start_pos,
									((Status *)data)->size, ((Status *)data)->width, ((Status *)data)->hight, ((Status *)data)->work_func);
	}
	for (int i = 0; i < 16; i++)
	{
		gtk_widget_hide(((Status *)data)->entry[i]);
	}



	gtk_widget_hide(((Status *)data)->wind);


}

void draw(GtkWidget* drawarea, GdkEventExpose* event, gpointer data)
{
	Pos * tmp;
	cairo_t *cr = gdk_cairo_create(drawarea->window);

	((Status *)data)->width = event->area.width;
	((Status *)data)->hight = event->area.height;


	Func * func = ((Status *)data)->work_func;
	cairo_set_line_width(cr, ((Status *)data)->size * 0.1);
	cairo_rectangle (cr, event->area.x, event->area.y, event->area.width, event->area.height);

	if (((Status *)data)->st_sp)
	{
		func->SetSignal(((Status *)data)->imp);
	}


    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_stroke (cr);

    draw_func_line(cr, ((Status *)data)->work_func, ((Status *)data)->size, &((Status *)data)->color,event->area.width, event->area.height, ((Status *)data)->st_sp );
    Color col = {100, 100, 0};
    int i, j;

    for (i = 0; i < func->ElementCount(); i++)
    {
    		draw_basic(cr,func->Element(i),	((Status *)data)->size, func->Element(i) == ((Status *)data)->elem ? &col : &((Status *)data)->color, ((Status *)data)->st_sp);
    }

    for (i = 0; i < func->LineCount(); i++)
    {
    	draw_line(cr, func->Lines(i), ((Status *)data)->size, &((Status *)data)->color , ((Status *)data)->st_sp);
    	if (func->Lines(i)->StartPoint()->Amount() > 1)
    		  for (j = 0; j < func->LineCount(); j++)
    			  if( func->Lines(i)->StartPoint() == func->Lines(j)->StartPoint())
    			  {
    				  if ((*func->Lines(i))[1]->x == (*func->Lines(j))[1]->x)
    				  {
    					  tmp = (*func->Lines(i))[2]->y > (*func->Lines(j))[2]->y ? (*func->Lines(i))[2] : (*func->Lines(j))[2];
    				  }
    				  else
    					  tmp = (*func->Lines(i))[1]->x < (*func->Lines(j))[1]->x ? (*func->Lines(i))[1] : (*func->Lines(j))[1];

    				  cairo_set_line_width(cr, ((Status *)data)->size * 0.2);
    				  cairo_move_to(cr, tmp->x - 0.1 * ((Status *)data)->size, tmp->y);
    				  cairo_line_to(cr,tmp->x + 0.1 * ((Status *)data)->size, tmp->y);
    				  cairo_stroke (cr);
    				  cairo_set_line_width(cr, ((Status *)data)->size * 0.1);

    			  }
    }

    for (i = 0; i < func->FuncCount(); i++)
		draw_func(cr, func->Function(i), ((Status *)data)->size, &((Status *)data)->color, ((Status *)data)->st_sp);


    if (((Status *)data)->start_point || ((Status *)data)->finish_point)
    {
    	cairo_move_to(cr, ((Status *)data)->start_pos.x, ((Status *)data)->start_pos.y);
    	cairo_line_to (cr,((Status *)data)->mouse_pos.x, ((Status *)data)->mouse_pos.y);
    	cairo_stroke (cr);
    }
}

void draw_func_line(cairo_t * cr, Func * func, int size, Color * color, int width, int height, bool st_sp)
{
	int piece = height / (func->EnterCount() + 1);
	int y = piece;
	Color fls = {0, 0, 0};
	Color tr = {0, 0, 0};
	if(st_sp)
	{
	  tr.red = 0;
	  tr.green = 100;
	  tr.blue = 0;
	}
	Color * c_line = & fls;
	for (int i = 0; i < func->EnterCount(); i++, y += piece)
	{
		c_line = func->TakeEnter(i)->Value() ? &tr : &fls;
		cairo_set_source_rgb (cr, c_line->red, c_line->green, c_line->blue);
		cairo_move_to(cr, 0, y);
		cairo_line_to (cr,size, y);
		cairo_stroke (cr);

		cairo_set_line_width(cr, size * 0.2);
		cairo_move_to(cr, 0.8 * size, y);
		cairo_line_to(cr, size, y);
		cairo_stroke (cr);

		cairo_set_line_width(cr, size * 0.1);
	}

	piece = height / (func->ExitCount() + 1);
	y = piece;
	for (int i = 0; i < func->ExitCount(); i++, y += piece)
	{
		c_line = func->TakeExit(i)->Value() ? &tr : &fls;
		cairo_set_source_rgb (cr, c_line->red, c_line->green, c_line->blue);
		cairo_move_to(cr, width - size, y);
		cairo_line_to (cr, width, y);
		cairo_stroke (cr);

		cairo_set_line_width(cr, size * 0.2);

		cairo_move_to(cr, width -  size , y);
		cairo_line_to(cr, width - 0.8 * size, y);
		cairo_stroke (cr);

		cairo_set_line_width(cr, size * 0.1);
	}
	cairo_set_source_rgb (cr, 0, 0, 0);

}

void draw_line(cairo_t * cr, Line * line, int size, Color * color, bool st_sp)
{

	cairo_set_line_width(cr, size * 0.1);
	Color fls = {0, 0, 0};
	Color tr = {0, 0, 0};
	if(st_sp)
	{
	  tr.red = 0;
	  tr.green = 100;
	  tr.blue = 0;
	}
	Color * c_line;
	for (int i = 0; i < line->PosCount() - 1; i++)
	{
		c_line = line->Value() ? &tr : &fls;
		cairo_set_source_rgb (cr, c_line->red, c_line->green, c_line->blue);

		cairo_move_to(cr, (*line)[i]->x, (*line)[i]->y);
		cairo_line_to (cr,(*line)[i + 1]->x, (*line)[i + 1]->y);
		cairo_stroke (cr);


	}
	cairo_set_source_rgb (cr, color->red, color->green, color->blue);
}

void draw_func(cairo_t * cr, Func * func, int size, Color * color, bool st_sp)
{
	cairo_set_line_width(cr, size * 0.1);
	Color fls = {0, 0, 0};
	Color tr = {0, 0, 0};
	if(st_sp)
	{
	  tr.red = 0;
	  tr.green = 100;
	  tr.blue = 0;
	}
	Color * c_line;

	cairo_set_source_rgb (cr, color->red, color->green, color->blue);


	cairo_move_to(cr, func->Position()->x - 2 * size, func->Position()->y - (3 * size));
	cairo_line_to(cr, func->Position()->x - 2 * size, func->Position()->y + (3 * size));
	cairo_stroke (cr);

	cairo_move_to(cr, func->Position()->x - 2 * size, func->Position()->y + (3 * size));
	cairo_line_to(cr, func->Position()->x + 2 * size, func->Position()->y + (3 * size));
	cairo_stroke (cr);

	cairo_move_to(cr, func->Position()->x + 2 * size, func->Position()->y + (3 * size));
	cairo_line_to(cr, func->Position()->x + 2 * size, func->Position()->y - (3 * size));
	cairo_stroke (cr);

	cairo_move_to(cr, func->Position()->x + 2 * size, func->Position()->y - (3 * size));
	cairo_line_to(cr, func->Position()->x - 2 * size, func->Position()->y - (3 * size));
	cairo_stroke (cr);

	cairo_move_to(cr, func->Position()->x - size, func->Position()->y - (3 * size));
	cairo_line_to(cr, func->Position()->x - size, func->Position()->y + (3 * size));
	cairo_stroke (cr);

	cairo_move_to(cr, func->Position()->x + size, func->Position()->y - (3 * size));
	cairo_line_to(cr, func->Position()->x + size, func->Position()->y + (3 * size));
	cairo_stroke (cr);
	Pos * pos = func->Position();

	int i;
	int y;
	int x;
	int piece;

	for (i = 0, piece = (6 * size) / (func->EnterCount() + 1), y = pos->y - (3 * size) + piece, x = pos->x - 2.5 * size; i < func->EnterCount(); i++, y+= piece)
	{
		c_line = func->TakeEnter(i)->Value() ? &tr : &fls;

		cairo_set_source_rgb (cr, c_line->red, c_line->green, c_line->blue);
		cairo_move_to(cr, x, y);
		cairo_line_to (cr, x + 0.5 * size, y);
		cairo_stroke (cr);

		cairo_set_line_width(cr, size * 0.2);
		cairo_move_to(cr, x, y);
		cairo_line_to(cr, x + 0.2 * size, y);
		cairo_stroke (cr);

		cairo_set_line_width(cr, size * 0.1);
	}

	for (i = 0, piece = (6 * size) / (func->ExitCount() + 1), y = pos->y - (3 * size) + piece, x = pos->x + 2 * size; i < func->ExitCount(); i++, y+= piece)
	{
		c_line = func->TakeExit(i)->Value() ? &tr : &fls;

		cairo_set_source_rgb (cr, c_line->red, c_line->green, c_line->blue);
		cairo_move_to(cr, x, y);
		cairo_line_to (cr, x + 0.5 * size, y);
		cairo_stroke (cr);

		cairo_set_line_width(cr, size * 0.2);
		cairo_move_to(cr, x + 0.3 * size, y);
		cairo_line_to(cr, x + 0.5 * size, y);
		cairo_stroke (cr);

		cairo_set_line_width(cr, size * 0.1);
	}

	cairo_set_source_rgb (cr, color->red, color->green, color->blue);

}

void draw_basic(cairo_t * cr, Basic * el, int size, Color * color, bool st_sp)
{

	int hight = size * 3;
	Color fls = {0, 0, 0};
	Color tr = {0, 0, 0};
		if(st_sp)
		{
		  tr.red = 0;
		  tr.green = 100;
		  tr.blue = 0;
		}
	Color * c_line;

	cairo_set_source_rgb (cr, color->red, color->green, color->blue);


	cairo_move_to(cr, el->Position()->x - size, el->Position()->y - (1.5 * size));
	cairo_line_to (cr,el->Position()->x - size, el->Position()->y + (1.5 * size));
	cairo_stroke (cr);

	cairo_move_to(cr,el->Position()->x - size, el->Position()->y + (1.5 * size));
	cairo_line_to(cr, el->Position()->x + size, el->Position()->y + (1.5 * size));
	cairo_stroke (cr);

	cairo_move_to(cr, el->Position()->x + size, el->Position()->y + (1.5 * size));
	cairo_line_to (cr,el->Position()->x + size, el->Position()->y - (1.5 * size));
	cairo_stroke (cr);

	cairo_move_to(cr,el->Position()->x + size, el->Position()->y - (1.5 * size));
	cairo_line_to (cr,el->Position()->x - size, el->Position()->y - (1.5 * size));
	cairo_stroke (cr);

	int i;
	double lenth;
	for (i = 0, lenth = hight / (el->EnterCount() + 1); i < el->EnterCount(); i++)
	{
		c_line = (*el)[i].Value() ? &tr : &fls;
		cairo_set_source_rgb (cr, c_line->red, c_line->green, c_line->blue);

		cairo_move_to(cr, el->Position()->x - size, el->Position()->y - (1.5 * size) + lenth * (1 + i));
		cairo_line_to(cr, el->Position()->x - size - 0.5 * size, el->Position()->y - (1.5 * size) + lenth * (1 + i));
		cairo_stroke (cr);

		cairo_set_line_width(cr, size * 0.2);
		cairo_move_to(cr, el->Position()->x - size - 0.5 * size, el->Position()->y - (1.5 * size) + lenth * (1 + i));
		cairo_line_to(cr, el->Position()->x - size - 0.5 * size + size * 0.2, el->Position()->y - (1.5 * size) + lenth * (1 + i));
		cairo_stroke (cr);

		cairo_set_line_width(cr, size * 0.1);
	}


	c_line = el->TakeExit()->Value() ? &tr : &fls;
	cairo_set_source_rgb (cr, c_line->red, c_line->green, c_line->blue);
	cairo_move_to(cr, el->Position()->x + size, el->Position()->y - (1.5 * size) + (hight / 2));
	cairo_line_to(cr, el->Position()->x + size + (0.5 * size), el->Position()->y - (1.5 * size) + (hight / 2));
	cairo_stroke (cr);



	cairo_set_line_width(cr, size * 0.2);
	cairo_move_to(cr, el->Position()->x + size + (0.5 * size), el->Position()->y - (1.5 * size) + (hight / 2));
	cairo_line_to(cr, el->Position()->x + size + (0.5 * size) - size * 0.2, el->Position()->y - (1.5 * size) + (hight / 2));
	cairo_stroke (cr);
	cairo_set_line_width(cr, size * 0.1);


	cairo_set_source_rgb (cr, color->red, color->green, color->blue);
	cairo_text_extents_t te;
	cairo_set_source_rgb (cr, color->red, color->green, color->blue);
	cairo_select_font_face (cr, "Georgia", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size (cr, size);


	//cairo_move_to (cr, el->Position()->x + size - te.width / 2 - te.x_bearing, el->Position()->y + 2 * size - te.height / 2 - te.y_bearing);
	switch (el->KindOfElement())
	{
	case basic_not:
		cairo_text_extents (cr, "not", &te);
		cairo_move_to (cr, el->Position()->x - te.width / 2 - te.x_bearing, el->Position()->y  -  size - te.height / 2 - te.y_bearing);
		cairo_show_text (cr, "not");
		break;
	case basic_and:
			cairo_text_extents (cr, "&", &te);
			cairo_move_to (cr, el->Position()->x - te.width / 2 - te.x_bearing, el->Position()->y  -  size - te.height / 2 - te.y_bearing);
			cairo_show_text (cr, "&");
			break;
	case basic_or:
			cairo_text_extents (cr, "1", &te);
			cairo_move_to (cr, el->Position()->x - te.width / 2 - te.x_bearing, el->Position()->y -  size - te.height / 2 - te.y_bearing);
			cairo_show_text (cr, "1");
			break;
	}
}


void exits_changed(GtkWidget* widget, gpointer data)
{
	((Status *)data)->ext =  gtk_spin_button_get_value_as_int ((GtkSpinButton * )widget);
}

void enter_changed(GtkWidget* widget, gpointer data)
{
	((Status *)data)->ent =  gtk_spin_button_get_value_as_int ((GtkSpinButton * )widget);
}

void re_join_window(Func * func, int size, int hight, int width)
{

	int i, j;
	Pos pos;
	int piece = hight / (func->ExitCount() + 1);

	pos.x = width - size * 0.5;
	pos.y = 0;
	for(i = 0; i < func->ExitCount(); i++)
	{
		pos.y += piece;
		for(j = 0; j < func->LineCount(); j++)
			if (func->Lines(j)->FinishPoint() == func->TakeExit(i) )
			{
						func->Lines(j)->ChangePos( &pos, true, hight);
			}
	}
	piece = hight / (func->EnterCount() + 1);
	pos.x = size * 0.5;
	pos.y = 0;
	for(i = 0; i < func->EnterCount(); i++)
	{
		for(j = 0; j < func->LineCount(); j++)
		{
			cout << pos.y << endl;
			pos.y += piece;
			if (func->Lines(j)->StartPoint() == func->TakeEnter(i) )
						func->Lines(j)->ChangePos(&pos, false, hight);
		}
	}
	for(i = 0; i < func->FuncCount(); i++)
		re_join_window(func->Function(i), size, hight, width);

}

void re_join(Func * func, int size, int hight, int width)
{
	Pos pos;

	Basic * el;

	int i, j , k;
	int piece;
	for(k = 0; k < func->ElementCount(); k++)
	{
		el = func->Element(k);
		pos = *el->Position();
		pos.x -= 1.5 * size;
		pos.y -= 1.5 * size;
		piece = ( 3 * size) / (el->EnterCount() + 1);
		for (i = 0; i < el->EnterCount(); i++)
		{
			pos.y += piece;
			for(j = 0; j < func->LineCount(); j++)
				if (func->Lines(j)->FinishPoint() == &(*el)[i] )
					func->Lines(j)->ChangePos( &pos, true, hight);
		}
		pos.x = el->Position()->x + 1.5 * size;
		pos.y = el->Position()->y;
		for(j = 0; j < func->LineCount(); j++)
			if (func->Lines(j)->StartPoint() == el->TakeExit())
				func->Lines(j)->ChangePos( &pos, false, hight);
	}


	piece = hight / (func->EnterCount() + 1);
	for(k = 0, pos.y = 0; k < func->EnterCount(); k++)
	{
		pos.x = 0.5 * size;
		pos.y += piece;
		for(j = 0; j < func->LineCount(); j++)
			if (func->Lines(j)->StartPoint() == func->TakeEnter(k) )
					func->Lines(j)->ChangePos( &pos, false, hight);
	}

	piece = hight / (func->ExitCount() + 1);
	for(k = 0, pos.y = 0; k < func->EnterCount(); k++)
	{
		pos.x = width - 0.5 * size;
		pos.y += piece;
		for(j = 0; j < func->LineCount(); j++)
			if (func->Lines(j)->FinishPoint() == func->TakeExit(k) )
					func->Lines(j)->ChangePos( &pos, true, hight);
	}

	Func * function;
	for(k = 0; k < func->FuncCount(); k++)
	{
		//cout << "Yes, found\n";
		function = func->Function(k);
		pos = *function->Position();
		pos.x -= 2.5 * size;
		pos.y -= 3 * size;


		piece = ( 6 * size) / (function->EnterCount() + 1);
		for (i = 0; i < function->EnterCount(); i++)
		{
			pos.y += piece;
			for(j = 0; j < func->LineCount(); j++)
				if (func->Lines(j)->FinishPoint() == function->TakeEnter(i) )
						func->Lines(j)->ChangePos( &pos, true, hight);
		}

		pos.x =  function->Position()->x + 2.5 * size;
		pos.y =  function->Position()->y - 3 * size;
		piece = ( 6 * size) / (function->ExitCount() + 1);
		for (i = 0; i < function->ExitCount(); i++)
		{
			pos.y += piece;
			for(j = 0; j < func->LineCount(); j++)
				if (func->Lines(j)->StartPoint() == function->TakeExit(i) )
						func->Lines(j)->ChangePos( &pos, false, hight);
		}
	}
	for(k = 0; k < func->FuncCount(); k++)
	{
		re_join(func->Function(k), size, hight, width);
	}

}

void window_change_size(GtkWidget* widget, GtkAllocation *allocation,  gpointer data)
{



	if (((Status *)data)->main_func)
		re_join(((Status *)data)->main_func, ((Status *)data)->size, ((Status *)data)->hight, ((Status *)data)->width);

}



void size_changed(GtkWidget* widget, gpointer data)
{
	Pos pos;
	int size = ((Status *)data)->size =  gtk_spin_button_get_value_as_int ((GtkSpinButton * )widget);
	re_join(((Status *)data)->main_func, size, ((Status *)data)->hight, ((Status *)data)->width);

}

void click_on_func(GtkWidget* widget, gpointer data)
{
	((Status *)data)->on_func = true;
}

void click_on_empty(GtkWidget* widget, gpointer data)
{
	((Status *)data)->on_func = false;
}

bool text_to_equation(const gchar * text, Equation * equt, int enters)
{
	gchar tmp[4];
	int j = 0;
	int * arr = new int[int (std::pow(2,enters))];
	int num;
	int i = 0;
	int count = 0;
	while(!std::isdigit(text[i]))
	{
		i++;
		if (text[i] == '\0')
			return false;
	}
	equt->form = bool (text[i] - '0');

	while(text[i] != '(')
	{
		i++;
	}
	while(text[i] != ')' && text[i] != '\0')
	{

		j = 0;
		while(!std::isdigit(text[i]))
		{
			i++;
		}
		while(std::isdigit(text[i]))
		{
			tmp[j] = text[i];
			i++;
			j++;
		}
		tmp[j] = '\0';
		arr[count++] = std::atoi(tmp);
		i++;
	}
	delete [] equt->num_mm;
	equt->num_mm = new int[count];
	equt->len = count;
	equt->variables = enters;
	equt->size = 0;
	equt->not_use = NULL;
	for(i = 0; i < count; i++)
		equt->num_mm[i] = arr[i];
	delete [] arr;

	if(count < 1)
		return false;
	return true;
}

void main_enter_changed(GtkWidget* widget, gpointer data)
{
	if (((Status *)data)->st_sp == false)
	{
		//cout << gtk_spin_button_get_value_as_int ((GtkSpinButton * )widget) << endl;
		int ent =  gtk_spin_button_get_value_as_int ((GtkSpinButton * )widget);
		int i,j;
		Func * func = ((Status *)data)->main_func;
		for (i = 0; i < func->EnterCount(); i++)
		{
			for (j = 0; j < func->LineCount(); j++)
				if (func->Lines(j)->StartPoint() == func->TakeEnter(i))
				{
					func->DeleteLine(j);
					j--;
				}
		}
		func->ChangeEnterNum(ent);
		for (i = 0; i < func->EnterCount(); i++)
		{
			func->TakeEnter(i)->SetEnter(NULL);
			func->TakeEnter(i)->SetElement(NULL);
		}
	}
}

void main_exit_changed(GtkWidget* widget, gpointer data)
{
	if (((Status *)data)->st_sp == false)
	{

		int ext =  gtk_spin_button_get_value_as_int ((GtkSpinButton * )widget);
		int i,j;
		Func * func = ((Status *)data)->main_func;
		for (i = 0; i < func->ExitCount(); i++)
		{
			for (j = 0; j < func->LineCount(); j++)
				if (func->Lines(j)->FinishPoint() == func->TakeExit(i))
				{
					func->DeleteLine(j);
					j--;
				}
		}
		func->ChangeExitNum(ext);
		for (i = 0; i < func->ExitCount(); i++)
		{
			func->TakeExit(i)->NewExit(NULL);
			func->TakeExit(i)->SetElement(NULL);
		}
	}
}
