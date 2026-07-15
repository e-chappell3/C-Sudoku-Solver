#include <stdio.h>
#include <stdlib.h>

#define EXIT_FAILURE 1
#define SIZE 3
#define N (SIZE * SIZE)

int grid[N][N];

int loadPuzzle (){
    FILE* f;
    char filename[] = "puzzle.txt";

    f = fopen(filename, "r");
    if (f != NULL){
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
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
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            printf("%d ", grid[i][j]);
            if((j + 1) % SIZE == 0 && j != N - 1){
                printf("| ");
            }
        }
        if ((i + 1) % SIZE == 0 && i != N - 1){
            printf("\n----------------------");
        }
        printf("\n");
    }
}

void createMatrix(){
    int prefilled[N][3];
    int pfilledCtr = 0;

    for (int R = 0; R < N; R++){
        for (int C = 0; C < N   ; C++){
            if (grid[R][C] > 0){
                prefilled[pfilledCtr][0] = grid[R][C];
                prefilled[pfilledCtr][1] = R;
                prefilled[pfilledCtr][2] = C;
                pfilledCtr++;
                printf("Prefilled: %d at Row: %d, Col: %d\n", grid[R][C], R, C);
            }
        }
    }

    int matrix[N*N*N][4*N*N] = {0};
    // N^3 given representing row, column and number possibilities as matrix rows
    // N^2 given repersenting 4 contraints across rows and columns as matrix columns
    for (int V = 0; V < N; V++){
        // iterate through each number/value possibility
        for (int R = 0; R < N; R++){
            // iterate through each row possibility
            for (int C = 0; C < N; C++){
                // iterate through each column possibility
                if (grid[R][C] == 0){
                    // if cell is not prefilled
                    int rowIndex = C + (N * R) + (N * N * V);
                    int blockIndex = (C / SIZE) + ((R / SIZE) * SIZE);
                    // column indexes for constraints per cell, row, column and box
                    int indexCell = R * N + C;
                    // calculate row with N aka 1st row = 0, 2nd = 1 and then add column to get unique index for each cell
                    int indexRow = N * N + R * N + V;
                    // size^2 to be past index of indexCell then r * size to represent grid index and V to represent value at index
                    int indexCol = 2 * N * N + C * N + V;
                    // same as above but for columns instead of rows and added offset of another size^2
                    int indexBox = 3 * N * N + (blockIndex * N + V);
                    // 3 * size^2 to be past index (know 3 indexes taking up N^2 space)

                    // fill all constraints with a 1
                    matrix[rowIndex][indexCell] = 1;
                    matrix[rowIndex][indexRow] = 1;
                    matrix[rowIndex][indexCol] = 1;
                    matrix[rowIndex][indexBox] = 1;

                    printf("Row: %d, Col: %d, Val: %d all filled w/ 1s is empty\n", R, C, V + 1);
                }
                else if (grid[R][C] - 1 == V){
                    // if cell is prefilled, do the same as above but only when V matches the prefilled value
                    int rowIndex = C + (N * R) + (N * N * V);
                    int blockIndex = (C / SIZE) + ((R / SIZE) * SIZE);
                    int indexCell = R * N + C;
                    int indexRow = N * N + R * N + V;
                    int indexCol = 2 * N * N + C * N + V;
                    int indexBox = 3 * N * N + (blockIndex * N + V);

                    // fill all constraints with a 1
                    matrix[rowIndex][indexCell] = 1;
                    matrix[rowIndex][indexRow] = 1;
                    matrix[rowIndex][indexCol] = 1;
                    matrix[rowIndex][indexBox] = 1;
                    printf("Row: %d, Col: %d, Val: %d all filled w/ 1s as prefilled\n", R, C, V + 1);
                }
            }
        }
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
        createMatrix();
    }
}
