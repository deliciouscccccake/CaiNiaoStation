#include <iostream>
#include <cmath>
#include <iomanip>
#include <cstring>
#include <vector>
#include <list>
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
Tree::Tree(std::vector<int> f, std::vector<long long> d) // 只要一层双亲数组、双亲边的长度和结点数量即可建树，注意根节点的双亲应该是自己
:fa(new int*[f.size()]), n(f.size() - 1), lgn(0), dep(new int[f.size()]), maxd(new long long*[f.size()])//lgn先等于0
{
	fa[0] = nullptr;
	while((1 << lgn) < n)lgn++;
	for(int i = 1;i <= n;++i)
	{
		fa[i] = new int[lgn + 1];
		maxd[i] = new long long[lgn + 1];
		fa[i][0] = f[i];
		dep[i] = 0;
		maxd[i][0] = d[i];
	}
	for(int i = 1;i <= lgn;++i)
	{
		for(int j = 1;j <= n;++j)
		{
			fa[j][i] = fa[fa[j][i - 1]][i - 1];
			maxd[j][i] = std::max(maxd[j][i - 1], maxd[fa[j][i - 1]][i - 1]);
		}
	}
	for(int i = 1;i <= n;++i)
	{
		init_dfs(i);
	}
}

long long Tree::MaxdOnLCA(int a, int b) // 返回树中结点a和结点b之间路径上的最长边（如果先暂时不返回LCA了，等需要再说）
{
	long long res = 0;
	if(dep[a] > dep[b])std::swap(a, b);//确保dep[a] <= dep[b];
	for(int i = lgn;i >= 0 && dep[a] < dep[b];i--)
	{
		if(dep[a] <= dep[fa[b][i]])
		{
			res = std::max(res, maxd[b][i]);
			b = fa[b][i];
		}
	}
	if(a == b)return res;
	for(int i = lgn;i >= 0;i--)
	{
		if(fa[a][i] != fa[b][i])
		{
			res = std::max(res, std::max(maxd[a][i], maxd[b][i]));
			a = fa[a][i];
			b = fa[b][i];
		}
	}
	res = std::max(res, std::max(maxd[a][0], maxd[b][0]));
	// a = fa[a][0];
	// b = fa[b][0];
	return res;
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

std::list<long long> FindPath(int i, int j, long long **path) // 通过floyd的path数组返回i和j之间的最短路经过的点集（注意不包括i，j本身）
{
	if(path[i][j] == i || path[i][j] == j)
	{
		return list<long long>();
	}
	else
	{
		std::list<long long> res;
		res.splice(res.end(), FindPath(i, path[i][j], path));
		res.push_back(path[i][j]);
		res.splice(res.end(), FindPath(path[i][j], j, path));
		return res;
	}
}

string task4(const string& command)
{
	std::istringstream iss(command);
	int num, v, wc, wl, k, n = g.Loclist.size(); // 只有v, n是有效变量
	iss >> num >> v >> wc >> wl >> k;
	vector<long long> s;
	long long **dis = new long long*[n + 1], **path = new long long*[n + 1];// disloyd记录最短路长和最短路
	long long **f = new long long*[1 << (k + 1)], **pre = new long long*[1 << (k + 1)];
	for(int i = 0;i < (1 << (k + 1));++i)
	{
		f[i] = new long long[k + 1];
		pre[i] = new long long[k + 1];
		for(int j = 0;j <= k;++j)
		{
			f[i][j] = LONG_LONG_MAX >> 1;
		}
	}
	for(int i = 1;i <= n;++i)
	{
		dis[i] = new long long[n + 1];
		path[i] = new long long[n + 1];
		for(int j = 1;j <= n;++j)
		{
			dis[i][j] = LONG_LONG_MAX >> 1;
		}
		for(const auto &e:g.Roads[i])
		{
			dis[i][e.to] = e.distance;
			path[i][e.to] = i; // 保证计算两个结点之间的最短路的递归边界
		} // 超绝慢速预处理
	}
	for(int mid = 1;mid <= n;++mid)
		for(int i = 1;i <= n;++i)
			for(int j = 1;j <= n;++j)
			{
				if(dis[i][mid] + dis[mid][j] < dis[i][j])
				{
					dis[i][j] = dis[i][mid] + dis[mid][j];
					path[i][j] = mid;
				}
			}
	f[1][0] = 0; // 确保只能从起点开始
	s.push_back(1); // 设置起点
	for(int i = 1;i <= k;++i)
	{
		string poss;
		iss >> poss;
		s.push_back(g.Loclist[poss]);
	}
	for(int sta = 3;sta < (1 << (k + 1));++sta) // 本来想用递归，但太多参数要带了，设为全局变量的话，项目里还是不要太多全局变量，所以算了
	{
		if((sta & 1) == 0)continue;
		for(int i = 1;i <= k;++i) // 枚举去掉的城市/终点
		{
			if(sta & (1 << i))
			{
				for(int j = 0;j <= k;++j) // 枚举上一个开始的城市
				{
					if((sta ^ (1 << i)) & (1 << j))
					{
						if(f[sta ^ (1 << i)][j] + dis[s[j]][s[i]] < f[sta][i])
						{
							f[sta][i] = f[sta ^ (1 << i)][j] + dis[s[j]][s[i]];
							pre[sta][i] = j;
						}
					}
				}
			}
		}
	}
	int fin = 1; //最后的结点
	long long minf = f[(1 << (k + 1)) - 1][1] + dis[s[1]][s[0]];
	for(int i = 2;i <= k;++i)
	{
		if(f[(1 << (k + 1)) - 1][i] + dis[s[i]][s[0]] < minf)
		{
			minf = f[(1 << (k + 1)) - 1][i] + dis[s[i]][s[0]];
			fin = i;
		}
	}

	list<long long> res = FindPath(s[fin], s[0], path);
	res.push_back(s[0]);
	for(int p = fin, sta = (1 << (k + 1)) - 1;p;sta ^= (1 << p), p = pre[sta | (1 << p)][p])
	{
		res.push_front(s[p]);
		res.splice(res.begin(), FindPath(s[pre[sta][p]], s[p], path));
	}
	// 得到路线（结点id表示），为了方便输出，没有起点

	string ress;
	vector<const string*> locStr(n + 1); // 获取id对应字符串的列表
	for(const auto &i:g.Loclist)
	{
		locStr[i.second] = &i.first;
	}
	ress += "The shortest path: " + std::to_string(minf) + '\n' + "Path: " + *locStr[s[0]] + ' ';
	for(const auto &i:res)
	{
		ress += "-> " + *locStr[i];
	}

	for(int i = 1;i <= n;++i)
	{
		delete[] dis[i], path[i];
	}
	for(int i = 0;i < (1 << k);++i)
	{
		delete[] f[i], pre[i];
	}

	return ress;
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
	std::ifstream input("g.txt");
	Init(string(istreambuf_iterator<char>(input), istreambuf_iterator<char>()));
	input.close();
	
	input.open("in.txt");
	cout << task4(string(istreambuf_iterator<char>(input), istreambuf_iterator<char>()));
	input.close();
	return 0;
}