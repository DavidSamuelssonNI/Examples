#ifndef F5DEEDE6_71EF_4E05_A513_041054FB0443
#define F5DEEDE6_71EF_4E05_A513_041054FB0443
#include <termios.h>
#include <unistd.h>

#include <stdio.h>

inline int _getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

#endif /* F5DEEDE6_71EF_4E05_A513_041054FB0443 */
