#include <stdio.h>
#include <stdlib.h>

#define Rows 3
#define Cols 3
#define empty 0
#define max_operator 4
#define maxlenght 400

const char* action[] = {"First State", "Move Cell EMTY to UP", "Move Cell EMTY to DOWN", "Move Cell Emty to Left", "Move Cell Emty To Right"};

typedef struct {
    int Eight_puzzle [Rows][Cols];
    int Emtyrows;
    int Emtycols;
} State;

// ham in trang thai cua 8 puzzle
void print_State(State state){
    int rows, cols;
    printf("\n------------------\n");
    for(rows = 0; rows < Rows; rows ++){
        for(cols = 0; cols < Cols; cols++){
            printf("|%d", state.Eight_puzzle[rows][cols]);
        }
        printf("\n");
    }
    printf("--------------\n");
}


int compareStates(State state1, State state2){
    if(state1.Emtyrows != state2.Emtyrows || state1.Emtycols != state2.Emtycols){
        return 0;
    }
    int row, col;
    for(row = 0; row < Rows; row++){
        for(col = 0; col < Cols; col++){
            if(state1.Eight_puzzle[row][col] != state2.Eight_puzzle[row][col]){
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
    int empRowCurrent = state.Emtyrows, empColCurrent = state.Emtycols;
    if(empRowCurrent > 0){
        result -> Emtyrows = empRowCurrent - 1;
        result -> Emtycols = empColCurrent;
        result -> Eight_puzzle[empRowCurrent][empColCurrent] = state.Eight_puzzle[empRowCurrent- 1][empColCurrent];
        result ->  Eight_puzzle[empRowCurrent - 1][empColCurrent] = empty;
        return 1;
    }
    return 0;
}

int downOperator(State state, State *result){
    *result = state;
    int empRowCurrent = state.Emtyrows, empColCurrent = state.Emtycols;
    if(empRowCurrent < Rows - 1){
        result -> Emtyrows = empRowCurrent + 1;
        result -> Emtycols = empColCurrent;
        result -> Eight_puzzle[empRowCurrent][empColCurrent] = state.Eight_puzzle[empRowCurrent + 1][empColCurrent];
         result ->  Eight_puzzle[empRowCurrent + 1][empColCurrent] = empty;
         return 1;
    }
    return 0;
}

int leftOperator(State state, State *result){
    *result = state;
    int empRowCurrent = state.Emtyrows, empColCurrent = state.Emtycols;
    if(empColCurrent > 0){
        result -> Emtycols = empColCurrent - 1;
        result -> Emtyrows = empRowCurrent;
        result -> Eight_puzzle[empRowCurrent][empColCurrent] = state.Eight_puzzle[empRowCurrent][empColCurrent - 1];
        result ->Eight_puzzle[empRowCurrent][empColCurrent - 1] = empty;
        return 1;
    }
    return 0;
}

int rightOperator(State state, State *result){
    *result = state;
    int empRowCurrent = state.Emtyrows, empColCurrent = state.Emtycols;
    if(empColCurrent < Cols - 1){
        result -> Emtycols = empColCurrent + 1;
        result -> Emtyrows = empRowCurrent;
        result -> Eight_puzzle[empRowCurrent][empColCurrent] = state.Eight_puzzle[empRowCurrent][empColCurrent + 1];
        result -> Eight_puzzle[empRowCurrent][empColCurrent + 1] = empty;
        return 1;
    }
    return 0;
}

int callOpertors(State state, State *result, int opt){
    switch(opt){
        case 1: return upOperator(state, result);
        case 2: return downOperator(state, result);
        case 3: return leftOperator(state, result);
        case 4: return rightOperator(state, result);
        default: printf("canot call operator");
            return 0;
    }
}

int main() {
        State state, result ;
    state.Emtycols = 1;
    state.Emtyrows = 1;
    state.Eight_puzzle[0][0] = 3;
    state.Eight_puzzle[0][1] = 4;
    state.Eight_puzzle[0][2] = 5;
    state.Eight_puzzle[1][0] = 1;
    state.Eight_puzzle[1][1] = 0;
    state.Eight_puzzle[1][2] = 2;
    state.Eight_puzzle[2][0] = 6;
    state.Eight_puzzle[2][1] = 7;
    state.Eight_puzzle[2][2] = 8;
    printf("Trang thai bat dau \n");
    print_State(state);
    int opt;
    for(opt = 1; opt <= 4; opt++){
        callOpertors(state, &result, opt);
        if(!compareStates(state, result)){
            printf("hanh dong %s thanh cong\n", action[opt]);
            print_State(result);
        }
        else{
            printf("hanh dong %s khong thanh cong\n", action[opt]);
        }
    }
    return 0;
}







