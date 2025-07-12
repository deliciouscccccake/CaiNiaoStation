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
MAP ecmap;
struct Edge
{
	int v;
	long long d;
	struct cmp {
		bool operator()(const Edge& a, const Edge& b) {
			return a.d > b.d;
		} // %���ȽϵĽṹ��Ž���Edge������
	};
};
bool MAP::InsertLocation(const string& target) {
	auto it = Loclist.find(target);
	if (it == Loclist.end())
	{
		long long index = Loclist.size();
		Loclist[target] = index;
		Roads.push_back(std::vector<EdgeNode>());
		return true;
	}
	return false;
}
bool MAP::InsertRoad(int id1, int id2, int distance) {
	Roads[id1].push_back({ id2, distance });
	Roads[id2].push_back({ id1, distance });
	return true;
}
vector<long long> MAP::Dijkstra(int start) {	// %���޸���dijkstra��ʹ֮�����޸ĺ�����ݽṹ
	vector<int> visited(Loclist.size(), 0);//��¼�������Ľڵ�
	vector<long long> dist(Loclist.size(), LLONG_MAX);//��¼���������·����
	//vector<int> path(Loclist.size(), 0);//��¼��һ���ڵ㣬��ʱ����
	priority_queue<Edge, vector<Edge>, Edge::cmp> prqueues;//������õ�dist

	//��ʼ��
	dist[start] = 0;
	prqueues.push({ start, 0 });
	while (!prqueues.empty())
	{
		auto cur = prqueues.top();
		prqueues.pop();
		if (visited[cur.v])continue;
		visited[cur.v] = true;
		for (const auto& ptr : Roads[cur.v])
		{
			if (dist[cur.v] + ptr.distance < dist[ptr.to])
			{
				dist[ptr.to] = dist[cur.v] + ptr.distance;
				prqueues.push({ ptr.to, dist[ptr.to] });
			}
		}
	}
	return dist;
}
string task3(const string& command) {
	set<long long> TOpackage;//��¼��ʱ�İ�����˳�����
	map<long long, long long> Cpackage;//��¼ÿ������������ۣ�˳�����������Ϊ������+��ʻ����
	long long distance=0;//��¼�Ѿ����ߵľ���
	long long TimeOutPackages = 0;//��¼��ʱ������
	long long costs = 0;//��¼�ܴ���
	stringstream ss(command);
	int carnum; ss >> carnum;
	trolley car;
	for (int i = 0; i < carnum; i++) //��ʽ������ʵֻ��һ����
		ss >> car.speed >> car.dweight >> car.maxlweight;
	long long packnum; ss >> packnum;
	vector<Package> packages(packnum);
	for (long long i = 0; i < packnum; i++)
		ss >> packages[i].id >> packages[i].weight >> packages[i].dest >> packages[i].Stime >> packages[i].Ttime;
	long long curtime = 0;//��¼��ǰʱ��
	vector<vector<long long>> dist(ecmap.Loclist.size());
	for (long long i = 1; i < ecmap.Loclist.size(); i++) {
		dist[i] = ecmap.Dijkstra(i);
	}//��ÿ���㵽��������̾���
	while (!packages.empty()) {//����ÿһ�ˣ��������û����
		sort(packages.begin(), packages.end(),
			[&](const Package& a, const Package& b) {
				bool IfTimeOut_a = dist[1][a.dest] / car.speed > a.Ttime - curtime;
				bool IfTimeOut_b = dist[1][b.dest] / car.speed > b.Ttime - curtime;
				//����Ԥ���Ƿ�ʱ���������·���������ʹ���·�����Ծɻᳬʱ������������ʱ
				if (IfTimeOut_a != IfTimeOut_b)//�������һ����ʱ����һ��û�У���ôû��ʱ�ĵ�ǰ��
					return IfTimeOut_a < IfTimeOut_b;
				if (a.Ttime != b.Ttime)//��������ᳬʱ��������ʱ������ν��������ȵĵ�ǰ��
					return a.Ttime < b.Ttime;
				return dist[1][a.dest] < dist[1][b.dest];//������վ�������ȣ������о�����ν
			});//��ȷ�����ȶ����ܷ�����ⲿ����ʵʱ���£�ÿ��ѭ��ʹ��sort����
		map<long long, priority_queue <Package, vector<Package>, Package::ttimecmp>> selected_packages;
		for (long long i = 0; i < packages.size();i++) {
			if (car.curload + packages[i].weight <= car.maxlweight) {//˳��ѡ������ֱ���ﵽ������ػ���û�б�İ�����ѡ
				selected_packages[packages[i].dest].push(packages[i]);
				car.curload += packages[i].weight;
				packages.erase(packages.begin() + i); i--;
				if (car.curload == car.maxlweight)
					break;
			}
		}
		long long current_spot = 1;
		HeapSort_For_PA prqueues(dist, selected_packages, current_spot);
		for (auto it : selected_packages) 
			prqueues.push(it.first);
		long long orders = prqueues.pop();
		costs += (car.curload + car.dweight) * dist[1][orders];
		curtime += dist[1][orders] / car.speed;
		distance += dist[1][orders];
		while (!selected_packages[orders].empty()) {
			Package temp = selected_packages[orders].top(); selected_packages[orders].pop();
			car.curload -= temp.weight;
			Cpackage[temp.id] =  distance * temp.weight;
			if (curtime > temp.Ttime)
			{
				TimeOutPackages++;
				TOpackage.insert(temp.id);
			}
		}
		while (!prqueues.empty()) {
			current_spot = orders;
			prqueues.Resort();
			orders = prqueues.pop();
			costs += (car.curload + car.dweight) * dist[current_spot][orders];
			curtime += dist[current_spot][orders] / car.speed;
			distance += dist[current_spot][orders];
			while (!selected_packages[orders].empty()) {
				Package temp = selected_packages[orders].top();
				selected_packages[orders].pop();
				car.curload -= temp.weight;
				Cpackage[temp.id] = distance * temp.weight;
				if (curtime > temp.Ttime)
				{
					TimeOutPackages++;
					TOpackage.insert(temp.id);
				}
			}
		}
		costs += (car.curload + car.dweight) * dist[orders][1]; 
		curtime += dist[orders][1] / car.speed;
		distance = 0; car.curload = 0;
	}
	//���
	string result = "";
	result += "total cost:"; result += to_string(costs); result += '\n';
	result += "total time out packages:"; result += to_string(TimeOutPackages); result += '\n';
	result += "cost of each package:\n";
	for (auto it : Cpackage) {
		result += "id:"; result += to_string(it.first); result += " cost:";
		result += to_string(it.second); result += '\n';
	}
	result += "Time out packages:\n";
	for (auto it : TOpackage)
	{
		result += to_string(it); result += '\n';
	}
	return result;
}
string extask1(const string& command) {
	set<long long> TOpackage;//��¼��ʱ�İ�����˳�����
	map<long long, long long> Cpackage;//��¼ÿ������������ۣ�˳�����������Ϊ������+��ʻ����
	long long distance = 0;//��¼�Ѿ����ߵľ���
	long long TimeOutPackages = 0;//��¼��ʱ������
	long long costs = 0;//��¼�ܴ���
	vector<long long> curtime (2,0);//��¼ÿ��С����ʱ��


	stringstream ss(command);
	int carnum; ss >> carnum;
	vector<trolley> cars(carnum);
	for (int i = 0; i < carnum; i++) 
		ss >> cars[i].speed >> cars[i].dweight >> cars[i].maxlweight;
	long long packnum; ss >> packnum;
	vector<Package> packages(packnum);
	for (long long i = 0; i < packnum; i++)
		ss >> packages[i].id >> packages[i].weight >> packages[i].dest >> packages[i].Stime >> packages[i].Ttime;


	vector<vector<long long>> dist(ecmap.Loclist.size()); int curCarID = 0;
	for (long long i = 1; i < ecmap.Loclist.size(); i++) {
		dist[i] = ecmap.Dijkstra(i);
	}//��ÿ���㵽��������̾���


	while (!packages.empty()) {//����ÿһ�ˣ��������û����
		if (curtime[!curCarID] <= curtime[curCarID])//�ȱȽϣ����֮ǰδ��ѡ�еĳ�����ѡ�г���ǰ�ѻ���
			if (curtime[!curCarID] == curtime[curCarID])//���ͬʱ�ִ��С����ͬʱ���������ڰ���˳������ִ�ʱ�����ȼ��ϸߣ����ѡ���ٶȿ��
				curCarID = cars[0].speed < cars[1].speed ? 1 : 0;
			else//������ǣ���δѡ��С���ѳ���������������δѡ��С��·�߹滮
				curCarID = ~curCarID;
		//���δѡ��С����δ�ִ�������ǰС��
		sort(packages.begin(), packages.end(),
			[&](const Package& a, const Package& b) {
				bool IfAvaliable_a = a.Stime <= curtime[curCarID];
				bool IfAvaliable_b = b.Stime <= curtime[curCarID];
				if (IfAvaliable_a != IfAvaliable_b)
					return IfAvaliable_a < IfAvaliable_b;
				bool IfTimeOut_a = dist[1][a.dest] / cars[curCarID].speed > a.Ttime - curtime[curCarID];
				bool IfTimeOut_b = dist[1][b.dest] /cars[curCarID].speed > b.Ttime - curtime[curCarID];
				//����Ԥ���Ƿ�ʱ���������·���������ʹ���·�����Ծɻᳬʱ������������ʱ
				if (IfTimeOut_a != IfTimeOut_b)//�������һ����ʱ����һ��û�У���ôû��ʱ�ĵ�ǰ��
					return IfTimeOut_a < IfTimeOut_b;
				if (a.Ttime != b.Ttime)//��������ᳬʱ��������ʱ������ν��������ȵĵ�ǰ��
					return a.Ttime < b.Ttime;
				return dist[1][a.dest] < dist[1][b.dest];//������վ�������ȣ������о�����ν
			});//��ȷ�����ȶ����ܷ�����ⲿ����ʵʱ���£�ÿ��ѭ��ʹ��sort����
		map<long long, priority_queue <Package, vector<Package>, Package::ttimecmp>> selected_packages;
		for (long long i = 0,earltime=LLONG_MAX,earlpid=1; i < packages.size(); i++) {
			if ((cars[curCarID].curload + packages[i].weight <= cars[curCarID].maxlweight)&&
				(cars[curCarID].curload + packages[i].weight <= cars[curCarID].maxlweight/10
					|| packages[i].Stime<=curtime[curCarID]||
					packages[i].Stime+dist[1][earlpid]<=earltime))
			{//˳��ѡ���������������Է����Ұ�������ȥ����С��������ص�1/10
				//���߰�������Ӱ������Ȱ���������߰����ѵ���򽫰�������
				if (earltime > packages[i].Ttime&&packages[i].Ttime<=curtime[curCarID]+dist[1][packages[i].dest])
				{
					earltime = packages[i].Ttime;
					earlpid = packages[i].id;
				}
				selected_packages[packages[i].dest].push(packages[i]);
				cars[curCarID].curload += packages[i].weight;
				packages.erase(packages.begin() + i); i--;
				if (cars[curCarID].curload == cars[curCarID].maxlweight)
					break;
			}
		}
		long long current_spot = 1;
		HeapSort_For_PA prqueues(dist, selected_packages, current_spot);
		for (auto it : selected_packages)
			prqueues.push(it.first);
		long long orders = prqueues.pop();
		costs += (cars[curCarID].curload + cars[curCarID].dweight) * dist[1][orders];
		curtime[curCarID] += dist[1][orders] / cars[curCarID].speed;
		distance += dist[1][orders];
		while (!selected_packages[orders].empty()) {
			Package temp = selected_packages[orders].top(); selected_packages[orders].pop();
			cars[curCarID].curload -= temp.weight;
			Cpackage[temp.id] = distance * temp.weight;
			if (curtime[curCarID] > temp.Ttime)
			{
				TimeOutPackages++;
				TOpackage.insert(temp.id);
			}
		}
		while (!prqueues.empty()) {
			current_spot = orders;
			prqueues.Resort();
			orders = prqueues.pop();
			costs += (cars[curCarID].curload + cars[curCarID].dweight) * dist[current_spot][orders];
			curtime[curCarID] += dist[current_spot][orders] / cars[curCarID].speed;
			distance += dist[current_spot][orders];
			while (!selected_packages[orders].empty()) {
				Package temp = selected_packages[orders].top();
				selected_packages[orders].pop();
				cars[curCarID].curload -= temp.weight;
				Cpackage[temp.id] = distance * temp.weight;
				if (curtime[curCarID]> temp.Ttime)
				{
					TimeOutPackages++;
					TOpackage.insert(temp.id);
				}
			}
		}
		costs += (cars[curCarID].curload + cars[curCarID].dweight) * dist[orders][1];
		curtime[curCarID] += dist[orders][1] / cars[curCarID].speed;
		distance = 0; cars[curCarID].curload = 0;
	}
	//���
	string result = "";
	result += "total cost:"; result += to_string(costs); result += '\n';
	result += "total time out packages:"; result += to_string(TimeOutPackages); result += '\n';
	result += "cost of each package:\n";
	for (auto it : Cpackage) {
		result += "id:"; result += to_string(it.first); result += " cost:";
		result += to_string(it.second); result += '\n';
	}
	result += "Time out packages:\n";
	for (auto it : TOpackage)
	{
		result += to_string(it); result += '\n';
	}
	return result;
}

