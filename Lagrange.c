// Coded by ScratchyCode
// Interpolazione polinomiale di Lagrange
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "tinyexpr.h"

#define PASSO 0.001
#define DIM 1024

double pLagrange(double x, double xint[], double fint[], int N);
double erroreRel(double valore, double stima);
void controlloGnuplot(void);

int main(){
    long long int i, N;
    double x, min, max, valore, stima, errore, maxErrore=-1E12, maxPunto;
    double *xint, *fint;
    te_variable vars[] = {{"x",&x}};
    char espressione[DIM];
    
    controlloGnuplot();
    
    printf("\nInserisci la funzione f(x) interpolante: ");
    fgets(espressione,DIM,stdin);
    printf("\nInserisci il numero di punti noti del dominio della f(x): ");
    scanf("%lld",&N);
    
    // alloco dinamicamente la memoria necessaria
    xint = calloc(N,sizeof(double));
    fint = calloc(N,sizeof(double));
    if(xint == NULL || fint == NULL){
        perror("\nError");
        exit(1);
    }
    
    printf("\nInserisci l'inf dell'intervallo su cui interpolare la f(x): ");
    scanf("%lf",&min);
    printf("\nInserisci il sup dell'intervallo su cui interpolare la f(x): ");
    scanf("%lf",&max);
    if(min > max){
        printf("\nSei coglione o cosa?\n");
        exit(1);
    }
    
    // prendo i punti del dominio noti
    for(i=0; i<N; i++){
        printf("\nInserisci il punto noto x%lld: ",i);
        scanf("%lf",&xint[i]);
    }
    
    // inizializzo il parser
        int err;
        te_expr *n = te_compile(espressione,vars,1,&err);
    
    // genero il vettore immagine della funzione
    for(i=0; i<N; i++){
        if(n){
            x = xint[i];
            fint[i] = te_eval(n);
        }else{
            printf("\t%*s^\nErrore",err-1,"");
            exit(1);
        }
    }
    
    // apro i file su cui scrivere i risultati
    FILE *pf = fopen("info.txt","w");
    FILE *polin = fopen("interpolazione.dat","w");
    FILE *funz = fopen("funzione.dat","w");
    if(pf == NULL || polin == NULL || funz == NULL){
        perror("\nError");
        exit(1);
    }
    
    // valuto l'interpolazione
    for(x=min; x<=max; x+=PASSO){
        valore = te_eval(n);
        stima = pLagrange(x,xint,fint,N);
        errore = erroreRel(valore,stima);
        if(errore > maxErrore){
            maxErrore = errore;
            maxPunto = x;
        }
        // stampo su file le info dell'interpolazione e i punti per il plotting
        fprintf(pf,"x = %.5lf\t\tf(x) = %.5lf\t\tpLagrange(x) = %.5lf\t\terrore = %.3lf%%\n",x,valore,stima,errore*100);
        fflush(pf);
        fprintf(funz,"%lf %lf\n",x,valore);
        fflush(funz);
        fprintf(polin,"%lf %lf\n",x,stima);
        fflush(polin);
    }
    
    fprintf(pf,"\nErrore massimo = %.3lf%% nel punto x = %.5lf\n",maxErrore*100,maxPunto);
    
    // chiudo tutti i file e libero la memoria
    fclose(pf);
    fclose(polin);
    fclose(funz);
    free(xint);
    free(fint);
    te_free(n);
    
    printf("\nTerminato.\n");

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

double erroreRel(double valore, double stima){
    return fabs((stima - valore) / valore);
}

void controlloGnuplot(void){
    char path[] = "/usr/bin/gnuplot";
    
    FILE *pf = fopen(path,"r");
    if(pf == NULL){
        fclose(pf);
        printf("\nImpossibile trovare gnuplot.\nAssicurarsi di averlo installato eseguendo: sudo apt-get install gnuplot\n");
        exit(1);
    }else{
        fclose(pf);
        return ;
    }
}
