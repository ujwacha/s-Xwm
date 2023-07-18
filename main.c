#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

static int (*xerrorxlib)(Display *, XErrorEvent *);

Display* display;

int check_for_another_window_manager(Display* display) {
  

  return 0;
}

int run(Display *display) { return 0; }


void die(char *error_message) {
  fprintf(stderr, "%s", error_message);
}

int xerrorstart(Display *display, XErrorEvent* error) {
  
	die("dwm: another window manager is already running");
	return -1;
}


int xerror(Display *display, XErrorEvent *error) {
  
	die("dwm: another window manager is already running");
	return -1;
}



void
checkotherwm(void)
{
	xerrorxlib = XSetErrorHandler(xerrorstart);
	/* this causes an error if some other window manager is running */
	XSelectInput(display, DefaultRootWindow(display), SubstructureRedirectMask);
	XSync(display, False);
	XSetErrorHandler(xerror);
	XSync(display, False);
}




int main() {
  display =  XOpenDisplay(NULL);

  
  if (display == NULL) {
    printf("Failled to open display\n");
  } else {
    printf("Sucessfully opened display\n");
  }

  Window rootwin = DefaultRootWindow(display);

  printf("Default root window created\n");
  
  printf("Closing the display\n");
  int value = XCloseDisplay(display);

  printf("the return int was %i\n", value);
  
}
