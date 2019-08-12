#include<iostream>

using namespace std;

typedef int arrT[5];
arrT* makearray()
{
    int a[5] = {1};
    cout << &a << endl;
    return &a;
}

int main()
{
    arrT *p = makearray();
    cout << p << endl;
    for(int i = 0; i < 5; i++){
        cout << (*p)[0] << endl;
    }
    return 0;
}