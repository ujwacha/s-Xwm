#include <X11/X.h>
#include <X11/Xproto.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xutil.h>

static int (*xerrorxlib)(Display *, XErrorEvent *);

void loop();


void OnCreateNotify(XEvent *e);
void OnDestroyNotify(XEvent *e);
void OnReparentNotify(XEvent *e);
void OnMapNotify(XEvent *e);
void OnUnmapNotify(XEvent *e);
void OnConfigureNotify(XEvent *e);
void OnMapRequest(XEvent *e);
void OnConfigureRequest(XEvent *e);
void OnButtonPress(XEvent *e);
void OnButtonRelease(XEvent *e);
void OnMotionNotify(XEvent *e);
void OnKeyPress(XEvent *e);
void OnKeyRelease(XEvent *e);





void add_keybindings(Window w) ;
/* static void (*handler[LastEvent]) (XEvent *) = { */
/*   [ButtonPress] = OnButtonPress, */
/*   [ClientMessage] = clientmessage, */
/*   [ConfigureRequest] = configurerequest, */
/*   [ConfigureNotify] = configurenotify, */
/*   [DestroyNotify] = destroynotify, */
/*   [EnterNotify] = enternotify, */
/*   [Expose] = expose, */
/*   [FocusIn] = focusin, */
/*   [KeyPress] = keypress, */
/*   [MappingNotify] = mappingnotify, */
/*   [MapRequest] = maprequest, */
/*   [MotionNotify] = motionnotify, */
/*   [PropertyNotify] = propertynotify, */
/*   [UnmapNotify] = unmapnotify */
/* } */




Window root;


typedef struct client{
  Window w;
  struct client *next;

} Client ;


int exists(Client c, Window wi) {
  Client *next = c.next;

  if (c.w == wi) return 1;

  if (next == NULL) return 0;

  return exists(*next, wi);
}


int client_count(Client c) {
  if (c.next == NULL){
    return 1;
  } else {
    return 1 + client_count(*c.next);
  }
}

int add_new_window(Client *cl, Window wi) {

  Client *next = cl->next;

  if (next != NULL) return add_new_window(next, wi);

  Client *newc = malloc(sizeof(Client));

  if (newc == NULL) return -1; // return null if malloc fails 
  
  newc->w = wi;

  cl->next = newc;

  return 0;
}

Client *c;


Display* display;


void frame(Window w, int already_here);

void die(char *error_message) {
  fprintf(stderr, "%s", error_message);
  exit(-1);
}

int xerrorstart(Display *display, XErrorEvent* error) {
  
  die("dwm: another window manager is already running");
  return -1;
}


int xerror(Display *display, XErrorEvent *ee) {

  
  if (ee->error_code == BadWindow
      || (ee->request_code == X_SetInputFocus && ee->error_code == BadMatch)
      || (ee->request_code == X_PolyText8 && ee->error_code == BadDrawable)
      || (ee->request_code == X_PolyFillRectangle && ee->error_code == BadDrawable)
      || (ee->request_code == X_PolySegment && ee->error_code == BadDrawable)
      || (ee->request_code == X_ConfigureWindow && ee->error_code == BadMatch)
      || (ee->request_code == X_GrabButton && ee->error_code == BadAccess)
      || (ee->request_code == X_GrabKey && ee->error_code == BadAccess)
      || (ee->request_code == X_CopyArea && ee->error_code == BadDrawable))
    return 0;

  fprintf(stderr, "dwm: fatal error: request code=%d, error code=%d\n",
	  ee->request_code, ee->error_code);
  
  
  return xerrorxlib(display, ee); /* may call exit */
}




void checkotherwm(void)
{
  xerrorxlib = XSetErrorHandler(xerrorstart);
  /* this causes an error if some other window manager is running */
  XSelectInput(display, DefaultRootWindow(display), SubstructureRedirectMask);
  XSync(display, False);
  XSetErrorHandler(xerror);
  XSync(display, False);
}


void run() {
  // grab the display connection
  XGrabServer(display);

  Window returned_root, returned_parent;
  Window* top_level_windows;
  unsigned int num_top_level_windows;

  // If XQueryTree sucessfully runs, do the thing in the if block
  if(XQueryTree(
	     display,
	     root,
	     &returned_root,
	     &returned_parent,
	     &top_level_windows,
	     &num_top_level_windows))
    {

      for (int i = 0 ; i < num_top_level_windows ; ++i) {

	frame(top_level_windows[i], 1);
	
      }
      
    }





  loop();

  
}





void loop() {
    // 2. Main event loop.
  while (1) {
    // 1. Get next event.
    XEvent e;
    XNextEvent(display, &e);

    // 2. Dispatch event.
    switch (e.type) {
      case CreateNotify:
        OnCreateNotify(&e);
        break;
      case DestroyNotify:
        OnDestroyNotify(&e);
        break;
      case ReparentNotify:
        OnReparentNotify(&e);
        break;
      case MapNotify:
        OnMapNotify(&e);
        break;
      case UnmapNotify:
        OnUnmapNotify(&e);
        break;
      case ConfigureNotify:
        OnConfigureNotify(&e);
        break;
      case MapRequest:
        OnMapRequest(&e);
        break;
      case ConfigureRequest:
        OnConfigureRequest(&e);
        break;
      case ButtonPress:
        OnButtonPress(&e);
        break;
      case ButtonRelease:
        OnButtonRelease(&e);
        break;
      case MotionNotify:
        // Skip any already pending motion events.
        while (XCheckTypedWindowEvent(display, e.xmotion.window, MotionNotify, &e)) {}
        OnMotionNotify(&e);
        break;
      case KeyPress:
        OnKeyPress(&e);
        break;
      case KeyRelease:
        OnKeyRelease(&e);
        break;
      default:
	printf("Ignored XEvent\n");
    }
  }
}


// do nothing in the following cases
void OnCreateNotify(XEvent *e) {}
void OnDestroyNotify(XEvent *e) {}
void OnReparentNotify(XEvent *e) {}
void OnMapNotify(XEvent *e) {}
void OnUnmapNotify(XEvent *e) {}
















void frame(Window w, int already_here) {

  const unsigned int BORDER_WIDTH = 3;
  const unsigned long BORDER_COLOR = 0xff0000;
  const unsigned long BG_COLOR = 0x0000ff;

  if (exists(*c, w) == 0) {
    return ;
  }


  XWindowAttributes x_window_attrs;
  if (!XGetWindowAttributes(display, w, &x_window_attrs)) {
    return;
  }



  if (already_here == 1) {
    if (x_window_attrs.override_redirect || x_window_attrs.map_state == IsViewable) {
      return;
    }
  }


    // 3. Create frame.
  Window frame = XCreateSimpleWindow(
      display,
      root,
      x_window_attrs.x,
      x_window_attrs.y,
      x_window_attrs.width,
      x_window_attrs.height,
      BORDER_WIDTH,
      BORDER_COLOR,
      BG_COLOR);
  // 4. Select events on frame.
  XSelectInput(
      display,
      frame,
      SubstructureRedirectMask | SubstructureNotifyMask);
  // 5. Add client to save set, so that it will be restored and kept alive if we
  // crash.
  XAddToSaveSet(display, w);
  // 6. Reparent client window.
  XReparentWindow(
      display,
      w,
      frame,
      0, 0);  // Offset of client window within frame.
  // 7. Map frame.
  XMapWindow(display, frame);
  // 8. Save frame handle.

  add_new_window(c, frame);

  add_keybindings(frame);
}


void add_keybindings(Window w) {


  XGrabButton(
      display,
      Button1,
      Mod1Mask,
      w,
      0,
      ButtonPressMask | ButtonReleaseMask | ButtonMotionMask,
      GrabModeAsync,
      GrabModeAsync,
      None,
      None);
  //   b. Resize windows with alt + right button.
  XGrabButton(
      display,
      Button3,
      Mod1Mask,
      w,
      0,
      ButtonPressMask | ButtonReleaseMask | ButtonMotionMask,
      GrabModeAsync,
      GrabModeAsync,
      None,
      None);
  //   c. Kill windows with alt + f4.
  XGrabKey(
      display,
      XKeysymToKeycode(display, XK_F4),
      Mod1Mask,
      w,
      0,
      GrabModeAsync,
      GrabModeAsync);
  //   d. Switch windows with alt + tab.
  XGrabKey(
      display,
      XKeysymToKeycode(display, XK_Tab),
      Mod1Mask,
      w,
      0,
      GrabModeAsync,
      GrabModeAsync);

}


int main() {
  display =  XOpenDisplay(NULL);

  
  if (display == NULL) {
    printf("Failled to open display\n");
  } else {
    printf("Sucessfully opened display\n");
  }

  checkotherwm();

  root = DefaultRootWindow(display);


  run();
  
  
  printf("Closing the display\n");
  int value = XCloseDisplay(display);

  printf("the return int was %i\n", value);
  
}
