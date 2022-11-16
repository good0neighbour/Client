// cpp_stl_list.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

//STL Standard Template Library
//STL의 세가지 구성요소:
/*
* container: 자료구조를 일반화하여 범용적으로 만들어놓은 것
* iterator: container와 algorithm과 같이 사용할 수 있게 만들어진 일반화된 포인터
* algorithm: 알고리즘을 일반화하여 범용적으로 만들어 놓은 것
*/

//stl의 list컨테이너
#include <iostream>
#include <list>
using namespace std;

class CUnit
{
private:
    int mVal = 0;

public:
    CUnit(int tVal)
        :mVal(tVal){

    }

    void SetVal(int tVal)
    {
        mVal = tVal;
    }
    int GetVal()
    {
        return mVal;
    }
};

//stl의 list는 double linked list로 만들어졌다.

int main()
{
    /*
    CUnit으로 다루면 복사되어 CUnit객체가 새로 만들어지는 것이다.

    list<CUnit> tUnitList;

    CUnit tUnit(0);
    tUnitList.push_back(tUnit);
    */

    //CUnit타입의 객체를 다루는 링크드리스트 선언
    list<CUnit*> tUnitList;

    CUnit* tpUnit = nullptr;

    //0번 아이디를 가진 유닛
    tpUnit = nullptr;
    tpUnit = new CUnit(0);
    tUnitList.push_back(tpUnit);

    //1번 아이디를 가진 유닛
    tpUnit = nullptr;
    tpUnit = new CUnit(1);
    tUnitList.push_back(tpUnit);

    //2번 아이디를 가진 유닛
    tpUnit = nullptr;
    tpUnit = new CUnit(2);
    tUnitList.push_back(tpUnit);

    list<CUnit*>::iterator tItor;
    for (tItor = tUnitList.begin(); tItor != tUnitList.end(); ++tItor)
    {
        cout << "원소의 값" << (*tItor)->GetVal() << endl;
    }

    //인덱스 접근 연산자는 지원하지 않는다. 링크드 리스트이므로 임의의 접근은 불가능하다.
    //int tId = tUnitList[0]->GetVal();

    //순차검색 O(n)
    for (tItor = tUnitList.begin(); tItor != tUnitList.end(); ++tItor)
    {
        if (2 == (*tItor)->GetVal())
        {
            (*tItor)->SetVal(777);
            break;
        }
    }

    cout << endl;
    for (tItor = tUnitList.begin(); tItor != tUnitList.end(); ++tItor)
    {
        cout << "원소의 값" << (*tItor)->GetVal() << endl;
    }

    cout << endl;

    //역순으로 순회
    //stl의 list는 더블링크드리스트로 만들어졌으므로 역순으로 순회가 가능하다, reverse_iterator가 존재한다
    list<CUnit*>::reverse_iterator tRItor;
    for (tRItor = tUnitList.rbegin(); tRItor != tUnitList.rend(); ++tRItor)
    {
        cout << "원소의 값" << (*tRItor)->GetVal() << endl;
    }

    cout << endl;

    //순차검색 O(n)
    for (tItor = tUnitList.begin(); tItor != tUnitList.end();)
    {
        if (1 == (*tItor)->GetVal())
        {
            if (nullptr != (*tItor))
            {
                delete (*tItor);
            }

            //list의 erase는 다음 이터레이터를 리턴한다.
            tItor = tUnitList.erase(tItor);
        }
        else
        {
            ++tItor;
        }
    }

    //모두 지우기

    //동적으로 할당된 것들은 모두 삭제해주어야 한다.
    for (tItor = tUnitList.begin(); tItor != tUnitList.end(); ++tItor)
    {
        cout << "원소의 값" << (*tItor)->GetVal() << endl;
    }

    for (tItor = tUnitList.begin(); tItor != tUnitList.end(); ++tItor)
    {
        if (nullptr != (*tItor))
        {
            delete (*tItor);
            (*tItor) = nullptr;
        }
    }
    //리스트의 원소들을 모두 지운다
    tUnitList.clear();

    cout << endl;
    cout << "리스트의 원소 표시===" << endl;
    for (tItor = tUnitList.begin(); tItor != tUnitList.end(); ++tItor)
    {
        cout << "원소의 값" << (*tItor)->GetVal() << endl;
    }

    return 0;
}