#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

typedef struct wood_t {
    int start;
    int end;
} wood;

typedef struct pair_t {
    int order;
    int step;
} pair;

// global variables
int first_wood;
int last_wood;
bool intersection[1005][1005];
wood* arr;
pair* queue;
int queue_head = 0;
int queue_tail = 0;
int wood_num;

bool intersect(int i, int j) {
    if(arr[i].end <= arr[j].start || arr[i].start >= arr[j].end ) {
        return false;
    }
    return true;
}

void push(pair* queue, int num, int steps) {
    queue[queue_tail].order = num;
    queue[queue_tail].step = steps;
    queue_tail++;
}

void pop(pair* queue) {
    queue[queue_head].order = -1;
    queue_head++;
}

pair front(pair* queue) {
    return queue[queue_head];
}

bool empty(pair* queue) {
    for(int i = queue_head; i < queue_tail; ++i) {
        if(queue[i].order != -1)
            return false;
    }
    return true;
}

void clear(pair* queue) {
    for(int i = 0; i < 3000; ++i)
        queue[i].order = -1;
}

void printQueue() {
    for(int i = queue_head; i < queue_tail; ++i) {
        printf("%d ", queue[i].order);
    }
    printf("\n");
}

int BFS(int start_wood) {
    if(start_wood == last_wood)
        return 0;
    queue = (pair*) malloc (3000 * sizeof(pair));
    bool* visited = (bool*) malloc (wood_num * sizeof(bool));
    for(int i = 0; i < wood_num; ++i)
        visited[i] = false;
    clear(queue);
    visited[start_wood] = true;
    push(queue, start_wood, 0);
    //printQueue();
    while(!empty(queue)) {
        pair temp = front(queue);
        for(int i = 0; i < wood_num; ++i) {
            if(intersection[temp.order][i] && i == last_wood)
                return temp.step + 1;
            else if(intersection[temp.order][i] && !visited[i]) {
                push(queue, i, temp.step + 1);
                visited[i] = true;
            } else
                continue;
            //printQueue();
        }
        pop(queue);
        //printf("pop %d\n", temp.order);
    }
    return -1; // signify error
}

int main() {
    scanf("%d", &wood_num);
    arr = (wood*) malloc (wood_num * sizeof(wood));
    int i = 0;
    while(i < wood_num) {
        scanf("%d %d", &arr[i].start, &arr[i].end);
        // build intersection
        for(int j = i-1; j >= 0; --j) {
            if(intersect(i, j)) {
                intersection[i][j] = true;
                intersection[j][i] = true;
            } else {
                intersection[i][j] = false;
                intersection[j][i] = false;
            }
        }
        i++;
    }
    /*for(int i = 0; i < wood_num; ++i) {
        for(int j = 0; j < wood_num; ++j) {
            printf("%d ", intersection[i][j]);
        }
        printf("\n");
    }
    printf("\n");*/
    first_wood = 0, last_wood = wood_num - 1;
    int ans = BFS(first_wood);
    printf("%d\n", ans);
    return 0;
}
