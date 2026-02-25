#include <termios.h>
#include <unistd.h>

int main(void) {
  struct termios old_t, new_t;
  tcgetattr(STDIN_FILENO, &old_t);
  new_t = old_t;
  char c;

  new_t.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_t);
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q')
    ;

  return 0;
}
