#include "Environment-dependent.h"
#include <stdlib.h>
#include <stdio.h>
#if !defined (__cplusplus) || (defined(_MSC_VER) && _MSC_VER < 1900)
#define noexcept
#endif
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
/* reads from keypress, doesn't echo */
int _getch(void){
	struct termios oldattr, newattr;
	int ch;
	tcgetattr(STDIN_FILENO, &oldattr);
	newattr = oldattr;
	newattr.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
	return ch;
}
#endif

void clear_console() noexcept {
	system("cls");
}

int pause_console() noexcept {
	puts("続行するには何かキーを押してください . . .");
	return _getch();
}
