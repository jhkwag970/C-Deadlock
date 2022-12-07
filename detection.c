#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define PROCESS_NO 5
#define RESOURCE_NO 3
#define MAX_RESOURCE 3
#define FINISH 1
#define UNFINISH 0
#define DEADLOCK -1


/*
printTable:
    Print the Currently assigned Allocated Recourse, Request Resource by the processes and Avaiable Resource in the system
params:
    int ** allocation: Resources that are assgined to the processors
    int ** request: Resources that processes need to complete the process
    int* avaiable: All the resources that is not assigned to any processors
*/
void printTable(int** allocation, int** request, int* available){
    printf("\nprintTable\n");
    int i,j;
    int newLine=2;
    printf("----------Allocation----------\n");

    for(i=0;i<PROCESS_NO;i++){
        for(j=0;j<RESOURCE_NO;j++){
            printf("%d ",allocation[i][j]);
            if(j==newLine){
                printf("\n");
            }
        }
    }

    printf("----------Request----------\n");

    for(i=0;i<PROCESS_NO;i++){
        for(j=0;j<RESOURCE_NO;j++){
            printf("%d ",request[i][j]);
            if(j==newLine){ 
                printf("\n");
            }
        }
    }
    printf("----------Available----------\n");
    for(i=0;i<RESOURCE_NO;i++){
        printf("%d ", available[i]);
    }
    printf("\n---------------------------\n");

}

/*
findDeadlock:
    Check whether any of the processor have not assigned the resource
params:
    int * finish: list of processor checklist 0 for unassigned processor, -1 for assinged but cannot be finished, 1 for assigned and finished
Return:
    boolean values whether there is notAssigned processor
*/

bool findDeadlock(int* finish){
    int i;
    for(i=0;i<PROCESS_NO;i++){
        if(finish[i]==UNFINISH){
            return true;
        }
    }
    
    return false;
}
/*
resetDealock:
    Initialize the finsish array that contains the conditon of processor work. If assigned (not satisfy the condtion of finish process), reset to assigned to check again
params:
    int * finish: list of processor checklist 0 for unassigned processor, -1 for assinged but cannot be finished, 1 for assigned and finished
*/
void resetDeadlock(int* finish){
    int i;
    int assigned=-1;
    int notAssigned=0;
    for(i=0;i<PROCESS_NO;i++){
        if(finish[i]==DEADLOCK){
            finish[i]=UNFINISH;
        }
    }
}
/*
checkDeadlock:
    check finish array that contains the conditon of processor work. This method run after detecting the deadlock among the processor 
params:
    int * finish: list of processor checklist 0 for unassigned processor, -1 for assinged but cannot be finished, 1 for assigned and finished
returns
    boolean value true if there is no -1 (processor that is assigned resource but cannot be finished) and false if there is -1
*/

bool checkDeadlock(int* finish){
    int i;
    for(i=0;i<PROCESS_NO;i++){
        if(finish[i]==DEADLOCK){
            return true;
        }
    }

    return false;
}

/*
initializeFinish:
    initialize the finish array with 0
params:
    int ** allocation: Resources that are assgined to the processors
    int * finish: list of processor checklist 0 for unassigned processor, -1 for assinged but cannot be finished, 1 for assigned and finished
*/

void initialzieFinish(int* finish, int** allocation){
    int i,j;
    for(i=0;i<PROCESS_NO;i++){
        for(j=0;j<RESOURCE_NO;j++){
            if(allocation[i][j] != UNFINISH){
                finish[i]=UNFINISH;
                break;
            }
        }
    }
}

/*
printFinish:
    print the values of finish array
parms:
    int * finish: list of processor checklist 0 for unassigned processor, -1 for assinged but cannot be finished, 1 for assigned and finished
*/

void printFinish(int* finish){
    int i;
    printf("\n----------Finish----------\n");
    for(i=0;i<PROCESS_NO;i++){
        printf("Process %d state: %d\n",i,finish[i]);
    }
    printf("\n--------------------------\n");
}

/*
detection:
    detecting the cycle among the processors using the allocation, request, and available.
params:
    int ** allocation: Resources that are assgined to the processors
    int ** request: Resources that processes need to complete the process
    int* avaiable: All the resources that is not assigned to any processors
returns
    boolean value true if there is no -1 (processor that is assigned resource but cannot be finished) and false if there is -1
*/

bool detection(int** allocation, int** request, int* available){
    printf("\n===================================================================================================================================\n");
    printf("===================================================================================================================================\n");
    printf("\ndetection\n");
    printTable(allocation, request, available);
    int i,j;
    int* work = (int*) malloc(sizeof(int)*RESOURCE_NO);
    int* finish = (int*) malloc(sizeof(int)*PROCESS_NO);

    memcpy(work, available, RESOURCE_NO*sizeof(int));
    initialzieFinish(finish, allocation);


    int idx=0;
    bool checkCycle = findDeadlock(finish);    
    while(findDeadlock(finish)){
        
        if(finish[idx]==1){
            idx++;
            continue;
        }
        
        for(i=0;i<RESOURCE_NO;i++){
            if (request[idx][i] > work[i]){
                finish[idx]=DEADLOCK;
                break;
            }
            if(i==RESOURCE_NO-1){
                for(j=0;j<RESOURCE_NO;j++){
                    work[j]+=allocation[idx][j];
                    finish[idx]=1;
                }
                idx=DEADLOCK;
                resetDeadlock(finish);
            }
        }
        
        checkCycle = findDeadlock(finish);
        idx++;
    }

    printFinish(finish);
    bool result = checkDeadlock(finish);
    
    free(work);
    free(finish);

    return result;
}


/*
yieldProcess:
    check whether allocation resource can be sacrificed to fulfile the request
params:
    int ** allocation: Resources that are assgined to the processors
    int * need: resources that each processor need to finish their processes
    int *yield: list of processors that contains number that processes are needed to solve the deadlock
*/
void yieldProcess(int** allocation, int* need, int* yield, int idx){
    int i,j;
    int size = RESOURCE_NO-1;
    for(i=0;i<PROCESS_NO;i++){
        if (i!=idx){
            for(j=0;j<RESOURCE_NO;j++){
                if(allocation[i][j] < need[j]){
                    break;
                }
                if(j==size){
                    yield[i]++;
                }
            }
        }
        
    }
}
/*
initialzieArray:
    initialize the array(yield and need) to have 0 value
params:
    int* array: array that will be passed to initialize the value
    int size: size of array
*/
void initializeArray(int*array, int size){
    int k;
    for(k=0;k<size;k++){
        array[k]=UNFINISH;
    }

}

/*
findVictim:
    find the vicitim among the processes to solve the deadlock
params:
    int *yield: list of processors that contains number that processes are needed to solve the deadlock
returns:
    int that is index of yield which is process number that will be sacrificed 
*/

int findVictim(int* yield){
    int i, max=0, idx=0;
    for(i=0;i<PROCESS_NO;i++){
        if(max < yield[i]){
            max=yield[i];
            idx=i;
        }
    }
    if(max!=0){
        yield[idx]=0;
    }
    return idx;
    
}

/*
initializeNeed:
    intialize need array with the the resouces that current process need (available resource-request resouce)
params:
    int ** allocation: Resources that are assgined to the processors
    int ** request: Resources that processes need to complete the process
    int* avaiable: All the resources that is not assigned to any processors
    int * need: resources that each processor need to finish their processes
    int *yield: list of processors that contains number that processes are needed to solve the deadlock
*/

void initializeNeed(int** allocation, int** request, int* available, int* need, int* yield){
    int i,j;
    for(i=0;i<PROCESS_NO;i++){
        for(j=0;j<RESOURCE_NO;j++){
            if(request[i][j] > available[j]){
                need[j]=request[i][j]-available[j];
            }
        }
        yieldProcess(allocation, need, yield, i);
        initializeArray(need,RESOURCE_NO);
    }
}

/*
recovery:
    if there is deadlock find the victim to resolve the deadlock.
params:
    int ** allocation: Resources that are assgined to the processors
    int ** request: Resources that processes need to complete the process
    int* avaiable: All the resources that is not assigned to any processors
*/

void recovery(int** allocation,int** request,int* available){

    printf("Recovery\n");
    
    int i,j,k;
    int* yield = (int*) malloc(sizeof(int)*PROCESS_NO);
    int* need = (int*) malloc(sizeof(int)*RESOURCE_NO);

    initializeArray(need,RESOURCE_NO);
    initializeArray(yield,PROCESS_NO);

    initializeNeed(allocation, request, available, need, yield);

    while(detection(allocation, request, available)){

        printf("Deadlock Detected\n");

        int victim = findVictim(yield);
        printf("\nVictim is Process: %d\n", victim);

        
        for(i=0;i<RESOURCE_NO;i++){
            request[victim][i] += allocation[victim][i];
            available[i] += allocation[victim][i];
            allocation[victim][i]=0;
        }

    }

    printf("No Deadlock\n");

    free(yield);
    free(need);
}

/*
assignResource:
    assign the random number of resource to the allocation, request, and avaible
params:
    int ** allocation: Resources that are assgined to the processors
    int ** request: Resources that processes need to complete the process
    int* avaiable: All the resources that is not assigned to any processors
*/

void assignResource(int** allocation, int** request, int* available){
    printf("\nassignResource\n");
    int i,j;
    int* resouceMax = (int*) malloc(sizeof(int)*RESOURCE_NO);
    for(i=0;i<PROCESS_NO;i++){
        for(j=0;j<RESOURCE_NO;j++){
            int resouce=rand()%MAX_RESOURCE;
            allocation[i][j]=resouce;
            resouceMax[j]+=resouce;
        }
    }
    
    for(i=0;i<RESOURCE_NO;i++){
        int resource=rand()%MAX_RESOURCE;
        available[i]=resource;
        resouceMax[i]+=resource;
    }

    int smallResource=0;
    for(i=0;i<PROCESS_NO;i++){
        for(j=0;j<RESOURCE_NO;j++){
            int max=resouceMax[j]-3;
            if(max > smallResource){
                request[i][j]=rand()%max;
            }else{
                int min=1;
                request[i][j]=rand()%min;
            }
        }
    }
}

/*
main:
    create the avaialble, allocation, request array with dynamic allocation and run assigneResource to initialize the available, allocation, requset array
    Then, it runs recovery method to find deadlock and recover it if deadlock exists.
    Then, it frees all the dynamic allocated array.
*/
 
int main(){
    srand(time(NULL));
    int i,j;
    int *available = (int*) malloc(sizeof(int)*RESOURCE_NO);
    int **allocation = (int **) malloc(sizeof(int*)*PROCESS_NO);
    int **request = (int **) malloc(sizeof(int*)*PROCESS_NO);
    for(i=0; i<PROCESS_NO;i++){
        allocation[i] = (int*)malloc(sizeof(int)*RESOURCE_NO);
        request[i] = (int*)malloc(sizeof(int)*RESOURCE_NO);
    }
    
    
    assignResource(allocation, request, available);
    
    recovery(allocation,request,available);
    

    for(i=0; i<PROCESS_NO;i++){
        free(allocation[i]);
        free(request[i]);
    }
    free(allocation);
    free(request);
    free(available);
}