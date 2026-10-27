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

char *myhash(char *path){

}

int is_excluded(const char *path, const struct fim_config *config){

    struct conf_path *ptr = config->head;

    while(ptr){
        if(ptr->is_excluded == 1){
            size_t l =my_strlen(ptr->path);
            while( l > 1 && ptr->path[l-1] == '/'){
                l = l-1;
            }

            if(strncmp(path, ptr->path, l) == 0){
                if(path[l] == '\0' || path[l] == '/'){
                    return 1;
                }
            }
        } 
        
        ptr = ptr->next;
    }

    return 0;

}

int push_entries(const char *path, struct llist *s){

    struct fim_entry *new = malloc(sizeof(struct fim_entry));

    if(!new){
        return NULL;
    }

    if(entry_from_path(path, new) == -1){
        free(new);
        return 0;
    }

    new->next = s->head;
    s->head = new;
    s->llist_size++;

    return 0;
}

int entry_from_path(const char *path, struct fim_entry *entry){


}

char *join_path(const char *dir, const char *name){

    size_t l_dir = my_strlen(dir);
    size_t l_name = my_strlen(name);

    int need_slash = 1;
    if(l_dir > 0 && dir[l_dir-1] == '/'){
        need_slash = 0;
    }
    
    size_t tot = l_dir + l_name + 1;
    if(need_slash == 1){
        tot++;
    }

    char *concat = malloc(tot);

    if(!concat){
        return NULL;
    }

    if(need_slash == 1){
        snprintf(concat, tot, "%s/%s", dir, name);
    }
    else{
        snprintf(concat, tot, "%s%s", dir, name);   
    }

    return concat;

}

int scan_path(const char *path, struct fim_config *config, struct llist *s){

    if(is_excluded(path, config)){
        return 0;
    }

    struct stat st;
    if(lstat(path, &st) == -1){
        errx(3, "minifim: error file not readable");
        return 0;
    }

    if(push_entry(s, path) == -1){
        return -1;
    }

    if(!S_ISDIR(st.st_mode)){
        return 0;
    }

    DIR *rep = opendir(path);
    
    if (!rep) {
        errx(3, "minifim: dir not readable");
        return 0;
    }
    
    struct dirent *dir;
    while ((dir = readdir(rep)) != NULL) {
      
        if(strcmp(dir->d_name, '.') == 0 || strcmp(dir->d_name, '..') == 0){
            continue;
        }

        char *child = join_path(path, dir->d_name);
        
        if(!child){
            closedir(rep);
            return -1;
        }

        if(scan_path(child, config, s) == -1){
            free(child);
            closedir(rep);
            return -1;
        }

        free(child);

    }

    closedir(rep);
    return 0;
}

int scan(struct fim_config *config, struct llist *s){

    if(config->head == NULL){
        return -1;
    }

    struct conf_path *ptr = config->head;
    
    while(ptr){
        if(ptr->is_excluded == 0){
            if(scan_path(ptr->path, config, s) == -1){
                return -1;
            }
        }

        ptr = ptr->next;
    } 
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
    int success_loading = config_load(conf_filename, &config);

    if (success_loading == -1)
    {
        errx(3, "minifim: error loading conf file\n");
        config_free(&config);
        return 1;
    }

    struct llist s = {
        .llist_size = 0,
        .head = NULL,
    };

    int success_scan = scan(&config, &s);

    if(success_scan == -1){
        errx(3, "minifim: error loading conf file\n");
        config_free(&config);
        return 1;
    }

    config_free(&config);
    return 0;

}
