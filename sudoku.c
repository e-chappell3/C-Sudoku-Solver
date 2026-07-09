#include <stdio.h>
#include <stdlib.h>

#define EXIT_FAILURE 1

int grid[9][9];

int loadPuzzle (){
    FILE* f;
    char filename[] = "puzzle.txt";

    f = fopen(filename, "r");
    if (f != NULL){
        for (int i = 0; i < 9; i++){
            for (int j = 0; j < 9; j++){
                if (fscanf(f, "%d", &grid[i][j]) != 1){
                    fclose(f);
                    return -1;
                }
            }
        }
        fclose(f);
        return 1;
    }
    else{
        return -1;
    }
}

int main(int argc, char* argv[]){
    int puzzleError = loadPuzzle();
    if (puzzleError == -1){
        printf("Error loading puzzle\n");
        exit(1);
    }
    else{
        for (int i = 0; i < 9; i++){
            for (int j = 0; j < 9; j++){
                printf("%d ", grid[i][j]);
            }
            printf("\n");
        }
    }
}
