#include <iostream>
#include <typeinfo>

using namespace std;

template<typename T> void foo(T& a)
{
    cout << typeid(T).name() << endl;
}
int main()
{
    int c = 10;
    const int& r = c;
    foo(r);
}
