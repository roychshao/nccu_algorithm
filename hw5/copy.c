#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define BUS 1
#define MRT 2
#define station_num 1050
#define MAX_TIME 2147483647
#define min(a,b) (a < b) ? a : b

// struct for origin edge
typedef struct Edge_t {
    int from;
    int to;
    int time;
    int line;
    int type;
} Edge;


typedef struct Station_t {
    int father;
    int order;
    int line;
    int type;
    int nextidx;
    Edge nexts[1000050];
    struct Station_t* nsta; // just for construct intersection
} Station;

typedef struct StaDis_t {
    Station* station;
    int dis;
} StaDis;

StaDis distance[1000050];
bool visited[1000050] = { false };
Station* intersection[station_num] = { NULL };
int staidx = 0;

void push(Station* node) {
    // 同樣father的放在同一個linked list
    if(intersection[node->father] == NULL) {
        intersection[node->father] = node;
    }
    else {
        Station* tmp = intersection[node->father];
        Station* pre = tmp;
        while(tmp != NULL) {
            // e_tmp表示從node轉tmp
            // e_tmp2表示從tmp轉node
            Edge e_tmp, e_tmp2;
            e_tmp.from = node->order;
            e_tmp.to = tmp->order;
            e_tmp.line = -1;    // 站內轉乘沒有line
            e_tmp.type = -1;
            // 由type決定轉乘時間
            if(node->line == tmp->line && node->type == tmp->type)
                e_tmp.time = 0;
            else if(tmp->type == BUS)
                e_tmp.time = 5;
            else if(tmp->type == MRT)
                e_tmp.time = 10;

            e_tmp2.from = tmp->order;
            e_tmp2.to = node->order;
            e_tmp2.line = -1;
            e_tmp2.type = -1;
            if(node->line == tmp->line && node->type == tmp->type)
                e_tmp2.time = 0;
            else if(node->type == BUS)
                e_tmp2.time = 5;
            else if(node->type == MRT)
                e_tmp2.time = 10;
            // 站內出口應該雙向
            node->nexts[node->nextidx++] = e_tmp;
            tmp->nexts[tmp->nextidx++] = e_tmp2;
            pre = tmp;
            tmp = tmp->nsta;
        }
        pre->nsta = node;
        tmp = node;
    }
}

bool allVisited() {
    for(int i = 0; i < staidx; ++i) {
        if(visited[i] == false)
            return false;
    }
    return true;
}

Station* extractMin() {
    int minidx = -1;
    int mindis = MAX_TIME;
    for(int i = 0; i < staidx; ++i) {
        if(!visited[i] && distance[i].dis < mindis) {
            minidx = i;
            mindis = distance[i].dis;
        }
    }
    //printf("extract: %d\n", minidx);
    if(minidx == -1 || mindis == MAX_TIME)
        return NULL;
    return distance[minidx].station;
}

void decreaseKey(Station* node, int dis) {
    for(int i = 0; i < node->nextidx; ++i) {
        Edge e_tmp = node->nexts[i];
        distance[e_tmp.to].dis = min((e_tmp.time + dis), distance[e_tmp.to].dis);
    }
    /*
    for(int i = 0; i < 16; ++i) {
        printf("%d ", distance[i].dis);
    }
    printf("\n");
    */
}

// wrong in dijkstra
int dijkstra(int begin, int end) {

    // 將起始站的所有轉點設定距離為0
    Station* tmp = intersection[begin];
    while(tmp != NULL) {
        distance[tmp->order].dis = 0;
        tmp = tmp->nsta;
    }
    while(!allVisited()) {
        tmp = extractMin();
        if(tmp == NULL)
            break;
        visited[tmp->order] = true;
        if(tmp->father == end)
            return distance[tmp->order].dis;
        decreaseKey(tmp, distance[tmp->order].dis);
    }
    return -1;
}

int main() {
    char dataType;
    while(true) {
        scanf("%c",&dataType);
        getchar();
        if(dataType == 'E')
            break;
        if(dataType == 'L') {
            // 整個用string讀再改
            char tmp[10];
            fgets(tmp, 10, stdin);
            //printf("tmp :%s\n", tmp);
            int line_order, transtype;
            int pass_num;
            // 讀資料
            if(isalpha(tmp[0])) {
                line_order = (int) (tmp[0] - 'A');
                transtype = MRT;
                if(strlen(tmp) > 5) {
                    pass_num = 100 * (int) (tmp[2] - '0') + 10 * (int) (tmp[3] - '0') + (int) (tmp[4] - '0');
                }
                else if(strlen(tmp) > 4) {
                    pass_num = 10 * (int) (tmp[2] - '0') + (int) (tmp[3] - '0');
                } else
                    pass_num = (int) (tmp[2] -'0');
            } else {
                transtype = BUS;
                if(isdigit(tmp[1])) {
                    line_order = 10 * (int) (tmp[0] - '0') + (int) (tmp[1] - '0');
                    if(strlen(tmp) > 6) {
                        pass_num = 100 * (int) (tmp[3] - '0') + 10 * (int) (tmp[4] - '0') + (int) (tmp[5]);
                    }
                    else if(strlen(tmp) > 5) {
                        pass_num = 10 * (int) (tmp[3] - '0') + (int) (tmp[4] - '0');
                    } else {
                        pass_num = (int) (tmp[3] - '0');
                    }
                } else {
                    line_order = (int) (tmp[0] - '0');
                    if(strlen(tmp) > 5) {
                        pass_num = 100 * (int) (tmp[2] - '0') + 10 * (tmp[3] - '0') + (tmp[4] -'0');
                    }
                    if(strlen(tmp) > 4) {
                        pass_num = 10 * (int) (tmp[2] - '0') + (int) (tmp[3] - '0');
                    } else {
                        pass_num = (int) (tmp[2] - '0');
                    }
                }
            }
            for(int i = 0; i < pass_num; ++i) {
                Station* fstation = (Station*) malloc (sizeof(Station));
                Station* tstation = (Station*) malloc (sizeof(Station));
                Edge e;
                //Edge* temp2 = (Edge*) malloc (sizeof(Edge));
                int from, to, time;
                scanf("%d", &from);
                getchar();
                scanf("%d", &to);
                getchar();
                scanf("%d", &time);
                getchar();
                //printf("from: %d,to: %d, time: %d\n",from, to,time);
                // from-station
                fstation->father = from;
                fstation->nextidx = 0;
                fstation->order = staidx;
                fstation->line = line_order;
                fstation->type = transtype;
                fstation->nsta = NULL;
                // to-station
                tstation->father = to;
                tstation->nextidx = 0;
                tstation->order = staidx + 1;
                tstation->line = line_order;
                tstation->type = transtype;
                tstation->nsta = NULL;
                // edge
                e.from = fstation->order;
                e.to = tstation->order;
                e.line = line_order;
                e.time = time;
                e.type = transtype;
                // connect from and to
                fstation->nexts[fstation->nextidx++] = e;
                distance[staidx++].station = fstation;
                distance[staidx++].station = tstation;
                push(fstation);
                push(tstation);
            }
        } else if(dataType == 'Q') {
            for(int i = 0; i < staidx; ++i) {
                distance[i].dis = MAX_TIME;
                visited[i] = false;
            }
            int begin, end;
            scanf("%d",&begin);
            getchar();
            scanf("%d",&end);
            getchar();
            //printf("begin: %d,end: %d\n", begin, end);
            int ans = dijkstra(begin, end);
            printf("%d\n", ans);
        }
    }
    /* 
       for(int i = 0; i <= 10; ++i) {
       Station* tmp = intersection[i];
       while(tmp != NULL) {
       printf("order: %d\n",tmp->order);
       for(int j = 0; j < tmp->nextidx; ++j) {
       printf("  from: %d,to: %d, time: %d, line: %d,type: %d\n",tmp->nexts[j].from, tmp->nexts[j].to, tmp->nexts[j].time, tmp->nexts[j].line, tmp->nexts[j].type);
       }
       tmp = tmp->nsta;
       }
       }
       */
    return 0;
}
