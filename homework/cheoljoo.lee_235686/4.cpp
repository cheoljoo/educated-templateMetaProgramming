#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <vector>

using namespace std;

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
    vector<int> v;

    const string p = static_cast<const string>(value);
    int sc = countSubstring(p,"%d");
	(v.push_back(args), ...);

    cout << "cpp_print(\"" << p << "\"";
    //(cout << ... << args);  // - binary left fold : (init op ... op args) -> (((init op E1) op E2) op E3) op E4
    for(int i=0;i<v.size();i++){
        cout << " ," << v[i] ;
    }
    cout << ") ==> " ;

    if(sc != v.size())  throw "Different argument's count";
    else printf(value,args...);
}

int main() {
    printf("%d\n", 1, 2, 3); // 인자가 너무 많습니다. 
    printf("%d%d\n", 1); // 인자가 부족 합니다.

    // cpp_print를 만들어 보세요
    try
    {
        cpp_print("%d", 1, 2, 3); // 예외가 나오게 하세요 
    }
    catch (...)
    {
        cout << "예외 발생 1" << endl; 
    }
    cout << endl;

    try
    {
        cpp_print("%d%d", 1); // 예외가 나오게 하세요
    }
    catch (...)
    {
        cout << "예외 발생 2" << endl; 
    }
    cout << endl;

    try
    {
        cpp_print("%d %d", 1,2);  // normal
    }
    catch (...)
    {
        cout << "예외 발생 3" << endl; 
    }
    cout << endl;
}
