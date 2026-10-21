#include "../include/minifim.h"

int config_load(const char *filename, struct fim_config *config)
{
    if (!filename)
    {
        return -1;
    }

    FILE *file = fopen(filename, "r");

    if (!file)
    {
        return -1;
    }

    char *line = NULL;
    size_t size = 0;
    ssize_t nread;

    while ((nread = getline(&line, &size, file)) != -1)
    {
        fwrite(line, nread, 1, stdout);
    }

    if (config)
    {
        return 0;
    }

    return 0;
}

void init(void)
{
    struct fim_config config = {
        .conf_size = 0,
        .head = NULL,
    };
    char *conf_filename = "./minifim.conf";
    int success = config_load(conf_filename, &config);

    if (success == -1)
    {
        errx(3, "minifim: error loading conf file\n");
    }
}
