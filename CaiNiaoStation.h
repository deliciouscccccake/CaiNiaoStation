#pragma once

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
	bool InsertRoad(int id1, int id2, int distance);
	std::vector<long long> Dijkstra(int start);//计算单个点到各个点的最小距离
};
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
string task3(const string& command);

// aaa