// cpp_stl_map.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

/*
* map
* 
* 균형이 잡힌 이진탐색트리
*/
#include <iostream>
#include <map>
using namespace std;

typedef map<int, string> CRyuMap;
typedef CRyuMap::value_type ValueType;

int main()
{
    //키/값 쌍의 데이터를 다루도록 만들어져있다.
    //  왜냐하면, 이진탐색트리는 중복된 데이터가 없어야 하는데, 실제 담아둘 데이터들은 중복될 수도 있으므로
    //  검색용 데이터를 따로 두고 실제 데이터와 매핑해둔 것이다.
    
    CRyuMap tMap;  //이진탐색트리

    tMap.insert(ValueType(100, "One Hundred"));

    return 0;
}