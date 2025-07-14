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
		int id = Loclist.size();
		Loclist[target] = id + 1;
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
	vector<int> visited(Loclist.size() + 1, 0);//记录遍历过的节点
	vector<long long> dist(Loclist.size() + 1, LONG_LONG_MAX);//记录到起点的最短路径长
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
            if(dist[cur.v] + ptr.distance < dist[ptr.to]) // 不需要visited判断，因为遍历过的一定存的是最短路径（Prim需要）
            {
                dist[ptr.to] = dist[cur.v] + ptr.distance;
                prqueues.push({ptr.to, dist[ptr.to]});
            }
        }
    }
	return dist;
}

std::pair<std::vector<int>, std::vector<long long>> MAP::Prim(int s=1)
{
	vector<int> visited(Loclist.size() + 1, 0); // 记录遍历过的节点
	vector<long long> dist(Loclist.size() + 1, LONG_LONG_MAX); //记录能拓展到该节点的最短边
	vector<int> pre(Loclist.size() + 1, 0);//记录父节点
	priority_queue<Edge, vector<Edge>, Edge::cmp> prqueues;//存入可用的dist // 没有用手写的优先队列，这又何尝不是一种背叛（）

    //初始化
    dist[s] = 0;
	prqueues.push({s, 0});
	pre[s] = s;
    while(!prqueues.empty())
    {
        auto cur = prqueues.top();
        prqueues.pop();
        if(visited[cur.v])continue;
        visited[cur.v] = true;
        for(const auto &ptr:Roads[cur.v])
        {
            if(!visited[ptr.to] && ptr.distance < dist[ptr.to])
            {
                dist[ptr.to] = ptr.distance;
				pre[ptr.to] = cur.v;
                prqueues.push({ptr.to, dist[ptr.to]});
            }
        }
    }
	return {pre, dist};
}

int Tree::init_dfs(int p)
{
	if(fa[p][0] == p)return dep[p] = 0;
	else if(dep[p])return dep[p]; // 只有根结点和没被遍历过的结点的深度才是0
	else return dep[p] = init_dfs(fa[p][0]) + 1;
}

Tree::~Tree()
{
	for(int i = 1;i <= n;++i)
	{
		delete[] fa[i], maxd[i];
	}
	delete[] fa, maxd;
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

string task5(const string &command) // 根据样例给的输入，函数内还需建一个新图
{
	MAP g;//覆盖全局变量g
	int n, m, q;
	string outStr = "";
	std::istringstream iss(command);

	iss >> n >> m;
	for(int i = 1;i <= n;++i)
		g.InsertLocation(std::to_string(i)); // 单纯是为了配合动态建图的过程，不想再新写一个没有地点名称的建图了
	for(int i = 1;i <= m;++i)
	{
		int u, v, w;
		iss >> u >> v >> w;
		g.InsertRoad(u, v, w);
	}
	auto mSTRes = g.Prim(); // 最小生成树的结果
	Tree mST = Tree(mSTRes.first, mSTRes.second);
	iss >> q;
	for(int i = 1;i <= q;++i)
	{
		int x, y;
		iss >> x >> y;
		outStr += to_string(mST.MaxdOnLCA(x, y)) + '\n';
	}
	return outStr;
}

int main() // 只是为了方便测试，到时迁移到Qt时需要删去
{
	cout << task5("4 4\n1 2 4\n2 3 3\n3 1 1\n1 4 2\n3\n3 4\n1 2\n4 2\n");
	return 0;
}