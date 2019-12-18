#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

static void destroyWindowCb(GtkWidget *widget, GtkWidget *window);
static gboolean closeWebViewCb(WebKitWebView *webView, GtkWidget *window);

char abspath[200] = "file://";
char path[150];

int main(int argc, char *argv[])
{
	//get current working directory absolute path
	if(getcwd(path,150) == NULL) exit(1);
	strcat(path,"/index.html");	
	strcat(abspath, path);
	printf("%s\n",abspath);
	
	// Initialize GTK+
	gtk_init(&argc, &argv);
	GdkRectangle workarea = {0};
	gdk_monitor_get_workarea(gdk_display_get_primary_monitor(gdk_display_get_default()),
    &workarea);
	// printf("%d %d\n",workarea.width, workarea.height);
	// Create an 800x600 window that will contain the browser instance
	GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	// GtkWidget *main_window = gtk_window_new(GTK_WINDOW_POPUP);
	gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 300);
	
	// Create a browser instance
	WebKitWebView *webView = WEBKIT_WEB_VIEW(webkit_web_view_new());

	gtk_widget_set_app_paintable(main_window, TRUE); // not sure this is needed
	const GdkRGBA color = {.0, .0, .0, .0};
	GdkScreen *screen = gtk_widget_get_screen(main_window);
	GdkVisual *visual = gdk_screen_get_rgba_visual(screen);
	gtk_widget_set_visual(main_window, visual);
	gtk_widget_set_visual(GTK_WIDGET(webView), visual);
	webkit_web_view_set_background_color(webView, &color);
	gtk_widget_set_sensitive(GTK_WIDGET(webView),FALSE);
	// gtk_widget_set_focus_on_click(GTK_WIDGET(webView),FALSE);
	// gtk_widget_override_background_color(main_window, GTK_STATE_FLAG_NORMAL, &color);
	// gtk_widget_override_background_color(GTK_WIDGET(webView), GTK_STATE_FLAG_NORMAL, &color);
	// Put the browser area into the main window
	gtk_container_add(GTK_CONTAINER(main_window), GTK_WIDGET(webView));

	// Set up callbacks so that if either the main window or the browser instance is
	// closed, the program will exit
	g_signal_connect(main_window, "destroy", G_CALLBACK(destroyWindowCb), NULL);
	g_signal_connect(webView, "close", G_CALLBACK(closeWebViewCb), main_window);

	// Load a web page into the browser instance
	webkit_web_view_load_uri(webView, abspath);

	// Make sure that when the browser area becomes visible, it will get mouse
	// and keyboard events
	// gtk_widget_grab_focus(GTK_WIDGET(webView));
	//HERE
	gtk_window_set_decorated(GTK_WINDOW(main_window), FALSE);
	gtk_window_set_resizable(GTK_WINDOW(main_window), FALSE);
	// Make sure the main window and all its contents are visible

	gtk_widget_show_all(main_window);
	//HERE
	gtk_window_set_type_hint(GTK_WINDOW(main_window), GDK_WINDOW_TYPE_HINT_DESKTOP);

	// gtk_window_set_type_hint(GTK_WINDOW(main_window), GDK_WINDOW_TYPE_HINT_DOCK);
	// gtk_window_set_type_hint(GTK_WINDOW(main_window), GDK_WINDOW_TYPE_HINT_SPLASHSCREEN);
	

	//prevent the window from appearing in taskbar
	gtk_window_set_skip_taskbar_hint(GTK_WINDOW(main_window), TRUE);
	//prevent from appering in tab switch (not working)
	gtk_window_set_skip_pager_hint(GTK_WINDOW(main_window), FALSE);
	gtk_window_set_accept_focus(GTK_WINDOW(main_window), FALSE);
	// gtk_window_set_position(GTK_WINDOW(main_window),(GTK_WIN_POS_CENTER));
	// Run the main GTK+ event loop
	gtk_window_set_keep_below(GTK_WINDOW(main_window), TRUE);
	//HERE
	gtk_window_set_gravity(GTK_WINDOW(main_window),GDK_GRAVITY_NORTH);
	//HERE
	// gtk_window_move(GTK_WINDOW(main_window), workarea.width/2,workarea.height/2);
	gtk_window_move(GTK_WINDOW(main_window), workarea.width/2-150,workarea.height/2-100);
	// gtk_widget_unrealize(GTK_WIDGET(main_window));
	gtk_main();

	return 0;
}

static void destroyWindowCb(GtkWidget *widget, GtkWidget *window)
{
	gtk_main_quit();
}

static gboolean closeWebViewCb(WebKitWebView *webView, GtkWidget *window)
{
	gtk_widget_destroy(window);
	return TRUE;
}
