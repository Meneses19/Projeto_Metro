// Programação 2025/26
// Aula Prática 2A - Matrizes

#include <stdio.h>

void printMat(int mat[][3], int nLin){
    int i, j;
    for(i=0; i<nLin; i++){
        for(j=0; j<3; j++)
            printf("%d\t", mat[i][j]);
        putchar('\n');
    }
}
void preencheMat(int mat[][3], int nLin){
    int i, n;
    for(i=0; i<nLin; i++) {
    printf("\nIntroduzir valor para a linha %d:\n ", i+1);
    scanf("%d",&n);
        mat[i][0]=n;
        mat[i][1]=n*n;
        mat[i][2]=n*n*n;
    }
}

int main(){
    int m1[4][3] = {{1,2,3},{6,7,8},{10,11,12},{20,30,40}};
    int m2[10][3] = {0};

    printMat(m1, 4);

    preencheMat(m2, 10);
    printf("\nMatriz preenchida:\n");
    printMat(m2, 10);

    return 0;
}

