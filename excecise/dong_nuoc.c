#include <stdio.h>
#include <stdlib.h>

#define tankcapacity_X 9
#define tankcapacity_Y 4
#define emty 0
#define goal 6
#define maxlenght 100

typedef struct {
    int x;
    int y;
}State;

// khai bao max min
int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

// khoi tao trang thai binh
void makeNullState(State *state){
    state ->x = 0;
    state ->y = 0;
}

// kiem tra luong nuoc
void print_State(State state){
    printf("\n   x:%d --- %d", state.x, state.y);
}

// kiem tra trang thai muc tieu
int goalcheck(State state){
    return (state.x==goal || state.y==goal);
}

// lam day binh nuoc x
int pourWaterFullX(State cur_state, State *result){
    if(cur_state.x < tankcapacity_X){
        result->x = tankcapacity_X;
        result->y = cur_state.y;
        return 1;
    }
    return 0;
}

// lam day binh nuoc y
int pourWaterFullY(State cur_state, State *result){
    if(cur_state.y < tankcapacity_Y){
        result->y = tankcapacity_Y;
        result->x = cur_state.x;
        return 1;
    }
    return 0;
}

// lam rong nuoc trong binh x
int pourWaterEmtyX(State cur_state, State *result){
    if(cur_state.x > 0){
        result->x = emty;
        result->y = cur_state.y;
        return 1;
    }
    return 0;
}

int pourWaterEmtyY(State cur_state, State *result){
    if(cur_state.y > 0){
        result->y = emty;
        result->x = cur_state.x;
        return 1;
    }
    return 0;
}

int pourWaterXY(State cur_state, State *result){
    if(cur_state.x>0 && cur_state.y < tankcapacity_Y){
        result->x = max(cur_state.x - (tankcapacity_Y - cur_state.y), emty);
        result->y = min(cur_state.x + cur_state.y, tankcapacity_Y);
        return 1;
    }
    return 0;
}

int pourWaterYX(State cur_state, State *result){
    if(cur_state.y>0 && cur_state.x < tankcapacity_X){
        result->y = max(cur_state.y - (tankcapacity_X - cur_state.x), emty);
        result->x = min(cur_state.y + cur_state.x, tankcapacity_X);
        return 1;
    }
    return 0;
}

int call_operator(State cur_state, State *result, int option){
    switch(option){
        case 1: return pourWaterFullX(cur_state, result);
        case 2: return pourWaterFullY(cur_state, result);
        case 3: return pourWaterEmtyX(cur_state, result);
        case 4: return pourWaterEmtyY(cur_state, result);
        case 5: return pourWaterXY(cur_state, result);
        case 6: return pourWaterYX(cur_state, result);
        default: printf("Error calls operarors");
            return 0;
    }
}


const char* action[] = {"First State","pour WAter Full X","pour Water Full Y","pour Water Emty X", "pour Water Emty Y",
"pour Water X to Y", "pour Water Y to X"};

int main() {
    State cur_sate = {5, 4}, result;
    printf("trang thai bat dau");
    print_State(cur_sate);
    int opt;
    for(opt = 1; opt<=6; opt++){
        int thuchien = call_operator(cur_sate, &result, opt);
        if(thuchien == 1){
            printf("\nhanh dong thanh cong %s thanh cong", action[opt]);
            print_State(result);
        }
        else
            printf("\n hanh dong %s khong thanh cong", action[opt]);
    }
    return 0;
}