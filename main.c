//
//  main.c
//  Knapsack
//
//  Created by shijia hu on 5/4/17.
//  Copyright © 2017 shijia hu. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


struct Pack{
    int profit;
    int weight;
    double fraction;
    
}item[10];

int B[10][100];
int X[10];
int maxprofit;
int num;
//int bound;
int include[10];
int bestset[10];
int flag[10][100]={0};
int solution;
int tempP1,tempW1,tempW2 = 0;

int bruteForce(int n,int W){
    int temp,tempP,tempW;
    tempP1 = 0;
    tempW1 = 0;
    int maxP = 0;
    for(int num=0;num<pow(2,n);num++) //each num is a solution
    {
        temp = num;
        tempW = 0;
        tempP = 0;
        for(int i=0;i<n;i++) //binary n bit
        {
            if(temp%2==1){
                tempW+=item[i].weight;
                tempP+=item[i].profit;
            }
            temp = temp/2;          //binary
        }
        
        
        if(tempW<=W){
            if(tempP>maxP){
                maxP=tempP;
                solution = num;
            }
        }
    }
    
    return maxP;
}

//void exchangeSort(int n){
//    int i,j;
//    for (i = 0; i < n - 1; i++) {
//        for (j = i+1; j<n ; j++ ) {
//            if (item[j].fraction>item[j+1].fraction) {
//                exchange(&item[j],&item[j+1]);
//            }
//        }
//    }
//}

int RDynamicP(int n, int w){

//    for (int w = 0; w<W; w++) {
//        B[0][w] = 0;
//        //flag[0][w] = 0;
//    }
//    for (int k = 1; k<=n; k++) {
//        B[k][0] = 0;
//        for (int w = 1; w<=W; w++) {
//            if ((item[k-1].weight<=w) && (B[k-1][w-item[k-1].weight]+item[k-1].profit > B[k-1][w])){
//                B[k][w] = B[k-1][w-item[k-1].weight] + item[k-1].profit;
//                //flag[k][w] = 1;
//            }else B[k][w] = B[k-1][w];
//        }
//    }
    if (n == 0 || w<=0) {
        return 0;
    }else{
        for (int i = n-1; i>=0; i--) {
            if (item[i].weight > w) {
                flag[n][w] = 0;
                return RDynamicP(n-1, w);
            }else{
                
                int tmp1 = RDynamicP(n-1, w);
                int tmp2 = item[i].profit + RDynamicP(n-1, w-item[i].weight);
                
                if (tmp1 > tmp2) {
                    flag[n][w] = 0;
                    B[n][w] = tmp1;
                    return tmp1;
                }else{
                    flag[n][w] = 1;
                    B[n][w] = tmp2;
                    return tmp2;
                }
            }
        }
    }
    

    
    return 0;
}

int KWF2(int i, int weight, int profit, int n, int W){
    int bound = profit;
    for (int j = i; j<=n; j++) {
        X[j] = 0;
    }
    float sbx;
    while (weight < W && i<=n) {
        if (weight + item[i-1].weight <= W) {
            //X[i] = 1;
            weight = weight + item[i-1].weight;
            bound = bound + item[i-1].profit;
        }else{
            sbx = (float)(W-weight)/(float)item[i-1].weight;
            weight = W;
            bound = bound + item[i-1].profit * sbx;
        }
        i += 1;
    }
    return bound;
}

int Promising(int i, int profit, int W, int weight, int n){
    if (weight >= W) {
        return 0;
    }
    int bound = KWF2(i+1, weight, profit, n, W);
    
    printf("(%d, %d, %d)\n",weight,profit,bound);
    return (bound > maxprofit);
}

void knapsack(int i, int profit, int weight, int W,int n){
    if (weight <= W && profit > maxprofit) {
        maxprofit = profit;
        num = i;
        //bestset[i] = include[i];
        for (int j = 1; j<=n; j++) {
            bestset[j] = include[j];
        }
    }
    if (Promising(i, profit, W, weight, n)) {
        include[i+1] = 1;
        knapsack(i+1, profit + item[i].profit, weight + item[i].weight, W, n);
        include[i+1] = 0;
        knapsack(i+1, profit, weight, W, n);
    }
    
}

void Knapsack(int W, int n){
    num = 0;
    maxprofit = 0;
    int tempweight = 0;
    
    knapsack(0, 0, 0, W, n);
    printf("\nThe final solution(selected items) is: \n\n");
    printf("Item:    Weight    Profit\n\n");
    for (int i = 1; i<=num; i++) {
        if (bestset[i]==1) {
            tempweight += item[i-1].weight;
            printf("%d         %d        %d\n",i+1,item[i-1].weight, item[i-1].profit);
        }
    }
    printf("\nThe max profit is : %d\n", maxprofit);
    printf("The max weight is : %d\n\n", tempweight);
}

int main(int argc, const char * argv[]) {
    srand(time(0));
    int n;
    n = rand() % (8-4+1)+4;
    printf("Selected original items: %d\n\n",n);
    printf("Item:    Weight    Profit    Fraction(Profit/Weight)\n\n");
    //srand(time(0));
    int totalW = 0;
    int totalP = 0;
    for (int i = 0; i<n; i++) {
        
        item[i].profit = rand() % (30-10+1)+10;
        item[i].weight = rand() % (20-5+1)+5;
        item[i].fraction = (double)item[i].profit/item[i].weight;
        totalW += item[i].weight;
        totalP += item[i].profit;
    }
    for (int i = 0; i<n; i++) {
        printf("%d         %d        %d         %f\n",i+1,item[i].weight, item[i].profit, item[i].fraction);
    }
    printf("\n");
    int W = 0.6*totalW;
    
    n=5;
    
    W = 13;
    item[0].profit = 20;
    item[1].profit = 30;
    item[2].profit = 35;
    item[3].profit = 12;
    item[4].profit = 3;
    
    item[0].weight = 2;
    item[1].weight = 5;
    item[2].weight = 7;
    item[3].weight = 3;
    item[4].weight = 1;
    
    item[0].fraction = 10;
    item[1].fraction = 6;
    item[2].fraction = 5;
    item[3].fraction = 4;
    item[4].fraction = 3;
    totalW = 18;
    totalP = 100;
    
    
    printf("total weight is:%d\ntotal profit is:%d\nthe capacity of the knapsack is:%d\n\n"
           ,totalP,totalW,W);
    
    printf("====================================================\n\n");
    printf("1.  Brute Force algorithm:\n\n");
    int maxP = bruteForce(n, W);
    
    printf("The final solution(selected items) is: \n\n");
    printf("Item:    Weight    Profit\n\n");
    for(int i=0;i<n;i++) //binary n bit
    {
        if(solution%2==1){
            tempW1+=item[i].weight;
            printf("%d         %d        %d\n",i+1,item[i].weight, item[i].profit);
        }
        solution = solution/2;          //binary
    }
    
    printf("\nThe max profit is : %d\n", maxP);
    printf("The max weight is : %d\n\n", tempW1);
    
    printf("====================================================\n\n");
    printf("2.  Refinement algorithm of dynamic programming approach:\n\n");
    
    
    printf("The max profit is : %d\n\n", RDynamicP(n, W));
    
    printf("\nThe final solution(selected items) is: \n\n");
    printf("Item:    Weight    Profit\n\n");
    int ssb, ssk;
    ssb = n; ssk =W;
    for (int i = 1; i<=n; i++) {
        if (flag[ssb][ssk]==1) {
            tempW2 += item[ssb-1].weight;
            printf("%d         %d        %d\n",ssb,item[ssb-1].weight, item[ssb-1].profit);
            ssk -=item[ssb-1].weight;
        }
        ssb--;
    }
    printf("\nThe max weight is : %d\n\n", tempW2);
    printf("The matrix B is:\n\n");
    for (int i = 0; i<=n; i++) {
        for (int j = 0; j<=W; j++) {
            printf("%d ",B[i][j]);
        }
        printf("\n");
    }
      int sb, sk;
    for(sb = n; sb >=1; sb--)
    {
        printf("The %d th row entries:", sb);
        for(sk = 0; sk <=W; sk++)
        {
            if(B[sb][sk]!=0)
                printf("{%d, %d} ",sb, sk);
        }
        printf("\n");
    }
    
    
    
    
    //printf("\nThe max profit is : %d\n", maxprofit);
    
    printf("\n");
    
    
    
    printf("====================================================\n\n");
    printf("3.  Backtracking algorithm:\n\n");
    for(int i=0;i<n-1;i++)
    {
        for(int j=i+1;j<n;j++)
        {
            if(item[j].fraction>item[i].fraction)
            {
                struct Pack temp;
                temp=item[i];
                item[i]=item[j];
                item[j]=temp;
            }
        }
    }
    printf("sorted items:\n");
    printf("Item:    Weight    Profit\n\n");
    for (int i = 0; i<n; i++) {
        printf("%d         %d        %d \n",i+1,item[i].weight, item[i].profit);
    }
    printf("\n");
    printf("(weight, profit, bound) for each node:\n");
    Knapsack(W, n);
    
    
    return 0;
}
