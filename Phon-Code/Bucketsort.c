#include <omp.h>
#include <stdio.h>

#define N 1000
#define NofThread 4

int main(){
    int unsortedarray[N];
    int ax;
    for (int x = 0;x < N;x++){
        unsortedarray[x] = rand() % 10000;
    }
    int chunkSize = N / NofThread;
    // ! need to increase by 1 for the processor that need to store one more element
    int array[chunkSize + 1];
    #pragma omp parallel private(ax,array) num_threads(NofThread){
        int tid = omp_get_thread_num();
        int start = tid * chunkSize;
        if (tid == NofThread - 1){
            int end = (tid + 1) * chunkSize;
        }else{
            int end = (tid + 1) * chunkSize - 1;
        }
        int i = 0;
        for (ax = 0; ax < N; ax++){
            if (unsortedarray[ax] >= start && unsortedarray[ax] <= end){
                array[i] = unsortedarray[ax];
                i++;
            }  
        }
        // * find the size of array since [int = 4 bytes long] -> array[0] because we need to divide by 4 bytes
        size_t arraysize = sizeof(array) /sizeof(array[0]);
        int temp = -1;
        for (int x = 0; x < arraysize - 1;x++){
            for (int y = x + 1; y < arraysize;y++){
                if (array[x] > array[y]){
                    temp = array[x];
                    array[x] = array[y];
                    array[y] = temp;
                }
            }
        }
        // TODO find the way to put sorted array in list [indexing problem]
    }
}