// Lagrange's polynomial interpolation
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "tinyexpr.h"

#define STEPS 0.1
#define DIM 1024

double pLagrange(double x, double xint[], double fint[], int N);
double errorRel(double value, double approx);
void controlGnuplot(void);

int main(){
    long long int i, N;
    double x, min, max, value, approx, error, maxerror=-1E12, maxPoint;
    double *xint, *fint;
    te_variable vars[] = {{"x",&x}};
    char expression[DIM];
    
    controlGnuplot();
    
    printf("\nEnter the f(x) function: ");
    fgets(expression,DIM,stdin);
    printf("\nEnter the number of known f(x) domain points: ");
    scanf("%lld",&N);
    
    // dynamically allocate memory
    xint = calloc(N,sizeof(double));
    fint = calloc(N,sizeof(double));
    if(xint == NULL || fint == NULL){
        perror("\nError");
        exit(1);
    }
    
    printf("\nEnter the INF of f(x) interval: ");
    scanf("%lf",&min);
    printf("\nEnter the SUP of f(x) interval: ");
    scanf("%lf",&max);
    if(min > max){
        printf("\nTry again next time lol.\n");
        exit(1);
    }
    
    // taking known domain points
    for(i=0; i<N; i++){
        printf("\nEnter the known point x%lld: ",i);
        scanf("%lf",&xint[i]);
    }
    
    // parsing expression
        int err;
        te_expr *n = te_compile(expression,vars,1,&err);
    
    // create the vector image of the function
    for(i=0; i<N; i++){
        if(n){
            x = xint[i];
            fint[i] = te_eval(n);
        }else{
            printf("\t%*s^\nerror",err-1,"");
            exit(1);
        }
    }
    
    // open files
    FILE *pf = fopen("info.txt","w");
    FILE *polin = fopen("interpolation.dat","w");
    FILE *funz = fopen("function.dat","w");
    if(pf == NULL || polin == NULL || funz == NULL){
        perror("\nError");
        exit(1);
    }
    
    // evaluate the interpolation
    for(x=min; x<=max; x+=STEPS){
        value = te_eval(n);
        approx = pLagrange(x,xint,fint,N);
        error = errorRel(value,approx);
        if(error > maxerror){
            maxerror = error;
            maxPoint = x;
        }
        // print on file the interpolation info and plotting points
        fprintf(pf,"x = %.5lf\t\tf(x) = %.5lf\t\tpLagrange(x) = %.5lf\t\terror = %.3lf%%\n",x,value,approx,error*100);
        fflush(pf);
        fprintf(funz,"%lf %lf\n",x,value);
        fflush(funz);
        fprintf(polin,"%lf %lf\n",x,approx);
        fflush(polin);
    }
    
    fprintf(pf,"\nMax error = %.3lf%% Point x = %.5lf\n",maxerror*100,maxPoint);
    
    // close all files and free the memory
    fclose(pf);
    fclose(polin);
    fclose(funz);
    free(xint);
    free(fint);
    te_free(n);
    
    printf("\nEnd.\n");

    return 0;
}

double pLagrange(double x, double xint[], double fint[], int N){
    long long int i, j;
    double sommatoria=0, produttoria;
    
    for(i=0; i<N; i++){
        produttoria = 1;
        for(j=0; j<N; j++){
            if(i != j){
                produttoria *= (x - xint[j]) / (xint[i] - xint[j]);
            }
        }
        sommatoria += fint[i] * produttoria;
    }
    
    return sommatoria;
}

double errorRel(double value, double approx){
    return fabs((approx - value) / value);
}

void controlGnuplot(void){
    char path[] = "/usr/bin/gnuplot";
    
    FILE *pf = fopen(path,"r");
    if(pf == NULL){
        fclose(pf);
        printf("\nGnuplot not found.\nMake sure you have it installed by running: sudo apt-get install gnuplot\n");
        exit(1);
    }else{
        fclose(pf);
        return ;
    }
}
