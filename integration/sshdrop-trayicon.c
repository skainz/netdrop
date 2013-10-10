#include <gtk/gtk.h>
#include <libnotify/notify.h>
#include <stdio.h>
#include <stdlib.h>
#include <gio/gio.h>
#include <string.h>
#include <unistd.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include <canberra-gtk.h>

#include <qrencode.h>

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

GtkWindow *qr_window;
GtkWidget *qr_img;
GtkWidget *event_box;
GdkPixbuf *bild;
ca_context *context=NULL;

void setblock(GdkPixbuf *bild,int y,int x,int red,int green,int blue,int dotsize)
{
  
  guchar *p;
  
  int n_channels = gdk_pixbuf_get_n_channels (bild);
  guchar * pixels=gdk_pixbuf_get_pixels(bild);
  int rowstride = gdk_pixbuf_get_rowstride (bild);
  
  int a,b;
  

  
  for (b=x*dotsize;b<(x*dotsize)+dotsize;b++)
    for (a=y*dotsize;a<(y*dotsize)+dotsize;a++)
      {
	p=pixels+b*rowstride+a*n_channels;
	p[0]=red;
	p[1]=green;
	p[2]=blue;
      }
}

GdkPixbuf* init_qr(int dotsize,char* content)
{
  QRcode* qr=NULL;
  int i,j;

  qr=QRcode_encodeString8bit(content,0,QR_ECLEVEL_H);
  
  // Size of QR code image needs to be qr->width+2 to paint a border around.
  // This helps cameras to find the code easier.
  bild=gdk_pixbuf_new(GDK_COLORSPACE_RGB,FALSE,8,(qr->width+2)*dotsize,(qr->width+2)*dotsize );
  
  for (i=0;i<qr->width+2;i++)
    {
      setblock(bild,i,0,255,255,255,dotsize);
    }
  
  for (i=0;i<qr->width;i++)
    {
      setblock(bild,0,i+1,255,255,255,dotsize);
      for (j = qr->width - 1; j >= 0; j--) 
	{
	  if ((qr->data[j*qr->width + i] & 0x1) == 0)
	    {
	      setblock(bild,qr->width-j,i+1,255,255,255,dotsize);
	    } 
	  else
	    {
	      setblock(bild,qr->width-j,i+1,0,0,0,dotsize);
	    }
	}
      setblock(bild,qr->width+1,i+1,255,255,255,dotsize);
    }

  for (i=0;i<qr->width+2;i++)
    {
      setblock(bild,i,qr->width+1,255,255,255,dotsize);
    }
  
    free(qr);
 
  return bild;

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

static void click_qr_callback()
{
  gtk_widget_destroy(GTK_WIDGET(qr_window));
}

static void notif_libnotify_callback_qrcode ( NotifyNotification *n, gchar *action, gpointer user_data ) {

  GtkClipboard* cb=gtk_clipboard_get(GDK_SELECTION_PRIMARY);
  event_box=gtk_event_box_new();
  
 
  bild=init_qr(16,data);


  qr_img=gtk_image_new_from_pixbuf(bild);
  qr_window = GTK_WINDOW(gtk_window_new (GTK_WINDOW_TOPLEVEL));
  gtk_window_set_resizable(qr_window,FALSE);
  gtk_window_set_title(qr_window,"Click to close.");

  gtk_container_add(GTK_CONTAINER(event_box),qr_img);
  gtk_container_add (GTK_CONTAINER(qr_window), event_box);
  
  
  gtk_widget_show (event_box);
  gtk_widget_show (qr_img);
  gtk_window_present (qr_window);
  
  g_signal_connect(G_OBJECT(event_box),"button_press_event", G_CALLBACK(click_qr_callback),  qr_img);


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

void callback (GFileMonitor *mon, GFile *first, GFile *second, GFileMonitorEvent event, gpointer udata)
{
  if (event == G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT)
    {

      //Need this to make the popup displayed for sure
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

	  int retval=ca_context_play(context, 0, CA_PROP_MEDIA_FILENAME, "/usr/share/sounds/freedesktop/stereo/complete.oga", NULL);
	  printf ("soundplay %d:%s\n",retval,ca_strerror(retval));


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



void tray_icon_on_click(GtkStatusIcon *status_icon, gpointer user_data)
{
  
  gtk_clipboard_request_text (gtk_clipboard_get (GDK_SELECTION_PRIMARY), global_text_receive, NULL);
  
}

static void trayIconPopup(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu)
{
    gtk_menu_popup(GTK_MENU(popUpMenu), NULL, NULL, gtk_status_icon_position_menu, status_icon, button, activate_time);
}




void init_canberra()
{
  printf("Context create: %d\n", ca_context_create(&context));
  printf("Context props: %d\n", ca_context_change_props(context, CA_PROP_CANBERRA_XDG_THEME_NAME, "theme-name", NULL));

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



   
  file=g_file_new_for_path(argv[1]);
  mon=g_file_monitor(file,G_FILE_MONITOR_EVENT_CHANGED,NULL,NULL);

  g_signal_connect (mon, "changed", G_CALLBACK (callback), NULL);
  

    // initialize gtk
    gtk_init(&argc,&argv);
    g_type_init(); 
    char* name = "SSHDrop";
    
    // initiate notify
    notify_init(name);
    
    // create a new notification
   
    example = notify_notification_new(name,"",NULL);
    
    
    addIcon(example);
    notify_notification_add_action(example,"copy","Copy to Clipboard",(NotifyActionCallback)notif_libnotify_callback_open,name,NULL);
    notify_notification_add_action(example,"copy","QR-Code",(NotifyActionCallback)notif_libnotify_callback_qrcode,name,NULL);


    g_signal_connect(example,"closed",G_CALLBACK (status_icon_notification_closed_cb),NULL);
    

    // set the timeout of the notification to the default value
    notify_notification_set_timeout(example,NOTIFY_EXPIRES_DEFAULT);
    
    // set the category so as to tell what kind it is
    char* category = "Testing Notifications";
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
    
    
   

    init_canberra();
    
    
    
    
    gtk_main();
}

