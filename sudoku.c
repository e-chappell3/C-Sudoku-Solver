#include <stdio.h>
#include <stdlib.h>

#define EXIT_FAILURE 1
#define SIZE 9

int grid[SIZE][SIZE];

int loadPuzzle (){
    FILE* f;
    char filename[] = "puzzle.txt";

    f = fopen(filename, "r");
    if (f != NULL){
        for (int i = 0; i < SIZE; i++){
            for (int j = 0; j < SIZE; j++){
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

void printPuzzle(){
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            printf("%d ", grid[i][j]);
            if((j + 1) % 3 == 0 && j != SIZE - 1){
                printf("| ");
            }
        }
        if ((i + 1) % 3 == 0 && i != SIZE - 1){
            printf("\n----------------------");
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]){
    int puzzleError = loadPuzzle();
    if (puzzleError == -1){
        printf("Error loading puzzle\n");
        exit(1);
    }
    else{
        printPuzzle();
    }
}
