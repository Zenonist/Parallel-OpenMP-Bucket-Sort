#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10000
#define NoOfThreads 4

int main(){
    int unsortedarray[N];
    for (int x = 0;x < N;x++){
        unsortedarray[x] = rand() % 10000;
    }
    int array[N];
    int temparray[N];
    int indexing = 0;
    int tid;
    int ax, i;
    int start;
    int end;
    int count;
    #pragma omp parallel
    // for shared(temparray,indexing) num_threads(NoOfThreads)
    {
        tid = omp_get_thread_num();
        start = tid * N;
        if (tid == NoOfThreads - 1){
            end = (tid + 1) * N;
        }else{
            end = (tid + 1) * N - 1;
        }
        i = 0;
        for (ax = 0; ax < N; ax++){
            if (unsortedarray[ax] >= start && unsortedarray[ax] < end){
                array[i] = unsortedarray[ax];
                i++;
            }
        }
        
        for (int ay = i + 1;ay < N;ay++){
            array[i] = 999999;  
        }
        
        #pragma omp barrier
            int temp = -1;
            for (int x = 0; x < i - 1; x++){
                for (int y = x + 1; y < i; y++){
                    if (array[x] > array[y]){
                        temp = array[x];
                        array[x] = array[y];
                        array[y] = temp;
                    }
                }
            }
        
        
        #pragma omp critical 
        // private(tid,array,i)
            count = 0;
            for (int x = indexing; x < indexing + i;x++){
                temparray[x] = array[count];
                count++;
            }
    }
    for (int x = 0; x < N;x++){
        printf("%d", temparray[x]);
    }
}