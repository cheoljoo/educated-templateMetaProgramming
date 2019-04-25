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

#define METHOD 1

#if METHOD == 1
// best link => https://caiorss.github.io/C-Cpp-Notes/CPP-template-metaprogramming.html#org3bbb4bc
// answer link => https://stackoverflow.com/questions/5970333/how-to-determine-if-a-c-class-has-a-vtable

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
#endif


#if METHOD == 2
#define  has_virtual_function is_polymorphic
#endif

template<typename T> void foo(T& c) {
    if (has_virtual_function<T>::value)
        cout << typeid(T).name() << " has virtual function" << endl;
    else
        cout << typeid(T).name() << " has not virtual function" << endl;
}

int main() {
    V v;
    NV nv;
    E e;
    F f;
    D d;

    foo(v);
    foo(nv);
    foo(e);
    foo(f);
    foo(d);
}
