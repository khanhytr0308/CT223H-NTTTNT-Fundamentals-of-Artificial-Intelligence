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
int goalCheck(State state){
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


//khai bao cau truc dinh de dung cay tim kiem
typedef struct Node{
    State state;
    struct Node* Parent;
    int no_function;
}Node;

//khai bao cau truc stack de luu trang thai duyet
typedef struct {
    Node* Elements[maxlenght];
    int Top_idx;
}Stack;


//kiem tra ngan xep co day khong
int full_Stack(Stack stack){
    return stack.Top_idx == 0;
}

// dua 1 phan tu len dinh ngan xep
void push(Node* x, Stack *stack){
    if(full_Stack(*stack)){
        printf("error! Stack is full");
    }
    else{
        stack -> Top_idx -= 1;
        stack -> Elements[stack -> Top_idx] = x;
    }
}

//kiem tra ngan xep rong
int empty_Stack(Stack stack) {
    return stack.Top_idx == maxlenght;
}

// khoi tao ngan xep rong
void makeNull_Stack(Stack *stack) {
    stack->Top_idx = maxlenght;
}

//tra ve phan tu dinh ngan xep
Node* top(Stack stack){
    if(!empty_Stack(stack)){
        return stack.Elements[stack.Top_idx];
    }
    return NULL;
}

//xoa phan tu tai dinh ngan xep
void pop(Stack *stack){
    if(!empty_Stack(*stack)){
        stack->Top_idx += 1;
    }
    else{
        printf("error! stack is empty");
    }
}


int compareStates(State state1, State state2) {
    return state1.x == state2.x && state1.y == state2.y;
}

// tim trang thai trong stack open / close
int find_State(State state, Stack openStack) {
    while (!empty_Stack(openStack)) {
        if (compareStates(top(openStack)->state, state))
            return 1;

        pop(&openStack);
    }
    return 0;
}

// Thuat toan duyet theo chieu sau
Node* DFS_Algorithm(State state) {
    // Khai bao hai ngan xep Open va Close
    Stack Open_DFS;
    Stack Close_DFS;
    makeNull_Stack(&Open_DFS);
    makeNull_Stack(&Close_DFS);

    // Tao nut trang thai cha
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->Parent = NULL;
    root->no_function = 0;
    push(root, &Open_DFS);

    while (!empty_Stack(Open_DFS)) {
        // Lay mot dinh trong ngan xep
        Node* node = top(Open_DFS);
        pop(&Open_DFS);
        push(node, &Close_DFS);

        // Kiem tra xem node lay ra co phai trang thai muc tieu?
        if (goalCheck(node->state))
            return node;

        int opt;

        // Goi cac phep toan tren trang thai
        for (opt = 1; opt <= 6; opt++) {
            State newstate;
            makeNullState(&newstate);

            if (call_operator(node->state, &newstate, opt)) {
                // Neu trang thai moi sinh ra da ton tai thi bo qua
                if (find_State(newstate, Close_DFS) ||
                    find_State(newstate, Open_DFS))
                    continue;

                // Neu trang thai moi chua ton tai thi them vao ngan xep
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->state = newstate;
                newNode->Parent = node;
                newNode->no_function = opt;
                push(newNode, &Open_DFS);
            }
        }
    }

    return NULL;
}

// In ket qua chuyen nuoc de dat den trang thai muc tieu
void print_WaysToGetGoal(Node* node) {
    Stack stackPrint;
    makeNull_Stack(&stackPrint);

    // Duyet nguoc ve nut parent de
    while (node->Parent != NULL) {
        push(node, &stackPrint);
        node = node->Parent;
    }

    push(node, &stackPrint);

    // In ra thu tu hanh dong chuyen nuoc
    int no_action = 0;

    while (!empty_Stack(stackPrint)) {
        printf("\nAction %d: %s", no_action,
               action[top(stackPrint)->no_function]);
        print_State(top(stackPrint)->state);
        pop(&stackPrint);
        no_action++;
    }
}

int main() {
    State cur_state = {0, 0};
    Node* p = DFS_Algorithm(cur_state);
    print_WaysToGetGoal(p);
    return 0;
}

