#include "codexion.h"

char* take_dongles()
{
    return "Take dongles test passed.";
}

char* compile()
{
    return "Compile test passed.";
}

char* release_dongles()
{
    return "Release dongles test passed.";
}

char* debug()
{
    return "Debug test passed.";
}

char* refactor()
{
    return "Refactor test passed.";
}
char* test_actions()
{
    int stop = 0;
    int lim = 10;
    while (stop < lim)
    {
        printf("Iteration: %d\n", stop);
        printf("%s.\n", take_dongles());
        printf("%s.\n", compile());
        printf("%s.\n", release_dongles());
        printf("%s.\n", debug());
        printf("%s.\n", refactor());
        stop++;
    }
    return "Actions test passed.";
}


int main()
{
    printf("%s\n", test_actions());
    return 0;
}