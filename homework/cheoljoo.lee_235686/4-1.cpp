#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <vector>

using namespace std;

class V  { virtual void f() { }      };
class NV {         void f() { }      };
class E  {                           };
class F  { virtual void f() final{ } }; // Bonus (unspecified expected output)
class D : public V { void t(){ }      };

template<class T>
class has_virtual_function
{
public :
    class _Derived_ : public T
    {
        virtual void _force_the_vtable(){}
    };
    enum { value = (sizeof(T) == sizeof(_Derived_)) };
};

template<typename T> void foo(T& c) {
    if (has_virtual_function<T>::value)
        cout << typeid(T).name() << " has virtual function" << endl;
    else
        cout << typeid(T).name() << " has not virtual function" << endl;
}

// https://stackoverflow.com/questions/22489073/counting-the-number-of-occurrences-of-a-string-within-a-string
int countSubstring(const std::string& str, const std::string& sub)
{
    if (sub.length() == 0) return 0;
    int count = 0;
    for (size_t offset = str.find(sub); offset != std::string::npos; offset = str.find(sub, offset + sub.length()))
    {
        ++count;
    }
    return count;
}


template<typename T,typename ... Types>
void
cpp_print(T value,Types ... args)
{
    cout << typeid(T).name() << endl;
    const string p = static_cast<const string>(value);
    int sc = countSubstring(p,"%d");
    cout << p << " : " << sc  << endl;
    vector<int> v;
    //(args , ...); // 1, ( 2, 3)
	(v.push_back(args), ...);
	  // v.push_back(1),(v.push_back(2), v.push_back(3))
	for(auto n : v)  cout << n << endl;
    cout << "size:" << v.size() << endl;

    if(sc != v.size())  throw "Different argument's count";
    else printf(value,args...);
}

int main() {
    printf("%d\n", 1, 2, 3); // 인자가 너무 많습니다. 
    printf("%d%d\n", 1); // 인자가 부족 합니다.

    // cpp_print를 만들어 보세요
    try
    {
        cpp_print("%d\n", 1, 2, 3); // 예외가 나오게 하세요 
    }
    catch (...)
    {
        cout << "예외 발생 1" << endl; 
    }

    try
    {
        cpp_print("%d%d\n", 1); // 예외가 나오게 하세요
    }
    catch (...)
    {
        cout << "예외 발생 2" << endl; 
    }

    try
    {
        cpp_print("%d %d\n", 1,2);  // normal
    }
    catch (...)
    {
        cout << "예외 발생 3" << endl; 
    }
}
