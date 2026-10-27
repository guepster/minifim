#ifndef MINIFIM_H
#define MINIFIM_H

#define _GNU_SOURCE

#include <err.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

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

struct llist
{
    size_t llist_size;
    struct fim_entry *head;
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

struct fim_entry push_entries(path, entry, s);
size_t my_strlen(const char *a);
int strdiff(const char *a, const char *b);
int config_load(const char *filename, struct fim_config *config);
void config_free(struct fim_config *config);
void cli(void);
void init(void);

#endif // MINIFIM_H
