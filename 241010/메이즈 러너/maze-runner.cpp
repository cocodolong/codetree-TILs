#include <iostream>
#include <vector>
#include <unordered_map>
#include <math.h>
using namespace std;

int n, m, k;
vector<vector<int>> maze;
unordered_map<int, pair<int, int>> people;
vector<vector<int>> tempmaze;
int pclear[11];
int score[11];
int dy[4] = { 1,-1,0,0 };
int dx[4] = { 0,0,1,-1 };
int end_i, end_j;

int cal_dist(int i1, int j1, int i2, int j2) {
	return abs(i1 - i2) + abs(j1 - j2);
}

void rotation(int i, int j, int k) {

	//벽 내구도 감소
	for (int ii = i; ii < i + k; ii++) {
		for (int jj = j; jj < j + k; jj++) {
			if (maze[ii][jj] > 0) {
				maze[ii][jj]--;
			}
		}
	}
	//돌리기
	vector<int> temptemp(k, 0);
	vector<vector<int>> copymaze(k, temptemp);
	vector<vector<int>> copy2maze(k, temptemp);

	for (int ii = i; ii < i + k; ii++) {
		for (int jj = j; jj < j + k; jj++) {
			copymaze[ii - i][jj - j] = maze[ii][jj];
		}
	}

	for (int i = 0; i < k; i++) {
		for (int j = 0; j < k; j++) {
			copy2maze[j][k - i - 1] = copymaze[i][j];
		}
	}

	for (int ii = i; ii < i + k; ii++) {
		for (int jj = j; jj < j + k; jj++) {
				maze[ii][jj] = copy2maze[ii - i][jj - j];
		}
	}

	//출구 90도 돌리기
	int temp_end_i = end_i;
	end_i = i + (end_j - j);
	end_j = j + (i + k - 1 - temp_end_i);
	//cout << end_i << " " << end_j << "\n";
	//사람 90도 돌리기
	for (int temppeople = 1; temppeople <= m; temppeople++) {
		if (pclear[temppeople] == 1) continue; //탈출 인원 제외
		auto pre_people = people[temppeople];
		int people_i = pre_people.first;
		int people_j = pre_people.second;
		if (people_i < i || people_i > i + k - 1 || people_j < j || people_j > j + k - 1) continue;

		people[temppeople] = { i + (people_j  - j) , j + (i + k - 1 - people_i) };
	}

}

void findrotateed() {
	for (int k = 2; k < n; k++) {
		for (int i = 0; i < n; i++) {
			if (i + k - 1 >= n) continue;
			if (end_i < i  || end_i > i + k - 1) continue;
			for (int j = 0; j < n; j++) {
				if (j + k - 1 >= n) continue;
				if (end_j < j || end_j > j + k - 1) continue;
				for (int ii = i; ii < i + k; ii++) {
					for (int jj = j; jj < j + k; jj++) {
						if (tempmaze[ii][jj] > 0) {
							rotation(i, j, k);
							//cout << "rotation " << i << " " << j << " " << k << "\n";
							return;
						}
					}
				}
			}
		}
	}
}
void printboard() {

	/*for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << maze[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";*/
	for (int temp = 1; temp <= m; temp++) {
		if (pclear[temp] == 1) {
			//cout << temp << "는 탈락\n";
			continue;
		}
		auto pre_people = people[temp];
		int temp_i = pre_people.first;
		int temp_j = pre_people.second;
		//cout << temp << "는 "<< temp_i << " " << temp_j<<"\n";
	}
}


int main() {
	cin >> n >> m >> k;
	vector<int> tempvector(n, 0);
	maze.assign(n, tempvector);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> maze[i][j];
		}
	}

	for (int temppeople = 1; temppeople <= m; temppeople++) {
		int temp_i, temp_j;
		cin >> temp_i >> temp_j;
		temp_i--;
		temp_j--;
		people[temppeople] = { temp_i,temp_j };
	}
	cin >> end_i >> end_j;
	end_i--;
	end_j--;
	k++;
	while (--k) {

		//참가자 이동
		for (int temppeople = 1; temppeople <= m; temppeople++) {
			if (pclear[temppeople] == 1) continue; //탈출 인원 제외
			auto pre_people = people[temppeople];
			int i = pre_people.first;
			int j = pre_people.second;
			int min_dist = cal_dist(end_i, end_j, i, j);
			int dirindex = -1;
			for (int tempdir = 0; tempdir < 4; tempdir++) {
				int ny = i + dy[tempdir];
				int nx = j + dx[tempdir];
				if (ny >= n || nx >= n || ny < 0 || nx < 0) continue;
				if (maze[ny][nx] > 0) continue;
				int tempdist = cal_dist(ny, nx, end_i, end_j);
				if (tempdist < min_dist) {
					min_dist = tempdist;
					dirindex = tempdir;
				}
			}
			// dirindex == -1 이동 안함
			if (dirindex == -1) continue;
			int ny = i + dy[dirindex];
			int nx = j + dx[dirindex];
			people[temppeople] = { ny,nx };
			score[temppeople]++;
		}

		//이동 후 출구에 있는 인원들 탈출
		for (int temp = 1; temp <= m; temp++) {
			if (pclear[temp] == 1) continue;
			auto pre_people = people[temp];
			int temp_i = pre_people.first;
			int temp_j = pre_people.second;
			if (temp_i == end_i && temp_j == end_j) {
				pclear[temp] = 1;
			}
		}
		
		//cout << 9-k <<"라운드####\n";
		//printboard();
		//돌리기 선정 및 돌리기
		tempmaze.clear();
		tempmaze.assign(n, tempvector);

		for (int temp = 1; temp <= m; temp++) {
			if (pclear[temp] == 1) continue;
			auto pre_people = people[temp];
			int temp_i = pre_people.first;
			int temp_j = pre_people.second;
			tempmaze[temp_i][temp_j] = temp;
		}
		findrotateed();

		//참가자가 모두 탈출했다면 종료
		int flag = 1;
		for (int temp = 1; temp <= m; temp++) {
			if (pclear[temp] == 0) {
				flag = 0;
				break;
			}
		}
		if (flag == 1) break;
	}
	int result = 0;
	for (int temp = 1; temp <= m; temp++) {
		result += score[temp];
	}
	cout << result << "\n";
	cout << end_i + 1 << " " << end_j + 1;
}