/*
 * Utils.h
 *
 *  Created on: 20.11.2012
 *      Author: roma
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "Basic.h"
#include "Func.h"
#include <gtk/gtk.h>
#include <glib.h>
#include <cmath>




struct Color
{
	int red;
	int green;
	int blue;
};


struct Status
{
	Func * work_func;
	Func * main_func;

	Detals detal;
	int size;
	int ent;
	int ext;

	Equation ** equations;
	int equa_size;

	Color color;

	Basic * elem;

	Func * function;

	Point * start_point;
	Point * finish_point;
	Pos start_pos;
	Pos mouse_pos;

	int width;
	int hight;

	bool st_sp;

	bool * imp;

	int el_ent;
	int el_ext;

	bool on_func;
	GtkWidget * wind;
	GtkWidget ** entry;

};




void delete_element(Func * func, int num_el);
void destroy(GtkWidget*, gpointer);
void draw(GtkWidget* drawarea, GdkEventExpose* event, gpointer data);
void draw_basic(cairo_t * cr, Basic * el, int size, Color * color, bool st_sp);
void draw_line(cairo_t * cr, Line * line, int size, Color * color, bool st_sp);
void draw_func_line(cairo_t * cr, Func * func, int size, Color * color, int width, int height, bool st_sp);
void draw_func(cairo_t * cr, Func * func, int size, Color * color, bool st_sp);

void click_button_not(GtkWidget* widget, gpointer data);
void click_button_func(GtkWidget* widget, gpointer data);
void click_button_and(GtkWidget* widget, gpointer data);
void click_button_or(GtkWidget* widget, gpointer data);
void click_button_line(GtkWidget* widget, gpointer data);
void click_button_del(GtkWidget* widget, gpointer data);
void click_button_st_sp(GtkWidget* widget, gpointer data);
void click_button_back(GtkWidget* widget, gpointer data);
void click_button_ent(GtkWidget* widget, gpointer data);

void click_on_func(GtkWidget* widget, gpointer data);
void click_on_empty(GtkWidget* widget, gpointer data);


void enter_changed(GtkWidget* widget, gpointer data);
void exits_changed(GtkWidget* widget, gpointer data);
void size_changed(GtkWidget* widget, gpointer data);
void main_enter_changed(GtkWidget* widget, gpointer data);
void main_exit_changed(GtkWidget* widget, gpointer data);
void window_change_size(GtkWidget* widget, GtkAllocation *allocation,  gpointer data);

gboolean delete_event(GtkWidget *  widget, GdkEvent *  event,  gpointer data);
void destroy(GtkWidget *  widget, gpointer data);
gboolean delete_event_question(GtkWidget *  widget, GdkEvent *  event,  gpointer data);
void destroy_question(GtkWidget *  widget, gpointer data);
gboolean time_handler(GtkWidget* widget);
gboolean move(GtkWidget * widget, GdkEventMotion *  event, gpointer data);

bool IsFree(Func * func, Pos * pos, int size);
int FindEl(Func * func, Pos * pos, int size);
Point * FindLine(Func * func, Pos * pos, int size, Pos * point);
int FindStartPoint(Func * func, Pos * pos, int size, int width, int hight );
int FindFunc(Func * func, Pos * pos, int size);

gboolean click(GtkWidget * widget, GdkEventButton *  event, gpointer data);
gboolean click_release(GtkWidget * widget, GdkEventButton *  event, gpointer data);

bool text_to_equation(const gchar * text, Equation * equt, int enters);

#endif /* UTILS_H_ */
