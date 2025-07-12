#pragma once

#include <string>
#include <vector>
#include <map>

using std::string;

class MAP {
public:
	// 边结点
	struct EdgeNode {
		// 边的目的地id， 距离 // %：id改成了to
		int to;
		long long distance;
	};
	std::map<string, int> Loclist;//地点集
	std::vector<std::vector<EdgeNode>> Roads;//邻接表，第一个元素为空 //%：改用vector更方便使用
	MAP() { Roads.push_back(std::vector<EdgeNode>()); }
	bool InsertLocation(const string& target);//添加元素
	bool InsertRoad(int id1, int id2, int distance);
	std::vector<long long> Dijkstra(int start);//计算单个点到各个点的最小距离,求所有节点也可使用Floyd算法
};
struct Package {
	int id;
	int weight;
	int dest;//目的地
	int Stime;//包裹送达驿站时间
	int Ttime;//包裹最晚送达时间
	struct ttimecmp {
		bool operator()(const Package& a, const Package& b) {
			return a.Ttime > b.Ttime;
		}
	};
};
struct trolley {//快递小车
	int speed;
	int dweight;//自重
	int maxlweight;//最大装载重量
	int curload = 0;
};
class HeapSort_For_PA {
private:
	vector<long long> elems;
	vector<vector<long long>>& dists;
	map<long long, priority_queue <Package, vector<Package>,Package::ttimecmp>>& spack;
	long long& spot;
public:
	HeapSort_For_PA(vector<vector<long long>>& dists,
		map<long long, priority_queue <Package, vector<Package>, Package::ttimecmp>>& spack,long long& spot)
		:spack{spack}, dists{ dists }, spot{spot}{}
	void push(long long elem);
	bool compare(const long long& a, const long long& b);
	void swiftdown(long long i, long long n);//i为根节点位置，n为节点总数
	long long pop();
	void Resort();
	bool empty();
};
string task3(const string& command);
string extask1(const string& command);
//struct Node { int loc; /* … */ };
//auto cmp = [&](const Node& a, const Node& b) { /* 动态比较 */ };
//std::priority_queue<Node, vector<Node>, decltype(cmp)> pq(cmp);