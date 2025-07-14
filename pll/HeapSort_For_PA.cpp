
#include <iostream>
#include <cmath>
#include <iomanip>
#include <cstring>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>
#include <cassert>
#include <unordered_map>
#include <queue>
#include <unordered_set>
#include <filesystem>
#include <windows.h>
#include <climits>
using namespace std;
#include <set>
#include <cctype>
#include <stack>
#include "CaiNiaoStation.h"
bool HeapSort_For_PA::compare(const long long& a, const long long& b) {
	if (spack[a].top().Ttime != spack[b].top().Ttime)
		return spack[a].top().Ttime < spack[b].top().Ttime;
	return dists[spot][a] < dists[spot][b];
}//����ص�������İ��������������ǰ�棬���һ�����򿿽���ǰ�ص����ǰ�棬����cost����
void HeapSort_For_PA::push(long long elem) {
	elems.push_back(elem);
	long long k = elems.size() - 1;
	while (k > 0 && compare(elems[k], elems[(k - 1) / 2])) {
		swap(elems[k], elems[(k - 1) / 2]);
		k = (k - 1) / 2;
	}
}
void HeapSort_For_PA::swiftdown(long long i, long long n) {
	long long j, t;
	t = elems[i];
	while ((j = 2 * i + 1) < n) {
		if (j < n - 1 && compare(elems[j + 1], elems[j])) j++;
		if (compare(elems[j], t)) {//���t���ӽ��С�������϶ѵĶ���
			elems[i] = elems[j]; //���ϴ��ֵ�����a[i]��
			elems[j] = t; // ��ɡ�������
			i = j; //���£����a[j]��Ϊ�µ�Ҫ�����������ĸ�
		}
		else
			break;
	}
}
long long HeapSort_For_PA::pop() {
	long long result = elems[0];
	swap(elems[0], elems[elems.size() - 1]);
	elems.pop_back(); 
	if(!empty())
		swiftdown(0, elems.size());
	return result;
}
void HeapSort_For_PA::Resort() {
	long long i;
	//�����һ����Ҷ�ӽ�㿪ʼ�������������ϣ�һֱ����a[0],���ɳ�ʼ��
	for (i = (elems.size() - 1) / 2; i >= 0; i--)
		swiftdown( i, elems.size());
}
bool  HeapSort_For_PA::empty() {
	if (elems.size() == 0)
		return true;
	return false;
}
//���ȿ���ʱ������ԣ����ٳɱ�;
bool HeapSort_For_PA_Cost_First::compare(const long long& a, const long long& b) {
	long long costA = (curLoad + dweight) * dists[spot][a];
	long long costB = (curLoad + dweight) * dists[spot][b];
	if (costA != costB)
		return costA < costB;
	return spack[a].top().Ttime < spack[b].top().Ttime;
}