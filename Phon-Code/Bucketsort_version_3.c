#include <omp.h>
#include <stdio.h>
#include <stdlib.h>


int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int main(){
    int Nchoice , N, NoOfThreads;
    printf("Enter the number of threads (1. N = 10000, 2. N = 100000, 3. N = 1000000): ");
    scanf("%d" , &Nchoice);
    if (Nchoice == 1){
        N = 10000;
    }else if (Nchoice == 2){
        N = 100000;
    }else if (Nchoice == 3){
        N = 1000000;
    }else{
        N = -1;
    }
    printf("Enter the amount of threads to process: ");
    scanf("%d",&NoOfThreads);
    if (NoOfThreads <=  0){
        NoOfThreads = -1;
    }
    if (N != -1 && NoOfThreads != -1){
        int unsortedarray[N];
        for (int x = 0;x < N;x++){
            unsortedarray[x] = rand() % 10000;
        }
        static int* temparray;
        temparray = (int*)malloc(N * sizeof(int));
        int count;
        int indexing[NoOfThreads];
        FILE *fptr;
        fptr = fopen("result.txt","w");
        if (fptr == NULL){
            printf("ERROR!");
        }
        fprintf(fptr,"Array size = %d, Amount of threads = %d\n",N,NoOfThreads);
        double wtime[NoOfThreads];
        #pragma omp parallel shared(indexing,wtime) num_threads(NoOfThreads) 
        {
            
            int tid = omp_get_thread_num();
            double starttime = omp_get_wtime();
            wtime[tid] = starttime;
            int start = tid * (10000 / NoOfThreads);
            int end;
            int array[N];
            int index = 0;
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
            
                /*for (int ay = i;ay < N;ay++){
                    //printf("*%d-%d* \n",ay,tid);
                    array[ay] = 99999;  
                    //printf("%d-%d-%d \n",array[ay],ay,tid);
                }*/
                
                /*for (int x = 0; x < i; x++){
                    printf("%d-%d-%d-%d \n",array[x],x,i,tid);
                }*/
            indexing[tid] = i;
            #pragma omp barrier
                /*int temp = -1;
                int count = 0;
                for (int x = 0; x < i - 1; x++){
                    for (int y = x + 1; y < i; y++){
                        if (array[x] > array[y]){
                            temp = array[x];
                            array[x] = array[y];
                            array[y] = temp;
                        }
                        count++;
                        printf("%d-%d**\n",count,tid);
                    }
                }*/
                qsort(array,i,sizeof(int),cmpfunc);

            //printf("test**\n");
            for (int x = 0; x < tid;x++){
                index = index + indexing[x];
            }
            //printf("%d-%d-%d\n",index,i,tid);
            int count = 0;
            //temparray[0] = -2;
            //printf("%d",temparray[0]);
            #pragma omp private(count) 
                for (int x = index; x < index + i;x++){
                    //printf("**%d-%d-%d-%d-%d**\n",x,count,index,index + i,tid);
                    temparray[x] = array[count];
                    count++;
                }
            double endtime = omp_get_wtime();
            wtime[tid] = endtime - wtime[tid];
        }
        for (int x = 0; x < N;x++){
            printf("%d ", temparray[x]);
            fprintf(fptr,"%d\n",temparray[x]);
        }
        float avg_time = 0;
        float finished_time = 0;
        printf("\nArray size = %d, Amount of threads = %d\n",N, NoOfThreads);
        for (int x = 0; x < NoOfThreads;x++){
            printf("t%d -> %f seconds\n",x,wtime[x]);
            avg_time += wtime[x];
            if (finished_time < wtime[x]){
                finished_time = wtime[x];
            }
        }
        printf("avg_time = %f seconds\n", avg_time / NoOfThreads);
        printf("finished_time = %f seconds\n", finished_time);
        fclose(fptr);
    }else{
        printf("Error: Invalid Input\n");
    }
}