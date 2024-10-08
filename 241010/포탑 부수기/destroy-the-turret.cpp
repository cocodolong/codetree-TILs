#include <iostream>
#include <vector>
#include <queue>

using namespace std;
int n, m, k;
vector<vector<int>> board;
vector<vector<int>> visit;
vector<pair<int, int>> road;
vector<pair<int, int>> temproad;

vector<vector<int>> roundhist;
vector<vector<vector<pair<int,int>>>> visitque;


int hist[10][10];

int raser_flag = 1000000000;

int attacker_i;
int attacker_j;
int target_i;
int target_j;

int dy[8] = { 0,1,0,-1,1,1,-1,-1 };
int dx[8] = { 1,0,-1,0,1,-1,-1,1 };

void raser_bfs(int i, int j, int cnt) {
	vector<pair<int,int>> temp;
	vector<vector<pair<int, int>>> temptemp(n, temp);
	visitque.assign(n, temptemp);

	queue<pair<pair<int, int>, int>> q;
	q.push({ {i, j}, 0 });
	visit[i][j] = 0;

	while (!q.empty()) {
		int cy = q.front().first.first;
		int cx = q.front().first.second;
		int cnt = q.front().second;
		q.pop();

		if (raser_flag <= cnt) continue;
		if (cy == target_i && cx == target_j) {
			raser_flag = cnt;
			road.assign(visitque[target_i][target_j].begin(), visitque[target_i][target_j].end());
		}

		for (int dir = 0; dir < 4; dir++) {
			int ny = cy + dy[dir];
			int nx = cx + dx[dir];
			if (ny == n) ny = 0;
			if (ny == -1) ny = n - 1;
			if (nx == n) nx = 0;
			if (nx == -1) nx = n - 1;

			if (board[ny][nx] == 0) continue;
			if (visit[ny][nx] <= cnt + 1) continue;
			visit[ny][nx] = cnt + 1;
			vector<pair<int, int>> tempque;
			tempque.assign(visitque[cy][cx].begin(), visitque[cy][cx].end());
			tempque.push_back({ ny, nx });
			visitque[ny][nx].assign(tempque.begin(), tempque.end());
			q.push({ { ny,nx },cnt + 1 });
		}
	}
}

void booming() {

	//board[target_i][target_j] -= board[attacker_i][attacker_j]; //main문에서 처리

	for (int dir = 0; dir < 8; dir++) {
		int ny = target_i + dy[dir];
		int nx = target_j + dx[dir];
		if (ny == n) ny = 0;
		if (ny == -1) ny = n - 1;
		if (nx == n) nx = 0;
		if (nx == -1) nx = n - 1;
		if (board[ny][nx] == 0) continue;

		if (board[ny][nx] - (board[attacker_i][attacker_j] / 2) < 0) board[ny][nx] = 0;
		else board[ny][nx] -= (board[attacker_i][attacker_j] / 2);
		roundhist[ny][nx] = 1;
	}
}

void printboard() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << board[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

int main() {
	cin >> n >> m >> k;
	vector<int> tempvector(n, 0);
	board.assign(n, tempvector);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}

	for (int round = 1; round <= k; round++) {

		//cout << round << "라운드 정보\n";
		roundhist.assign(n, tempvector);
		
		//공격자 선정
		
		int min_atk = 1000000000;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (board[i][j] == 0)continue;
				if (board[i][j] < min_atk) {
					attacker_i = i;
					attacker_j = j;
					min_atk = board[i][j];
				}
				else if (board[i][j] == min_atk) {
					if (hist[i][j] > hist[attacker_i][attacker_j]) {
						attacker_i = i;
						attacker_j = j;
						min_atk = board[i][j];
					}
					else if (hist[i][j] == hist[attacker_i][attacker_j]) {
						if (i+j > attacker_i + attacker_j) {
							attacker_i = i;
							attacker_j = j;
							min_atk = board[i][j];
						}
						else if (i + j == attacker_i + attacker_j) {
							if (j > attacker_j) {
								attacker_i = i;
								attacker_j = j;
								min_atk = board[i][j];
							}
						}
					}
				}
			}
		}
		hist[attacker_i][attacker_j] = round;
		//공격자 강화
		board[attacker_i][attacker_j] += n + m;
		//cout << "공격자: " << attacker_i << " " << attacker_j << "\n";

		//대상 선정
		int max_target = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (board[i][j] > max_target) {
					target_i = i;
					target_j = j;
					max_target = board[i][j];
				}
				else if (board[i][j] == max_target) {
					if (hist[i][j] < hist[attacker_i][attacker_j]) {
						target_i = i;
						target_j = j;
						max_target = board[i][j];
					}
					else if (hist[i][j] == hist[attacker_i][attacker_j]) {
						if (i + j < attacker_i + attacker_j) {
							target_i = i;
							target_j = j;
							max_target = board[i][j];
						}
						else if (i + j == attacker_i + attacker_j) {
							if (j < attacker_j) {
								target_i = i;
								target_j = j;
								max_target = board[i][j];
							}
						}
					}
				}
			}
		}

		//cout << "타겟: " << target_i << " " << target_j << "\n";

		//레이저 공격
		raser_flag = 1000000000;
		vector<int> tempvector2(n, 1000000000);
		visit.assign(n, tempvector2);
		road.clear();
		temproad.clear();
		raser_bfs(attacker_i, attacker_j, 0);
		
		board[target_i][target_j] -= board[attacker_i][attacker_j];
		if (raser_flag != 1000000000)  road.pop_back(); //타켓은 경로에서 없애고 별도로 처리
		for (int i = 0; i < road.size(); i++) {
			auto temptarget = road[i];
			if (board[temptarget.first][temptarget.second] -= board[attacker_i][attacker_j] / 2 < 0) board[temptarget.first][temptarget.second] = 0;
			else board[temptarget.first][temptarget.second] -= board[attacker_i][attacker_j]/2;
			roundhist[temptarget.first][temptarget.second] = 1;
		}
		
		//레이저 공격이 안될 때
		if (raser_flag == 1000000000) {
			//포격
			booming();
		}

		//printboard();
		//미참여자 추가점수
		roundhist[attacker_i][attacker_j] = 1;
		roundhist[target_i][target_j] = 1;
		int endflag = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (roundhist[i][j] == 0 && board[i][j] != 0) board[i][j]++;
				if (board[i][j] > 0) endflag++;
			}
		}

		//printboard();
		if (endflag < 2) break;
	}

	int result = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (result < board[i][j]) result = board[i][j];
		}
	}
	cout << result;
}