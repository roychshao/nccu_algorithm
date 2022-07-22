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
    struct Edge_t* next;
} Edge;


typedef struct Station_t {
    int father;
    int order;
    int line;
    int type;
    Edge* nexts;
} Station;

typedef struct StaDis_t {
    Station* station;
    int dis;
} StaDis;

StaDis distance[1000050];
bool visited[1000050] = { false };
int staidx = 0;

void pushEdge(Station* node) {
    //printf("pushEdge\n");
    //printf("staidx: %d\n", staidx);
    for (int i = 0; i < staidx; ++i) {
        //printf("i: %d ",i);
        Station* tmp = distance[i].station;
        if (node->father == tmp->father) {
            Edge* e_tmp = (Edge*)malloc(sizeof(Edge)), *e_tmp2 = (Edge*)malloc(sizeof(Edge));
            // e_tmp表示從node到tmp
            // e_tmp2表示從tmp到node
            e_tmp->from = e_tmp2->to = node->order;
            e_tmp->to = e_tmp2->from = tmp->order;
            e_tmp->line = e_tmp->type = e_tmp2->line = e_tmp2->type = -1;
            e_tmp->next = e_tmp2->next = NULL;
            if (node->line == tmp->line && node->type == tmp->type) {
                e_tmp->time = e_tmp2->time = 0;
            }
            else {
                if (tmp->type == BUS)
                    e_tmp->time = 5;
                else if (tmp->type == MRT)
                    e_tmp->time = 10;
                if (node->type == BUS)
                    e_tmp2->time = 5;
                else if (node->type == MRT)
                    e_tmp2->time = 10;
            }
            Edge* t = node->nexts, *p = t;
            
            while (t != NULL) {
                p = t;
                t = t->next;
            }
            t = e_tmp;
            if (p == NULL)
                node->nexts = e_tmp;
            else {
                t = e_tmp;
                p->next = t;
            }
            
            Edge* t2 = tmp->nexts, *p2 = t2;
            while (t2 != NULL) {
                p2 = t2;
                t2 = t2->next;
            }
            if (p2 == NULL)
                tmp->nexts = e_tmp2;
            else {
                t2 = e_tmp2;
                p2->next = t2;
            }
        }
    }
}

bool allVisited() {
    for (int i = 0; i < staidx; ++i) {
        if (visited[i] == false)
            return false;
    }
    return true;
}

int extractMin() {
    int minidx = -1;
    int mindis = MAX_TIME;
    for (int i = 0; i < staidx; ++i) {
        if ((!visited[i]) && (distance[i].dis < mindis)) {
            minidx = i;
            mindis = distance[i].dis;
        }
    }
    //printf("extract: %d\n", minidx);
    return minidx;
}

void decreaseKey(Station* node, int dis) {
    //printf("decrease key\n");
    Edge* e = node->nexts;
    while (e != NULL) {
        distance[e->to].dis = min((e->time + dis), distance[e->to].dis);
        e = e->next;
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
    //printf("dijkstra\n");
    // 將起始站的所有轉點設定距離為0
    for (int i = 0; i < staidx; ++i) {
        if (distance[i].station->father == begin)
            distance[i].dis = 0;
    }
    while (!allVisited()) {
        int idx = extractMin();
        if (idx == -1)
            break;
        Station* tmp = distance[idx].station;
        visited[tmp->order] = true;
        if (tmp->father == end)
            return distance[tmp->order].dis;
        decreaseKey(tmp, distance[tmp->order].dis);
    }
    return -1;
}

int main() {
    char dataType;
    while (true) {
        scanf("%c", &dataType);
        getchar();
        if (dataType == 'E')
            break;
        if (dataType == 'L') {
            // 整個用string讀再改
            char tmp[200];
            fgets(tmp, 200, stdin);
            //printf("tmp :%s\n", tmp);
            int line_order, transtype;
            int pass_num;
            // 讀資料
            if (isalpha(tmp[0])) {
                line_order = (int)(tmp[0] - 'A');
                transtype = MRT;
                if (strlen(tmp) > 5) {
                    pass_num = 100 * (int)(tmp[2] - '0') + 10 * (int)(tmp[3] - '0') + (int)(tmp[4] - '0');
                }
                else if (strlen(tmp) > 4) {
                    pass_num = 10 * (int)(tmp[2] - '0') + (int)(tmp[3] - '0');
                }
                else
                    pass_num = (int)(tmp[2] - '0');
            }
            else {
                transtype = BUS;
                if (isdigit(tmp[1])) {
                    line_order = 10 * (int)(tmp[0] - '0') + (int)(tmp[1] - '0');
                    if (strlen(tmp) > 6) {
                        pass_num = 100 * (int)(tmp[3] - '0') + 10 * (int)(tmp[4] - '0') + (int)(tmp[5]);
                    }
                    else if (strlen(tmp) > 5) {
                        pass_num = 10 * (int)(tmp[3] - '0') + (int)(tmp[4] - '0');
                    }
                    else {
                        pass_num = (int)(tmp[3] - '0');
                    }
                }
                else {
                    line_order = (int)(tmp[0] - '0');
                    if (strlen(tmp) > 5) {
                        pass_num = 100 * (int)(tmp[2] - '0') + 10 * (tmp[3] - '0') + (tmp[4] - '0');
                    }
                    if (strlen(tmp) > 4) {
                        pass_num = 10 * (int)(tmp[2] - '0') + (int)(tmp[3] - '0');
                    }
                    else {
                        pass_num = (int)(tmp[2] - '0');
                    }
                }
            }
            for (int i = 0; i < pass_num; ++i) {
                Station* fstation = (Station*)malloc(sizeof(Station));
                Station* tstation = (Station*)malloc(sizeof(Station));
                Edge* e = (Edge*)malloc(sizeof(Edge));
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
                fstation->order = staidx;
                fstation->line = line_order;
                fstation->type = transtype;
                fstation->nexts = NULL;
                // to-station
                tstation->father = to;
                tstation->order = staidx + 1;
                tstation->line = line_order;
                tstation->type = transtype;
                tstation->nexts = NULL;
                // edge
                e->from = fstation->order;
                e->to = tstation->order;
                e->line = line_order;
                e->time = time;
                e->type = transtype;
                e->next = NULL;
                // connect from and to
                fstation->nexts = e;
                pushEdge(fstation);
                distance[staidx++].station = fstation;
                pushEdge(tstation);
                distance[staidx++].station = tstation;
            }
        }
        else if (dataType == 'Q') {
            for (int i = 0; i < staidx; ++i) {
                distance[i].dis = MAX_TIME;
                visited[i] = false;
            }
            int begin, end;
            scanf("%d", &begin);
            getchar();
            scanf("%d", &end);
            getchar();
            //printf("begin: %d,end: %d\n", begin, end);
            int ans = dijkstra(begin, end);
            printf("%d\n", ans);
        }
    }
    /*
    for(int i = 0; i <= staidx; ++i) {
        Station* tmp = distance[i].station;
        if (tmp == NULL)
            continue;
        printf("order: %d\n",tmp->order);
        Edge* e = tmp->nexts;
        while(e != NULL) {
            printf("  from: %d,to: %d, time: %d, line: %d,type: %d\n",e->from, e->to, e->time, e->line, e->type);
            e = e->next;
        }
    }
    */
    return 0;
}
