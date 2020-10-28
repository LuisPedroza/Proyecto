#include<stdio.h>

void imprime(int a[], int tam){
    for(int i = 0 ; i < tam ; ++i){
        printf("%d ", a[i]);
    }
    printf("\n");
}

void algo(int a[], int tam){
    for(int i = 0 ; i < tam ; ++i){
        a[i] += 1;
    }    
}

int main(){

    int n;
    scanf("%d",&n);

    int arr[n];
    for(int i = 0 ; i < n ; ++i){
        scanf("%d",&arr[i]);
    }
    
    imprime(arr, n);
    algo(arr, n);
    imprime(arr, n);    

    return 0;

}
