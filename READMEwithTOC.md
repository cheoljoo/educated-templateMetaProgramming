
Table of Contents
=================

   * [template basic](#template-basic)
      * [1. template 기본 개념](#1-template-기본-개념)
      * [2. template instantiation #1](#2-template-instantiation-1)
      * [3. template instantiation #2](#3-template-instantiation-2)
      * [4. lazy instantiation](#4-lazy-instantiation)
      * [5. template type deduction](#5-template-type-deduction)
   * [class template](#class-template)
      * [6. generic copy constructor](#6-generic-copy-constructor)
      * [7. template과 friend 함수](#7-template과-friend-함수)
      * [8. typename and template](#8-typename-and-template)
      * [9. template parameter](#9-template-parameter)
      * [10. MISC](#10-misc)
   * [specialization](#specialization)
      * [11. specialization 개념](#11-specialization-개념)
      * [12. specialization 활용](#12-specialization-활용)
      * [13. template meta programming](#13-template-meta-programming)
   * [type traits](#type-traits)
      * [14. type tratis 개념](#14-type-tratis-개념)
      * [15. integral constant](#15-integral-constant)
      * [16. type modification : 기존에 있던 type에 변형된 code](#16-type-modification--기존에-있던-type에-변형된-code)
      * [17. traits 정리](#17-traits-정리)
   * [variadic template : 가변인자 template](#variadic-template--가변인자-template)
      * [18. variadic template #1](#18-variadic-template-1)
      * [19. variadic template #2](#19-variadic-template-2)
      * [20. variadic template 활용 #1](#20-variadic-template-활용-1)
      * [21. variadic template 활용 #2](#21-variadic-template-활용-2)
   * [template design](#template-design)
      * [22. thin template](#22-thin-template)
      * [23. CRTP (Curiously Recurring Template Pattern)](#23-crtp-curiously-recurring-template-pattern)
      * [24. policy base design](#24-policy-base-design)
   * [25. SFINAE](#25-sfinae)
   * [C   20](#c-20)
      * [26. concept (C  20)](#26-concept-c20)
   * [what does this class teach?](#what-does-this-class-teach)

Created by [gh-md-toc](https://github.com/ekalinin/github-markdown-toc)





[MOOC][중급] C++ Template Programming


# template basic
## 1. template 기본 개념
```
template <typename T>    나  template <class T> 나 같은 것이다. 아무것이나 써도 무방
square<int>(3) 대신 compiler가 알때는 square(3) 이라고 써도 무방하다.
```

- 나는 틀만 제공하겠다.  사용자가 결정해라. ``` tempalte <typename T> ```

## 2. template instantiation #1
- template의 타입이 결정되어 실제 코드를 만드는 과정을 인스턴스화(Instantiation)이라고 한다.
- compile 결과 확인 방법 : assembly를 확인    g++  -S 를 붙이면 된다.
- C++ 중간언어 모양으로 보고 싶을 때 : g++ -fdump-tree-original file.cpp ->/ file.cpp.003t.original

- 명시적 인스턴스화
```cpp
tempalate<typename T> T square (T a){ return a*a; }
// 모두 가능
template int square<int>(int);
template int square<>(int);
template int square(int);

// member함수 선언하는 것에서 foo만 instance화 : 전체가 아닌 member 한개만 instance 화 가능
template void Test<int>::foo();
template class Test<int>;
```
- 암시적 인스턴스화 : 위와 같이 명시적으로 instance 화를 시키지 않고 , 바로 함수를 사용하면 compiler에 의해서 instance화가 되어 사용가능한다.
  - class template에서는 사용한 member 함수의 instance 만을 만들어준다. (lazy instantiation)

- class template type dedeuction - C++17
  - 인자를 보고 compiler가 type을 추론
  - class의 type 추론은 C++17부터 가능하다.
```cpp
using namesapce std;
template<typename T> class Vector { 
  T* buff;
  Vector(){}
  Vector(int sz,T v){}
};
// user define deduction guide when Vector v3; fails.  권장하지 않음. 실제로는 error가 더 좋다.
Vector()->Vector<int>;    // 아무것도 없을때 int로... C++17부터 가능
int main(){
  Vector<int> v1(10,3);
  Vector v2(10,3);  // C++17부터 가능  뒤의 3을 보고 T가 int로 유추를 함.
  Vector v3;    // 추론할수 있는 근거가 없다. error
  
  //list<int> s = {1,2,3};
  list s= {1,2,3};  // C++17 style
}
```

## 3. template instantiation #2
- object generator
- 왜 make_pair를 사용하는가?   C++17부터는 make_pair 사용할 필요가 없을 듯
- make_pair같은 것을 "object generator" 라는 기법으로 불린다.  C++17부터는 이런 기법이 줄어들 것이다.
```cpp
template<typename T> void foo(T a){}
template<typename T,typename U> struct pair
{
  T first;
  U second;
  pair(const T& a, const U& b):first(a),second(b) {}
};
 
// 함수 템플릿
template <typename T , typename U> 
pair<T,U> make_pair(const T& a , const U& b)
{
  return pair<T,U>(a,b);
}

int main() {
  pair<int,double> p(1,1.3);
  foo(p);
  
  foo( pair<int,double> p(1,1.3) ); // 임시 객체로 전달
  foo( make_pair(1, 2.3) );  // 함수 텔플릿은 이것을 보고 int와 double version의 pair를 만들어준다.
  
  foo(tuple<int,double,int>(1, 3.4 , 1) );
  foo(make_tuple(1, 3.4 , 1 );
}
```

- identity 라는 template
- error에서와 같이 컴파일러에 의한 타입 추론을 **막는** 테크닉
- identity라는 것을 사용했다면 , 여기서는 인자의 type을 꼭 적어주어야 한다고 생각해야 한다.
```cpp
tempalte <typename T> struct identity {
  typedef T type;
};

template<typename T> void foo(T a){}
template<typename T> void goo(typename identity<T>::type a){}

int main() {
  identity<int>::type n;   // typename identity<T>::type은 T안의 type
  
  foo(3);  // ok : 3을 보고 int 추론 가능
  foo<int>(3);  // ok
  
  goo(3);  // error 
  goo<int>(3);   // ok
}
```

- template 과 함수
- template을 사용할때 주의할 점 : square는 template , square<int> 는 함수
- template에서는 주소를 뽑을 수 없다.
```cpp
#include <iostream>
using namespace std;

template<typename T> T square(a){ return a*a; }

int main(){
  printf("%p\n",&square); // error  : 아직 instance화가 되지 않음.
  printf("%p\n",&square<int>); // ok

  printf("%p\n",static_cast<int(*)(int)>&square); // ok : 최소 정보를 주어서
  
  auto p=&square; //error
  auto p= &square<int>;  // ok
}
```
- template과 다중파일
- .h .cpp 모두 분리하면 , 에러가 나올수 있다.  그러므로 , template에 대한 구현체도 header에 같이 선언해야 문제가 발생하지 않는다.
- template은 소스로 제공하는게 아닌 header로 제공해야 한다.
- class template의 member함수도 같이 넣어야지 다른 파일로 빼면 안된다.


## 4. lazy instantiation
- 아래것은 error가 안 나온다.  이유는 실제로 foo가 사용된 적이 없기 때문이다.
- lazy instantiation : 사용되지 않는 코드는 실제 C++코드로 만들지 않는다.
- 잘못쓰면 bug를 유발할수 있다.
```cpp
template<typename T> class A
{
  T data;
  void foo(T n){ *n =100; }   // 일반적으로 여기서 에러, 그러나, 실제로 foo가 사용된 적이 없기 때문이다. 
};

int main(){
  A<int> a;
}
```

- template에 만들면 resource2가 불리지 않게 된다. 
- template안의 static도 사용하지 않으면 instance화 되지 않는다.
```cpp
struct resource1 { .... }
struct resource2 { ... }
template<typename T> struct Test {
  resource1 res1;
  static resource2 res2;    // 생성자가 불리지를 않음.
};
template <typename T> resource2 Test<T>::rest;  

int main() {
  cout << "main" << endl;
  
  Test t;
}
```

- lazy instantiation #2
```cpp
template <typename T> void foo(T n){  *n = 10; }

int main() {
  if(false){
  	foo(10);
  }
}
```
	- if 문은 실행시간 조건 조사를 하는 것으로 foo(10)을 사용하게 되는 것으로 compile은 일단 해두게 된다. 그러므로 compile error.  최적화에서는 아예 지워질지도 모름
```cpp
template <typename T> void foo(T n){  *n = 10; }

int main() {
  if constexpr (false){
  	foo(10);
  }
}
```
	- C++ 17의 if constexpr 가 나온다.  **constexpr()은 compile 시간 조건문** 이라는 깃이다.  C++17부터는 이것때문에 코드를 짜는게 많이 변경이 된다.
```cpp
template <typename T> void foo(T n,int){
  *n = 10; 
}
template <typename T> void foo(T n,double){
  
}

int main() {
  foo(1,3.4);
}
```
	- 실행시간이면 2개를 모두 만들어야해서 에러가 발생하고 , 컴파일 시간에 결정되면 에러가 발생하지 않는다. 


## 5. template type deduction
- boost type_index : type 조사
- C++ 표준의 typeid() 연사자 사용 : const ,volatile , reference (cvr) 를 조사할수 없다. 그러므로 , boost type_index를 사용하면 type을 정확히 조사할수 있다. 
- type_id_with_cvr<T>().pretty_name() 사용
- boost사용하기 동영상 강좌 참조 
  - const는 앞이나 뒤에 어디에 붙어도 상관없다.
```cpp
#include <iostream>
#include <boost\type_index.hpp>
using namespace std;
using namespace boost::typeindex;

template <typename T> void foo(const T a){
  cout << typeid(T).name() << endl;
  cout << typeid(a).name() << endl;
  cout << type_id_with_cvr<T>().pretty_name() << endl;
  cout << type_id_with_cvr<decltype(a)>().pretty_name() << endl;   // < > 안에는 type만 들어가므로 declaretype을 이용한다.
}
int main() {
  foo(1);  // T : int , a : const int
  foo(3.3);  // T:double , a : const double
}
```

- type deduction rule
- 아래 예제는 모두 int로 결정된다.   함수 인자의 타입과 완전히 동일한 타입으로 결정되지 않는다. 
- 규칙1 : T a   **인자** 가 가진  메모리의 속성(const , volatile , reference)은 제거
  - 함수 인자가 가진 const , volatile , reference을 제거하고 T의 type을 결정
  - 주의! 인자가 가진 const 속성만 제거
```cpp
template<typename T> void foo(T a)
{
  cout << type_id_with_cvr<T>().pretty_name() << endl;
}

int main(){
  int n=0;
  int& r=n;
  const int c = n;
  const int& cr = c;
  
  foo(n);  // T : int
  foo(c);  // T : const int ? int  메모리의 속성(const , volatile , reference)는 제거 하므로 int
  foo(r);  // T : int& ? int   CVR을 제거하므로 int
  foo(cr);	// int
  
  const char* s1 = "hello";   // s1을 따가면 const (따라간다는 것은 pointer)  답 : char const*  (pointer앞쪽에 const이므로 같다.)
  foo(s1);
  
  const char* const s2 = "hello";  // s2에서는 따라가지 전에도 const가 있으므로 답 : char const*  (pointer앞쪽에 const이므로 같다.)
  foo(s2);
}
```
- 규칙2 : T& a   선언이 **참조 타입 인자** 일때  const , volatile는 유지 /  reference은 제거
  - 함수 인자가 가진 reference 속성만 제거하고 T 타입 결정
  - const / volatile 속성은 유지한다.  단 , 템플릿 인자가 const T&일 경우는 함수 인자가 가진 const를 제거하고 T 타입을 결정
```cpp
// 함수 템플릿 인자가 참조 타입일때  T&
template<typename T> void foo(T& a)
{
  cout << type_id_with_cvr<T>().pretty_name() << endl;
  cout << type_id_with_cvr<decltype(a)>().pretty_name() << endl;
}
template<typename T> void goo(const T& a)
{
  cout << type_id_with_cvr<T>().pretty_name() << endl;
  cout << type_id_with_cvr<decltype(a)>().pretty_name() << endl;
}

int main(){
  int n=0;
  int& r=n;
  const int c = n;
  const int& cr = c;
  
  foo(n);  // T : int , a : int&
  foo(c);  // 복사본이 아니고 가리키는 것이므로 T : const int , a : const int&
  foo(r);  // T : int   a: int&    이미 T&으로 되어져있으므로 T는 int가 되어도 무방
  foo(cr);	// T : const int , a : const int&

  goo(n);    // T : int , a : const int&
  goo(c);    // T : int , a : const int&
  goo(r);    // T : int , a : const int&
  goo(cr);   // T : int , a : const int&
}
```
- 규칙3 : template 인자가 forwarding reference일때 ( T&& )
  - lvalue와 rvalue를 모두 전달 받는다. C++ intermediate과정의 perfect forwarding 과정 참조
```cpp
template <typename T> void foo(T&& a) {}
```
- 규칙4 : 배열을 전달받을때 - argument decay 발생
  - "array name" 동영상 시청후 , "argument decay" 동영상 참조
```cpp
int x[3] = {1,2,3};
foo(x); // 배열을 함수 템플릿에 전달할때의 주의할 점.
```


- array name
- 배열의 이름은 배열의 주소가 아니다.
```cpp
int main(){
  int n ;  // 변수 이름 n , type : int
  int *p1 = &n; 
  double d;  // 변수 이름 d , type : double
  double *p2 = &d;
  
  int x[3] = {1,2,3};  // 변수이름 x , type : int[3]
  
  // 배열 x의 주소 
  int (*p3)[3] = &x ; // 배열의 주소
  int *p4 = x; // 배열의 주소가 아님.
  
  int n2 = n;  // 모든 변수는 자산과 동일한 타입의 변수로 초기화 (복사) 될수 있다.
  doucle d3 = d;
  int x1[3] = {1,2,3};  // x1의 타입 : int[3]
  int x2[3] = x1;   // error . 배열은 자신과 동일한 타입의 배열로 복사가 안됨.
  
  int *p1 = x1; // 배열의 이름은 첫번째 요소의 주소로 암시적 형변환 된다.
}
```
- 배열의 주소와 배열의 요소의 주소
```cpp
int main(){
  int x[3] = {1,2,3}; 
  
  // 배열 x의 주소 : 가리키는 곳은 같지만, size가 다르다.
  int (*p1)[3] = &x ; // 배열의 주소
  int *p2 = x; // 배열의 이름이 배열의 첫번째 요소의 주소로 임시적 형변환된 표현
  
  printf("%p , %p\n", p1, p1+1);  // diff 12 byte
  printf("%p , %p\n", p2, p2+1);  // diff 4 byte
  
  // p1은 배열의 주소 , *p1 : 배열이 나옴
  (*p1)[1] = 10;   // 으로 접근 가능
  // p2: 요소의 주소 , (int*)
  *(++p2) = 10;
}
```

- argument decay : 배열을 template으로 전달할때 현상
  - 1. 배열의 특징
    - 자신과 동일한 타입으로 초기화 될수 없다.
    - 배열의 이름은 배열의 1번째 요소의 주소로 임시적 형 변환
    - 배영을 가리키는 참조를 만들수 있다. 
  - 2. 함수 템플릿을 만들때 
    - 배열을 값으로 받으면 T는 요소 타입의 포인터로 결정된다.
    - 배열을 참조로 받으면 T는 배열 타입으로 결정된다.
```cpp
// 함수 템플릿 인자가 참조 타입일때  T&
// void foo(T a)  -> void foo(int a[3])
template<typename T> void foo(T a)   // void foo(int *a)
{
  cout << type_id_with_cvr<T>().pretty_name() << endl;   // 답 : int*
  cout << type_id_with_cvr<decltype(a)>().pretty_name() << endl;   // 답 : int*
}
template<typename T> void goo(T& a)   // void goo (int (&a)[3])
{
  cout << type_id_with_cvr<T>().pretty_name() << endl;  // 답 : int[3]
  cout << type_id_with_cvr<decltype(a)>().pretty_name() << endl;  // 답 : int (&)[3]
}

int main(){
  int x[3] = {1,2,3};
  
  int y[3] = x; // error
  int *p = x; // ok : 첫번째 요소의 주소
  int (&r)[3] = x;  // ok : 참조 변수여서 가리키기만 하면 된다.
  
  foo(x);  // T : int[3] error 
  goo(x);  // T : int[3] ok
}
```
- 위험한 코드
- Argument Decay
  - 1. 문자열의 타입 : char 배열
  - 2. 문자열을 값으로 받으면 T는 const char* 결정되고 , 참조로 받으면 const char[]로 결정된다. 
  - 3. 크기가 다른 배열은 다른 타입니다.
```cpp
template<typename T> void foo(T a,T b)   // void foo(int *a)
{
  cout << type_id_with_cvr<T>().pretty_name() << endl;   // 답 : int*
  cout << type_id_with_cvr<decltype(a)>().pretty_name() << endl;   // 답 : int*
}
template<typename T> void goo(T& a, T& b)   // void goo (int (&a)[3])
{
  cout << type_id_with_cvr<T>().pretty_name() << endl;  // 답 : int[3]
  cout << type_id_with_cvr<decltype(a)>().pretty_name() << endl;  // 답 : int (&)[3]
}

int main(){
  foo("orange","apple");  // ok
  goo("orange","apple");  // error  2개 argument가 배열 사이즈가 달라서 서로 다른 type이므로 같은  T가 될수 없다. 
  
  goo("orange","aapple");   // ok. 글자의 객수가 같아 같은 type T
}
```



# class template
## 6. generic copy constructor
- class안에서는 Complex<T> 대신 Complex를 사용해도 알아서 Complex<T>로 해석해준다.
```cpp
template<typename T> class Complex
{
	T re,im;
	void foo(Complex c) // ok : class member함수 일때는 void foo(Complex<T> c) 로 해석을 해줌.
	{
		Complex c2;   // class안에서는 Complex<T> c2;
	}
}
/*
void foo(Complex c) // error
{}
*/
int main(){
  Complex c1; // error  : template
  Complex<int> c2; // ok  : type
}
```
	
- 0으로 초기화보다는 {}으로 일관되게 초기화해주면 된다.
  - T a = T(); // C++98/03
  - T a = {};  // C++11
- member 함수를 외부에 구현할때는 꼭 ```template <typename T>``` 를 붙여주어야 한다. ```template<typename T>  T Complex<T>::```   
- member variable을 외부에 선언할때도 ```template <typename T>``` 을 붙여주어야 한다.
- member 함수가 template일때  ```template<typename T> template<typename U>``` 를 붙임.
```cpp
template<typename T> class Complex
{
	T re,im;
	void Complex(T a=0,T b=0) : re(a),im(b) {}   // 0 보다 {}
	T getReal() const;
	static int cnt;
	// 클래스 템플릿의 멤버함수 템플릿
	template<typename U> T func(const U& c);
}
template<typename T> template<typename U> T Complex<T>::func(const U& c){}

template<typename T> int Complex<T>::cnt = 0;
	
template<typename T>  T Complex<T>::getReal() const
{
	return re;
}
int main(){

}
```


- generic copy constructor
- member 함수가 template일때  ```template<typename T> template<typename U>``` 를 붙임.  이것이 필요한 이유
  - ```U<int>```가 ```T<double>```로 복사 가능하다면 ```Complex<U>```도 ```Complex<T>```에 복사 가능해야 한다.
```cpp
template<typename T> class Complex
{
	T re,im;
	void Complex(T a={},T b={}) : re(a),im(b) {}
	
	// 클래스 템플릿의 멤버함수 템플릿
	// 일반화된 복사 생성자
	Complex(const Complex<T>&c){}   // 이렇게만 적어주면 자신과 동일한 type만을 받을수 있다. 
	Complex(const Complex<int>&c){}    // 이것으로 하면 아래 error가 없어지지만 , int만 된다. 
	template<typename U> Complex(const Complex<U>&c){}   // 일반화된 복사 생성자 : 이렇게 만들면 일반적인 모든 type에 대해서 받을수 있다. 
	
	template<typename> friend class Complex;
}
template<typename T> template<typename U> 
Complex<T>::Complex(const Complex<U>& c) : re(c.re) , im (c.im)  // re,im이 private이기 때문이다.
{
}

int main(){
  Complex<int> c1(1,1); // ok
  Complex<int> c2 = c1; // ok
  Complex<double> c3 = c1; // error :  2개는 다른 type이 된다. 복사 생성자는 같은 type을 복사하는 것이다.
}
```
- C++11의 smartptr인 shared ptr에 대해서 다룬다.
  - android는 class sp  :  refer to android class sp source code 
```cpp
#include <memory>
using namespace std;

class Animal {};
class Dog : public Animal {};

int main(){
  shared_ptr<Dog> p1(new Dog);
  shared_ptr<Animal> p2 = p1;   // 이렇게 된다는 것은 shared_ptr의 복사 생성자가 template으로 되어져있다는 것임.  대응 , 비교 연산자....
  
  p2 = p1;
  
  if(p2 == p1) {}
  if(p2 != p1) {}
}
```



## 7. template과 friend 함수
- friend 함수의 문제점
```cpp
#include <iostream>
using namepsace std;

template<typename T> class Point {
  T x,y;
 public:
  Point(T a=0,T b=0) : x(a),y(b) {}
  friend ostream& operator<<(ostream& os,const Point<T>& p);   // clss가  template이지 , 함수가 template은 아니다.  아래와 같이 선언해주어한다.
  	// 선언부가 여기 있으면 <int> 생성시 , friend ostream& operator<<(ostream& os,const Point<int>& p); 로 변하고, 이게 선언부분만 있으므로  에러가 발생한다.
	// 여기 선언된 함수는 template이 아니다. 
};

template<typenameT>
ostream& operator<<(ostream& os , const Point<T>& p){   // 함수 template이다. 
  return os << p.x << "," << p.y;
}

int mian(){
  Point<int> p(1,2);
  cout << p << endl;
}
```
- 함수 템플릿 보다는 입란함수가 우선해서 선택된다. (exactly matching)
- 함수 템플릿이 있어도 동일한 탕비의 인자를 가지는 일반함수의 선언만 있다면 link 에러가 발생한다.
```cpp
#include <iostream>
using namespace std;

template<typename T> class Point{};
template<typename T> void foo(Point<T> a){ cout << "T" << endl; }
void foo(Point<int>);     // 선언만 했다면 
int main(){
  Point<int> p;
  foo(p);
}
```
- 문제점 해결
  - friend 함수를 선언시 함수 자체를 template으로 선언
    - ```class Point<int>``` 는 T랑 friend이므로 모든 ```operator<<(Point<T> ..  operator<<(Point<double> ..  operator<<(Point<int> .. ``` 등 모든 것들과 friend가 된다.  ```class Point<double>```도 모든 << operator와 friend가 될 것이다. 그러나, 이렇게 할 이유는 없다. 
```cpp
#include <iostream>
using namepsace std;

template<typename T> class Point {
  T x,y;
 public:
  Point(T a=0,T b=0) : x(a),y(b) {}
  template<typename U>
  friend ostream& operator<<(ostream& os,const Point<U>& p);   // clss가  template이지 , 함수가 template은 아니다.  아래와 같이 선언해주어한다.
    // friend의 위치는 함수 return type 앞에 
};

template<typenameT>
ostream& operator<<(ostream& os , const Point<T>& p){   // 함수 template이다. 
  return os << p.x << "," << p.y;
}

int mian(){
  Point<int> p(1,2);
  cout << p << endl;
}
```
  - class안에 구현부까지 넣는다.  1:1로 형성이 되게 한다. T,U 2가지를 쓸 이유가 없다. 
```cpp
#include <iostream>
using namepsace std;

template<typename T> class Point {
  T x,y;
 public:
  Point(T a=0,T b=0) : x(a),y(b) {}
  template<typename U>
  friend template<typenameT>
	ostream& operator<<(ostream& os , const Point<T>& p){   // 함수 template이다. 
  return os << p.x << "," << p.y;
}
};

template<typenameT>
ostream& operator<<(ostream& os , const Point<T>& p){   // 함수 template이다. 
  return os << p.x << "," << p.y;
}

int mian(){
  Point<int> p(1,2);
  cout << p << endl;
}
```


## 8. typename and template
- 클래스 이름::  으로 접근가능한 요소들
	- 값 : enum 상수 , static member variable
	- 타입 : typedef , using
```cpp
class Test
{
public:
	enum { value1 = 1};
	static int value2;
	
	typedef int INT;
	using SHORT = short; // C++11
	
	class innerclass{};
};
int Test::value2=0;

int main(){
	int n1 = Test::value1;
	int n2 = Test::value2;
	Test::INT a;
	Test::SHORT b;
	Test::innerclass c;
}
	
```
- template 일때  **T를 해석시 값/타입 중에 뭘로 해석할지 모름.**
  - 기본은 값으로 해석 (enum 이나 static)
	- 타입으로 해석시는 앞에 **typename**이라고 붙여야 한다.
```cpp
int p=0;

class Test
{
public:
	// ...
	enum {DWORD = 4 };
	typedef int DWRORD;
};

template<typeName T>
int foo(T t) // T:Test
{
	T::DWORD *p ; // 값으로 해석시 : 5 * p    // 타입 : 지역변수 p(pointer) 선언
				// 기본으로 값으로 해석
	typename T::DWORD *p;   // 타입으로 해석시는 앞에 typename이라고 붙여야 한다.
	return 0;
}	
```


- value_type
	- 모든 컨테이너는 자신이 저장하는 타입이 있다. 타입을 value_type이라는 이름에 저장을 해둔다. 
		- ```list<int>::value_type``` 이라고 하면 이것은 int가 된다.
```cpp
#include <iostream>
#include <vector>
#include <list>
using namepsace std;

/*
template<typename T>
void print_first_element(vector<T>& v){
	T n = v.front();
	cout << n << endl;
}
*/

// 모든 container를 받기를 원할때
template<typename T>
void print_first_element(T& v){
	// T : list<double> 이지만, 우리는 double이 필요한 것이다. 
		// ? n = v.front();
	typename T::value_type n = v.front();
	// auto n = v.front;  도 가능하다. C++11
	cout << n << endl;
}

int mian(){
/*  vector<double> v = {1,2,3}; */
	list<double> v = {1,2,3};
	print_first_element(v);
}
```

- class template type_deduction
```cpp
#include <iostream>
#include <vector>
#include <list>
using namepsace std;

template<typename T> class  Vector
{ 
	T* buff;
	int size;
public:
	Vector(int sz , T value){}
	template<typename C> Vector(C c){}
	
};
template<typename C> Vector(C c)->Vector< typename C::value_type >;  // 외부에 가이드라인을 제공해야 추론이 가능

int mian(){
	Vector v(10,3);
	list s = {1,2,3};   // C++17   , C++11->list<int>
	Vector v2(s);
	
	// 다른 컨테이너의 반복자로 초기화환 Vector  (숙제)
	Vector v3(s.begin(),s.end());
	
	// 숙제 2 : 진짜 pointer임.
	int x[10] = {1,2,3,4,5,6,7,8,9,10};
	Vector v4(x,x+10);
}
```


- template keyowrd
```cpp
class Test
{
public:
	template<typename T> static void f(){}
	template<typename T> class Complex{};
};

template<typename T> void foo(T a)	// T : Test
{
	Test::f<int>(); // ok : f가 tempalte인것을 알고 있어 <int> 처리 가능
				// () 때문에 함수인지를 안다. 
	T::f<int>(); // error : f가 template인지를 알수가 없음. <이 비교연산자인지 template인지 알수 없음.
	T::template f<int>();  // ok : f가 template이라는 것을 알려주어 <int> 처리 가능
	
	Test::Complex<int> c1; // ok
	T::complex<int> c2; // error
	typename T::complex<int> c3; // error
	typename T::template T::complex<int> c4; // ok
			// ()도 없어서 함수인지? (값인지 type인지 모름)  : typename  (값이 아닌 type인지 알려줌)
			// T::template 이라고 적어주어야 한다.
			
	// typename T::template Complex<int> foo(T a)   -> OK
}
int main(){
	Test t;
	foo(t);
}
```


## 9. template parameter
- non-type parameter
  - 정수형 상수 /  enum / 포인터 (전역) / 함수 포인터 / 멤버 함수 포인터
  - c++17 : auto

- template parameter로 template을 받는다.
```cpp
list s1; //error . list는 타입은 아니고 템플릿
list<int> s2; // ok. list<int> 타입
template<typename T> class list {};
template<typename T , template<typename> class C> class stack{
  C c; //error , list c
  C<T> c; // ok ..  list<int> c
};
stack<int , list> s3; // ok   여기는 template 을 달라는 것임
```

- default parameter  (template도 default가능하다.)
```cpp
template<typename T=int , int N=10> struct stack {};

stack <int , 10> s1;
stack<int> s2;
stack<> s3;
```

- template parameter의 종류
  - 1. type
  - 2. non-type : 정수형 상수 , enum , pointer , auto((c++17)
  - 3. template  (default값 사용 가능)
  - C++11:가변인자 템플릿
```cpp
template< typename ... T> ..
```

## 10. MISC
- using : typedef 대신 using을 사용한다.
  - using의 장점은 type , class 모두에 별명으로 사용할수 있다. 
  - typedef는 type에 대한 별명을 만들수 있다. (alias)
```cpp
using DWARD = int;
template<typename T, typenameU> struct Pair {
  T v1;
  U v2;
};
typedef Pair Point;  // error
template <typename T , typename U> using point = Pair<T,U>; 
template<typename T>
using Point2 = Pair<T,T>;

Point<int,double> p;  // Pair<T,U>
Point2<int> p2; // Pair<int,int>
```
- variable template
```cpp
template<typename T>
constexpr T pi = 3.14;
templae<typename T> void foo(T a,T b) {}

float f = 3.3;
foo(f,pi<float>);
double d = pi<double>
```
- template 종류
  - function 
  - variable (C++14)
  - class 




# specialization 

## 11. specialization 개념
- 특별한 type에 대해서 특별한 처리를 하고 싶을때
  - 특수 버젼이 있다고해서 binary가 늘어나는 것은 아니다. (소스는 커져도 목적 코드 양은 같다)
```cpp
using namespace std;

// Primary template
template<typename T> class Stack
{
public:
  void push(T a) { cout << "T" << endl;
};

// partial specialization (부분 특수화)
template<typename T> class Stack<T*>
{
public:
  void push(T a) { cout << "T*" << endl; }
};

// specialization (특수화)
template<> class Stack<char*>
{
public:
  void push(T a) { cout << "char*" << endl; }
};

Stack<int> s1; s1.push(0);
Stack<int*> s2;
Stack<char*> s3;
```

- primary template의 인자가 여러개 여도 partial specialization에서는 줄여도 된다.
- 항상 specialization이 우선
- 첫번째 인자 임의타입  , 두번째 인자는 다시 template Test가 들어옴.
  - 1. primary template의 템플릿 인자가 2개라면 , 사용자는 반드시 템를릿 인자 2개를 전달해야 한다.
  - 2. 부분특수화를 할때 템플릿 인자의 갯수는 primary templae의 인다보다 적을수도 많을수도 있다.  단 , 부분 특수화의 선언에는 반드시 템플릿 인자를 2개 전달해야 한다.
  - 3. 재귀적 모양의 부분특수와 만드는 모양이 중요.
```cpp
templae<typename T , typename U , typename V> 
struct Test<T , Test<U,V>>{ static void foo() {cout << "T, Test(U,V)" << endl;  };
```

-  specialization 주의사항
  - 부분특수화가 있을때 적용될때의 주의점 <T*> 이 선택되면 <int*> 일때 T는 int가 된다.
  - 부분 특수화에서는 default값을 쓸수는 없다.  대신 primary template의 default값을 사용하게 된다.
```cpp
template<typename T , int N=10> struct stack
{
  T buff[N];
};
template<typename T , int N> struct stack<T*,N>
{
  T* buff[N];
};

int main(){
stack<int, 10> s1;
stack<int> s2;  // N => 10
stack<int*> s3;  // N => 10
```

- 특정한 멤버함수 한개만 특수화  (부분특수화는 안되고 , 특수화만 가능하다)
```cpp
template<typename T> void stack<T>::push(T a){
  cout << "T" << endl;
}
template<> void stack<char*>::push(char* a){
  cout << "char*" << endl;
}
```

## 12. specialization 활용
- IfThenElse 만들기
  - compile시간에 bool값으로 type을 선택
```cpp
template<bool b,typename T , typename F> struct IfThenElse
{
  typedef T type;
};
template<typename T , typename F> 
struct IfThenElse IfThenElse<false,T,F>        // 부분특수화
{
  typedef F type;
};

int main() {
  IfThenElse<true,int,double>::type t0;   // int
  IfThenElse<false,int,double>::type t0;   // double
  cout << typeid(t0).name() << endl;
  cout << typeid(t1).name() << endl;
}
```
  - 비트관리할때 IfThenElse를 사용한다.
  - 1. 컴파일 시간 bool 값에 따라 type을 선택하는 도구
  - 2. IfThenElse , IF , Select라는 이름으로 알려져있다.
  - 3. C++ 표준에는 conditional이라는 이름으로 제공. <type_traits> 헤더
```cpp
#include <type_traits>
template<size_t N> struct bit
{
  //using type = typename  IfThenElse( (N<=8), char , int>::type;
  using type = typename  conditional( (N<=8), char , int>::type;

  type bitmap; // 32bit 관리
};
int main(){
  bit<32> b1;
  bit<8> b2;   // 8 일때는 8bit만 사용하면 좋을 거이다.
  cout << sizeof(b1) << " " << siezeof(b2) << endl;
}
```

- couple template  (부분특수화의 활용)
  1. 템를릿의 인자로 자기 자신의 타입을 전달하는 코드
  2. 부분특수화를 만들때 템플릿 인자의 개수
  3. N의 값을 표현하는 방법
```cpp
template<typename T> void printN(const T& cp)
{
  cout << T::N << endl;
}
template<typename T , typename U> struct couple
{
  T v1;
  U v2;
  enum{ N =2; };
};
// 부분특수화 : 2번째 인자가 couple일 경우  <= 여기때문에 3이 나옴.
template<typename A, typename B,typename C> 
struct couple<A , couple<B,C> >
{
  A  v1;
  couple<B,C> v2;
  enum{ N =couple<B,C>::N+1; };
};
// struct couple<couple<A,B> , C> 도 만들어야 할 것이다.
// 두개의 인자가 모두 couple 일때는 이것 용으로 다시 만들어준다.

int main(){
  couple<int,double> c2;
  couple<int,couple<int,char>> c3;    // template은 안에 자신을 받을수 있다. 그러므로 여기서는 3개 보관가능하다.
  printN(c2);  // 2
  printN(c3);   // 3나오기를 원하나 부분특수화를 하지 않으면 2
}
```

<br>

- couple과 같이 재귀적이 아닌 선형으로 사용할수 있게 해보자. (tuple 만들기)
  1. empty class 
    - 아무 멤버도 없는 클래스
    - 항상 크기가 1이다.
    - 아무 멤버 없지만 "타입"이므로 함수 오버로딩이나 템플릿 인자로 활용가능
  2. 상속을 사용하는 기술
  3. 개수이 제한을 없앨수 있을까? => C++11 Variadic template
```cpp
//  xtuple<int , int, char> t3 ;  // 이것처럼 되었으면 함.

template<typename T, typename U> struct couple
{
};
struct Null {}; // empty class
// 2개 이상 5개 미만을 보낼수 있다. 
template< typename p1, typename p2, typename p3=Null,typename p4=Null,typename p5=Null>
class xtuple
   : public couple<P1, xtuple<p2,p3,p4,p5,Null>>
{
};
// N, N , N일때의 부분특수화
template<typename p1, typename p2>
class xtuple<p1,p2,Null , Null , Null>
  : public couple<p1,p2>
{
};
int main(){
  // couple <int, xt<char,long,short,double,Null>>
  //                     couple <c , xt<long,short , double,Null, Null>>
  //                                         couple<long, xt<short , double,N , N, N>  : 여기서 끝내가 위해서 부분 특수화가 필요
  //                                                                couple<short , double>
  xtuple<int , char , long,short,double> t5;
  cout << sizeof(t5) << endl;
}
```

## 13. template meta programming
- compile time에 사용하는 것을 template meta programming이라고 한다.
- enum을 사용하는 이유는 구조체 안에서 초기화 가능한게 이것만 있었는데.
- C++11부터는 static constexpr을 사용
```cpp
template<int N> struct factorial
{
  //enum { value = N * factorial<N-1>::value };
  static constexpr int value = N * factorial<N-1>::value;
};
// 재귀 종료를 위해서 특수화 문법 사용
template<int N> struct factorial<1>
{
  //enum { value = 1 };
  static constexpr int value = 1;
};
int main(){
  int n=factorial<5>::value;  // 5*4*3*2*1 => 120;
}
```

<br>

- constexpr function
- 간단함 함수들은 모두 constexpr이라고 붙이면 속도가 빨라질수 있다.
```cpp
// constexpr함수 C++11
constexpr int add(int a , int b)
{
  return a+b;
}
int main(){
  int n = add(1,2);  // compile 시간에 처리 됨.
  check< add(1,2) > c; // ok... compile time 이므로

  int n1=1,n2=2;
  int c = add(n1,n2);  // ok
  check < add(n1,n2) >  c;   // error
}
```

# type traits
## 14. type tratis 개념
- 컴파일 시간에 타이벵 대한 정보를 얻거나 변형된 타입을 얻을때 사용하는 도구 (메타함수: compile time에 사용하는 함수)
- <type_traits> 헤더로 제공됨 C++11
  1. primary template에서 false return
  2. partial specialization에서 true return
```cpp
templae<typename T> struct is_pointer
{
  enum{ value = false};
};
// 핵심은 pointer 타입에 대해서 부분특수화
templae<typename T> struct  xis_pointer<T*>
{
  enum{ value = true};
};

template<typename T> void printv(T v)
{
  if( xis_pointer<T>::value )
    cout <<"pointer" << endl;
  else 
    cout << "not pointer" << endl;
}
int main(){
  int n=3;
  double d=3.4;
  printv(n);
  printv(d);
  printv(&d);
}
```

- enum을 쓴 이유는 위에서도 언급했듯이 변수 초기화가 안되고 , 상수가 아니면 문제가 되어 enum 상수 사용
- C++ 11 static constexprt 로 compile 상수를 만들수 있다. 
```cpp
templae<typename T> struct is_pointer
{
  static constexpr bool value = false;
};
// 핵심은 pointer 타입에 대해서 부분특수화
templae<typename T> struct  xis_pointer<T*>   // 비상수 pointer만됨.
{
  static constexpr bool value = true;
};
<T* const>
<T* voliatile> 
<T* const volatile>도 모두 만들어주어야 한다.
```

<br>

- is_array : 배열인지 아닌지 조사
- 배열의 크기도 구할수 있다.  c++11 : 
    extent<T,0>::value      1차 크기
    <T,1> 이면 2차 크기
- 
```cpp
template<typename T> struct xis_arry
{
  static constexpr bool value = false;
}
template<typename T, size_t N> struct xis_arry<T[N]>
{
  static constexpr bool value = true;
  static constexpr size_t size = N;   // 이렇게 저장을 해두면 나중에 배열의 size도 저절로 구할수 있다.
}
// 크기를 알수 없는 배열
template<typename T> struct xis_arry<T[]>
{
  static constexpr bool value = true;
}
template<type T> void foo(T& a)  // 꼭 T& 로 받아야 한다.
{
  if ( xis_array<T>::value) ...
}
// int x[3] 의 변수 이름 x ,  타입 int[3]  임의의 배열의 표현은 T[N]으로 표현
```

## 15. integral constant
- int2type  : 정수를 각기 타입으로 만들어주는 것이다. 
  1. 컴파일 시간 정수형 상수를 각각의 독립된 타입으로 만드는 도구
  2. int2type을 사용하면 캄파일 시간에 결정된 정수형 상수를 모두 다른 타입으로 만들수 있다. (77,88은 같은 타입이지만, int2type<77> int2type<88>은 다른 타입니다.
  3. int3type을 사용하면 캄파일 시간에 결정된 정수형 상수를 가지고 함수 오버로딩에 사용하거나, 템플릿 인자, 상속등에서 사용할수 있다.
```cpp
template<int N> struct int2type
{
  static constexprt int value = N;
};
void foo(int n){}
void foo(double d){}
int main(){
  foo(3); // foo(int)
  foo(3.4); // foo(double)
  
  // 아래 2개를 다른 함수를 부르게 하자는게 int2type의 개념 : 인자의 갯수와 type이 같은데 가능할까?
  foo(0);
  foo(1); 

  // 이 2개는 다른 구조체이므로  다른 타입이다.
  int2type<0> t0;
  int2type<1> t1;
  foo(t0);   foo(t1);  // 은 다른 함수를 부르게 된다.
```

<br>

- int2type의 활용
- c++에서 if 조건문에 false여도 그 안의 내용을 compile을 해본다.  false되었을때 compile하지 않게 하려면 C++17의 compile if를 사용하면 된다.
  ```if constexpr( xis_pointer<T>::value) { AAA} ``` 이라고 하면 AAA 을 compile하지 않는다. 
>
- 위 이슈의 해결책 : 함수는 compile time 분기
  1. 동일한 이름을 가지는 함수가 여러개 있을때  어느 함수를 호출할지 결정하는 것은 캄파일 시간에 이루어진다. 선택되지 않은 함수가 템플릿이었다면 "instantiation"되지 않는다.
  2. 포인터 일때와 포인터 아닐때 서로 다른 타입화해서 함수 오버로딩의 인자로 활용한다.
```cpp
template<typename T> void printv(T v)
{
  // printv_imp(v, xis_pointer<T>::value);  //  0,1로 되므로 동일
   printv_imp(v, int2type< xis_pointer<T>::value>() );  //  int2type<1> , int2type<0>으로 들어가게 된다
}
templae<typename T>
void printv_imp(T v, int2type<1> ) {  }   // 함수 오버로딩
templae<typename T>
void printv_imp(T v, int2type<0> ) {  }
```

- integral constant
  1. int뿐만 아니라 모든 정수 계열(long , bool , char, short , int , long long)의 상수 값을 타입으로 만들수 있게 하자. (참고, 실수는 템를릿 인자로 사용할수 없다.)
  2. integral_constant : 모든 정수계열의 컴파일 시간 상수 를 타입으로 만드는 템플릿
- is_pointer<T>() 의 return 값이 false_type / true_type  의  객체가 오게 됨.  왜냐면 is_pointer struct를 만들때 , false/true_type을 상속하게 되기 때문임.
```cpp
integral_constant<int,0> t0;
integral_constant<short,0> t3;

typedef integral_constant<bool , true> true_type;
typedef integral_constant<bool , false> false_type;  // 이 2개가 다른 타입이다.

template<typename T>  struct is_pointer : false_type { };   //  이것으로 끝. 귿이 value를 넣어줄 것도 없다. 타입 자체가 다르기 때문이다.  (기반 클래스로 이용)

is_pointer<T>()   ;   ()으로 객체가 만들어지는 것임.
```

<br>

- traits 사용법 : 템플릿 만들때 타입을 조사하는 방법
  1. <type_traits>헤더 포함
  2. ::value값을 조사하는 방법 (is_pointer<T>::value)
    - if 문 사용시에는 *v를 사용할수 없다.
    - c++17의 if constexpr 사용시는 *v 사용 가능
    - C++17부터는 is_pointer_v<T> 표현식도 제공
  3. true_type / false_type을 사용한 함수 오버로딩


## 16. type modification : 기존에 있던 type에 변형된 code
- type traits 기능
  1. type에 대한 query - is_pointer<> , is_array<> , extent<>
  2. type에 대한 변형 타입 구하기 - remove_pointer> , add_pointer<> 
- remove pointer : 의 구현 (xremove_pointer)
  1. primary template을 만들도 "typedef T type" 을 제공한다. (using도 ok)
  2. const 와 volatile도 고려
```cpp
template <typename T> struct xrmove_pointer
{
  typedef T type;
}
template <typename T> struct xrmove_pointer<T*>
{
  typedef T type;
}

template<typename T> void foo(T a)
{
  typename xremove_pointer<T>::type t;   // type을 뽑을 것이므로 typename 이라고 적어야 한다.
  cout << typeid(t).name() << endl;
}
```

<br>

- remove_all_pointer  : 표준에는 all remove pointer는 없다.
```cpp
template <typename T> struct xrmove_pointer_all
{
  typedef T type;
}
template <typename T> struct xrmove_pointer_all<T*>
{
  typedef xremove_pointer_all<T>::type type;   // 재귀 사용
}

template<typename T> void foo(T a)
{
  xremove_pointer_all<int***>::type t;   // 계속 *이 떨어져 int만 남음
  cout << typeid(t).name() << endl;
}
```

<br>

- 함수 리턴  타입 구하기 ( return type  구하기)
```cpp
double hoo(shart a , int b){ return 0; }
template<typename T> struct result_type
{
  // typedef T type;    //  주석으로 막는 이유는 함수가 아닌 것일때는 error를 내겠다는 의도로 일부러 주석으로 막는 것이다.
}
template<typename R,typename A1,typename A2> struct result_type<R(A1,A2)>   // return도 표시가 되네!!!  부분 특수화를 이렇게!!
{
  typedef R type;
}

templae<typename T> void foo(T& t)
{
  // T : double (short , int)
  typename result_type<T>::type ret;
  cout << typeid(ret).name() << endl;  // double이 나오기를 원한다. 

  result_type<int>::type ret; // int
}
int main() {
  foo(hoo);
}
```

<br>

- 함수의 인자 타입 구하기
```cpp
double hoo(shart a , int b){ return 0; }
template<typename T,size_t N> struct arguement_type
{
  typedef T type;  
}
/* 이 표현은 ? 을 못 넣음.
template<typename R,typename A1,typename A2, size_t N> struct result_type<R(A1,A2),N>   // return도 표시가 되네!!!  부분 특수화를 이렇게!!
{
  typedef ?  type;
}
*/
template<typename R,typename A1,typename A2, size_t N> struct result_type<R(A1,A2),0> 
{
  typedef A1  type;
}
template<typename R,typename A1,typename A2, size_t N> struct result_type<R(A1,A2),1> 
{
  typedef A2  type;
}

templae<typename T> void foo(T& t)
{
  // T : double (short , int)
  typename argument_type<T,0>::type ret;
  cout << typeid(ret).name() << endl;  // double이 나오기를 원한다. 

  result_type<int>::type ret; // int
}
int main() {
  foo(hoo);
}
```



## 17. traits 정리
- C++  표준 type_traits
  - is_pointer / is_array / extend (배열의 크기) / remove_pointer  / result_of / invoke_result 
  - is_pointer<T>::value
- 1. 변형된 타입을 얻는 traits
```cpp
// 포인터를 제거한 타입을 구하고 싶다.
typename remove_pointer<T>::type n;

template<typename T> using remove_pointer_t = typename remove_pointer<T>::type;
remove_pointer_t<T> n;   // C++14부터 기본 제공 : 위의 using선언하지 않아도 됨.
```
- 2. 타입의 정보를 조사하는 traits
```cpp
// variable template 을 이용
template(typename T> inline constexpr bool is_pointer_v = is_pointer<T>::value;

bool b = is_pointer_v<t>;  // C++17  : 방법 1
if( is_pointer_v<T>) {  *a} // 문제 발생

foo_imp(is_pointer<T>());  // 이것으로 문제 해결 ; 방법 2 (overloading)
foo_imp(true_ype){}
foo_imp(false_type){}

bool b = is_pointer<T>::value ; // 방법 3

if constexpr (is_pointer_v<T>)   // 방법 4 : C++17
```

<br>

- 배열 정보 조사
```cpp
// 배열 정보 조사
// food(T a)이면 array => pointer : decay
template<typename T> void foo(T& a)  // array를 전달시는 T a 에서는 poiner로 받아진다. decay
    // 그러므로 array를 받을때는 T& a로 받아야 한다.
{
  if ( is_array<T>::value)
  { 
    cout << "array" << endl;
    cout << extend<T,0>::value << endl; // 3
    cout << extend<T,1>::value << endl; // 2
  }
}
int main(){
  int x[3][2] = { 1,2,3,4,5,6};
  foo(x);
}
```

<br>

- remove_cv
```cpp
template<typename T , typename U> void foo(T a , U b)
{
  // bool ret == is_same<T,U>::value;
 bool ret == is_same<remove_cv<T>::type ,
                                    remove_cv<U>::type >::value;  // remove_cv는 const와 volatile
  cout << ret << endl;
}
int main(){
  foo<int,const int>(0,0);  
}
```

- array가 pointer로 바뀌는 것을 decay라고 함. 
```cpp
template<typename T , typename U> void foo(T a , U b)
{
  bool ret == is_same< typename decay<T>::type ,
                                     typename decay<U>::type >::value;
  boot ret == is_base_of<T,U>::value;  // 상속관계인지?
  // www.cppreference.com
  cout << ret << endl;
}
int main(){
  foo<int[3], int*>(0,0);
}
```

> www.cppreference.com -> Type support -> type traits 설명
- trivially / scalar / standard layout / aggregate 등이 뭔지 의미를 알아두어야 한다.



# variadic template : 가변인자 template
## 18. variadic template #1
- 가변인자 템플릿
  - c++11부터 지원되는 문법
  - variadic template의 기본 모양
  - variadic template의 템플릿 인자인 "Types"는 여러개의 타입을 나타낸다.
  - variadic template의 함수 인자인 args안에는 여러개의 값이 들어있다. "Parameter Pack" 이라고 한다.


```cpp
// 가변인자 클래스 템플릿
template<typename ... Types> class tuple   // ...을 찍어서 1개 2개 이상 또는 0개를 보내도 문제 없다.
{
};
// 가변인자 함수 템플릿
template<typename ... Types>
void foo(Types ... arg)
{
}
int main(){
  tuple<int> t1;
  tuple<int,char> t2;
  foo();
  foo(1, 3.4 , "A");
}
```

<br>

- parameter pack 
  - parameter pack
  - sizeof...(args)
  - pack expansion
```cpp
void goo(int n, double d , const char* s)
{
	cout << "goo" << n << " " << d << " " << s << endl;
}
template<typename ... Types>
void foo(Types .. args)
{
	// args : Parameer Pack
	cout << sizeof...(args) << endl; // 3
	cout << sizeof...(Types) << endl;  // 3

	goo(args);  // error : parameter pack은 함수 인자가 될수 없다.
	goo(args...);   // pack expansion
	                // goo (e1,e2,e3)
                        // goo (a, 3.4 , "AAA")
}
int main(){
  foo(1, 3.4 , "AAA");
	// Types : int , double , const char*
	// args : 1, 3.4 , "AAA"
}
```

<br>

- pack expansion
  - parameter pack을 사용하는 패턴...  -> 패턴(e1), 패턴(e2) , 패턴(e3)
```cpp
void goo(int n, double d , const char* s)
{
	cout << "goo" << n << " " << d << " " << s << endl;
}
int hoo(int a){ return -a; }
template<typename ... Types>
void foo(Types .. args)
{
	int x[] = {args...}; // pack expansion
	int x[] { (++args)... };  // { (++e1) , (++e2) , (++e3) }
				// { 2,3,4}
	int x[] = { hoo(args...) };   // hoo(1,2,3) error
	int x[] = { hoo(args)... };   // { hoo(1) , hoo(2) , hoo(3) }
				// { -1 , -2, -3 }
	goo(args...);  // goo(1,2,3); ok
	goo(hoo(args...)); // goo(hoo(1,2,3)) error
	goo(hoo(args)...); // goo(hoo(1), hoo(2), hoo(3));  ok
	for(auto n : x)
		cout << n << endl;
}
int main(){
  foo(1,2,3);	// Types : int , int , int  , args : 1,2,3
  foo(1, 3.4 , "AAA");
	// Types : int , double , const char*
	// args : 1, 3.4 , "AAA"
}
```
- pack expansion을 사용하는 패턴... => 패턴(e1) , 패턴(e2) , 패턴(e3)
- ...의 위치에 주의한다.
  - goo(hoo(args...)) -> goo(hoo(e1,e2,e3))
  - goo(hoo(args)...) -> goo(hoo(e1),hoo(e2),hoo(e3))
- pack expansion은 함수의 호출인자 또는 list 초기화를 사용한 표현식에서만 쓸수 있다. 전역적인 공간에서의 pack expansion은 안된다.
```cpp
int pirnt (int a)
{
	cout << a << "," ;
	return 0;
}
template<typename ... Types> void foo(Types ... args)
{
	print(args);  // error
	print(args...); // pirnt(1,2,3)  error
	print(args)...;	// print(1), print(2),print(3)   error  : 전역 expansion

	int x[] = { args... }; // {1,2,3} ok
	int x[] = { print(args)... } // { print(1) , print(2) , print(3) }  으로 문제가 도지 않는다.
	int x[] = { 0, (print(args),0)... }; // print가 void이어도 처리가 됨. 모두 0으로 초기화됨.
	                // {0, (print(1),0) , (print(2),0) , (print(3),0)};
	initializer_list<int> e = { (print(args),0)... };	// empty가 되어도 문제가 없다.
}
int main(){
	foo(1,2,3);	// args: 1,2,3
	foo();	// build시 에러... 초기화시 {}으로 되어버리기 때문이다.  g++은 되는데 visual c++은 안됨.
	        // int x[] = { 0, print(args)... }   이렇게 앞에 0을 한개 넣어주면 에러 안 남
}
```

<br>

- 템플릿 인자의 expansion  (Types...) : ...의 위치 파악
```cpp
int pirnt (int a)
{
	cout << a << "," ;
	return 0;
}
template<typename ... Types> void foo(Types ... args)
{
	int x[] = { args... };  // 다른 type들의 값을 담지는 않는다.
	pair<Types...> p1;  // pair<int , double>
	tuple<Types...> t1;	// tuple<int,double>

	tuple<pair<Types...>> t2;  // tuple<pair<int,double>> t2;  ok
	tuple<pair<Types>...> t3;   // tuple<pair<int>,pair<double>> t3; error
	tuple<pair<int,Types>...> t4;  // tuple< pair<int,int> , pair<int,double>> t4;  ok

	pair<tuple<Types...>> p2;  // pair<tuple<int,double>> p2; error
	pair<tuple<Types>...> p3;  // pair<tuple<int> , tuple<double>> p3;  ok
}
int main(){
	foo(1, 3.4);	// args: 1, 3.4
}
```

## 19. variadic template #2
- using recursive
  - pack expansion : 배열 또는 tuple에 담는다.
  - 재귀호출
  - fold expression : c++17
- pack expansion : 배열 또는 tuple에 담는다.
```cpp
template<typename ... Types> void foo(Types ... args)
{
	int x[] = { args... }; // error : 다른 type이 값들을 담지는 않는다.
	tuple<Types...> tp(args...);

	cout << get<0>(tp) << endl;
	cout << get<1>(tp) << endl;  // get이 template이므로 인자로 변수로 넣을수는 없다. 
}
int main(){
	foo(1, 3.4);	// args: 1, 3.4
}
```

 - 재귀호출
    - 첫번째 인자만큼은 이름있는 것에 독립적으로 받는다.
  - 재귀호출이 아니라 foo() , foo(int) , foo(int,double).. 등 다양한 함수가 만들어진다.  큰 함수는 이렇게 쓰면 코드가 커져 안 좋다.
```cpp
void foo(){} // 재귀의 종료

template<typename T, typename ... Types> void foo(T value,Types ... args)
{
	static int n = 0;
	++n;

	cout << n << endl;     // 위와 같이 재귀가 아니므로 n을 찍으면 모두 1이 나온다.  다른 파일에 만들어진다는건가?  static인데...
	cout << value << endl;
	foo(args...); // foo(3.4 , "AA")
	              // foo("AA")
	              // foo() 
}
int main(){
	foo(1, 3.4 , "AA");	// value 1 , args: 3.4 , "AA"
}
```

- fold expression : c++17
  1. 이항 연산자를 사용해서 parameter pack안에 있는 요소에 연산을 수행하는 문법
  2. parameter packe의 이름에서  ...을 붙이지 않고 사용한다.
    - args... : pack expansion
    - (args + ...) : folder expression
  3. 4가지의 형태
    - unary right fold : (args op ...)  -> E1 op (E2 op (E3 op E4))
    - unary left fold : (... op args) -> ((E1 op E2) op E3) op E4
    - binary right fold : (args op ... op init) -> E1 op (E2 op (E3 op (E4 op init)))
    - binary left fold : (init op ... op args) -> (((init op E1) op E2) op E3) op E4
```cpp
template<typename ... Types> int foo(Types ... args)
{
	int x[] = { args...}; // pack expansion

	// C++17
	int n = (args + ...); // fold expression  반드시 ( )로 묶어주어야 한다.
	                      // 1+(2+(3+4))
	int n   = (... + args);
	int n   = (args + ... + 10);  1+(2+(3+(4+10)))
	int n   = (10+ ... + args); 

	return n;
}
int main(){
	foo(1, 2,3,4);	//  args: 1,2,3,4
	cout << n << endl;
}
```

- fold expression 이용한 예제
```cpp
template<typename ... Types> void foo(Types ... args)
{
	(cout << ... << args);  // - binary left fold : (init op ... op args) -> (((init op E1) op E2) op E3) op E4
	  // ((cout << 1) << 2) << 3
	  // 초기값이 객체이면 많은 일을 할수 있다. 
}
int main(){
	(((cout << 1) << 2) << 3);  // 123
	foo(1,2,3);
}
```

- 가변인자의 모든 값을 vector에 옮겨 담는다.
```cpp
#include <vector>
template<typename ... Types> void foo(Types ... args)
{
	(args , ...); // 1, ( 2, 3)
	(v.push_back(args), ...);
	  // v.push_back(1),(v.push_back(2), v.push_back(3))
	for(auto n : v)  cout << n << endl;
}
int main(){
	foo(1,2,3);
}
```


## 20. variadic template 활용 #1
- result 만들기
  - 함수의 정보를 구하는 traits 만들기
    - primary tempalte을 만들고 typedef T type을 제공
    - 리턴 타입(double)을 얻을수 있도록 부분 특수화한다 T => R(A1,A2)
    - N번째 인자의 타입 구하기
      - 0번째 인자의 type을 구하는 부분특수화 작성
      - N번째 인자의 타입을 구하는 부분 특수화작성 : N==0일때까지 recursive 사용
```cpp
#include <vector>

int goo(){ return 0; }
double hoo(short a , int b){ return 0; }

/* 이것은 인자가 2개까지만 가능하게 된다.
template<typename T> struct result_type
{
	typedef T type;
}
template(typename R,typename A1 , typename A2> struct result_type<R(A1,A2)>
{ 
	typedef R type;
}
*/

template<typename T> struct result_type;  // 방법 3: 의도적으로 선언부만 만든다. 선언부가 없으면 아래의 부분 특수화가 안된다.


template<typename T> struct result_type
{
	// typedef T type;   // 방법 1 : 주석처리해서 type 을 반환하지 못하게하여 error
	static_assert(is_function<T>::value,"error");  // 방법 2 : 이렇게 static_assert를 사용하여 error를 이쁘게 보여줄수 있다. 
}
// 인자수에 상관없이 다 됨.
template(typename R,typename ... Types> struct result_type<R(Types...)>
{ 
	typedef R type;
}

int main(){
	foo(hoo);
	foo(goo)
	
	int n=0;
	foo(n);		// int
	  // typedef T type을 없애면 error가 발생한다. type에 없기 때문이다.
}
```

<br>

- arguement 만들기 : 가변인자를 사용하여 확장
```cpp
double hoo(short a, int b){return 0; }


template<size_t N,typename T> struct argument_type
{
	typedef T type;
};
/* 
template<size_t N,typename R,template A1, template A2> 
struct argument_type<0, R(A1,A2)>
{
	typedef A1 type;
};
*/

template<size_t N,typename R,template A1, template ...Types> 
struct argument_type<0, R(A1,Types...)>
{
	typedef A1 type;
};

template<typename T> void foo(const T& t)
{
	// T : double(short,int)
	typename argument_type<0,T>::type ret;

	cout << typeid(ret).name() << endl;
}
int main(){
	foo(hoo);
}
```
- N번째 argument를 구하는 방법 : recursive사용
double hoo(short a, int b){return 0; }

template<size_t N,typename T> struct argument_type
{
	typedef T type;
};

// N==0일때
template<size_t N,typename R,template A1, template ...Types> 
struct argument_type<0, R(A1,Types...)>
{
	typedef A1 type;
};

// N일때
template<size_t N,typename R,template A1, template ...Types> 
struct argument_type<N, R(A1,Types...)>
{
	typedef arguement_type<N-1,R(Types...)>::type type;
};

template<typename T> void foo(const T& t)
{
	// T : double(short,int)
	typedef typename argument_type<2,T>::type ret; // template에 의존적으로 ::type을 끄집어 내는 것으로 앞에 typename을 붙여주어야 한다.

	cout << typeid(ret).name() << endl;
}
int main(){
	foo(hoo);
}
```

<br>

- tuple 만들기
  - 
```cpp
#include <tuple>
using namespace std;

// primary template
template<typename ... Types> struct xtuple
{
	static constexpr int N = 0;
}

// 부분특수화
// struct xtuple<T,Types...> 만 써서 상속이 없으면 1개만 보관
template<typename T, typename ... Types> 
struct xtuple<T,Types...> : public xtuple<Types...>  // 상속을 사용해서 N개까지 보관가능
{
	T value;
	xtuple() = default;
	xtuple(const T& v, const Types& ... args):value(v),xtuple<Types...>(args...){}

	static constexprt int N = xtuple<Types...>::N +1 ;
}

int main()
{
	tuple<int,double,int,char> t4(1,3.4, 2, 'A');

	cout << get<2>(t4) << endl;  // 3.4
	get<2>(t4) = 10;
	cout << get<2>(t4) << endl;

	xtuple<> x0;   // primary
	xtuple<int> x1(1);   // 부분특수화 사용
	xtuple<int , double , char> t3(1, 3.4 , 'a'); // 부분특수화 사용
}
```


## 21. variadic template 활용 #2

- 기반클래스 멤버에 접근하는 방법
  1. 기반 클래스의 멤버와 파생클래스의 멤버의 이름이 동일할때는 자신 (파생 클래스)의 멤버가 우선시 된다.
  2. 기반 클래스의 멤버에 접근하는 방법 : d.Base::value  or static_cast<Base&>(d).value
  3. 값 캐스팅과 참조 캐스팅 : C++ Inermediate 가정 temporary참조
```cpp
struct Base
{
	int value = 10;
};
struct Derived : public Base
{
	int value = 20;
};
int main(){
	Derived d;
	cout << d.value << endl;  // 20
	cout << d.Base::value << endl; // 10

	cout << static_cast<Base>(d).value << endl; // 10 : 임시객체 - 이렇게 만들어 쓰는 것은 절대 쓰면 안됨. 별도의 메모리에 copy하는 것이다. 
	cout << static_cast<Base&>(d).value << endl; // 10 

	// 임시 객체는 lvalue가 될수 없다.
	static_cast<Base>(d).value = 30; //error
	static_cast<Base&>(d).value = 30; //ok
}
```

- get만들기 #1 : tuple에 있는 값을 꺼내기
  - tuple의 0번째 요소는 자기 자신이 보관한다.  1번째 요소는 1번째 기반 클래스가 보관한다 n 번째 요소는 n번째 기반 클래스가 보관한다.
```cpp
#incldue <iostream>
using namespace std;

template<size_t N, typename T>


template<size_t N , typename TP> // TP means tuple
?     // tuple TP의 N번째 요소의 타입&
xget(TP& tp)
{
	return static_cast<?>(tp).value;  // TP의 N번째 기반 클래스&
}

int main()
{
	xtuple<int,double,char> t3(1, 3.4 , 'a');  
	cout << t3.value << endl; // 1
	cout << static_cast< xtuple<double,char>&>(t3).value << endl;  // 3.4
	cout << static_cast< xtuple<char>&(t3).value << endl;	// 'a'

	char c  = xget<2>(t3);
}
```

- tuple element  0번째 구하기
```cpp
template <size_t N, typename TP> struct xtuple_element
{
	typedef TP type;
}

template <typename T,typename ... Types> struct xtuple_element<0,xtuple<T,Types...> >
{
	typedef T type;
}

int main()
{
	xtuple_element<0,xtuple<int,double,char> >::type n;
	cout << typeid(n).name() << endl;  // int
}
```

- tuple element  N번째 구하기
```cpp
template <size_t N, typename TP> struct xtuple_element;

// N == 0
template <typename T,typename ... Types> struct xtuple_element<0,xtuple<T,Types...> >
{
	typedef T type;  // 0번째 요소의 타입
	typedef  xtuple<T,Types...> tupletype;  // 0번째 요소를 저장(보관)하는 type
}
template <size_t N, typename T,typename ... Types> struct xtuple_element<N,xtuple<T,Types...> >
{
	typedef typename xtuple_element<N-1,xtuple<Types...>>::type type;
	typedef typename xtuple_element<N-1,xtuple<Types...>>::tupletype tupletype;
}

int main()
{
	xtuple<int,doublechar> t3(1, 3.4 , 'a'); // 0번째 요소의 type : int
	                                         // 0번째 요소를 저장하는 타입: xtuple<int,double,char>

	xtuple_element<2,xtuple<int,double,char> >::type n;  // char
	xtuple_element<2,xtuple<int,double,char> >::tupletype n;  // xtuple<char>
	cout << typeid(n).name() << endl;  // char
}
```

- get 만들기 : 앞의 ? 채우기
  - N번째 요소를 저장하는 type도 알아야 한다. xtuple_element::tupletype
```cpp
#incldue <iostream>
using namespace std;

template<size_t N, typename T>


template<size_t N , typename TP> // TP means tuple
typename xtuple_element<N,TP>::type&    // tuple TP의 N번째 요소의 타입&
xget(TP& tp)
{
	return static_cast<typename xtuple<N,TP>::tupletype&>(tp).value;  // TP의 N번째 기반 클래스&
}

int main()
{
	xtuple<int,double,char> t3(1, 3.4 , 'a');  
	cout << t3.value << endl; // 1
	cout << static_cast< xtuple<double,char>&>(t3).value << endl;  // 3.4
	cout << static_cast< xtuple<char>&(t3).value << endl;	// 'a'

	xget<1>(t3) = 1.1;
	cout << xget<1>(t3) << endl;
}
```


- print tuple
```cpp
#include <iostream>
#include <tuple>
using namespace std;

// C++17
// template<size_t ... N> struct index_sequence {};

template<typename TP,size_t ... I> void print_tuple_imp(const TP& tp,index_sequence<I...>&)  I : 0,1,2
{
	int x[] = { get<I>(tp)... };  // get<0>(tp),get<1>(tp) , get<2>(tp)
	for(auto& n : x)
		cout << n << ",";
	cout << endl;
}

template<typename TP> void print_tuple(const TP& tp)
{
	print_tuple_imp(tp,make_index_sesquence<tuple_size<TP>::value>());
}

int main()
{
	tuple<int,int,int> tp(1,2,3);
	//print_tuple(tp,index_sequence<0,1,2>() );
	// print_tuple(tp,make_index_sequence<3>() );
	print_tuple(tp);
}
```




# template design

## 22. thin template
- thin template  (template이 아닌 것들을 기반 클래스로 만들자.)
  - 함수의 갯수를 생각해보자
  - 4개의 함수 * 3개의 타입 => 12개 함수
```cpp
template<typename T> class Vector
{
  T* buff;
  int sz;
public:
  int size() const {}
  bool empty() const {}
  void push_front(const T& a) {}
  T& front() {}
};
int main() {
  Vector<int> v1;
  Vector<short> v2;
  Vector<double> v3;
}

```
- 줄여보자
  - T를 사용하지 않는 모든 멤버는 기반 클래스로 올리자.
  - 갯수 2개의 함수 * 3개의 타입 + 기반 클래스 함수 2 = 8 함수
```cpp
class VectorBase {
protected:
  int sz;
public:
  int size() const {}
  bool empty() const {}
};

template<typename T> class Vector : public VectorBase
{
  T* buff;
public:
  void push_front(const T& a) {}
  T& front() {}
};
int main() {
  Vector<int> v1;
  Vector<short> v2;
  Vector<double> v3;
}

```
- 모든 멤버를 기반클래스로 올리자. C 기술을 사용하여 void *
- 사용자는 void*를 casting을 하여 사용해야 하므로 , 
- casting만 책임지도록 파생 클래스를 만든다.
- casting만 하는 함수들은 모두 inline으로 만든다. 
- embedded 에서는 많이 사용한다.
```cpp
class VectorBase {
protected:
  void* buff;
  int sz;
public:
  int size() const {}
  bool empty() const {}
  void push_front(const void* a) {}
  void* front() {}
};

template<typename T> class Vector : private VectorBase
{
  T* buff;
public:
  inline int size() const {return VectorBase::size(); }
  inline bool empty() const { return VectorBase::empty(); }
  inline void push_front(const T& a) {VectorBase::push_front(static_cast<void*>(a)}
  inline T& front() { return static_cast<T&>(VectorBase::front()); }
};

int main() {
  Vector<int> v1;
  Vector<short> v2;
  Vector<double> v3;
}

```

## 23. CRTP (Curiously Recurring Template Pattern)
- 미래에 만들어질 파생 클래스의 이름을 알게 하는 기법
- 파생클래스를 만들때 template인자로 파생클래스 이름은 넘겨준다.
```cpp
template<typename T> calss Base
{
public:
	Base()
	{
		cout << typeid(T).name() << endl;
	}
};

class Derived : public Base<Derived>
{
};

int main()
{
	Derived d;
}
```

- CRTP 활용 : GUI library로 가정
  - 가상함수로 만들면 해결된다.  그러나 가상함수 overhead가 있으므로 
  - 여기서는 가상함수를 사용하지 말고 해결해보자.
- 비 가상함수를 가상 함수처럼 동작하게 만들기
  - this 포인터를 파생 클래스 타입으로 캐스팅 한 후 함수 호출.
```cpp
#include <iostream>
using namespace std;

template<typename T> class Window
{
public:
	void msgLoop()  // void msgLoop(Window* const this)
	{
		// onClick();  // this->Onclink()
		static_cast<T*>(this)->OnClick();
	}
	void OnClick() { cout << "Window onClick" << endl; }
};

class FrameWindow : public Window<FrameWindow>
{
public:
	void OnClick() { cout << "FrameWindow onClick" << endl; }
};

int main()
{
	FrameWindow fw;
	fw.msgLoop();
}
```


- CTRP를 사용한 singleton
  1. singletone : 하나의 객체만 생성할수 있게 하는 디자인 패턴
    - private 생성자.
    - 복사와 대입 금지
    - 하나의 객체를 만들어서 리턴하는 static member 함수
- android singleton code
```cpp
template<typename T> class Singleton
{
protected:
	Singleton(){}
public:
	cursor(const Singleton& c) = delete;
	void operator=(const Singleton& c)=delete;
	static T& getInstance()
	{
		static T instance;
		return instance;
	}
};

class Mouse : public Singleton< Mouse>
{

};

int main(){
	Mouse& m = Mouse::getInstance();
}
```


- Object가 쓰인 갯수를 구하고 , 
- mouse, keyboard로 객체 갯수를 구하고 싶다.
1. 기반 클래스의 static member data는 모든 파생클래스에 의해 공유된다.
2. 파생 클래스 별로 다른 static member data가 필요한 경우 , 서로 다른 기반 클래스를 사용해야 한다.
3. CRTP를 사용하면 모든 파생 클래스 별로 다른 타입의 기반 클래스를 만들수 있다. 
```cpp
#include <iostream>
using namespace std;

template<typename T> class Object
{
public:
	static int cnt;

	Object(){ ++ cnt; }
	~Object(){ -- cnt; }

	static int getCount(){ return cnt; }
};
template<typename T> int Object<T>::cnt = 0;  // static은 외부에 선언이 필요함.

class Mouse : public Object<Mouse>
{
};

class KeyBoard : public Object<Keyboard>
{
};

int main(){
	Object c1,c2;
	cout << c1.getCount() << endl;

	Mouse m1,m2;
	Keyboard k1,k2;
	cout << k1.getCount() << endl;  // template으로 해주어야 다른 object가 되므로 cnt가 각 class마다 존재한다.
}
```


## 24. policy base design
- 내부적으로 lock을 기본 제공하는게 좋을까요?  (multi thread) 안 좋을까? (single thread)
- 사용자가 선택할수 있게 하겠다. 
- policy base design
  1. 성능저하가 없다.
  2. 클래스가 사용하는 정책을 템플릿 인자를 통해서 교체 가능한 디자인
  3. 지켜야 하는 규칙
    - 규칙을 표현하는 코딩 방식은 없다. (인터페이스 사용시 가상 함수이므로 약간의 오버헤드 발생) c++20 concept 문법
    - 우리가 만든 동기화 정책 클래스는 "lock/unlock"함수가 필요하다.
    - STL에서 널리 사용되는 기법
```cpp
template<typename T, typename ThreadModel = NoLock> class List
{
	ThreadModel tm;
public:
	void push_front(const T& a)
	{
		tm.lock();
		// ...
		tm.unlock();
	}
};

class NoLock {
public:
	inline void lock(){}
	inline void unlock(){}
};
class MutexLock {
public:
	inline void lock(){ mutex.lock(); }
	inline void unlock(){mutex.unlock(); }
};

List<int, NoLock> s; // 전역 변수, 멀티스레드에 안전하지 않다.
	// 2번째 인자의 lock , unlock에 따라서 달라질 것이다
List<int, MutexLock> t;


int main()
{
	s.push_front(10);
}
```

- STL allocator
  - memory 할당 방법 : new , malloc , calloc , win32 api , linux system call 등 다양함.
  - C++표준 allocator는 복잡한 편이다. 참고 소스를 참조. Placement new 공부 필요
```cpp
// STL vector 
// 메모리 할당기가 있다. allocator
template <typename T> class allocator
{
pulbic:
	T* allocate(){}
	void deallocate(){}
};
templae<typename T, typename Ax=allocator<T> > class vector
{
	T *buff;
	Ax ax;
public:
	void resize(int n)
	{
		T* p = ax.allocate(n);
		ax.deallocate(p);
	}
};
int main()
{
	vector<int,MyAlloc<int> > v(10);
	v.resize(20);
}
```


- allocator rebind
```cpp
template<typename T> class allocator
{
	// 
	template<typename U> struct rebind
	{
		typename allocator<U> other;
	};
};

//rebind
template<typename T, typename Ax = allocator<T> > class list
{
	sttruct NODE
	{
		T data;
		NODE* next, *prev;
	};
	//Ax ax; // allocator<int>
	allocator<int>::rebind<NODE>::other ax;  // allocator<NODE> ax;
	typename Ax::template rebind<NODE>::other ax;	// allocator<NODE> ax;
public:
	void push_front(cont T& a)
	{
		ax.allocate(1); // memory할당할때 int를 하면 안되고 NODE를 할당해야 하므로 rebind라는 것을 사용
	}
};
```


# 25. SFINAE
- 함수 찾는 순서 
  1. exact matching (2번)
  2. template (1번)
  3. variable argument
```cpp
#include <iostream>
using namespace std;

template<typename T> void foo(T t){ cout << "T" << endl; }
void foo(int n){ cout << "int" << endl; }
void food(...){ cout << "..." << endl; }
int main()
{
	foo(3); // int
}
```

- SFINAE 개념
  - template에서 에러가 나오는게 아니라 , 가변인자를 사용하게 된다. (int::type은 없어서 error가 나야 정상일듯하지만....)
  1. Substitution Failure is Not An Error
  2. 함수 템플릿 을 사용시 T의 타입이 결정되고 함수를 생성(instantiation)할고 할때 리턴 타입이나 함수 인자등에서 치환에 실패하면 컴파일 에러가 아니라, 함수 후보군에서 제외한다.
  3. 동일한 이름의 다른 함수가 있다면 다른 함수를 사용하게 된다.
```cpp
#include <iostream>
using namespace std;

template<typename T> 
typename T::type foo(T t)  // int::type foo(int t) ,  int::type은 잘못된 표현
{ 
	cout << "T" << endl; 
	return 0; 
}
void food(...){ cout << "..." << endl; }
int main()
{
	foo(3); // T
}
```

- enable_if
  - C++표준에서 정의하는 도구
```cpp
template<bool b, typename T= void> struct enable_if
{
};
template<typename T> struct enable_if<true,T>
{
	typedef T type;
}
int main()
{
	enable_if<true,int>::type t0;
	enable_if<true>::type* t0;  // void*
	enable_if<false,int>::type* t0;  // error. type이 없다.
	enable_if<false>::type t3; // error . type이 없다.
}
```

- enable_if + SNINAE
- 조건을 만족할때만 함수를 생성하는 방법
  1. static_assert: 조건을 만족하지 않으면 컴파일 에러
  2. enable_if: 조건을 만족하지 않으면 함수를 행성하지 않음. : 동일 이름의 다른 함수가 있다면 사용.
```cpp
// 정수일때만 함수 코드를 생성하고 싶다.
/* 
template<typename T> void foo(T a)
{
	static_assert(is_integral<T>::value, "error");
}
*/

template<typename T> 
enable_if<is_integral<T>::value  >::type	// 3.4일 경우는 여기서 error이므로 SNINAE으로 후보군에서 제외
                                // 없으면 default void
foo(T a)
{
	cout << "T" << endl;
}

void foo(...)
{
	cout << "not integer" << endl;
}
int main()
{
	foo(3.4);
}
```

- using enable_if : enable_if의 사용 위치
  1. 함수 리턴 타입  (제일 많이 쓰임)
  2. 함수 인자 타입 -> 생성자에게 주로 사용
  3. template인자 -> 함수 자체의 모양이 단순해 보이는 장점이 있다. (나는 이것을 선호: 함수 모양을 해치지 않음)
```cpp
template<typename T> 
enable_if<is_integral<T>::value >::type	// return값이 복잡
foo(T a)
{
}

template<typename T>
void foo(T a,
       typename enable_if<is_integral<T>::value>::type* = nullptr)
{
}

// template<typename T, void* = nullptr>
template<typename T,
       typename enable_if<is_integral<T>::value>::type* = nullptr>
void foo(T a)
{}

void foo(...)
{
	cout << "not integer" << endl;
}
int main()
{
	foo(3.4);
}
```



- type의 종류에 따라 다르게 동작하는 방법
  1. type_traits(is_pointer) + if constexpr   C++17
  2. type_traits + 함수 오버로딩(false_type , true_type)
  3. type_traits + enable_if   (제일 많이 사용하는 기법)
```cpp
/* 방법 1
tempalte<typename T> void printv(const T& v)
{
	if constexpr (is_pointer<T>::value)   // C++17
		cout << v<< ":" << *v << endl;
	else 
		cout << v << endl;
}
*/

/*
// 방법 2
template <typename T>
void printv_imp(const T& v,true_type)
{
	cout << v << ":" << *v << endl;	
}
template <typename T>
void printv_imp(const T& v, false_type)
{
	cout << v << endl;
}
template<typename T> void printv(const T& v)
{
	printv_imp(v,is_pointer<T>());  // pointer인지에 따라 overloading
}
*/

// 방법 3
tempalte<typename T> 
typename enable_if<is_ponter<T>::value>::type
printv(const T& v)
{
	cout << v << ":" << *v << endl;
}
tempalte<typename T> 
typename enable_if<!is_ponter<T>::value>::type
printv(const T& v)
{
	cout << v << endl;
}

int main()
{
	int n=0;
	printv(n);
	printv(&n);
}
```


- enable_if vs integral_constant (android framework)
  - platform_system_core-master/libutils/include/utils/type_helpers.h
    - copy_type(TYPE* d,const TYPE* s, size_t n)






# C++ 20
## 26. concept (C++20)
- concept 개념
  1. 일반적으로 함수 템플릿 (또는 클래스 템플릿)은 모든 타입이 아니라 특정 조건을 만족하는 타입에 대해서 동작하게 된다. 
  2. **타입이 가져야 하는 조건을 코드로 표현** 한 것
  3. 2000년 중반부터 이야기 되었으나 아직 표준화되지 않음
  4. C++20 표준에 추가될 예정
  5. Concept분법이 도입될 경우 템플릿 코드에 많은 변화가 있게 된다.
```cpp
struct Point
{
	int x,y;
};
template <typename T> T Min(T x, Ty)
{
	return y < x ? y : x;
}

int main()
{
	Point p1,p2;
	Min(p1,p2);	// 컴파일하면 error발생 왜냐면 크기 연산자가 없기 때문에 "<"
}
```

- 이전 코드에 concept 적용
  - concet이 표준으로 들어오면 방법3과 같이 사용될 것이다.
```cpp
struct Point
{
	int x,y;
	bool operator<(const Point& p){ return true; }   // 이것을 넣으면 에러가 안 나옴
};

// 1. concept 만들기
template <typename T>
concept bool LessThanComparable = requires(T a,T b)
{
	{ a< b} -> bool;     
}

// 2. template 만들때 concept 표기
// -fconcept 를 붙여야 한다.
/*  방법 1
template <typename T>   requires LessThanComparable<T>      T Min(T x, T y)
{
	return y < x ? y : x;
}



// 방법 2
template <typename T>      T Min(T x, T y)    requires LessThanComparable<T>   
{
	return y < x ? y : x;
}
*/


// 방법 3 : T 대신 이것을 사용
LessThanComparable  Min(LessThanComparable x, LessThanComparable y)    
{
	return y < x ? y : x;
}

int main()
{
	Point p1,p2;
	Min(p1,p2);	// 컴파일하면 error발생 왜냐면 크기 연산자가 없기 때문에 "<"
}
```


# what does this class teach?
- template meta language. -> just runs in compile time.
