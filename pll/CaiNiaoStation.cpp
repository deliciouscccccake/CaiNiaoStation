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
		} // %：比较的结构类放进了Edge类里面
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
vector<long long> MAP::Dijkstra(int start) {	// %：修改了dijkstra，使之适配修改后的数据结构
	vector<int> visited(Loclist.size(), 0);//记录遍历过的节点
	vector<long long> dist(Loclist.size(), LLONG_MAX);//记录到起点的最短路径长
	//vector<int> path(Loclist.size(), 0);//记录上一个节点，暂时不用
	priority_queue<Edge, vector<Edge>, Edge::cmp> prqueues;//存入可用的dist

	//初始化
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
	set<long long> TOpackage;//记录超时的包裹，顺序输出
	map<long long, long long> Cpackage;//记录每个包裹运输代价，顺序输出，代价为包裹重+行驶距离
	long long distance=0;//记录已经行走的距离
	long long TimeOutPackages = 0;//记录超时包裹数
	long long costs = 0;//记录总代价
	stringstream ss(command);
	int carnum; ss >> carnum;
	trolley car;
	for (int i = 0; i < carnum; i++) //形式化，其实只有一辆车
		ss >> car.speed >> car.dweight >> car.maxlweight;
	long long packnum; ss >> packnum;
	vector<Package> packages(packnum);
	for (long long i = 0; i < packnum; i++)
		ss >> packages[i].id >> packages[i].weight >> packages[i].dest >> packages[i].Stime >> packages[i].Ttime;
	long long curtime = 0;//记录当前时间
	vector<vector<long long>> dist(ecmap.Loclist.size());
	for (long long i = 1; i < ecmap.Loclist.size(); i++) {
		dist[i] = ecmap.Dijkstra(i);
	}//求每个点到其他点最短距离
	while (!packages.empty()) {//对于每一趟，如果包裹没送完
		sort(packages.begin(), packages.end(),
			[&](const Package& a, const Package& b) {
				bool IfTimeOut_a = dist[1][a.dest] / car.speed > a.Ttime - curtime;
				bool IfTimeOut_b = dist[1][b.dest] / car.speed > b.Ttime - curtime;
				//包裹预计是否超时，根据最短路径，如果即使最短路径下仍旧会超时，那他铁定超时
				if (IfTimeOut_a != IfTimeOut_b)//如果其中一个超时，另一个没有，那么没超时的到前面
					return IfTimeOut_a < IfTimeOut_b;
				if (a.Ttime != b.Ttime)//如果都不会超时，（都超时了无所谓），最紧迫的到前面
					return a.Ttime < b.Ttime;
				return dist[1][a.dest] < dist[1][b.dest];//距离驿站近的优先，不过感觉无所谓
			});//不确定优先队列能否根据外部变量实时更新，每次循环使用sort重排
		map<long long, priority_queue <Package, vector<Package>, Package::ttimecmp>> selected_packages;
		for (long long i = 0; i < packages.size();i++) {
			if (car.curload + packages[i].weight <= car.maxlweight) {//顺序选包裹，直到达到最大载重或者没有别的包裹可选
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
	//结果
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
	set<long long> TOpackage;//记录超时的包裹，顺序输出
	map<long long, long long> Cpackage;//记录每个包裹运输代价，顺序输出，代价为包裹重+行驶距离
	long long distance = 0;//记录已经行走的距离
	long long TimeOutPackages = 0;//记录超时包裹数
	long long costs = 0;//记录总代价
	vector<long long> curtime (2,0);//记录每辆小车的时间


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
	}//求每个点到其他点最短距离


	while (!packages.empty()) {//对于每一趟，如果包裹没送完
		if (curtime[!curCarID] <= curtime[curCarID])//先比较，如果之前未被选中的车辆在选中车辆前已回来
			if (curtime[!curCarID] == curtime[curCarID])//如果同时抵达，则小车会同时出发，由于包裹顺序最晚抵达时间优先级较高，因此选择速度快的
				curCarID = cars[0].speed < cars[1].speed ? 1 : 0;
			else//如果不是，则未选中小车已出发，接下来运行未选中小车路线规划
				curCarID = ~curCarID;
		//如果未选中小车仍未抵达，则继续当前小车
		sort(packages.begin(), packages.end(),
			[&](const Package& a, const Package& b) {
				bool IfAvaliable_a = a.Stime <= curtime[curCarID];
				bool IfAvaliable_b = b.Stime <= curtime[curCarID];
				if (IfAvaliable_a != IfAvaliable_b)
					return IfAvaliable_a < IfAvaliable_b;
				bool IfTimeOut_a = dist[1][a.dest] / cars[curCarID].speed > a.Ttime - curtime[curCarID];
				bool IfTimeOut_b = dist[1][b.dest] /cars[curCarID].speed > b.Ttime - curtime[curCarID];
				//包裹预计是否超时，根据最短路径，如果即使最短路径下仍旧会超时，那他铁定超时
				if (IfTimeOut_a != IfTimeOut_b)//如果其中一个超时，另一个没有，那么没超时的到前面
					return IfTimeOut_a < IfTimeOut_b;
				if (a.Ttime != b.Ttime)//如果都不会超时，（都超时了无所谓），最紧迫的到前面
					return a.Ttime < b.Ttime;
				return dist[1][a.dest] < dist[1][b.dest];//距离驿站近的优先，不过感觉无所谓
			});//不确定优先队列能否根据外部变量实时更新，每次循环使用sort重排
		map<long long, priority_queue <Package, vector<Package>, Package::ttimecmp>> selected_packages;
		for (long long i = 0,earltime=LLONG_MAX,earlpid=1; i < packages.size(); i++) {
			if ((cars[curCarID].curload + packages[i].weight <= cars[curCarID].maxlweight)&&
				(cars[curCarID].curload + packages[i].weight <= cars[curCarID].maxlweight/10
					|| packages[i].Stime<=curtime[curCarID]||
					packages[i].Stime+dist[1][earlpid]<=earltime))
			{//顺序选包裹，当包裹可以放入且包裹加上去后仍小于最大载重的1/10
				//或者包裹不会影响最紧迫包裹到达或者包裹已到达，则将包裹放入
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
	//结果
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

