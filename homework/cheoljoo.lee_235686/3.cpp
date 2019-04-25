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

int main() {
    V v;
    NV nv;
    E e;
    F f;
    D d;

    cout << "class V  { virtual void f() { }      } => ";
    foo(v);
    cout << "class NV {         void f() { }      } => ";
    foo(nv);
    cout << "class E  {                           } => ";
    foo(e);
    cout << "class F  { virtual void f() final{ } } => ";
    foo(f);
    cout << "class D : public V { void t(){ }     } => ";
    foo(d);
}
