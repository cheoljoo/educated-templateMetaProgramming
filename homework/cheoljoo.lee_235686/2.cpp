#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <vector>

using namespace std;

template<typename T,size_t N>
double
average(T (&x)[N])
{
    double sum = 0.0;
    cout << typeid(T).name() << " size:" << N << endl;
    for(auto n:x){
        sum += n;
        cout << " " << n ;
    }
    cout << endl;
    return sum / N;
}

template<typename T>
double
average(const T& x)
{
    double sum = 0;
    size_t cnt = 0;
    cout << typeid(T).name() << endl;

    typename T::const_iterator pos;  // iterator to iterate over x
    typename T::const_iterator end(x.end());  // end position

    for (pos=x.begin(); pos!=end; ++pos) {
        cout << *pos << ' ';
        sum += static_cast<double>(*pos);
        cnt++;
    }
    cout << endl;

    return sum / cnt;
}


int main() {
    int x[10] = { 1,2,3,4,5,6,7,8,9,10 }; 
    vector<int> v = { 1,2,3,4,5,6,7,8,9,10 };

    cout << average(x) << endl;
    cout << average(v) << endl;
}
