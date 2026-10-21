#include "../include/minifim.h"

char *cp_line_to_tmp(char *line)
{
    size_t len_line = my_strlen(line);
    char *tmp = malloc(len_line + 1);

    if (!tmp)
    {
        return NULL;
    }

    size_t start = 0;
    size_t end = len_line;
    if (line[0] == '!')
    {
        start++;
    }

    if (line[len_line - 1] == '\n')
    {
        end--;
    }

    for (size_t i = start; i < end; i++)
    {
        tmp[i - start] = line[i];
    }

    tmp[end - start] = '\0';

    return tmp;
}

static struct conf_path *push_fim_conf_path(struct conf_path *s, size_t *size,
                                            char *line, int exclu_bool)
{
    struct conf_path *new = malloc(sizeof(struct conf_path));

    if (!new)
    {
        return NULL;
    }

    char *tmp = cp_line_to_tmp(line);

    if (!tmp)
    {
        free(new);
        return NULL;
    }

    new->path = tmp;
    new->is_excluded = exclu_bool;

    if (s == NULL)
    {
        new->next = NULL;
    }
    new->next = s;
    (*size)++;

    return new;
}

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
    struct conf_path *head_after_push;

    while ((nread = getline(&line, &size, file)) != -1)
    {
        if (line[0] == '/')
        {
            head_after_push =
                push_fim_conf_path(config->head, &config->conf_size, line, 0);
            if (head_after_push)
            {
                config->head = head_after_push;
            }
            else
            {
                free(line);
                fclose(file);
                return -1;
            }
        }

        if (line[0] == '!')
        {
            head_after_push =
                push_fim_conf_path(config->head, &config->conf_size, line, 1);

            if (head_after_push)
            {
                config->head = head_after_push;
            }
            else
            {
                free(line);
                fclose(file);
                return -1;
            }
        }
    }

    free(line);
    fclose(file);
    return 0;
}

void config_free(struct fim_config *config)
{
    struct conf_path *ptr = config->head;
    struct conf_path *addr;

    while (ptr)
    {
        addr = ptr;
        ptr = ptr->next;
        free(addr->path);
        free(addr);

        config->conf_size--;
    }

    config->head = NULL;
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
