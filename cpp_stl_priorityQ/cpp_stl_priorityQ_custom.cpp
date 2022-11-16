#include <iostream>
#include <queue>
using namespace std;

class CRectRyu
{
private:
	int mWidth = 0;
	int mHeight = 0;

public:
	CRectRyu(int tWidth, int tHeight)
		:mWidth(tWidth), mHeight(tHeight)
	{}

	bool operator>(const CRectRyu& t) const
	{
		return mWidth* mHeight > t.mWidth * t.mHeight;
	}

	bool operator<(const CRectRyu& t) const
	{
		return mWidth* mHeight < t.mWidth * t.mHeight;
	}

	void Display()
	{
		cout << "CRectRyu " << mWidth << " * " << mHeight << " = " << mWidth * mHeight << endl;
	}
};
//최소힙, 오름차순 정렬된 결과를 낸다
typedef priority_queue<CRectRyu, vector<CRectRyu>, greater<CRectRyu>> CPriorityQGreater;
//최대힙, 내림차순 정렬된 결과를 낸다
typedef priority_queue<CRectRyu, vector<CRectRyu>, less<CRectRyu>> CPriorityQLess;

int main()
{
	CPriorityQGreater tPriorityQ_0;

	CRectRyu tRect_0(1, 5);
	CRectRyu tRect_1(7, 6);
	CRectRyu tRect_2(1, 2);
	CRectRyu tRect_3(8, 2);

	tPriorityQ_0.push(tRect_0);
	tPriorityQ_0.push(tRect_1);
	tPriorityQ_0.push(tRect_2);
	tPriorityQ_0.push(tRect_3);

	while (!tPriorityQ_0.empty())
	{
		CRectRyu tRect = tPriorityQ_0.top();
		tPriorityQ_0.pop();

		tRect.Display();
	}

	//

	cout << endl;

	CPriorityQLess tPriorityQ_1;

	tPriorityQ_1.push(tRect_0);
	tPriorityQ_1.push(tRect_1);
	tPriorityQ_1.push(tRect_2);
	tPriorityQ_1.push(tRect_3);

	while (!tPriorityQ_1.empty())
	{
		CRectRyu tRect = tPriorityQ_1.top();
		tPriorityQ_1.pop();

		tRect.Display();
	}

	return 0;
}