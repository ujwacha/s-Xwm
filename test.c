#include <stdio.h>

int border_pixel = 5;
int scrwidth = 1366;
int scrheight = 697;
int barheight = 22;

void plot(int posx, int posy, int width, int height)
{
  // position of opposite pixel of window
  printf("%d        %d        %d           %d\n", posx, posy, width, height);
  int oposx = posx + width;
  int oposy = posy + height;
  if (posx == 0)
  {
    posx += border_pixel;
    width -= border_pixel;
  }
  if (posy == 0)
  {
    posy += border_pixel;
    height -= border_pixel;
  }
  if (oposx > scrwidth - 5 && oposx < scrwidth + 5)
    width -= border_pixel;
  if (oposy > scrheight - barheight - 5 && oposy < scrheight - barheight + 5)
  {
    height -= border_pixel;
  }
  posx += border_pixel;
  posy += border_pixel;
  width -= (2 * border_pixel);
  height -= (2 * border_pixel);
  printf("%d        %d        %d           %d\n", posx, posy, width, height);
}

void main()
{
  plot(0, 0, 683, 675);
  plot(683, 0, 683, 337);
  plot(683, 337, 683, 337);
}