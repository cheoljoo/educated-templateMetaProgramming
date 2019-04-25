Homework 1 :
- 1-1.cpp : # get sum with stattic_assert example
	- make 1-1
```
# stattic_assert example
g++ -std=c++1z 1-1.cpp -o 1-1
1-1.cpp:10:5: error: static_assert failed "not integral"
    static_assert( is_integral<T>::value, "not integral");
    ^              ~~~~~~~~~~~~~~~~~~~~~
1-1.cpp:28:5: note: in instantiation of function template specialization 'Sum<double>' requested here
    Sum(d1, d2); // error가 나와야 합니다
    ^
1 error generated.
make: *** [1-1] Error 1
```

- 1-2.cpp : # get sum with enable if example
	- make 1-2
```
# enable if example
g++ -std=c++1z 1-2.cpp -o 1-2
1-2.cpp:30:5: error: no matching function for call to 'Sum'
    Sum(d1, d2); // error가 나와야 합니다
    ^~~
1-2.cpp:10:1: note: candidate template ignored: requirement 'is_integral<double>::value' was not satisfied [with T = double]
Sum(T from,T to)
^
1 error generated.
make: *** [1-2] Error 1
```

- 1.cpp : # get sum well when we remove the double
	- make 1
        - work well : all sum are 45 about integer and short.
```
i
45
s
45
```

Homework 2 :
- 2.cpp : get average example
	- make 2
        - work well : average is 5.5 about integer array and stl container
```
i size:10
 1 2 3 4 5 6 7 8 9 10
5.5
NSt3__16vectorIiNS_9allocatorIiEEEE
1 2 3 4 5 6 7 8 9 10
5.5
```

Homework 3 :
- 3.cpp : hash vistual function
	- make 3
```
class V  { virtual void f() { }      } => 1V has virtual function
class NV {         void f() { }      } => 2NV has not virtual function
class E  {                           } => 1E has not virtual function
class F  { virtual void f() final{ } } => 1F has virtual function
class D : public V { void t(){ }     } => 1D has virtual function
```


Homework 4 :
- 4.cpp : arguments count error check 
	- make 4
```
1
1259
cpp_print("%d" ,1 ,2 ,3) ==> 예외 발생 1

cpp_print("%d%d" ,1) ==> 예외 발생 2

cpp_print("%d %d" ,1 ,2) ==> 1 2
```


Homework 5 :
- 5.cpp : Typelist
	- make 5
        - (1) TypeAt : N번째 타입이 무슨 타입인지 알아내는 템플릿 입니다.
```
i
c
d
s
```
        - (2) IndexOf : 특정 타입이 몇번째 있는지 찾아 내는 템플릿입니다.
```
2
```
        - (3) Erase : 특정 타입을 제거한 새로운 TypeList 만들기
```
8TypelistIiS_IdS_Is8NullTypeEEE
i
d
s
8NullType
```
        - (4) Replace : 특정 타입을 다른 타입으로 교체한 TypeList 만들기
```
8TypelistIiS_IfS_IdS_Is8NullTypeEEEE
i
f
d
s
```

- results

```
i
c
d
s
2
8TypelistIiS_IdS_Is8NullTypeEEE
i
d
s
8NullType
8TypelistIiS_IfS_IdS_Is8NullTypeEEEE
i
f
d
s
```

