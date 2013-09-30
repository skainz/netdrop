#include<gtk/gtk.h>
#include<libnotify/notify.h>
#include<stdio.h>
#include <stdlib.h>


NotifyNotification *example;
//int counter=0;
//char info[256];
FILE* fp;
char* filename;
long file_size;
char * data;

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


void sigusr_handler(int sig)
{
  //     write(0, "Ahhh! SIGUSR!\n", 14);

      notify_notification_close(example,NULL);

      fp=fopen(filename,"r");
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
      

      //   sprintf(info," %d",counter);
      GError *error = NULL;
      notify_notification_update(example,"SSHDrop",data,NULL);
      notify_notification_show(example,&error);
	} else
	{
	  printf("Unable to open file %s\n",filename);
	}
  


}



static void notif_libnotify_callback_open ( NotifyNotification *n, gchar *action, gpointer user_data ) {

  FILE *pf;
  char command[256];

 
  GtkClipboard* cb=gtk_clipboard_get(GDK_SELECTION_PRIMARY);

  gtk_clipboard_set_text(cb,data,-1);

  printf ("%s\n",user_data);
 
 

}


void status_icon_notification_closed_cb (NotifyNotification *notification,
				    gpointer  icon)
{

  printf ("Close reason:%d\n",notify_notification_get_closed_reason(notification));
  // GtkClipboard* cb=gtk_clipboard_get(GDK_SELECTION_PRIMARY);

 //  gtk_clipboard_set_text(cb,"foobar",-1);

  printf("popup closed\n");
  // gtk_clipboard_store(gtk_clipboard_get(GDK_SELECTION_CLIPBOARD));
  //      gtk_main_quit();


  //free(data);
}



int main(int argc, char **argv)
{

  if (argc<2)
    {
      printf ("Usage: %s <filename>\n",argv[0]);
      exit(1);
    }

  filename=argv[1];
  printf("Using file %s\n",argv[1]);

  
void sigint_handler(int sig);
//    char s[200];
    struct sigaction sa;

    sa.sa_handler = sigusr_handler;
    sa.sa_flags = 0; // or SA_RESTART
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }


    // initialize gtk
    gtk_init(&argc,&argv);
    
    char name[40] = "SSHDrop";
    
    // initiate notify
    notify_init(name);
    
    // create a new notification
   
    example = notify_notification_new(name,"",NULL);
    
      
    addIcon(example);
    notify_notification_add_action(example,"copy","Copy to Clipboard",(NotifyActionCallback)notif_libnotify_callback_open,name,NULL);

    g_signal_connect(example,"closed",G_CALLBACK (status_icon_notification_closed_cb),NULL);
    //    g_signal_connect(example,"timed-out",G_CALLBACK (status_icon_notification_closed_cb),NULL);


    //        notify_notification_add_action(example,"paste","Open Feed",(NotifyActionCallback)notif_libnotify_callback_open,name,NULL);
    // notify_notification_add_action(example,"open","Open Feed",(NotifyActionCallback)notif_libnotify_callback_open,NULL,NULL);


    // set the timeout of the notification to the default value
    notify_notification_set_timeout(example,NOTIFY_EXPIRES_DEFAULT);
    
    // set the category so as to tell what kind it is
    char category[30] = "Testing Notifications";
    notify_notification_set_category(example,category);
    
    // set the urgency level of the notification
    notify_notification_set_urgency (example,NOTIFY_URGENCY_CRITICAL);
    
    //    GError *error = NULL;
    //    notify_notification_show(example,&error);



    gtk_main();
}


 // gtk_main_quit();
//http://www.youtube.com/watch?v=yX8yrOAjfKM
