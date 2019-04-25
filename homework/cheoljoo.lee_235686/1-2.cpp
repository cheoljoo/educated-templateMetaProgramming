#include <iostream>
#include <typeinfo>
#include <type_traits>

using namespace std;

// 3.4일 경우는 여기서 error이므로 SNINAE으로 후보군에서 제외
// 없으면 default void
template<typename T>
typename enable_if< is_integral<T>::value >::type	
Sum(T from,T to)
{
    cout << typeid(T).name() << endl;

    T sum = 0;
    for(T i=from;i <= to ;i++){
        sum += i;
    }
    cout << sum << endl;

    return;
}

int main()
{
    int n1 = 5, n2 = 10;
    short s1 = 5, s2 = 10;
    double d1 = 3.4, d2 = 1.2;
    Sum(n1, n2); // ok
    Sum(s1, s2); // ok
    Sum(d1, d2); // error가 나와야 합니다
}
