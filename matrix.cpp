#include<iostream>

typedef int arrT[5];

arrT* makearray()
{
    int a[5] = {1};
    std::cout << &a << std::endl;
    return &a;
}

int main()
{
    arrT *p = makearray();
    std::cout << p << std::endl;
    for(int i = 0; i < 5; i++){
        std::cout << (*p)[0] << std::endl;
    }
    return 0;
}
