#ifndef MINIFIM_H
#define MINIFIM_H

#define _GNU_SOURCE

#include <err.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct fim_entry
{
    char *path;
    char hash[65];
    int64_t fim_size;
    unsigned int mode;
    unsigned int own;
    unsigned int grp;
    int64_t modif_time;
    struct fim_entry *next;
};

struct conf_path
{
    char *path;
    int is_excluded;
    struct conf_path *next;
};

struct fim_config
{
    size_t conf_size;
    struct conf_path *head;
};

struct llist
{
    size_t llist_size;
    struct fim_entry *head;
};

int strdiff(const char *a, const char *b);
int config_load(const char *filename, struct fim_config *config);
void config_free(struct fim_config *config);
void cli(void);
void init(void);

#endif // MINIFIM_H
