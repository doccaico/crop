#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define WIDTH 200
#define HEIGHT 200
#define FILENAME "a.png"
#define SCREENSHOT "import"
#define VIEW "display"


char out[64];
char cmd[128];

typedef struct {
  int x;
  int y;
} Cursor_Pos;

typedef struct {
  int w;
  int h;
} Screen_Size;


void set_filename(void) {
  sprintf(out,  "%s/Desktop/" FILENAME, getenv("HOME"));
}

void screenshot(Cursor_Pos pos1, Cursor_Pos pos2) {
  int x = pos1.x;
  int y = pos1.y;
  int width = pos2.x - x;
  int height = pos2.y - y;
  sprintf(cmd, SCREENSHOT " -window root -crop %dx%d+%d+%d %s",
      width, height, x, y, out);
  // printf("cmd length = %d\n", strlen(cmd)); puts(cmd);
  system(cmd);
}

void view() {
  sprintf(cmd, VIEW " %s", out);
  snprintf(cmd, sizeof(cmd), "%s %s", VIEW, out);
  // printf("cmd length = %d\n", strlen(cmd)); puts(cmd);
  system(cmd);
}

void imgur() {
  sprintf(cmd, "imgur_upload.py %s", out);
  // printf("cmd length = %d\n", strlen(cmd)); puts(cmd);
  system(cmd);
}

void update_cursor_pos(Display *dpy, XEvent event, Cursor_Pos *cursor_pos) {

  if (!XQueryPointer(dpy, RootWindow(dpy, DefaultScreen(dpy)),
        &event.xbutton.root, &event.xbutton.window,
        &event.xbutton.x_root, &event.xbutton.y_root,
        &event.xbutton.x, &event.xbutton.y,
        &event.xbutton.state)) {
    exit(1);
  }
  cursor_pos->x = event.xbutton.x_root;
  cursor_pos->y = event.xbutton.y_root;
}

void set_screen_size(Display *dpy, Screen_Size *screen_size) {
  Screen *sc = ScreenOfDisplay(dpy, 0);
  screen_size->w = sc->width;
  screen_size->h = sc->height;
}

void cmd_check(void) {
  char *notfound = NULL;
  if (system("which " SCREENSHOT " > /dev/null 2>&1")) {
    notfound = SCREENSHOT;
  } else if (system("which " VIEW " > /dev/null 2>&1")) {
    notfound = VIEW;
  }
  if (notfound) {
    fprintf(stderr, "`%s` is required but it was not found\n", notfound);
    exit(1);
  }
}

int main(int argc, char **argv) {

  cmd_check();

  Display *dpy;
  Window window;
  XEvent event = {0};
  int screen;
  Cursor_Pos cursor_pos1 = {0, 0};
  Cursor_Pos cursor_pos2 = {0, 0};
  Screen_Size screen_size = {0, 0};

  set_filename();

  // open connection with the server
  dpy = XOpenDisplay(NULL);
  if (!dpy) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }

  screen = DefaultScreen(dpy);

  set_screen_size(dpy, &screen_size);

  // want to position the window at the bottom right
  XSizeHints    hints = {0};
  hints.flags  = PPosition | PSize;
  hints.x      = screen_size.w - WIDTH;
  hints.y      = screen_size.h - HEIGHT;
  hints.width  = WIDTH;
  hints.height = HEIGHT;

  // create the window
  window = XCreateSimpleWindow(dpy, RootWindow(dpy, screen), hints.x, hints.y, hints.width, hints.height, 1,
      BlackPixel(dpy, screen), WhitePixel(dpy, screen));


  // set position the window
  XSetNormalHints(dpy, window, &hints);

  // select kind of events we are interested in
  XSelectInput(dpy, window, KeyPressMask);

  // show the window
  XMapWindow(dpy, window);


  bool finished = false;
  while (!finished) {

    XNextEvent(dpy, &event);

    if (event.type == KeyPress) {
      switch (event.xkey.keycode) {
        case 0x9: // 'ESC'
          finished = true;
          break;
        case 0xa: // '1'
          update_cursor_pos(dpy, event, &cursor_pos1);
          break;
        case 0xb: // '2'
          update_cursor_pos(dpy, event, &cursor_pos2);
          break;
        case 0xc: // '3'
          screenshot(cursor_pos1, cursor_pos2);
          break;
        case 0x1f: // 'i'
          imgur();
          break;
        case 0x37: // 'v'
          view();
          break;
        // default:
        //   printf("keycode: %x\n", event.xkey.keycode);
        //   break;
      }
    }
  }

  // close connection to server
  XCloseDisplay(dpy);

  return 0;
}
