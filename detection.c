#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define PROCESS_NO 5
#define RESOURCE_NO 3
#define MAX_RESOURCE 3


void printTable(int** allocation, int** request, int* available){
    printf("\nprintTable\n");
    int i,j;
    printf("--------------------\n");

    for(i=0;i<PROCESS_NO;i++){
        for(j=0;j<RESOURCE_NO;j++){
            printf("%d ",allocation[i][j]);
            if(j==2){
                printf("\n");
            }
        }
    }

    printf("--------------------\n");

    for(i=0;i<PROCESS_NO;i++){
        for(j=0;j<RESOURCE_NO;j++){
            printf("%d ",request[i][j]);
            if(j==2){ 
                printf("\n");
            }
        }
    }
    printf("--------------------\n");
    for(i=0;i<RESOURCE_NO;i++){
        printf("%d ", available[i]);
    }
    printf("\n--------------------\n");

}

bool findDeadlock(int* finish){
    //printf("findDeadlock\n");
    
    int i;
    for(i=0;i<PROCESS_NO;i++){
        if(finish[i]==0){
            return true;
        }
    }
    
    return false;
}
void resetDeadlock(int* finish){
    int i;
    for(i=0;i<PROCESS_NO;i++){
        if(finish[i]==-1){
            finish[i]=0;
        }
    }
}
bool checkDeadlock(int* finish){
    int i;
    for(i=0;i<PROCESS_NO;i++){
        if(finish[i]==-1){
            return true;
        }
    }

    return false;
}

bool detection(int** allocation, int** request, int* available){
    printf("\ndetection\n");
    int i,j;
    int* work = (int*) malloc(sizeof(int)*RESOURCE_NO);
    int* finish = (int*) malloc(sizeof(int)*PROCESS_NO);

    memcpy(work, available, RESOURCE_NO*sizeof(int));
    for(i=0;i<PROCESS_NO;i++){
        for(j=0;j<RESOURCE_NO;j++){
            if(allocation[i][j] != 0){
                finish[i]=0;
                break;
            }
        }
    }

    int idx=0;
    bool checkCycle = findDeadlock(finish);    
    while(findDeadlock(finish)){
        //printf("%d ", idx);
        
        if(finish[idx]==1){
            idx++;
            continue;
        }
        
        for(i=0;i<RESOURCE_NO;i++){
            if (request[idx][i] > work[i]){
                finish[idx]=-1;
                break;
            }
            if(i==RESOURCE_NO-1){
                printf("%d ",idx);
                for(j=0;j<RESOURCE_NO;j++){
                    work[j]+=allocation[idx][j];
                    finish[idx]=1;
                }
                idx=-1;
                resetDeadlock(finish);
            }
        }
        
        checkCycle = findDeadlock(finish);
        idx++;
    }
    
    
    

    printf("\nAfter work done----------------------\n");
    for(i=0;i<RESOURCE_NO;i++){
        printf("%d ",work[i]);
    }
    printf("\nAfter finish done----------------------\n");
    for(i=0;i<PROCESS_NO;i++){
        printf("%d ",finish[i]);
    }
    printf("\n----------------------\n");

    bool result = checkDeadlock(finish);
    
    free(work);
    free(finish);

    return result;
}


void yieldProcess(int** allocation, int* need, int* yield, int idx){
    printf("yield\n");
    int i,j;
    for(i=0;i<PROCESS_NO;i++){
        if (i!=idx){
            for(j=0;j<RESOURCE_NO;j++){
                if(allocation[i][j] < need[j]){
                    break;
                }
                if(j==RESOURCE_NO-1){
                    yield[i]+=1;
                }
            }
        }
        
    }
}
void initializeArray(int*array, int size){
    int k;
    for(k=0;k<size;k++){
        array[k]=0;
    }

}

void recovery(int** allocation,int** request,int* available){
    printf("Recovery\n");
    int i,j,k;
    int* yield = (int*) malloc(sizeof(int)*PROCESS_NO);
    int* need = (int*) malloc(sizeof(int)*RESOURCE_NO);

    /*
    for(k=0;k<RESOURCE_NO;k++){
        need[k]=0;
    }
    for(k=0;k<PROCESS_NO;k++){
        yield[k]=0;
    }
    */
    initializeArray(need,RESOURCE_NO);
    initializeArray(yield,PROCESS_NO);

    for(i=0;i<PROCESS_NO;i++){
        for(j=0;j<RESOURCE_NO;j++){
            if(request[i][j] > available[j]){
                need[j]=request[i][j]-available[j];
            }
        }
        
        for(k=0;k<RESOURCE_NO;k++){
            printf("%d ", need[k]);
        }printf("\n");
        
        yieldProcess(allocation, need, yield, i);
        initializeArray(need,RESOURCE_NO);
        /*
        for(k=0;k<RESOURCE_NO;k++){
            need[k]=0;
        }
        */
    }
    
    for(i=0;i<PROCESS_NO;i++){
        printf("%d ", yield[i]);
    }
    


    free(yield);
    free(need);
}


void assignResource(int** allocation, int** request, int* available){
    printf("\nassignResource\n");
    int i,j;
    int* resouceMax = (int*) malloc(sizeof(int)*RESOURCE_NO);
    for(i=0;i<PROCESS_NO;i++){
        for(j=0;j<RESOURCE_NO;j++){
            int resouce=rand()%MAX_RESOURCE;
            allocation[i][j]=resouce;
            //request[i][j]=rand()%MAX_RESOURCE;
            resouceMax[j]+=resouce;
        }
    }
    
    for(i=0;i<RESOURCE_NO;i++){
        int resource=rand()%MAX_RESOURCE;
        available[i]=resource;
        resouceMax[i]+=resource;
    }
    printf("\n----------------\n");
    for(i=0; i<RESOURCE_NO;i++){
        printf("%d ", resouceMax[i]);
    }
    printf("\n----------------\n");

    for(i=0;i<PROCESS_NO;i++){
        for(j=0;j<RESOURCE_NO;j++){
            int max=resouceMax[j]-3;
            if(max > 0){
                request[i][j]=rand()%max;
            }else{
                int min=1;
                request[i][j]=rand()%min;
            }
        }
    }
}
 
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
    
    /*
    assignResource(allocation, request, available);

    printTable(allocation, request, available);
    bool isDeadlock = detection(allocation, request, available);

    printf("%s\n", isDeadlock ? "true" : "false");
*/
    
    allocation[0][0]=2;
    allocation[0][1]=0;
    allocation[0][2]=0;

    allocation[1][0]=0;
    allocation[1][1]=2;
    allocation[1][2]=0;

    allocation[2][0]=0;
    allocation[2][1]=1;
    allocation[2][2]=2;

    allocation[3][0]=2;
    allocation[3][1]=2;
    allocation[3][2]=1;

    allocation[4][0]=0;
    allocation[4][1]=1;
    allocation[4][2]=1;

        request[0][0]=0;
    request[0][1]=2;
    request[0][2]=0;

    request[1][0]=1;
    request[1][1]=1;
    request[1][2]=0;

    request[2][0]=1;
    request[2][1]=2;
    request[2][2]=0;

    request[3][0]=1;
    request[3][1]=2;
    request[3][2]=0;

    request[4][0]=1;
    request[4][1]=2;
    request[4][2]=0;

    available[0]=2;
    available[1]=0;
    available[2]=0;

    printTable(allocation, request, available);    
    recovery(allocation,request,available);
    
















    


    for(i=0; i<PROCESS_NO;i++){
        free(allocation[i]);
        free(request[i]);
    }
    free(allocation);
    free(request);
    free(available);
    
}