#include "../include/minifim.h"

int main(int argc, char *argv[]){
    
    if(argc < 1){
        cli();
        return 1;
    }
    
    if(strdiff(argv[1], "init") == 0){
        return 99;
    }
    else if(strdiff(argv[1], "accept") == 0){
        if(argc < 3 || argc > 3){
            cli();
            return 1;
        }
        return 98;
    }
    else{
        cli();
        return 1;
    }

    return 0;
}
