#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <algorithm> // Thu vien cho std::sort

using namespace std;

#define ROWS 3
#define COLS 3
#define EMPTY 0
#define MAX_OPERATOR 4
#define Maxlength 500

const char* action[] = {
    "First State", 
    "Move cell EMPTY to UP", 
    "Move cell EMPTY to DOWN", 
    "Move cell EMPTY to LEFT", 
    "Move cell EMPTY to RIGHT"
};

// Khai bao cau truc trang thai
typedef struct {
    int eightPuzzel[ROWS][COLS];
    int emptyRow;
    int emptyCol;
} State;

// Khai bao cau truc Node cho cay tim kiem
typedef struct Node {
    State state;
    struct Node* parent;
    int no_function;
    int heuristic;
} Node;

// In trang thai
void printState(State state) {
    printf("\n-----------\n");
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            printf("| %d ", state.eightPuzzel[row][col]);
        }
        printf("|\n-----------\n");
    }
}

// So sanh 2 trang thai
int compareStates(State state1, State state2) {
    if (state1.emptyRow != state2.emptyRow || state1.emptyCol != state2.emptyCol)
        return 0;
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (state1.eightPuzzel[row][col] != state2.eightPuzzel[row][col])
                return 0;
        }
    }
    return 1;
}

// Kiem tra trang thai muc tieu
int goalcheck(State state, State goal) {
    return compareStates(state, goal);
}

// Hanh dong 1: Di chuyen o trong LEN TREN
int upOperator(State state, State *result) {
    *result = state;
    int empRowCurrent = state.emptyRow;
    int empColCurrent = state.emptyCol;
    if (empRowCurrent > 0) {
        result->emptyRow = empRowCurrent - 1;
        result->emptyCol = empColCurrent;
        result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent - 1][empColCurrent];
        result->eightPuzzel[empRowCurrent - 1][empColCurrent] = EMPTY;
        return 1;
    }
    return 0;
}

// Hanh dong 2: Di chuyen o trong XUONG DUOI
int downOperator(State state, State *result) {
    *result = state;
    int empRowCurrent = state.emptyRow;
    int empColCurrent = state.emptyCol;
    if (empRowCurrent < ROWS - 1) {
        result->emptyRow = empRowCurrent + 1;
        result->emptyCol = empColCurrent;
        result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent + 1][empColCurrent];
        result->eightPuzzel[empRowCurrent + 1][empColCurrent] = EMPTY;
        return 1;
    }
    return 0;
}

// Hanh dong 3: Di chuyen o trong SANG TRAI
int leftOperator(State state, State *result) {
    *result = state;
    int empRowCurrent = state.emptyRow;
    int empColCurrent = state.emptyCol;
    if (empColCurrent > 0) {
        result->emptyRow = empRowCurrent;
        result->emptyCol = empColCurrent - 1;
        result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent][empColCurrent - 1];
        result->eightPuzzel[empRowCurrent][empColCurrent - 1] = EMPTY;
        return 1;
    }
    return 0;
}

// Hanh dong 4: Di chuyen o trong SANG PHAI
int rightOperator(State state, State *result) {
    *result = state;
    int empRowCurrent = state.emptyRow;
    int empColCurrent = state.emptyCol;
    if (empColCurrent < COLS - 1) {
        result->emptyRow = empRowCurrent;
        result->emptyCol = empColCurrent + 1;
        result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent][empColCurrent + 1];
        result->eightPuzzel[empRowCurrent][empColCurrent + 1] = EMPTY;
        return 1;
    }
    return 0;
}

// Goi hanh dong theo option (1 -> 4)
int callOperators(State state, State *result, int opt) {
    switch (opt) {
        case 1: return upOperator(state, result);
        case 2: return downOperator(state, result);
        case 3: return leftOperator(state, result);
        case 4: return rightOperator(state, result);
        default: return 0;
    }
}

// Ham Heuristic 1: Dem so o sai khac
int heuristic1(State state, State goal) {
    int count = 0;
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (state.eightPuzzel[row][col] != EMPTY && state.eightPuzzel[row][col] != goal.eightPuzzel[row][col]) {
                count++;
            }
        }
    }
    return count;
}

// Ham Heuristic 2: Khoang cach Manhattan
int heuristic2(State state, State goal) {
    int count = 0;
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (state.eightPuzzel[row][col] != EMPTY) {
                for (int row_g = 0; row_g < ROWS; row_g++) {
                    for (int col_g = 0; col_g < COLS; col_g++) {
                        if (state.eightPuzzel[row][col] == goal.eightPuzzel[row_g][col_g]) {
                            count += abs(row - row_g) + abs(col - col_g);
                            col_g = COLS;
                            row_g = ROWS;
                        }
                    }
                }
            }
        }
    }
    return count;
}

// Tim trang thai trong vector Open hoac Close
Node* find_State(State state, vector<Node*> v, vector<Node*>::iterator *position) {
    vector<Node*>::iterator it = v.begin();
    if (v.size() == 0) return NULL;
    while (it != v.end()) {
        if (compareStates((*it)->state, state)) {
            *position = it;
            return *it;
        }
        it++;
    }
    return NULL;
}

// Tieu chi so sanh de sort vector giam dan (nut co heuristic nho hon se o cuoi vector)
bool compareHeuristic(Node* a, Node* b) {
    return a->heuristic > b->heuristic;
}

// Thuat toan Best-First Search
Node* best_first_search(State state, State goal) {
    vector<Node*> Open_BFS;
    vector<Node*> Close_BFS;

    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;
    root->heuristic = heuristic1(root->state, goal); // Co the doi thanh heuristic2

    Open_BFS.push_back(root);

    while (!Open_BFS.empty()) {
        // Lay phan tu co heuristic nho nhat tu cuoi vector (do da sort giam dan)
        Node* node = Open_BFS.back();
        Open_BFS.pop_back();

        Close_BFS.push_back(node);

        if (goalcheck(node->state, goal)) {
            return node;
        }

        for (int opt = 1; opt <= MAX_OPERATOR; opt++) {
            State newstate;
            if (callOperators(node->state, &newstate, opt)) {
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->state = newstate;
                newNode->parent = node;
                newNode->no_function = opt;
                newNode->heuristic = heuristic1(newstate, goal); // Co the doi thanh heuristic2

                vector<Node*>::iterator pos_Open, pos_Close;
                Node* nodeFoundOpen = find_State(newstate, Open_BFS, &pos_Open);
                Node* nodeFoundClose = find_State(newstate, Close_BFS, &pos_Close);

                if (nodeFoundOpen == NULL && nodeFoundClose == NULL) {
                    Open_BFS.push_back(newNode);
                } 
                else if (nodeFoundOpen != NULL && nodeFoundOpen->heuristic > newNode->heuristic) {
                    Open_BFS.erase(pos_Open);
                    Open_BFS.push_back(newNode);
                } 
                else if (nodeFoundClose != NULL && nodeFoundClose->heuristic > newNode->heuristic) {
                    Close_BFS.erase(pos_Close);
                    Open_BFS.push_back(newNode);
                }
            }
        }
        // Sap xep lai Open_BFS
        sort(Open_BFS.begin(), Open_BFS.end(), compareHeuristic);
    }
    return NULL;
}

// In chuoi hành dong va cac trang thai den muc tieu
void print_WaysToGetGoal(Node* node) {
    vector<Node*> vectorPrint;
    while (node != NULL) {
        vectorPrint.push_back(node);
        node = node->parent;
    }

    int no_action = 0;
    for (int i = vectorPrint.size() - 1; i >= 0; i--) {
        printf("\nAction %d: %s", no_action, action[vectorPrint.at(i)->no_function]);
        printState(vectorPrint.at(i)->state);
        no_action++;
    }
}

int main() {
    // Khoi tao trang thai bat dau
    State state;
    state.emptyRow = 1;
    state.emptyCol = 1;
    state.eightPuzzel[0][0] = 3; state.eightPuzzel[0][1] = 4; state.eightPuzzel[0][2] = 5;
    state.eightPuzzel[1][0] = 1; state.eightPuzzel[1][1] = 0; state.eightPuzzel[1][2] = 2;
    state.eightPuzzel[2][0] = 6; state.eightPuzzel[2][1] = 7; state.eightPuzzel[2][2] = 8;

    // Khoi tao trang thai muc tieu
    State goal;
    goal.emptyRow = 0;
    goal.emptyCol = 0;
    goal.eightPuzzel[0][0] = 0; goal.eightPuzzel[0][1] = 1; goal.eightPuzzel[0][2] = 2;
    goal.eightPuzzel[1][0] = 3; goal.eightPuzzel[1][1] = 4; goal.eightPuzzel[1][2] = 5;
    goal.eightPuzzel[2][0] = 6; goal.eightPuzzel[2][1] = 7; goal.eightPuzzel[2][2] = 8;

    printState(state);

    Node* result = best_first_search(state, goal);

    if (result != NULL) {
        print_WaysToGetGoal(result);
    } else {
        printf("\nKhong tim thay duong di toi muc tieu!\n");
    }

    return 0;
}