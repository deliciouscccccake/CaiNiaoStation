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
using namespace std;
#include <climits>
#include <set>
#include <cctype>
#include <stack>
#include "CaiNiaoStation.h"
int main() {
	int n, m;
	cin >> n >> m; cin.ignore();
	MAP ecmap; string temp;
	for (int i = 0; i < n; i++)
	{
		getline(cin, temp);
		ecmap.InsertLocation(temp);
	}
	for (int i = 0, u, v, w; i < m; i++) 
		ecmap.InsertRoad(u, v, w);
}