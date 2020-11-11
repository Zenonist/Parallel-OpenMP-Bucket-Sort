#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int cmpfunc (const void * a, const void * b) {
    // If it is -1 means a < b
    // If it is 0 means a == b
    // If it is 1 means a > b
    return ( *(int*)a - *(int*)b );
}


int main(){
    int Nchoice , N, NoOfThreads;
    // User choose the size of the array
    printf("Enter the number of threads (1. N = 10000, 2. N = 100000, 3. N = 1000000): ");
    scanf("%d" , &Nchoice);
    // If User choose the wrong choice then set amount of array as -1 for avoiding the error
    if (Nchoice == 1){
        N = 10000;
    }else if (Nchoice == 2){
        N = 100000;
    }else if (Nchoice == 3){
        N = 1000000;
    }else{
        N = -1;
    }
    // User choose the amount of threads
    printf("Enter the amount of threads to process: ");
    scanf("%d",&NoOfThreads);
    // If Thread is equal to 0 then set as -1 for avoiding the error
    if (NoOfThreads <=  0){
        NoOfThreads = -1;
    }
    // If Thread equals to -1 and Size of array equal to -1 then stop running the code
    if (N != -1 && NoOfThreads != -1){
        //Create array to sort the unsortedarray
        int unsortedarray[N];
        //Put the random number into the array
        for (int x = 0;x < N;x++){
            unsortedarray[x] = rand() % 10000;
        }
        // Create the array size of N [The one that user chooses]
        // We use this because we can't declare array with size of 1 million
        static int* sortedarray;
        sortedarray = (int*)malloc(N * sizeof(int));
        // This array uses to store the amount of elements in each bucket
        int indexing[NoOfThreads];
        // This array uses to store start time of each process
        double wtime[NoOfThreads];
        //Start the parallel step
        #pragma omp parallel shared(indexing,wtime) num_threads(NoOfThreads) 
        {
            // Variable to store if of process
            int tid = omp_get_thread_num();
            // Get the start time of each process then store it in array
            double starttime = omp_get_wtime();
            wtime[tid] = starttime;
            // Calculate the number that each process or bucket should take in
            int start = tid * (10000 / NoOfThreads);
            int end;
            if (tid == NoOfThreads - 1){
                end = (tid + 1) * (10000 / NoOfThreads);
            }else{
                end = (tid + 1) * (10000 / NoOfThreads) - 1;
            }
            // Create the local array to store the number for sorting.
            int array[N];
            // The variable that stores the index that process should start putting in the sortedarray
            int index = 0;
            // Variable that stores the number of elements in the bucket
            int i = 0;
            // Private variable and array to keep those things separate from each thread
            #pragma omp private(i,array)
                // Transfer the element from unsortedarray to local array[Private array of each thread] based on the start and end of each thread
                for (int ax = 0; ax < N; ax++){
                    if (unsortedarray[ax] >= start && unsortedarray[ax] <= end){
                        array[i] = unsortedarray[ax];
                        i++;
                    }
                }
            // Put the amount of elements in the bucket to the indexing array for calculating the index of each thread
            indexing[tid] = i;
            // Use barrier to make all threads wait for each other
            #pragma omp barrier
                // Sort the local array by using quick sort function [libray function]
                qsort(array,i,sizeof(int),cmpfunc);
            // Calculate the index that each thread should start putting their elements into the sorted array
            for (int x = 0; x < tid;x++){
                index = index + indexing[x];
            }
            // Variable to get the element of local array
            int count = 0;
            // Private the variable count to keep it separate from each other
            #pragma omp private(count) 
                for (int x = index; x < index + i;x++){
                    //printf("**%d-%d-%d-%d-%d**\n",x,count,index,index + i,tid);
                    sortedarray[x] = array[count];
                    count++;
                }
            // Get the finish time of that time
            double endtime = omp_get_wtime();
            // Calculate the used time of each threat and replace it in the array
            wtime[tid] = endtime - wtime[tid];
        }
        // Print out the time of each process [Avg and Total time]
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
        return 0;
    }else{
        printf("Error: Invalid Input\n");
        return 0;
    }
}