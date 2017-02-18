// Главный файл
#include "Utils.h"




int main(int argc, char ** argv)
{

    int enters = 4;
    int exits = 2;
	Func func(enters, exits);

	GtkWidget * window;
	GtkWidget * table;
	GtkWidget * drawing;
	GtkWidget * button;
	GtkAdjustment *adj;
	GtkWidget *spinner;
	GtkWidget * label;
	GSList *group;


	Equation ** fnc = new Equation*[16];
	for (int i = 0; i < 16; i++)
	{
		fnc[i] = new Equation;
		fnc[i]->num_mm = NULL;
		fnc[i]->not_use = NULL;
	}

	Status status = { &func, &func, basic_not, 20, enters, exits, fnc, 0, {0, 0, 0} , NULL, NULL, NULL, NULL, {0,0}, {0, 0}, 600, 400, false, NULL, enters, exits,
	false, NULL, NULL};



	gtk_init(&argc, &argv);
	
	bool * imp = new bool[enters];
	for (int i = 0; i < func.ExitCount(); i++)
		func.TakeExit(i)->NewExit(NULL);

	for (int i = 0; i < func.EnterCount(); i++)
	{
		imp[i] = false;
		func.TakeEnter(i)->SetEnter(NULL);
	}


	status.imp = imp;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_title(GTK_WINDOW(window), "Grapher");
	gtk_container_set_border_width(GTK_CONTAINER(window), 1);
	g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(delete_event), NULL);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(destroy), NULL);

	//g_signal_connect(G_OBJECT(window), "window_state_event", G_CALLBACK(window_change_size), &status);
	g_signal_connect(G_OBJECT(window), "size-allocate", G_CALLBACK(window_change_size), &status);

	status.wind = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(status.wind), "Function");
	gtk_container_set_border_width(GTK_CONTAINER(status.wind), 1);
	g_signal_connect(G_OBJECT(status.wind), "delete_event", G_CALLBACK(delete_event_question), NULL);
	g_signal_connect(G_OBJECT(status.wind), "destroy", G_CALLBACK(destroy_question), NULL);


	table = gtk_table_new (2, 17, false);
	gtk_container_add (GTK_CONTAINER (status.wind), table);
	gtk_widget_show (table);


	button = gtk_button_new_with_label ("Enter");
	gtk_table_attach_defaults (GTK_TABLE (table), button, 0, 2, 16, 17);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(click_button_ent), &status);
	gtk_widget_show (button);

	status.entry = new GtkWidget*[16];

	for (int i = 0; i < 16; i++)
	{

		//label = gtk_label_new("Enter function:");
		//gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, i, i + 1);
		//gtk_widget_show (label);


		status.entry[i] = gtk_entry_new ();
		gtk_entry_set_max_length (GTK_ENTRY (status.entry[i]), 50);
		gtk_editable_set_editable (GTK_EDITABLE (status.entry[i]), TRUE);
		gtk_entry_set_text (GTK_ENTRY (status.entry[i]), "Enter a function");
		gtk_editable_select_region (GTK_EDITABLE (status.entry[i]), 0, GTK_ENTRY (status.entry[i])->text_length);
		gtk_table_attach_defaults (GTK_TABLE (table), status.entry[i], 1, 2, i, i + 1);
		gtk_widget_show (status.entry[i]);
		gtk_widget_hide(status.entry[i]);

	}




	gtk_widget_show (table);
	gtk_widget_show(status.wind);
	gtk_widget_hide(status.wind);

	table = gtk_table_new (24, 18, false);
	gtk_container_add (GTK_CONTAINER (window), table);


	drawing = gtk_drawing_area_new();
	gtk_widget_set_size_request(drawing, 600, 400);
	gtk_table_attach_defaults (GTK_TABLE (table), drawing, 2, 18, 0, 24);
	gtk_widget_set_events (drawing, GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK |  GDK_BUTTON_PRESS_MASK |  GDK_BUTTON_RELEASE_MASK);
	g_signal_connect(G_OBJECT (drawing), "expose_event", G_CALLBACK (draw), &status);
	g_signal_connect(G_OBJECT(drawing), "motion_notify_event", G_CALLBACK(move), &status);
    g_signal_connect(G_OBJECT(drawing), "button-press-event", G_CALLBACK(click), &status);
    g_signal_connect(G_OBJECT(drawing), "button_release_event", G_CALLBACK(click_release), &status);
    gtk_widget_show (drawing);
    	
    	
	button = gtk_button_new_with_label ("   НЕ   ");
	gtk_table_attach_defaults (GTK_TABLE (table), button, 0, 1, 0, 1);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(click_button_not), &status);
    gtk_widget_show (button);

	button = gtk_button_new_with_label ("ФУНКЦИЯ");
	gtk_table_attach_defaults (GTK_TABLE (table), button, 1, 2, 0, 1);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(click_button_func), &status);
    gtk_widget_show (button);
    	
    button = gtk_button_new_with_label ("   И    ");
	gtk_table_attach_defaults (GTK_TABLE (table), button, 0, 1, 1, 2);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(click_button_and), &status);
    gtk_widget_show (button);
    	
    button = gtk_button_new_with_label ("  ИЛИ  ");
	gtk_table_attach_defaults (GTK_TABLE (table), button, 1, 2, 1, 2);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(click_button_or), &status);
    gtk_widget_show (button);
    	
    button = gtk_button_new_with_label (" ЛИНИЯ  ");
	gtk_table_attach_defaults (GTK_TABLE (table), button, 0, 1, 2, 3);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(click_button_line), &status);
    gtk_widget_show (button);
    	
    button = gtk_button_new_with_label ("УДАЛЕНИЕ");
	gtk_table_attach_defaults (GTK_TABLE (table), button, 1, 2, 2, 3);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(click_button_del), &status);
    gtk_widget_show (button);
    	
    button = gtk_button_new_with_label ("СТАРТ/СТОП");
	gtk_table_attach_defaults (GTK_TABLE (table), button, 0, 2, 3, 4);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(click_button_st_sp), &status);
    gtk_widget_show (button);

    button = gtk_button_new_with_label ("Назад");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 0, 2, 4, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(click_button_back), &status);
    gtk_widget_show (button);

    label = gtk_label_new("Входы:");
    gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 5, 6);
    gtk_widget_show (label);

    adj = (GtkAdjustment *)gtk_adjustment_new (enters, 1.0, 16.0, 1.0, 5.0, 0.0);
    spinner = gtk_spin_button_new (adj, 0, 0);
    gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), TRUE);
    gtk_table_attach_defaults (GTK_TABLE (table), spinner, 1, 2, 5, 6);
    g_signal_connect (G_OBJECT (spinner), "value_changed", G_CALLBACK (enter_changed), &status);
    gtk_widget_show (spinner);



    label = gtk_label_new("Выходы:");
    gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 6, 7);
    gtk_widget_show (label);

    adj = (GtkAdjustment *)gtk_adjustment_new (exits, 1.0, 16.0, 1.0, 5.0, 0.0);
    spinner = gtk_spin_button_new (adj, 0, 0);
    gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), TRUE);
    gtk_table_attach_defaults (GTK_TABLE (table), spinner, 1, 2, 6, 7);
    g_signal_connect (G_OBJECT (spinner), "value_changed", G_CALLBACK (exits_changed), &status);
    gtk_widget_show (spinner);


    label = gtk_label_new("Размеры:");
    gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 7, 8);
    gtk_widget_show (label);

    adj = (GtkAdjustment *)gtk_adjustment_new (20, 1.0, 40.0, 1.0, 5.0, 0.0);
    spinner = gtk_spin_button_new (adj, 0, 0);
    gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), TRUE);
    gtk_table_attach_defaults (GTK_TABLE (table), spinner, 1, 2, 7, 8);
    g_signal_connect (G_OBJECT (spinner), "value_changed", G_CALLBACK (size_changed), &status);
    gtk_widget_show (spinner);

    adj = (GtkAdjustment *)gtk_adjustment_new (enters, 1.0, 16.0, 1.0, 5.0, 0.0);
    spinner = gtk_spin_button_new (adj, 0, 0);
    gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), TRUE);
    gtk_table_attach_defaults (GTK_TABLE (table), spinner, 1, 2, 8, 9);
    g_signal_connect (G_OBJECT (spinner), "value_changed", G_CALLBACK (main_enter_changed), &status);
    gtk_widget_show (spinner);



    label = gtk_label_new("Главные входы:");
    gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 8, 9);
    gtk_widget_show (label);

    adj = (GtkAdjustment *)gtk_adjustment_new (exits, 1.0, 16.0, 1.0, 5.0, 0.0);
    spinner = gtk_spin_button_new (adj, 0, 0);
    gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner), TRUE);
    gtk_table_attach_defaults (GTK_TABLE (table), spinner, 1, 2, 9, 10);
    g_signal_connect (G_OBJECT (spinner), "value_changed", G_CALLBACK (main_exit_changed), &status);
    gtk_widget_show (spinner);

    label = gtk_label_new("Главные выходы:");
    gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 9, 10);
    gtk_widget_show (label);

    button = gtk_radio_button_new_with_label (NULL, "По функции");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 0, 1, 10, 11);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(click_on_func), &status);
    gtk_widget_show (button);

    group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (button));

    button = gtk_radio_button_new_with_label (group, "Пустая");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 1, 2, 10, 11);
    g_signal_connect(G_OBJECT (button), "clicked", G_CALLBACK (click_on_empty), &status);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), true);
    gtk_widget_show (button);

    g_timeout_add(100, (GSourceFunc) time_handler, (gpointer) window);
    gtk_widget_show (table);
	gtk_widget_show(window);
	gtk_main();
	return 0;
}



