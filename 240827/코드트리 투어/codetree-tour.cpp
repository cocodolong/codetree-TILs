#include <iostream>
#include <queue>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

#define creat_land 100
#define creat_product 200
#define cancel_product 300
#define opti_sales 400
#define change_start 500

struct Edge {
	int num;
	int cost;
	bool operator < (Edge right) const {
		return cost > right.cost;
	}
};

struct product {
	int id;
	int revenue;
	int *cost;
	bool operator < (product right) const {
		int profit1 = revenue - *cost;
		int profit2 = right.revenue - *(right.cost);
		if (profit1 != profit2) return profit1 > profit2;
		return id < right.id;
	}
};

vector<product> sale_product;
vector<Edge> alis[10001];
int n, m;
int dis[2001];
int pre_start_point = 0;

void dijkstra(int start) {
	for (int i = 0; i < n; i++) {
		dis[i] = 21e8;
	}
	priority_queue<Edge> pq;
	pq.push({ start, 0 });
	dis[start] = 0;

	while (!pq.empty()) {
		auto now = pq.top(); pq.pop();
		if (dis[now.num] < now.cost) continue;

		for (const Edge &next : alis[now.num]) {
			int nextcost = dis[now.num] + next.cost;
			if (nextcost < dis[next.num]) {
				dis[next.num] = nextcost;
				pq.push({ next.num, nextcost });
			}
		}
	}
}

int main() {
	int t;
	cin >> t;

	for (int test = 0; test < t; test++) {
		int index;
		cin >> index;

		// 코드트리 랜드 건설
		if (creat_land == index) {
			cin >> n >> m;
			for (int i = 0; i < m; i++) {
				int from, to, cost;
				cin >> from >> to >> cost;
				if (from == to) continue;
				alis[from].push_back({ to, cost });
				alis[to].push_back({ from, cost });
			}
			dijkstra(pre_start_point);
		}
		// 여행 상품 생성
		else if (creat_product == index) {
			int tempid, revenue, dest;
			cin >> tempid >> revenue >> dest;
			sale_product.push_back({ tempid, revenue, &dis[dest] });
		}
		// 여행 상품 취소
		else if (cancel_product == index) {
			int tempid;
			cin >> tempid;
			auto it = remove_if(sale_product.begin(), sale_product.end(), [tempid](product &p) {
				return p.id == tempid;
			});
			if (it != sale_product.end()) sale_product.erase(it, sale_product.end());
		}
		// 최적의 여행 상품 판매
		else if (opti_sales == index) {
			if (!sale_product.empty()) {
				sort(sale_product.begin(), sale_product.end());
				if (sale_product[0].revenue - *(sale_product[0].cost) >= 0) {
					cout << sale_product[0].id << "\n";
					sale_product.erase(sale_product.begin());
				}
				else {
					cout << -1 << "\n";
				}
			}
			else {
				cout << -1 << "\n";
			}
		}
		// 여행 상품의 출발지 변경
		else if (change_start == index) {
			int change;
			cin >> change;
			pre_start_point = change;
			dijkstra(pre_start_point);
		}
	}
	return 0;
}