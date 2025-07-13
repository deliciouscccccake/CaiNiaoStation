#pragma once

#include <string>
#include <vector>
#include <map>

using std::string;

class MAP {
public:
	// �߽��
	struct EdgeNode {
		// �ߵ�Ŀ�ĵ�id�� ���� // %��id�ĳ���to
		int to;
		long long distance;
	};
	std::map<string, long long> Loclist;//�ص㼯
	std::vector<std::vector<EdgeNode>> Roads;//�ڽӱ���һ��Ԫ��Ϊ�� //%������vector������ʹ��
	MAP() { Roads.push_back(std::vector<EdgeNode>()); }
	bool InsertLocation(const string& target);//���Ԫ��
	bool InsertRoad(int id1, int id2, int distance);
	std::vector<long long> Dijkstra(int start);//���㵥���㵽���������С����,�����нڵ�Ҳ��ʹ��Floyd�㷨
};
struct Package {
	int id;
	int weight;
	int dest;//Ŀ�ĵ�
	int Stime;//�����ʹ���վʱ��
	int Ttime;//���������ʹ�ʱ��
	struct ttimecmp {
		bool operator()(const Package& a, const Package& b) {
			return a.Ttime > b.Ttime;
		}
	};
};
struct trolley {//���С��
	int speed;
	int dweight;//����
	int maxlweight;//���װ������
	int curload = 0;
};
class HeapSort_For_PA {
protected:
	vector<long long> elems;
	vector<vector<long long>>& dists;
	map<long long, priority_queue <Package, vector<Package>,Package::ttimecmp>>& spack;
	long long& spot;
public:
	HeapSort_For_PA(vector<vector<long long>>& dists,
		map<long long, priority_queue <Package, vector<Package>, Package::ttimecmp>>& spack,long long& spot)
		:spack{spack}, dists{ dists }, spot{spot}{}
	void push(long long elem);
	virtual bool compare(const long long& a, const long long& b);
	void swiftdown(long long i, long long n);//iΪ���ڵ�λ�ã�nΪ�ڵ�����
	long long pop();
	void Resort();
	bool empty();
};
class HeapSort_For_PA_Cost_First :public HeapSort_For_PA {
public:
	HeapSort_For_PA_Cost_First(vector<vector<long long>>& dists,
		map<long long, priority_queue <Package, vector<Package>, Package::ttimecmp>>& spack, long long& spot) :
		HeapSort_For_PA(dists, spack, spot) {}
	bool compare(const long long& a, const long long& b) override;
};
void Initial_MAP(const string& command);
string task3(const string& command);
string extask1(const string& command);
string extask3(const string& command);
//struct Node { int loc; /* �� */ };
//auto cmp = [&](const Node& a, const Node& b) { /* ��̬�Ƚ� */ };
//std::priority_queue<Node, vector<Node>, decltype(cmp)> pq(cmp);