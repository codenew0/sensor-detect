#include <gtk/gtk.h>
#include "apue.h"

#define WARNING_TEMPERATURE 95

int flag = 0;

extern void *play_sound (void *filename);

int get_temper (const char *shell){

	FILE *stream; 
	char buf[1024], temper[4];
	int i, f, j, k = 0;

	memset (buf, 0, sizeof(buf));
	memset (temper, 0, sizeof(temper));

	stream = popen (shell , "r");
	fread (buf, sizeof(char), sizeof(buf), stream);

	pclose (stream); 

	for (i = 0; i < strlen (buf); i++)
	{
		if (buf[i] == '+')
		{
			f = i;	//The next character is the first number
		}
		if (buf[i] == '.')
		{
			break; // The next character is the last number
		}
	}

	for (j = f + 1; j < i; j++)
	{
		temper[k] = buf[j];
		k++;
	}

	int num = atoi (temper);
	
	return num;
}

static gboolean temper_show (gpointer data)
{
	GtkWidget *tray_icon = (GtkWidget *)data;

	int temper = get_temper ("sensors");
	char temper_str[32];
	memset (temper_str, 0, sizeof(temper_str));
	
	sprintf (temper_str, "Current temperature: %d°C", temper);

	gtk_status_icon_set_tooltip_text (GTK_STATUS_ICON (tray_icon), temper_str); 

	if (temper > 50 && flag == 0)
	{
		flag = 1;
		pthread_t tid;
		pthread_create (&tid, NULL, play_sound, (void*)"bell.wav");
	}
	else if (temper <= 50)
	{
		flag = 0;
	}

	return TRUE;
}

static gint tray_icon_clicked (GtkWidget *widget, GdkEvent *event)
{
	GdkEventButton *event_button;

	if (event->type == GDK_BUTTON_PRESS)
	{
		event_button = (GdkEventButton *) event;
		if (event_button->button == GDK_BUTTON_SECONDARY)
		{
			gtk_main_quit ();
			return TRUE;
		}
	}

	return FALSE;
}

/**************************************
  Main window
 **************************************/
int main (int argc, char *argv[])
{
	gtk_init (&argc, &argv);

	//Tray icon
	GtkWidget *tray_icon;
	tray_icon = gtk_status_icon_new_from_file ("icon.jpg");
	gtk_status_icon_set_tooltip_text (GTK_STATUS_ICON (tray_icon), "Timer");
	
	g_signal_connect (tray_icon, "button_press_event", G_CALLBACK (tray_icon_clicked), NULL);

	g_timeout_add (1000, (GSourceFunc)temper_show, tray_icon);

	gtk_main ();

	return 0;
}
