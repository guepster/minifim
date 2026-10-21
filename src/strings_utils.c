#include "../include/minifim.h"

int strdiff(const char *a, const char *b)
{
    for (size_t i = 0; a[i] != '\0' || b[i] != '\0'; i++)
    {
        if (a[i] != b[i])
        {
            return 1;
        }
    }

    return 0;
}
