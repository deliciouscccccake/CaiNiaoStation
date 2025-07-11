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
#include <set>
#include <cctype>
#include <stack>

#include "CaiNiaoStation.h"

using namespace std;

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
		Roads.push_back(std::vector<EdgeNode>());
		return true;
	}
	return false;
}
bool MAP::InsertRoad(int id1, int id2, int distance) {
	Roads[id1].push_back({id2, distance});
    Roads[id2].push_back({id1, distance});
	return true;
}

vector<long long> MAP::Dijkstra(int start){	
	vector<int> visited(Loclist.size(), 0);//记录遍历过的节点
	vector<long long> dist(Loclist.size(), INT_MAX);//记录到起点的最短路径长
	//vector<int> path(Loclist.size(), 0);//记录上一个节点，暂时不用
	priority_queue<Edge, vector<Edge>, Edge::cmp> prqueues;//存入可用的dist

    //初始化
    dist[start] = 0;
	prqueues.push({start, 0});
    while(!prqueues.empty())
    {
        auto cur = prqueues.top();
        prqueues.pop();
        if(visited[cur.v])continue;
        visited[cur.v] = true;
        for(const auto &ptr:Roads[cur.v])
        {
            if(dist[cur.v] + ptr.distance < dist[ptr.to])
            {
                dist[ptr.to] = dist[cur.v] + ptr.distance;
                prqueues.push({ptr.to, dist[ptr.to]});
            }
        }
    }
	return dist;
}

void Init(const string& command)
{
	istringstream iss(command);
	int n, m;
	iss >> n >> m;
	for(int i = 0;i < n;++i)
	{
		string t;
		iss >> t;
		g.InsertLocation(t);
	}
	for(int i = 0;i < m;++i)
	{
		int u, v, w;
		iss >> u >> v >> w;
		g.InsertRoad(u, v, w);
	}
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
