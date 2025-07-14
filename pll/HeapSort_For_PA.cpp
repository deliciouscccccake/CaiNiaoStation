
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
}//如果地点最紧急的包裹最紧急，则在前面，如果一样，则靠近当前地点的在前面，减少cost消耗
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
		if (compare(elems[j], t)) {//如果t比子结点小，不符合堆的定义
			elems[i] = elems[j]; //将较大的值放入根a[i]。
			elems[j] = t; // 完成“交换”
			i = j; //向下：结点a[j]作为新的要调整的子树的根
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
	//从最后一个非叶子结点开始调整，依次向上，一直到根a[0],构成初始堆
	for (i = (elems.size() - 1) / 2; i >= 0; i--)
		swiftdown( i, elems.size());
}
bool  HeapSort_For_PA::empty() {
	if (elems.size() == 0)
		return true;
	return false;
}
//优先考虑时间相近性，减少成本;
bool HeapSort_For_PA_Cost_First::compare(const long long& a, const long long& b) {
	long long costA = (curLoad + dweight) * dists[spot][a];
	long long costB = (curLoad + dweight) * dists[spot][b];
	if (costA != costB)
		return costA < costB;
	return spack[a].top().Ttime < spack[b].top().Ttime;
}