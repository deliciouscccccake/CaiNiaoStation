#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <map>

using std::string;

class MAP {
public:
	// 边结点
	struct EdgeNode {
		// 边的目的地id， 距离
		int to;
		long long distance;
	};
	std::map<string,int> Loclist;//地点集
	std::vector<std::vector<EdgeNode>> Roads;//邻接表，第一个元素为空
	MAP() {Roads.push_back(std::vector<EdgeNode>());}
	bool InsertLocation(const string &target);//添加元素
	bool InsertRoad(int id1, int id2, int distance);//添加id1和id2之间的双向边
	std::vector<long long> Dijkstra(int start);//计算单个点到各个点的最小距离
	std::pair<std::vector<int>, std::vector<long long>> Prim(int s);//从s开始计算最小生成树，返回两个vector，分别表示最小生成树上：结点的双亲编号，结点的双亲边的长度
}g;
struct Package {
	int id;
	int weight;
	string dest;//目的地
	int Stime;//包裹送达驿站时间
	int Ttime;//包裹最晚送达时间
};
struct trolley {//快递小车
	int speed;
	int dweight;//自重
	int maxlweight;//最大装载重量
};

class Tree
{
private:
	int **fa, n, lgn, *dep; // 存储双亲以及多层双亲，需确保根节点的双亲是自己，第一维是结点编号，第二维是双亲层数(fa[][j]是第2^j个双亲)；结点数；(log_2)结点数 取上界；结点的深度
	long long **maxd; // maxd[][j]是到第2^j个双亲的路径上的最长边
	// 没有保存根结点，等需要再说

	int init_dfs(int p);
public:
	Tree(std::vector<int> f, std::vector<long long> d) // 只要一层双亲数组、双亲边的长度和结点数量即可建树，注意根节点的双亲应该是自己
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
	~Tree();

	long long MaxdOnLCA(int a, int b) // 返回树中结点a和结点b之间路径上的最长边（如果先暂时不反悔LCA了，等需要再说）
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
};

void Init(const string& command); //接受的string是从文件读入的结果，预处理（如导入图），返回类型暂时为void，可根据需要更改

string task3(const string& command);
string task4(const string& command);
string task5(const string& command);
string extask1(const string& command);
string extask2(const string& command);
string extask3(const string& command);