#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

static void destroyWindowCb(GtkWidget *widget, GtkWidget *window);
static gboolean closeWebViewCb(WebKitWebView *webView, GtkWidget *window);

int main(int argc, char *argv[])
{
	// Initialize GTK+
	gtk_init(&argc, &argv);

	// Create an 800x600 window that will contain the browser instance
	GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);

	// Create a browser instance
	WebKitWebView *webView = WEBKIT_WEB_VIEW(webkit_web_view_new());

	gtk_widget_set_app_paintable(main_window, TRUE); // not sure this is needed
	const GdkRGBA color = {.0, .0, .0, .0};
	GdkScreen *screen = gtk_widget_get_screen(main_window);
	GdkVisual *visual = gdk_screen_get_rgba_visual(screen);
	gtk_widget_set_visual(main_window, visual);
	gtk_widget_set_visual(GTK_WIDGET(webView), visual);
	webkit_web_view_set_background_color(webView, &color);

	// gtk_widget_override_background_color(main_window, GTK_STATE_FLAG_NORMAL, &color);
	// gtk_widget_override_background_color(GTK_WIDGET(webView), GTK_STATE_FLAG_NORMAL, &color);
	// Put the browser area into the main window
	gtk_container_add(GTK_CONTAINER(main_window), GTK_WIDGET(webView));

	// Set up callbacks so that if either the main window or the browser instance is
	// closed, the program will exit
	g_signal_connect(main_window, "destroy", G_CALLBACK(destroyWindowCb), NULL);
	g_signal_connect(webView, "close", G_CALLBACK(closeWebViewCb), main_window);

	// Load a web page into the browser instance
	webkit_web_view_load_uri(webView, "file:///home/prakash/selfuniv/programming/gtkprograms/index.html");

	// Make sure that when the browser area becomes visible, it will get mouse
	// and keyboard events
	// gtk_widget_grab_focus(GTK_WIDGET(webView));
	gtk_window_set_decorated(GTK_WINDOW(main_window), FALSE);
	gtk_window_set_resizable(GTK_WINDOW(main_window), FALSE);
	// Make sure the main window and all its contents are visible

	gtk_widget_show_all(main_window);
	gtk_window_set_keep_below(GTK_WINDOW(main_window), TRUE);
	gtk_window_set_type_hint(GTK_WINDOW(main_window), GDK_WINDOW_TYPE_HINT_DOCK);

	//prevent the window from appearing in taskbar
	gtk_window_set_skip_taskbar_hint(GTK_WINDOW(main_window), TRUE);
	//prevent from appering in tab switch (not working)
	gtk_window_set_skip_pager_hint(GTK_WINDOW(main_window), FALSE);
	// Run the main GTK+ event loop
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
