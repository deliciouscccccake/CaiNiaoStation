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
struct Edge
{
    int v;
	long long d;
    struct cmp {
        bool operator()(const Edge& a, const Edge& b) {
            return a.d > b.d;
        }
	};
};
bool MAP::InsertLocation(const string &target) {
	auto it = Loclist.find(target);
	if (it == Loclist.end())
	{
		Loclist[target]=Loclist.size();
		Roads.push_back(nullptr);
		return true;
	}
	return false;
}
bool MAP::InsertRoad(int id1, int id2, int distance) {
	EdgeNode* temp = Roads[id1];
	Roads[id1] = new EdgeNode{ temp,id2,distance };
	temp = Roads[id2];
	Roads[id2] = new EdgeNode{ temp,id1,distance };
	return true;
}

vector<int> MAP::Dijkstra(int start){	
	vector<int> visited(Loclist.size(),0);//记录遍历过的节点
	vector<int> dist(Loclist.size(), LONG_LONG_MAX);//记录最小值
	//vector<int> path(Loclist.size(), 0);//记录上一个节点，暂时不用
	priority_queue<Edge, vector<Edge>, Edge::cmp> prqueues;//存入可用的边
	EdgeNode* ptr = Roads[start]; visited[start] = 1; dist[start] = 0;
	while (ptr != nullptr) {
		dist[ptr->id] = ptr->distance;
		prqueues.push({ptr->id, ptr->distance});
		ptr = ptr->next;
	}
	while (!prqueues.empty()) {
		int curV = prqueues.top().v; int curW = prqueues.top().d;
		prqueues.pop();
		if (visited[curV]) continue;
		ptr = Roads[curV];
		while (ptr != nullptr) {
			if (!visited[ptr->id] && ptr->distance + dist[curV] < dist[ptr->id]) {
				dist[ptr->id] = ptr->distance + dist[curV];
				prqueues.push({ ptr->id,dist[ptr->id] });
			}
				ptr = ptr->next;
		}
		visited[curV] = 1;
	}
	return dist;
}
string task3(const string& command) {
	stringstream ss(command);
	int carnum; ss >> carnum;
	trolley car;
	for (int i = 0; i < carnum; i++) //形式化，其实只有一辆车
		ss >> car.speed >> car.dweight >> car.maxlweight;
	int packnum; ss >> packnum;
	vector<Package> packages(packnum);
	for (int i = 0; i < packnum; i++)
		ss >> packages[i].id >> packages[i].weight >> packages[i].dest >> packages[i].Stime >> packages[i].Ttime;

}