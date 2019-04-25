#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <vector>

using namespace std;

struct NullType {};

template <class T, class U> struct Typelist {
    typedef T Head;
    typedef U Tail;
};

template <
typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType, typename T16 = NullType, typename T17 = NullType, typename T18 = NullType >
struct MakeTypelist
{
    private:
        typedef typename MakeTypelist<T2, T3, T4, T5, T6, T7, T8, T9, T10,T11, T12, T13, T14, T15, T16, T17, T18 >::Result TailResult;
    public:
        typedef Typelist<T1, TailResult> Result;
};

template<> struct MakeTypelist<>
{
    typedef NullType Result;
};

#if 0
int main()
{
    typedef MakeTypelist<int , char , double>::Result mm;
    cout << typeid(mm).name() << endl;

}
#endif

template <class TList> struct Length;
template <> struct Length<NullType>
{
    enum { value = 0 };
};
template <class T, class U> struct Length< Typelist<T, U> > {
    enum { value = 1 + Length<U>::value };
};

#if 0
int main() 
{
    typedef MakeTypelist<int, char, double>::Result myType; 
    cout << Length<myType>::value << endl; //3
}
#endif


template <class TList, class T> struct Append;

template <> struct Append<NullType, NullType>
{
    typedef NullType Result;
};

template <class T> struct Append<NullType, T> {
    typedef Typelist<T, NullType> Result;
};

template <class Head, class Tail>
struct Append<NullType, Typelist<Head, Tail> > {
    typedef Typelist<Head, Tail> Result; 
};

template <class Head, class Tail, class T> struct Append<Typelist<Head, Tail>, T>
{
    typedef Typelist<Head, typename Append<Tail, T>::Result> Result;
};


#if 0
int main()
{
    typedef MakeTypelist<int, char, double>::Result myType_1; 

    // myType 끝에 short를 추가한 타입을 만듭니다.
    typedef Append<myType_1, short>::Result myType_2; 
    cout << typeid(myType_2).name() << endl;

    // myType2 2개를 결합한 타입을 만듭니다.
    typedef Append<myType_2, myType_2>::Result myType_3; 
    cout << Length<myType_3>::value << endl; // 8
}
#endif

template <class TList,size_t N> struct TypeAt;
template <size_t N> struct TypeAt<NullType,N>
{
    typedef NullType Result;
    
};
template <class T,class U> struct TypeAt<Typelist<T,U>,0>
{
    typedef T Result;
    
};
template <class T, class U, size_t N> 
struct TypeAt< Typelist<T, U> , N> 
{
    typedef typename TypeAt<U,N-1>::Result Result;
};


// ----------------------------
template <class TList,typename K> struct IndexOf;
template <typename K> struct IndexOf<NullType,K>
{
    enum { value = 0 };
};
template <class U, class K> 
struct IndexOf<Typelist<K,U>,K>
{
    enum { value = IndexOf<NullType,K>::value };
};
template <class T,class U, class K> 
struct IndexOf<Typelist<T,U>,K>
{
    enum { value = 1 + IndexOf<U,K>::value };
};


// ----------------------------
template <class TList, typename K> struct Erase;

template <typename K> struct Erase<NullType, K>
{
    typedef NullType Result;
};

template <class Tail,typename K>
struct Erase<Typelist<K, Tail> , K> {
    typedef Tail Result; 
};

template <class Head, class Tail, typename K> struct Erase<Typelist<Head, Tail>, K>
{
    typedef Typelist<Head, typename Erase<Tail, K>::Result> Result;
};


// ----------------------------
template <class TList, typename K1, typename K2> struct Replace;

template <typename K1,typename K2> struct Replace<NullType, K1, K2>
{
    typedef NullType Result;
};

template <class Tail,typename K1, typename K2>
struct Replace<Typelist<K1, Tail> , K1, K2> {
    typedef Typelist<K2, Tail> Result; 
};

template <class Head, class Tail, typename K1, typename K2> 
struct Replace<Typelist<Head, Tail>, K1, K2>
{
    typedef Typelist<Head, typename Replace<Tail, K1, K2>::Result> Result;
};


int main()
{
    typedef MakeTypelist<int, char, double, short>::Result myType;
    // (1) TypeAt : N번째 타입이 무슨 타입인지 알아내는 템플릿 입니다. 
    TypeAt<myType, 1>::Result n; // n은 char 타입이어야 합니다.
    {
        TypeAt<myType, 0>::Result n0;  cout << typeid(n0).name() << endl;
        TypeAt<myType, 1>::Result n1;  cout << typeid(n1).name() << endl;
        TypeAt<myType, 2>::Result n2;  cout << typeid(n2).name() << endl;
        TypeAt<myType, 3>::Result n3;  cout << typeid(n3).name() << endl;
    }

    // (2) IndexOf : 특정 타입이 몇번째 있는지 찾아 내는 템플릿입니다. 
    cout << IndexOf<myType, double>::value << endl; // 2가 나와야 합니다.

    // (3) Erase : 특정 타입을 제거한 새로운 TypeList 만들기 
    //최초 발견된것 하나만 제거
    typedef Erase<myType, char>::Result myType2;
    cout << typeid(myType2).name() << endl; // int, double, short 의 TypeList입니다.
    {
        TypeAt<myType2, 0>::Result n0;  cout << typeid(n0).name() << endl;
        TypeAt<myType2, 1>::Result n1;  cout << typeid(n1).name() << endl;
        TypeAt<myType2, 2>::Result n2;  cout << typeid(n2).name() << endl;
        TypeAt<myType2, 3>::Result n3;  cout << typeid(n3).name() << endl;
    }

    // (4) Replace : 특정 타입을 다른 타입으로 교체한 TypeList 만들기
    typedef Replace<myType, char, float>::Result myType3;
    cout << typeid(myType3).name() << endl; // int, float, double, short
    {
        TypeAt<myType3, 0>::Result n0;  cout << typeid(n0).name() << endl;
        TypeAt<myType3, 1>::Result n1;  cout << typeid(n1).name() << endl;
        TypeAt<myType3, 2>::Result n2;  cout << typeid(n2).name() << endl;
        TypeAt<myType3, 3>::Result n3;  cout << typeid(n3).name() << endl;
    }
}


