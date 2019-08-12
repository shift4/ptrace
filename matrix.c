#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef int ia[8];
/*不允许返回数组
ia get_array()
{
    ia a;
    a = malloc(9 * sizeof(int));
    return a;
}
*/
typedef ia *iap;
iap get_matrix2()
{
    return malloc(8 * sizeof(ia));
}
/*
int **get_matrix()
{
    int **matrix;
    matrix = malloc(8 * sizeof(int *));
    for(int i = 0; i < 8; i++){
        matrix[i] = malloc(16 * sizeof(int));
    }
    return matrix;
}
void free_matrix(int **m)
{
    for(int i = 0; i < 8; i++)
        free(m[i]);
    free(m);
}
*/
void print_matrix(iap m)
{
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            printf("%4d",m[i][j]);
        }
        puts("");
    }
}
void init_matrix(iap m)
{
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            m[i][j] = i * 8 + j;
        }
    }
}
int main()
{
    iap m2 = get_matrix2();
    memset(m2,0,8*sizeof(ia));
    init_matrix(m2);
    print_matrix(m2);
    /*
    puts("---------------");
    int **m = get_matrix();
    for(int i = 0; i < 8; i++){
        printf("%p\n",m[i]);
        for(int j = 0; j < 0x8; j++){
            m[i][j] = 0xffffffff;
        }
    }
    free_matrix(m);
    */
    return 0;
}