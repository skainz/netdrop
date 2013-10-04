#include <gtk/gtk.h>
#include <libnotify/notify.h>
#include <stdio.h>
#include <stdlib.h>
#include <gio/gio.h>
#include <string.h>
#include <unistd.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

NotifyNotification *example;
//int counter=0;
//char info[256];
char preview[256];

FILE* fp;
char* filename;
long file_size;
char* data;
char* sender;

GtkWidget *menu, *menuItemView, *menuItemExit, *sep;
const char *global_text;
char *send_data;


void init_qr()
{
GdkPixbuf* bild=gdk_pixbuf_new(GDK_COLORSPACE_RGB,FALSE,8,400,400 );
 printf("%d\n",bild);

}

static void
put_pixel (GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue, guchar alpha)
{
  int width, height, rowstride, n_channels;
  guchar *pixels, *p;

  n_channels = gdk_pixbuf_get_n_channels (pixbuf);

  g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
  g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);
  g_assert (gdk_pixbuf_get_has_alpha (pixbuf));
  g_assert (n_channels == 4);

  width = gdk_pixbuf_get_width (pixbuf);
  height = gdk_pixbuf_get_height (pixbuf);

  g_assert (x >= 0 && x < width);
  g_assert (y >= 0 && y < height);

  rowstride = gdk_pixbuf_get_rowstride (pixbuf);
  pixels = gdk_pixbuf_get_pixels (pixbuf);

  p = pixels + y * rowstride + x * n_channels;
  p[0] = red;
  p[1] = green;
  p[2] = blue;
  p[3] = alpha;
}


static void trayExit(GtkMenuItem *item, gpointer user_data)
{
  
    printf("Shutting Down SSHDrop.\n");
    gtk_main_quit();
}


int setup_menu()
{
  menu = gtk_menu_new();
  menuItemView = gtk_menu_item_new_with_label ("SSHDrop");
  sep = gtk_separator_menu_item_new();
  menuItemExit = gtk_menu_item_new_with_label ("Exit");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuItemView);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), sep);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuItemExit);
  g_signal_connect (G_OBJECT (menuItemExit), "activate", G_CALLBACK (trayExit), NULL);
  gtk_widget_show_all (menu);

}


void addIcon( NotifyNotification * notify )
{
    int size = 32;
    GtkIconTheme * theme;
    GdkPixbuf * icon;

    gtk_icon_size_lookup( GTK_ICON_SIZE_DIALOG, &size, &size );
    theme = gtk_icon_theme_get_default( );
         icon = gtk_icon_theme_load_icon( theme, "edit-paste", size, 0, NULL );
 
   if( icon != NULL )
    {
        notify_notification_set_icon_from_pixbuf( notify, icon );
        g_object_unref( icon );
    }
}




static void notif_libnotify_callback_open ( NotifyNotification *n, gchar *action, gpointer user_data ) {

  FILE *pf;
  GtkClipboard* cb=gtk_clipboard_get(GDK_SELECTION_PRIMARY);

  gtk_clipboard_set_text(cb,data,-1);
  
  free(data);
  data=NULL;

}





void status_icon_notification_closed_cb (NotifyNotification *notification,
				    gpointer  icon)
{

  printf ("Close reason:%d\n",notify_notification_get_closed_reason(notification));

  printf("popup closed\n");
}

void
callback (GFileMonitor *mon, GFile *first, GFile *second, GFileMonitorEvent event, gpointer udata)
{
  if (event == G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT)
    {

 notify_notification_close(example,NULL);


  printf ("file changed %s\n",g_file_get_path(first));

  //  printf ("event: %d\n",event);

  sender=g_file_get_basename(first);
  fp=fopen(g_file_get_path(first),"r");
  if (!(fp==(FILE*)NULL))
    {
      fseek(fp,0,SEEK_END);
      file_size=ftell(fp);
      printf("filesize: %d\n",file_size);
      fseek(fp,0,SEEK_SET);
      data=malloc(sizeof(char)*file_size+1);
      data[file_size]=0;
      fread(data,1,file_size,fp);
      fclose(fp);
      
      if (data[file_size-1]=='\n')
	{
	  data[file_size-1]=0;
	}
      
      //   sprintf(info," %d",counter);
      GError *error = NULL;
      memcpy(preview,data,256);
      preview[255]=0;
      notify_notification_update(example,sender,preview,NULL);
      notify_notification_show(example,&error);
    } else
    {
      printf("Unable to open file %s\n",filename);
    }
 
    }

}


void global_text_receive (GtkClipboard *clipboard, const gchar *text, gpointer data)
{
  //  printf("global text_receive\n");
        global_text = text;

	FILE *output;
	output=popen("sshdrop -","w");
	if (!output)
	  {
	    printf("Error opening subprocess!!\n");
	  }
	printf ("cb content:%s\n",global_text);
	fprintf(output,"%s\n",global_text);
	int retval=pclose(output);

	printf("Exit code: %d\n",retval);
	
} 


int ssh_socket_present()
{

  char socketfile[512];
  snprintf(socketfile,512,"%s/.sshdrop/ssh_socket",getenv("HOME"));
  printf ("Socket fn:%s\n",socketfile);
  return access(socketfile,F_OK);

}

void tray_icon_on_click(GtkStatusIcon *status_icon, gpointer user_data)
{
  
  gtk_clipboard_request_text (gtk_clipboard_get (GDK_SELECTION_PRIMARY), global_text_receive, NULL);
  
}

static void trayIconPopup(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu)
{
    gtk_menu_popup(GTK_MENU(popUpMenu), NULL, NULL, gtk_status_icon_position_menu, status_icon, button, activate_time);
}



int main(int argc, char **argv)
{

  GFile *file;
  GFileMonitor *mon;

 g_type_init(); 
  if (argc<2)
    {
      printf ("Usage: %s <directory>\n",argv[0]);
      exit(1);
    }


  init_qr();
   
  file=g_file_new_for_path(argv[1]);
  mon=g_file_monitor(file,G_FILE_MONITOR_EVENT_CHANGED,NULL,NULL);

  g_signal_connect (mon, "changed", G_CALLBACK (callback), NULL);
  

    // initialize gtk
    gtk_init(&argc,&argv);
    g_type_init(); 
    char name[40] = "SSHDrop";
    
    // initiate notify
    notify_init(name);
    
    // create a new notification
   
    example = notify_notification_new(name,"",NULL);
    
      
    addIcon(example);
    notify_notification_add_action(example,"copy","Copy to Clipboard",(NotifyActionCallback)notif_libnotify_callback_open,name,NULL);

    g_signal_connect(example,"closed",G_CALLBACK (status_icon_notification_closed_cb),NULL);


    // set the timeout of the notification to the default value
    notify_notification_set_timeout(example,NOTIFY_EXPIRES_DEFAULT);
    
    // set the category so as to tell what kind it is
    char category[30] = "Testing Notifications";
    notify_notification_set_category(example,category);
    
    // set the urgency level of the notification
    notify_notification_set_urgency (example,NOTIFY_URGENCY_NORMAL);
    
    GtkStatusIcon *status=gtk_status_icon_new_from_stock(GTK_STOCK_PASTE);
    gtk_status_icon_set_tooltip_text(status, "SSHDrop - Click to post clipboard data");
    
    
    setup_menu();
    
    g_signal_connect(G_OBJECT(status), "activate",
		     G_CALLBACK(tray_icon_on_click), NULL);
    
    g_signal_connect(G_OBJECT(status),
		     "popup-menu",
		     G_CALLBACK(trayIconPopup), menu);
    
    
    gtk_main();
}

