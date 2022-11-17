// cpp_stl_map.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

/*
* map
* 
* 균형이 잡힌 이진탐색트리
* 
* 이진탐색트리: 탐색을 목적으로 자료구조
* 
* i) 이진트리여야 한다
* ii) 중복된 값이 있어야 한다
* iii) 왼쪽 서브트리의 값은 루트보다 작고, 오른쪽 서브트리의 값은 루트보다 크다
* iv) 서브트리도 이진탐색트리여야 한다
*/
#include <iostream>
#include <map>
#include <algorithm>
using namespace std;

//두 원소의 데이터를 비교하는 함수객체를 위한 클래스
//템플릿으로 만들었다.
template<typename F, typename S>
class CEqualValue
{
private:
    F first;
    S second;

public:
    //매개변수 있는 생산자
    CEqualValue(const S& t)
        :second(t)
    {}
    //비교 연산자 오버로드
    bool operator()(pair<F, S> tE)
    {
        return second == tE.second;
    }
};

typedef map<int, string> CRyuMap;
typedef CRyuMap::value_type ValueType;

int main()
{
    //키/값 쌍의 데이터를 다루도록 만들어져있다.
    //  왜냐하면, 이진탐색트리는 중복된 데이터가 없어야 하는데, 실제 담아둘 데이터들은 중복될 수도 있으므로
    //  검색용 데이터를 따로 두고 실제 데이터와 매핑해둔 것이다.
    
    CRyuMap tMap;  //이진탐색트리

    //-------데이터를 추가-------
    tMap.insert(ValueType(100, "One Hundred"));
    tMap.insert(ValueType(3, "Three"));
    tMap.insert(ValueType(150, "One Hundred and Fifty"));
    tMap.insert(ValueType(99, "Ninety Nine"));
    tMap.insert(ValueType(98, "Ninety Nine"));

    CRyuMap::iterator tItor;
    //데이터 출력
    //  map의 순회 구문은 중위순회로 설정되어 있다.
    //키에 대해서, 정렬된 결과를 얻는다. <-- 삽입과 동시에 자동으로 정렬된다.
    for (tItor = tMap.begin(); tItor != tMap.end(); ++tItor)
    {
        cout << "key: " << tItor->first << " value: " << tItor->second << endl;
    }

    //중복된 키는 무시한다.
    tMap.insert(ValueType(99, "ryu ryu ryu"));

    cout << endl;
    for (tItor = tMap.begin(); tItor != tMap.end(); ++tItor)
    {
        cout << "key: " << tItor->first << " value: " << tItor->second << endl;
    }

    cout << endl;
    //연관 배열
    // 배열처럼 표기법을 제공한다. 인덱스가 들어갈 자리에는 '키'를 넣는다. 그러면 '값'을 얻는다
    cout << tMap[3].c_str() << endl;

    cout << endl;
    //배열 표기법을 사용하여 없는 키를 적용하면, 에러가 나는 것이 아니라
    //새로운 원소로 인식하여 추가한다.
    cout << "tMap[1]: " << tMap[1].c_str() << endl;

    cout << endl;
    for (tItor = tMap.begin(); tItor != tMap.end(); ++tItor)
    {
        cout << "key: " << tItor->first << " value: " << tItor->second << endl;
    }

    //위의 특성을 이용하면, 새로운 원소를 추가하는 표기법을 아래와 같이 간단히 할 수 있다.
    tMap[777] = "GoodGoodGood";

    cout << endl;
    for (tItor = tMap.begin(); tItor != tMap.end(); ++tItor)
    {
        cout << "key: " << tItor->first << " value: " << tItor->second << endl;
    }


    //-------데이터를 검색-------
    // 이 함수의 검색 시간복잡도는 O(log n)이다. <-- 이진탐색트리, 키로 검색
    //tItor = tMap.find(777);
    tItor = tMap.find(123);
    if (tItor != tMap.end())
    {
        cout << endl;
        cout << "Find succeeded" << endl;
        cout << "key: " << tItor->first << " value: " << tItor->second << endl;
    }
    else
    {
        cout << endl;
        cout << "find failed" << endl;
    }

    tItor = find_if(tMap.begin(), tMap.end(), CEqualValue<int, string>("One Hundred and Fifty"));

    //검색 시간복잡도는 O(n)이다. <-- 값 데이터를 하나하나 다 비교해서 찾기 때문이다.
    //함수 객체를 생성해서 넘겨줌( 생성자 호출 )
    if (tItor != tMap.end())
    {
        cout << endl;
        cout << "Find succeeded" << endl;
        cout << "key: " << tItor->first << " value: " << tItor->second << endl;
    }
    else
    {
        cout << endl;
        cout << "find failed" << endl;
    }


    //-------데이터를 삭제-------
    tItor = tMap.find(3);
    if (tItor != tMap.end())
    {
        tMap.erase(tItor);
    }

    cout << endl;
    for (tItor = tMap.begin(); tItor != tMap.end(); ++tItor)
    {
        cout << "key: " << tItor->first << " value: " << tItor->second << endl;
    }

    //직접 값으로 검색
    for (tItor = tMap.begin(); tItor != tMap.end();)
    {
        if ("GoodGoodGood" == tItor->second)
        {
            //map의 erase는 다른 컨테이너와는 다르게 작동한다.
            //map의 erase는 리턴값으로 반복자의 다음 위치를 가리키지 않는다.
            //그러므로 순회문 안에서 안전한 제거를 위해 해당 구문을 사용하려면, 다음과 같이 처리해야 한다.

            //tItor = tMap.erase(tItor);
            tMap.erase(tItor++);
        }
        else
        {
            ++tItor;
        }
    }

    cout << endl;
    for (tItor = tMap.begin(); tItor != tMap.end(); ++tItor)
    {
        cout << "key: " << tItor->first << " value: " << tItor->second << endl;
    }

    //delete all
    tMap.clear();

    cout << endl;
    cout << "after clear" << endl;
    for (tItor = tMap.begin(); tItor != tMap.end(); ++tItor)
    {
        cout << "key: " << tItor->first << " value: " << tItor->second << endl;
    }

    return 0;
}