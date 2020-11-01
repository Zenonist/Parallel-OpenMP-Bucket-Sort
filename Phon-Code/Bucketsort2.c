#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100
#define NoOfThreads 4

int main(){
    int unsortedarray[N];
    for (int x = 0;x < N;x++){
        unsortedarray[x] = rand() % 10000;
    }
    int temparray[N];
    int indexing = 0;
    int count;
    #pragma omp parallel shared(temparray,indexing) num_threads(NoOfThreads) 
    {
        int tid = omp_get_thread_num();
        int start = tid * (10000 / NoOfThreads);
        int end;
        int array[N];
        if (tid == NoOfThreads - 1){
            end = (tid + 1) * (10000 / NoOfThreads);
        }else{
            end = (tid + 1) * (10000 / NoOfThreads) - 1;
        }
        
        //printf("%d %d %d \n",start,end,tid);
        int i = 0;
        #pragma omp private(i,array)
            
            for (int ax = 0; ax < N; ax++){
                if (unsortedarray[ax] >= start && unsortedarray[ax] <= end){
                    array[i] = unsortedarray[ax];
                    //printf("%d-%d \n",array[i],tid);
                    i++;
                }
            }
            
            //printf("*%d %d*\n",i,tid);
        
            for (int ay = i;ay < N;ay++){
                //printf("*%d-%d* \n",ay,tid);
                array[ay] = 99999;  
                //printf("%d-%d-%d \n",array[ay],ay,tid);
            }
            
            for (int x = 0; x < i; x++){
                printf("%d-%d-%d \n",array[x],x,tid);
            }
        // ! This port belows is not finished
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
            count = 0;
            for (int x = indexing; x < indexing + i;x++){
                temparray[x] = array[count];
                count++;
            }
    }
    /*for (int x = 0; x < N;x++){
        printf("%d", temparray[x]);
    }*/
}