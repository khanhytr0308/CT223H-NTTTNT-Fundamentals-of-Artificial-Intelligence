#include <stdio.h>
#include <stdlib.h>

#define tankcapacity_X 9
#define tankcapacity_Y 4
#define empty 0
#define goal 6
#define Maxlength 100

//Khai bao cau truc trang thai
typedef struct {
    int x; //Luong nuoc trong binh x
    int y; //Luong nuoc trong binh y
} State;

//Khai bao max min
int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

//Khoi tao trang thai binh X = 0 va Y = 0
void makeNullState(State *state) {
    state->x = 0;
    state->y = 0;
}

//In trang thai
void print_State(State state) {
    printf("\n   X:%d --- Y:%d", state.x, state.y);
}

//Ham kiem tra trang thai muc tieu
int goalcheck(State state) {
    return (state.x == goal || state.y == goal);
}

//Lam day nuoc binh X
int pourWaterFullX(State cur_state, State *result) {
    if (cur_state.x < tankcapacity_X) {
        result->x = tankcapacity_X;
        result->y = cur_state.y;
        return 1;
    }
    return 0;
}

//Lam day nuoc binh Y
int pourWaterFullY(State cur_state, State *result) {
    if (cur_state.y < tankcapacity_Y) {
        result->y = tankcapacity_Y;
        result->x = cur_state.x;
        return 1;
    }
    return 0;
}

//Ham lam rong nuoc trong X
int pourWaterEmptyX(State cur_state, State *result) {
    if (cur_state.x > 0) {
        result->x = empty;
        result->y = cur_state.y;
        return 1;
    }
    return 0;
}

//Ham lam rong nuoc trong binh Y
int pourWaterEmptyY(State cur_state, State *result) {
    if (cur_state.y > 0) {
        result->y = empty;
        result->x = cur_state.x;
        return 1;
    }
    return 0;
}

//Chuyen nuoc tu binh X sang binh Y
int pourWaterXY(State cur_state, State *result) {
    if (cur_state.x > 0 && cur_state.y < tankcapacity_Y) {
        result->x = max(cur_state.x - (tankcapacity_Y - cur_state.y), empty);
        result->y = min(cur_state.x + cur_state.y, tankcapacity_Y);
        return 1;
    }
    return 0;
}

//Chuyen nuoc tu binh Y sang binh X
int pourWaterYX(State cur_state, State *result) {
    if (cur_state.y > 0 && cur_state.x < tankcapacity_X) {
        result->y = max(cur_state.y - (tankcapacity_X - cur_state.x), empty);
        result->x = min(cur_state.y + cur_state.x, tankcapacity_X);
        return 1;
    }
    return 0;
}

//Goi cac phep toan tren trang thai
int call_operator(State cur_state, State *result, int option) {
    switch (option) {
        case 1: return pourWaterFullX(cur_state, result);
        case 2: return pourWaterFullY(cur_state, result);
        case 3: return pourWaterEmptyX(cur_state, result);
        case 4: return pourWaterEmptyY(cur_state, result);
        case 5: return pourWaterXY(cur_state, result);
        case 6: return pourWaterYX(cur_state, result);
        default: printf("Error calls operators");
            return 0;
    }
}

//Hang chuoi de in ra ten cac hanh dong
const char* action[] = {
    "First State",
    "pour Water Full X",
    "pour Water Full Y",
    "pour Water Empty X",
    "pour Water Empty Y",
    "pour Water X to Y",
    "pour Water Y to X"
};

/*-----------------------------------------------------------------*/
//Khai bao cau truc nut (dinh) de dung cay tim kiem
typedef struct Node {
    State state;
    struct Node* Parent;
    int no_function;
} Node;

/*-----------------------------------------------------------------*/
//Khai bao cau truc Queue (hang doi) de luu trang thai duyet
typedef struct {
    Node* Elements[Maxlength];
    int front, rear;
} Queue;

//Khoi tao hang doi rong
void makeNull_Queue(Queue *queue) {
    queue->front = -1;
    queue->rear = -1;
}

//Kiem tra xem hang doi co rong hay khong
int empty_Queue(Queue queue) {
    return queue.front == -1;
}

//Kiem tra xem hang doi co day hay khong
int full_Queue(Queue queue) {
    return ((queue.rear - queue.front + 1) % Maxlength) == 0;
}

//Tra ve phan tu dau hang doi
Node* get_Front(Queue queue) {
    if (empty_Queue(queue)) {
        printf("Queue is empty");
        return NULL;
    } else {
        return queue.Elements[queue.front];
    }
}

//Xoa bo mot phan tu khoi hang doi
void del_Queue(Queue *queue) {
    if (!empty_Queue(*queue)) {
        if (queue->front == queue->rear)
            makeNull_Queue(queue);
        else
            queue->front = (queue->front + 1) % Maxlength;
    } else {
        printf("Error, Delete");
    }
}

//Them phan tu vao hang doi
void push_Queue(Node* x, Queue *queue) {
    if (!full_Queue(*queue)) {
        if (empty_Queue(*queue))
            queue->front = 0;
        queue->rear = (queue->rear + 1) % Maxlength;
        queue->Elements[queue->rear] = x;
    } else {
        printf("Error, Push");
    }
}

//So sanh hai trang thai
int compareStates(State state1, State state2) {
    return (state1.x == state2.x && state1.y == state2.y);
}

//Tim trang thai trong Queue Open/Close
int find_State(State state, Queue openQueue) {
    while (!empty_Queue(openQueue)) {
        if (compareStates(get_Front(openQueue)->state, state))
            return 1;
        del_Queue(&openQueue);
    }
    return 0;
}

//Thuat toan duyet theo chieu rong (BFS)
Node* BFS_Algorithm(State state) {
    //Khai bao hai hang doi Open va Close
    Queue Open_BFS;
    Queue Close_BFS;
    makeNull_Queue(&Open_BFS);
    makeNull_Queue(&Close_BFS);

    //Tao nut trang thai cha
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->Parent = NULL;
    root->no_function = 0;
    push_Queue(root, &Open_BFS);

    while (!empty_Queue(Open_BFS)) {
        //Lay mot dinh trong hang doi
        Node* node = get_Front(Open_BFS);
        del_Queue(&Open_BFS);
        push_Queue(node, &Close_BFS);

        //Kiem tra xem dinh lay ra co phai trang thai muc tieu?
        if (goalcheck(node->state))
            return node;

        int opt;
        //Goi cac phep toan tren trang thai
        for (opt = 1; opt <= 6; opt++) {
            State newstate;
            makeNullState(&newstate);

            if (call_operator(node->state, &newstate, opt)) {
                //Neu trang thai moi sinh ra da ton tai thi bo qua
                if (find_State(newstate, Close_BFS) ||
                    find_State(newstate, Open_BFS))
                    continue;

                //Neu trang thai moi chua ton tai thi them vao hang doi
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->state = newstate;
                newNode->Parent = node;
                newNode->no_function = opt;
                push_Queue(newNode, &Open_BFS);
            }
        }
    }
    return NULL;
}

/*-----------------------------------------------------------------*/
//Khai bao cau truc Stack de ho tro in ket qua
typedef struct {
    Node* Elements[Maxlength];
    int Top_idx;
} Stack;

//Khoi tao ngan xep rong
void makeNull_Stack(Stack *stack) {
    stack->Top_idx = Maxlength;
}

//Kiem tra ngan xep rong
int empty_Stack(Stack stack) {
    return stack.Top_idx == Maxlength;
}

//Dua 1 phan tu len dinh ngan xep
void push(Node* x, Stack *stack) {
    if (stack->Top_idx == 0) {
        printf("Error! Stack is full");
    } else {
        stack->Top_idx -= 1;
        stack->Elements[stack->Top_idx] = x;
    }
}

//Tra ve phan tu tren dinh ngan xep
Node* top(Stack stack) {
    if (!empty_Stack(stack))
        return stack.Elements[stack.Top_idx];
    return NULL;
}

//Xoa phan tu tai dinh ngan xep
void pop(Stack *stack) {
    if (!empty_Stack(*stack))
        stack->Top_idx += 1;
    else
        printf("Error! Stack is empty");
}

//In ket qua chuyen nuoc de dat den trang thai muc tieu
void print_WaysToGetGoal(Node* node) {
    Stack stackPrint;
    makeNull_Stack(&stackPrint);

    //Duyet nguoc ve nut parent de
    while (node->Parent != NULL) {
        push(node, &stackPrint);
        node = node->Parent;
    }
    push(node, &stackPrint);

    //In ra thu tu hanh dong chuyen nuoc
    int no_action = 0;
    while (!empty_Stack(stackPrint)) {
        printf("\nAction %d: %s", no_action, action[top(stackPrint)->no_function]);
        print_State(top(stackPrint)->state);
        pop(&stackPrint);
        no_action++;
    }
}

int main() {
    State cur_state = {0, 0};
    Node* p = BFS_Algorithm(cur_state);
    print_WaysToGetGoal(p);
    printf("\n");
    return 0;
}
