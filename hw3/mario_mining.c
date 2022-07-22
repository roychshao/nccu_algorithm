#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#define max(a,b) (a > b) ? a : b

int maxArea1 = 0;
int maxArea0 = 0;
int area = 0;
int n = 0, m = 0;

// function declaration
void dfs(int i, int j, int** map, int target);

int main() {
    scanf("%d%d", &n, &m);
    //printf("%d %d\n", n,m);
    getchar();  // eliminate '\n'

    // 動態配置二維陣列
    int **map = (int**)malloc((n+5) * sizeof(int*));
    for (int i = 0; i < n+5; ++i) {
        map[i] = (int*)malloc((m+5) * sizeof(int));
    }
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            scanf("%d", &map[i][j]);
            getchar();  // eliminate space and '\n'
        }
    }
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            area = 0;
            if(map[i][j] == 0) {
                dfs(i, j, map, 0);
                maxArea0 = max(maxArea0, area);
            } else if(map[i][j] == 1) {
                dfs(i, j, map, 1);
                maxArea1 = max(maxArea1, area);
            }
        }
    }
    int ans = max(maxArea0, maxArea1);
    printf("%d\n", ans);
    return 0;
}

void dfs(int i, int j, int** map, int target) {
    map[i][j] = -1;
    area++;
    // rightward
    if((j+1) < m && map[i][j+1] == target) {
        dfs(i, j+1, map, target);
    }
    // downward
    if((i+1) < n && map[i+1][j] == target) {
        dfs(i+1, j, map, target);
    }
    // leftward
    if((j-1) >= 0 && map[i][j-1] == target) {
        dfs(i, j-1, map, target);
    }
    // upward
    if((i-1) >= 0 && map[i-1][j] == target) {
        dfs(i-1, j, map, target);
    }
}
