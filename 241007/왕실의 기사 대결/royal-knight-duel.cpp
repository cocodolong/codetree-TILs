#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;

vector<vector<int>> maparr;
vector<vector<int>> guard;
unordered_map <int,pair<pair<int, int>, pair<int, int>>> guardlist;
vector<int> visit;
int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };

int damege(int r,int c, int h, int w) {

	int tempsum = 0;
	for (int i = r; i < r + h; i++) {
		for (int j = c; j < c + w; j++) {
			if (maparr[i][j] == 1) tempsum++;
		}
	}
	return tempsum;
}

bool move(int target, int dir) {
	auto targetinfo = guardlist[target];
	int r = targetinfo.first.first;
	int c = targetinfo.first.second;
	int h = targetinfo.second.first;
	int w = targetinfo.second.second;
	r += dy[dir];
	c += dx[dir];

	for (int i = r; i < r + h; i++) {
		for (int j = c; j < c + w; j++) {
			if (maparr[i][j] == 2) return false;
			else if (guard[i][j] == 0) continue;
			if (visit[guard[i][j]] == 0) {
				visit[guard[i][j]] = 1;
				if (move(guard[i][j], dir) == false) {
					return false;
				}
			}
		}
	}
	return true;
}

int main() {
	int L, N, Q;
	cin >> L >> N >> Q;
	vector<int> temp(L + 2, 0);
	maparr.assign(L + 2, temp);
	
	vector<int> life(N + 1, 0);
	vector<int> start_life(N + 1, 0);
	for (int i = 0; i < L + 2; i++) {
		maparr[i][0] = 2;
		maparr[0][i] = 2;
		maparr[L + 1][i] = 2;
		maparr[i][L + 1] = 2;
	}
	for (int i = 1; i < L + 1; i++) {
		for (int j = 1; j < L + 1; j++) {
			cin >> maparr[i][j];
		}
	}
	for (int temp = 1; temp < N + 1; temp++) {
		int temp_r, temp_c, temp_h, temp_w, temp_k;
		cin >> temp_r >> temp_c >> temp_h >> temp_w >> temp_k;
		life[temp] = temp_k;
		start_life[temp] = temp_k;
		guardlist[temp] = { {temp_r,temp_c},{temp_h,temp_w} };
	}

	queue<pair<int, int>> order;

	for (int i = 0; i < Q; i++) {
		int tempnum, tempdir;
		cin >> tempnum >> tempdir;
		order.push({ tempnum, tempdir });
	}
	while (!order.empty()) {
		auto it = order.front();
		int target = it.first;
		int dir = it.second;
		order.pop();
		if (life[target] <= 0) continue;
		
		//이동
		guard.assign(L + 2, temp);
		for (int tempguardnum = 1; tempguardnum < N + 1; tempguardnum++) {
			if (life[tempguardnum] <= 0) continue;
			auto tempguardinfo = guardlist[tempguardnum];
			int r = tempguardinfo.first.first;
			int c = tempguardinfo.first.second;
			int h = tempguardinfo.second.first;
			int w = tempguardinfo.second.second;

			for (int i = r; i < r + h; i++) {
				for (int j = c; j < c + w; j++) {
					guard[i][j] = tempguardnum;
				}
			}
		}
		/*for (int i = 0; i < L + 2; i++) {
			for (int j = 0; j < L + 2; j++) {
				cout << guard[i][j] << " ";
			}
			cout << "\n";
		}
		cout << "#####################333\n";*/

		//이동 가능여부 확인 및 이동
		visit.assign(N + 1, 0);
		visit[target] = 2;
		if (move(target, dir)) {
			auto targetinfo = guardlist[target];
			int target_r = targetinfo.first.first;
			int target_c = targetinfo.first.second;
			int target_h = targetinfo.second.first;
			int target_w = targetinfo.second.second;
			target_r += dy[dir];
			target_c += dx[dir];
			guardlist[target] = { {target_r,target_c},{target_h,target_w} };
			for (int tempguardnum = 1; tempguardnum < N + 1; tempguardnum++) {
				if (life[tempguardnum] <= 0) continue;
				if (visit[tempguardnum] != 1) continue;
				auto tempguardinfo = guardlist[tempguardnum];
				int r = tempguardinfo.first.first;
				int c = tempguardinfo.first.second;
				int h = tempguardinfo.second.first;
				int w = tempguardinfo.second.second;
				r += dy[dir];
				c += dx[dir];
				guardlist[tempguardnum] = { {r,c},{h,w} };
				life[tempguardnum] -= damege(r, c, h, w);
			}
		}
		
	}
	int result = 0;
	for (int tempguardnum = 1; tempguardnum < N + 1; tempguardnum++) {
		if (life[tempguardnum] <= 0) continue;
		result += start_life[tempguardnum] - life[tempguardnum];
		//cout << "#" << tempguardnum << " " << start_life[tempguardnum] << " " << life[tempguardnum] << "\n";
	}
	cout << result;
}