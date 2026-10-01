#include <stdio.h>
#include <stdlib.h>

#define Rows 3
#define Cols 3
#define empty 0
#define max_operator 4
#define max_lenght 400

typedef struct{
    int Eight_puzzle [Rows][Cols];
    int emptyRows;
    int emptyCols;
}State;

const char* Action[] = {"Fist State", "move cell empty to up", "move cell empty to down", "move cell empty to left", "move cell empty to right"};

void printState(State state){
    int rows, cols;
    printf("-----------------\n");
    for(rows = 0; rows < Rows; rows++){
        for(cols = 0; cols < Cols; cols++){
            printf("|%d", state.Eight_puzzle[rows][cols]);
        }
        printf("\n");
    }
    printf("------------------\n");
}

int compareStates(State state1, State state2){
    if(state1.emptyRows != state2.emptyRows || state1.emptyCols != state2.emptyCols){
        return 0;
    }
    int rows, cols;
    for(rows = 0; rows < Rows; rows ++){
        for(cols = 0; cols < Cols; cols++){
            if(state1.Eight_puzzle[rows][cols] != state2.Eight_puzzle[rows][cols]){
                return 0;
            }
        }
    }
    return 1;
}

int goalcheck(State state, State goal){
    return compareStates(state, goal);
}

int upOperator(State state, State *result){
    *result = state;
    int emprowCurrent = state.emptyRows, empcolCurrent = state.emptyCols;
    if(emprowCurrent > 0){
        result -> emptyRows = emprowCurrent - 1;
        result -> emptyCols = empcolCurrent;
        result -> Eight_puzzle[emprowCurrent][empcolCurrent] = state.Eight_puzzle[emprowCurrent - 1][empcolCurrent];
        result -> Eight_puzzle[emprowCurrent - 1][empcolCurrent] = empty;
        return 1;
    }
    return 0;
}

int downOperator(State state, State *result){
    *result = state;
    int emprowCurrent = state.emptyRows, empcolCurrent = state.emptyCols;
    if(emprowCurrent < Rows - 1){
        result -> emptyRows = emprowCurrent + 1;
        result -> emptyCols = empcolCurrent;
        result -> Eight_puzzle[emprowCurrent][empcolCurrent] = state.Eight_puzzle[emprowCurrent + 1][empcolCurrent];
        result -> Eight_puzzle[emprowCurrent + 1][empcolCurrent] = empty;
        return 1;
    }
    return 0;
}

int leftOperator(State state, State *result){
    *result = state;
    int emprowCurrent = state.emptyRows, epmcolCurrent = state.emptyCols;
    if(epmcolCurrent > 0){
        result -> emptyRows = emprowCurrent;
        result -> emptyCols = epmcolCurrent - 1;
        result -> Eight_puzzle[emprowCurrent][epmcolCurrent] = state.Eight_puzzle[emprowCurrent][epmcolCurrent - 1];
        result -> Eight_puzzle[emprowCurrent][epmcolCurrent - 1] = empty;
        return 1;
    }
    return 0;
}

int rightOperator(State state, State *result){
    *result = state;
    int emprowCurrent = state.emptyRows, empcolCurrent = state.emptyCols;
    if(empcolCurrent < Cols - 1){
        result -> emptyRows = emprowCurrent;
        result -> emptyCols = empcolCurrent + 1;
        result -> Eight_puzzle[emprowCurrent][empcolCurrent] = state.Eight_puzzle[emprowCurrent][empcolCurrent + 1];
        result -> Eight_puzzle[emprowCurrent][empcolCurrent + 1] = empty;
        return 1;
    }
    return 0;
}

int callOperator(State state, State *result, int opt){
    switch(opt){
        case 1: return upOperator(state, result);
        case 2: return downOperator(state, result);
        case 3: return leftOperator(state, result);
        case 4: return rightOperator(state, result);
        default: printf("cannot call operator");
            return 0;
    }
}

int heuristic1(State state, State goal){
    int rows, cols, count = 0;
    for(rows = 0; rows < Rows; rows ++){
        for(cols = 0; cols < Cols; cols++){
            if(state.Eight_puzzle[rows][cols] != goal.Eight_puzzle[rows][cols]){
                count++;
            }
        }
    }
    return count;
}

int heuristic2(State state, State goal){
    int count = 0;
    int row, col, row_g, col_g;
    for(row = 0; row < Rows; row++){
        for(col = 0; col < Cols; col++){
            if(state.Eight_puzzle[row][col] != empty){
                for(row_g = 0; row_g < Rows; row_g ++){
                    for(col_g = 0; col_g < Cols; col_g ++){
                        if(state.Eight_puzzle[row][col] == goal.Eight_puzzle[row_g][col_g]){
                            count += abs(row - row_g) + abs(col - col_g);
                            col_g = Cols;
                            row_g = Rows;
                        }
                    }
                }
            }
        }
    }
    return count;
}


typedef struct Node{
    State state;
    struct Node* Parent;
    int no_function;
    int heuristic;
}Node;

typedef struct{
    Node* Elements[max_lenght];
    int size;
}List;

void makeNull_List(List *L){
    L -> size = empty;
}

int empty_List(List L){
    return L.size == empty;
}

int full_List(List L){
    return L.size == max_lenght;
}

Node* element_at(int p, List L){
   return L.Elements[p - 1];
}

void push_List(Node* x, int position, List *L){
    if(!full_List(*L)){
        int q;
        for(q = L -> size; q>= position; q--){
            L->Elements[q] = L->Elements[q - 1];
        }
        L -> Elements[position - 1] = x;
        L -> size++;
    }
    else printf("list is full");
}

//Tim trang thai state co thuoc Open hoac Close hay ko?
//Luu vi tri tim duoc vao bien *position
Node* find_State(State state, List list, int *position){
    int i;
    for(i=1; i<=list.size; i++)
        if(compareStates(element_at(i,list)->state,state)){
            *position = i;
            return element_at(i,list);
        }
    return NULL;
}

//Sap xep danh sach theo trong so heuristic
void sort_List(List *list){
    int i, j;
    for(i=0; i<list->size-1; i++)
        for(j=i+1; j<list->size; j++)
            if(list->Elements[i]->heuristic > list->Elements[j]->heuristic){
                Node* node = list->Elements[i];
                list->Elements[i] = list->Elements[j];
                list->Elements[j] = node;
            }
}
void delete_List(int position, List *L){
    int q;

    for(q = position; q < L->size; q++){
        L->Elements[q - 1] = L->Elements[q];
    }

    L->size--;
}

// Thuat toan tim kiem tot nhat dau tien
// Ham f = h
Node* best_first_search(State state, State goal) {
    List Open_BFS;
    List Close_BFS;
    makeNull_List(&Open_BFS);
    makeNull_List(&Close_BFS);
    
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->Parent = NULL;
    root->no_function = 0;
    root->heuristic = heuristic1(root->state, goal);
    
    push_List(root, Open_BFS.size + 1, &Open_BFS);
    
    while (!empty_List(Open_BFS)) {
        Node* node = element_at(1, Open_BFS);
        delete_List(1, &Open_BFS);
        push_List(node, Close_BFS.size + 1, &Close_BFS);
        
        if (goalcheck(node->state, goal))
            return node;
            
        int opt;
        for (opt = 1; opt <= max_operator; opt++) {
            State newstate;
            newstate = node->state;
            
            if (callOperator(node->state, &newstate, opt)) {
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->state = newstate;
                newNode->Parent = node;
                newNode->no_function = opt;
                newNode->heuristic = heuristic1(newstate, goal);
                
                // Kiem tra trang thai moi sinh ra co thuoc Open_BFS/ Close_BFS
                int pos_Open, pos_Close;
                Node* nodeFoundOpen = find_State(newstate, Open_BFS, &pos_Open);
                Node* nodeFoundClose = find_State(newstate, Close_BFS, &pos_Close);
                
                if (nodeFoundOpen == NULL && nodeFoundClose == NULL) {
                    push_List(newNode, Open_BFS.size + 1, &Open_BFS);
                }
                else if (nodeFoundOpen != NULL && nodeFoundOpen->heuristic > newNode->heuristic) {
                    delete_List(pos_Open, &Open_BFS);
                    push_List(newNode, pos_Open, &Open_BFS);
                }
                else if (nodeFoundClose != NULL && nodeFoundClose->heuristic > newNode->heuristic) {
                    delete_List(pos_Close, &Close_BFS);
                    push_List(newNode, Open_BFS.size + 1, &Open_BFS);
                }
            }
        }
        sort_List(&Open_BFS);
    }
    return NULL;
}

// Ham in ket qua cua thuat toan BFS
void print_WaysToGetGoal(Node* node) {
    List listPrint;
    makeNull_List(&listPrint);
    
    // Duyet nguoc ve nut parent
    while (node->Parent != NULL) {
        push_List(node, listPrint.size + 1, &listPrint);
        node = node->Parent;
    }
    push_List(node, listPrint.size + 1, &listPrint);
    
    // In ra thu tu hanh dong di chuyen o trong
    int no_action = 0, i;
    for (i = listPrint.size; i > 0; i--) {
        printf("\nAction %d: %s", no_action, Action[element_at(i, listPrint)->no_function]);
        printState(element_at(i, listPrint)->state);
        no_action++;
    }
}

int main() {
    State state;
    state.emptyRows = 1;
    state.emptyCols = 1;
    state.Eight_puzzle[0][0] = 3;
    state.Eight_puzzle[0][1] = 4;
    state.Eight_puzzle[0][2] = 5;
    state.Eight_puzzle[1][0] = 1;
    state.Eight_puzzle[1][1] = 0;
    state.Eight_puzzle[1][2] = 2;
    state.Eight_puzzle[2][0] = 6;
    state.Eight_puzzle[2][1] = 7;
    state.Eight_puzzle[2][2] = 8;

    State goal;
    goal.emptyRows = 0;
    goal.emptyCols = 0;
    goal.Eight_puzzle[0][0] = 0;
    goal.Eight_puzzle[0][1] = 1;
    goal.Eight_puzzle[0][2] = 2;
    goal.Eight_puzzle[1][0] = 3;
    goal.Eight_puzzle[1][1] = 4;
    goal.Eight_puzzle[1][2] = 5;
    goal.Eight_puzzle[2][0] = 6;
    goal.Eight_puzzle[2][1] = 7;
    goal.Eight_puzzle[2][2] = 8;

    Node* p = best_first_search(state, goal);
    print_WaysToGetGoal(p);
    return 0;
}