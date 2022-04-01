#include <stdio.h>
#include <string.h>

int main() {
    int arr[100010];
    int temp, idx = 0, Sum = 0;
    while(scanf("%d", &temp) != EOF){
        arr[idx] = temp, idx++, Sum += temp;
    }
    int max = 0, sum = 0;
    for(int i=0; i<idx; ++i){
        if((sum + arr[i]) >= 0){
            sum += arr[i];
            if(sum > max)
                max = sum;
        }else
            sum = 0;
    }
    int min = Sum;
    sum = 0;
    for(int i=0; i<idx; ++i){
        if((sum + arr[i]) < 0){
            sum += arr[i];
            if(sum < min)
                min = sum;
        }else
            sum = 0;
    }
    if(max > (Sum - min))
        printf("%d", max);
    else
        printf("%d", (Sum - min));
    return 0;
}
