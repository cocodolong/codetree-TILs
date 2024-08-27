#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string.h>
#include <math.h>
using namespace std;


int r, c, k;

int dx[4] = {0, 1,0,-1 };
int dy[4] = {1, 0,-1,0 };

int arr[74][74];
int check[74][74];

int findscore(int i, int j) {

	queue<pair<int, int>> q;
	memset(check, 0, sizeof(check));
	q.push({ i, j });
	int minrow = 100;
	check[i][j] = 1;
	while (!q.empty()) {
		auto temp = q.front();
		int y = temp.first;
		int x = temp.second;
		minrow = min(minrow, y);
		q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int nx = x + dx[dir];
			int ny = y + dy[dir];
			if (nx > c || ny > r || nx < 0 || ny < 0) continue;
			if (check[ny][nx] == 0 && (abs(arr[y][x]) == abs(arr[ny][nx]) || (arr[y][x] < 0 && arr[ny][nx] != 0))) {
				check[ny][nx] = 1;
				q.push({ ny,nx });
			}
		}
	}
	/*cout << "###" << r - minrow << endl;*/
	return r - minrow;
}

int main() {
	cin >> r >> c >> k;

	int pre_score;
	int score = 0;
	for (int round = 1; round <= k; round++) {
		int start, dir;
		cin >> start >> dir;
		//최대한 움직이기
		int i = r;
		int j = start;
		while (1) {
			//cout << i << " " << j << endl;
			if (i - 2 >= 0) {
				if (arr[i - 2][j] == 0 && arr[i - 1][j + 1] == 0 && arr[i - 1][j - 1] == 0) {
					i = i - 1;
					continue;
				}
			}
			if (j - 2 >= 1 && i - 2 >= 0) {
				if ((arr[i][j - 2] == 0 && arr[i + 1][j - 1] == 0 && arr[i - 1][j - 1] == 0) &&
					(arr[i - 1][j - 2] == 0 && arr[i - 2][j - 1] == 0)) {
					i = i - 1;
					j = j - 1;
					dir = dir + 3;
					continue;
				}
			}
			if (j + 2 <= c && i - 2 >= 0) {
				if ((arr[i][j + 2] == 0 && arr[i + 1][j + 1] == 0 && arr[i - 1][j + 1] == 0) &&
					(arr[i - 1][j + 2] == 0 && arr[i - 2][j + 1] == 0)) {
					i = i - 1;
					j = j + 1;
					dir++;
					continue;
				}
			}
			break;
		}

		// 골렘 박제하기
		arr[i][j] = round;
		for (int around = 0; around < 4; around++) {
			arr[i + dy[around]][j + dx[around]] = round;
		}
		arr[i + dy[dir % 4]][j + dx[dir % 4]] = -round; //나가는 길;
		//cout << "\n";
		//cout << "############" << round << "###############\n";
		//for (int i = r; i << r + 1 >= 0; i--) {
		//	for (int j = 1; j < c + 1; j++) {
		//		cout << arr[i][j] << " ";
		//	}
		//	cout << "\n";
		//}
		//cout << "\n";
		//골렘이 넘친다면 초기화
		int flag = 0;
		for (int j = 1; j <= c; j++) {
			if (arr[r][j] != 0) {
				flag = 1;
				break;
			}
		}

		if (flag == 1) {
			memset(arr, 0, sizeof(arr));
		}
		else {
			//정령 내려가기
			score += findscore(i, j);
		}
	}
	cout << score;
}