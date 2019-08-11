#include<iostream>

using namespace std;

void first_bigger(const int * & p, int val)
{
    while(*p < val){
        p++;
    }
}

int main(int argc, char *argv[])
{
    int nums[] = {1,4,26,64,462,46,46,46,4,6,44};
    const int *res = &nums[0];

    nums[0]++;
    cout << res << endl;
    cout << "before " << *res << endl;
    first_bigger(res, 123);
    cout << "after " << *res << endl;
    return 0;
}