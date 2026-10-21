#include "../include/minifim.h"

size_t my_strlen(const char *a)
{
    size_t l = 0;

    while (a[l] != '\0')
    {
        l++;
    }

    return l;
}

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
