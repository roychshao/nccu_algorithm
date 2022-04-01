
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define max(a,b) (a > b) ? a : b

typedef struct Interval {
    int start;
    int finish;
} itv;

int cmp(const void *a, const void *b) {
    itv *ia = (itv*)a;
    itv *ib = (itv*)b;
    if(ia->finish > ib->finish)
        return 1;
    else
        return -1;
}

int main() {
    int project_num = 0;
    itv temp[1000];
    int work_start_day, work_finish_day;
    scanf("%d", &project_num);
    for(int i = 0; i < project_num; ++i) {
        scanf("%d%d", &temp[i].start, &temp[i].finish);
    }
    scanf("%d%d", &work_start_day, &work_finish_day);
    int idx = 0;
    itv v[1000];
    for(int i = 0; i < project_num; ++i) {
        // if the project's time interval is in the working interval
        if(temp[i].start >= work_start_day && temp[i].finish <= work_finish_day) {
            v[idx] = temp[i];
            idx++;
        }
    }
    // 小到大排序
    qsort(v, idx, sizeof(itv), cmp);

    int prev[1000];
    for(int i = 0; i < idx; ++i) {
        int pretmp = -1;
        for(int j = i - 1; j >= 0; --j) {
            if(v[i].start >= v[j].finish) {
                pretmp = j;
                break;
            }
        }
        prev[i] = pretmp;
        //printf("%d ", prev[i]);
    }
    int dp[1000];
    dp[0] = v[0].finish - v[0].start;
    for(int i = 1; i < idx; ++i) {
        dp[i] = max(dp[i-1], dp[prev[i]] + (v[i].finish - v[i].start));
    }
    printf("%d", dp[idx-1]);
    return 0;
}
