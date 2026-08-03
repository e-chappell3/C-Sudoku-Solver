#include <stdio.h>
#include <stdlib.h>

#define EXIT_FAILURE 1
#define SIZE 3
#define N (SIZE * SIZE)

struct Node{
    // pointers to adjacent nodes by assignment row then constraint column
    struct Node* left;
    struct Node* right;
    struct Node* up;
    struct Node* down;
    // info on column and row
    struct ColHeadNode* col;
    // same as rowIndex to represent row, column and value from matrix
    int rowID;
};

struct ColHeadNode{
    // contains self as node, column size (number of nodes) and column index
    struct Node node;
    int size;
    int colID;
};

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

struct ColHeadNode* createDLL(int matrix[N*N*N][4*N*N]){
    // create first column header node as root and current column node
    struct ColHeadNode* root = (struct ColHeadNode*)malloc(sizeof(struct ColHeadNode));
    struct ColHeadNode* currentCol = root;
    // initialise root
    root->colID = -1;
    root->node.col = root;
    root->size = 0;
    // ensure circularity
    root->node.up = &root->node;
    root->node.down = &root->node;
    root->node.left = &root->node;
    root->node.right = &root->node;

    for (int c = 0; c < 4*N*N; c++){
        // iterate through each column of the matrix, setting ID to index and col to self
        currentCol->colID = c;
        currentCol->node.col = currentCol;
        // while column is empty except header make size 0 and point to self so still valid pointers
        currentCol->size = 0;
        currentCol->node.up = &currentCol->node;
        currentCol->node.down = &currentCol->node;

        if (c < 4*N*N - 1){
            // ensure next column not created for final column
            struct ColHeadNode* nextCol = (struct ColHeadNode*)malloc(sizeof(struct ColHeadNode));
            // link through pointers ensuring root is still circularly linked
            nextCol->node.left = &currentCol->node;
            nextCol->node.right = &root->node;
            currentCol->node.right = &nextCol->node;
            root->node.left = &nextCol->node;
            // move onto next column (right)
            currentCol = nextCol;
        }
    }

    // ensure DLL is circular by linking start/root and end/current
    currentCol->node.right = &root->node;
    root->node.left = &currentCol->node;

    for (int r = 0; r < N*N*N; r++){
        // iterate through each row of the matrix, starting with column after root
        currentCol = root->node.right->col;
        // create array of nodes for the row to link together after creation
        struct Node* rowNodes[4];
        int count = 0;
        // iterate through each column of the matrix
        for (int c = 0; c < 4*N*N; c++){
            if (matrix[r][c] == 1){
                // if a 1 is present in the matrix, create node to represent this
                struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
                // give node row and column info + increment column size
                newNode->rowID = r;
                newNode->col = currentCol;
                currentCol->size++;
                // add newly created node to array of nodes for the row
                rowNodes[count++] = newNode;

                // link new node to the bottom of the column as follows
                newNode->up = currentCol->node.up;
                currentCol->node.up->down = newNode;
                newNode->down = &currentCol->node;
                currentCol->node.up = newNode;
            }
            // move onto next column
            currentCol = currentCol->node.right->col;
        }
        for (int i = 0; i < count; i++){
            // iterate through newly created nodes to link row together
            rowNodes[i]->right = rowNodes[(i + 1) % count];
            rowNodes[i]->left = rowNodes[(i + count - 1) % count];
        }
    }

    // return root of DLL to traverse whole list through
    return root;
}

struct ColHeadNode* createMatrix(){
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
                }
            }
        }
    }

    return createDLL(matrix);
}

void coverNode(struct Node* node){
    // removes/covers a node from the list
    // change pointers to skip over this node, removing column from other columns
    printf("Covering node with rowID: %d and colID: %d\n", node->rowID, node->col->colID);
    node->right->left = node->left;
    node->left->right = node->right;
    printf("node->right->left now points to (%d, %d), node->left->right = (%d, %d)\n", node->left->right->rowID, node->left->right->col->colID, node->right->left->rowID, node->right->left->col->colID);

    // must also cover for all rows, starting w/ the first column down
    struct Node* currentRow = node->down;
    while (currentRow != node){
        // per column, traverse through all rows via node to the right until back circularly to this node
        struct Node* rightNode = currentRow->right;
        while (rightNode != currentRow){
            rightNode->up->down = rightNode->down;
            rightNode->down->up = rightNode->up;
            printf("node->up->down now points to (%d, %d), node->down->up = (%d, %d)\n", rightNode->up->down->rowID, rightNode->up->down->col->colID, rightNode->down->up->rowID, rightNode->down->up->col->colID);


            // adjust column size for removal then move onto next node to right
            rightNode->col->size--;
            rightNode = rightNode->right;
        }
        currentRow = currentRow->down;
    }
}

void uncoverNode(struct Node* node){
    // reverses coverNode() process to point back to this node across columns + rows
    printf("Uncovering node with rowID: %d and colID: %d\n", node->rowID, node->col->colID);
    struct Node* currentRow = node->up;
    while (currentRow != node){
        struct Node* leftNode = currentRow->left;
        while (leftNode != currentRow){
            leftNode->col->size++;

            leftNode->down->up = leftNode;
            leftNode->up->down = leftNode;
            printf("node->up->down now points to (%d, %d), node->down->up = (%d, %d)\n", leftNode->up->down->rowID, leftNode->up->down->col->colID, leftNode->down->up->rowID, leftNode->down->up->col->colID);

            leftNode = leftNode->left;
        }
        currentRow = currentRow ->up;
    }

    node->right->left = node;
    node->left->right = node;
    printf("node->right->left now points to (%d, %d), node->left->right = (%d, %d)\n", node->left->right->rowID, node->left->right->col->colID, node->right->left->rowID, node->right->left->col->colID);

}

int main(int argc, char* argv[]){
    int puzzleError = loadPuzzle();
    if (puzzleError == -1){
        printf("Error loading puzzle\n");
        exit(1);
    }
    else{
        printPuzzle();
        struct ColHeadNode* root = createMatrix();

        free(root);
    }
}
