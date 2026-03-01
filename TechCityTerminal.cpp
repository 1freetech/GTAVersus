#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <cctype>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

#ifdef _WIN32
static bool keyAvailable()
{
    return _kbhit() != 0;
}

static char readKey()
{
    return static_cast<char>(_getch());
}
#else
static bool keyAvailable()
{
    termios oldt{};
    tcgetattr(STDIN_FILENO, &oldt);

    termios newt = oldt;
    newt.c_lflag &= static_cast<unsigned>(~(ICANON | ECHO));
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    int c = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (c != EOF)
    {
        ungetc(c, stdin);
        return true;
    }
    return false;
}

static char readKey()
{
    termios oldt{};
    tcgetattr(STDIN_FILENO, &oldt);

    termios newt = oldt;
    newt.c_lflag &= static_cast<unsigned>(~(ICANON | ECHO));
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int c = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return (c == EOF) ? '\0' : static_cast<char>(c);
}
#endif
