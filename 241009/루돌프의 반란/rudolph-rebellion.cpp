#include <iostream>
#include <stack>
#include <unordered_map>
#define max_int 1000000000
using namespace std;

int n, m, p, c, d;
int arr[51][51];
unordered_map<int,pair<int,int>> santa;
int santalife[31];
int santascore[31];
int roo_i, roo_j;
//0~3 산타이동(4방) 0~7 루돌프이동(8방,대각선 포함)
int dy[8] = {-1,0,1,0,-1,1,-1,1};
int dx[8] = {0,1,0,-1,-1,1,1,-1};


int cal_dist(int r1, int c1, int r2, int c2) {
	return (r1 - r2)*(r1 - r2) + (c1 - c2)*(c1 - c2);
}

void interaction(int i, int j, int value, int dir) {
	//현재 위치가 맵 밖이라면
	if (i > n || j > n || i < 1 || j < 1) {
		santalife[value] = max_int; // 산타 탈락처리
		return;
	}
	//현재 위치가 빈 공간이라면, 상호작용 종료
	if (arr[i][j] == 0) {
		arr[i][j] = value;
		santa[value] = { i,j };
	}
	else {
		//현재 위치에도 산타가 있다면, 상호작용 발생
		int jumpsanta = arr[i][j]; //튕겨나가는 산타
		int ny = i + dy[dir];
		int nx = j + dx[dir];
		interaction(ny, nx, jumpsanta, dir);
		arr[i][j] = value;
		santa[value] = { i,j };
	}
	return;
}

int main() {
	//초기 입력
	cin >> n >> m >> p >> c >> d;

	//루돌프 입력
	cin >> roo_i >> roo_j;

	//산타입력
	for (int temp = 1; temp <= p; temp++) {
		int num, i, j;
		cin >> num >> i >> j;
		santa[num] = { i,j };
		arr[i][j] = num;
	}

	//라운드
	for (int round = 1; round <= m; round++) {
		//루돌프 이동
		//타켓 정하기
		int targetindex;
		int min_dist = max_int;
		int target_r, target_c;
		for (int tempsanta = 1; tempsanta <= p; tempsanta++) {
			if (santalife[tempsanta] == max_int) continue;
			auto tempsantapair = santa[tempsanta];
			int i = tempsantapair.first;
			int j = tempsantapair.second;
			int tempdist = cal_dist(roo_i, roo_j, i,j);

			if (tempdist < min_dist) {
				min_dist = tempdist;
				targetindex = tempsanta;
				target_r = i;
				target_c = j;
			}
			else if (tempdist == min_dist) {
				if (target_r < i) {
					min_dist = tempdist;
					targetindex = tempsanta;
					target_r = i;
					target_c = j;
				}
				else if (target_r == i) {
					if (target_c < j) {
						min_dist = tempdist;
						targetindex = tempsanta;
						target_r = i;
						target_c = j;
					}
				}
			}
		}
		// targetindex가 달려갈 산타, min_dist가 해당 산타까지의 거리
		//방향 정하기
		int dirindex;
		for (int tempdir = 0; tempdir < 8; tempdir++) {
			int ny = roo_i + dy[tempdir];
			int nx = roo_j + dx[tempdir];
			if (ny > n || nx > n || ny < 1 || nx < 1) continue; //맵을 넘어가는 경우 스킵

			int tempdist = cal_dist(target_r, target_c, ny, nx);
			if (tempdist < min_dist) {
				dirindex = tempdir;
				min_dist = tempdist;
			}
		}

		//루돌프 이동
		roo_i = roo_i + dy[dirindex];
		roo_j = roo_j + dx[dirindex];
		
		//루돌프가 이동한 자리에 산타가 있을때
		if (arr[roo_i][roo_j] != 0) {
			int jumpsanta = arr[roo_i][roo_j]; //튕겨나가는 산타 index 저장
			santascore[jumpsanta] += c; // 점수 획득
			santalife[jumpsanta] = round + 2; //기절

			//산타가 튕겨 도착하는 위치
			int ny = roo_i + c * dy[dirindex];
			int nx = roo_j + c * dx[dirindex];
			interaction(ny,nx, jumpsanta,dirindex);
			arr[roo_i][roo_j] = 0;
		}
		////루돌프가 빈공간으로 이동할때.
		//else {
		//
		//}

		//산타이동
		for (int santanum = 1; santanum <= p; santanum++) {
			if (santalife[santanum] > round) continue; // 기절 or 탈락한 산타 스킵
			auto movesanta = santa[santanum];
			int i = movesanta.first;
			int j = movesanta.second;

			//산타이동 방향 결정
			int santadirindex = -1;
			int min_dist = cal_dist(roo_i, roo_j, i, j);
			for (int tempdir = 0; tempdir < 4; tempdir++) {
				int ny = i + dy[tempdir];
				int nx = j + dx[tempdir];

				if (ny > n || nx > n || ny < 1 || nx < 1) continue; //맵을 넘어가는 경우 스킵
				if (arr[ny][nx] != 0) continue; // 이동할 곳에 산타가 있다면 pass;
				int tempdist = cal_dist(ny, nx, roo_i, roo_j);
				if (tempdist < min_dist) {
					min_dist = tempdist;
					santadirindex = tempdir;
				}
			}
			if (santadirindex == -1) continue;
			//산타 이동
			int santa_i = i + dy[santadirindex];
			int santa_j = j + dx[santadirindex];
			//그 위치에 루돌프가 있다면 튕겨 나감
			if (santa_i == roo_i && santa_j == roo_j) {

				santascore[santanum] += d;
				santalife[santanum] = round + 2;

				//반대 방향으로 d만큼 이동
				if (santadirindex < 2) santadirindex += 2;
				else santadirindex -= 2;

				santa_i += d * dy[santadirindex];
				santa_j += d * dx[santadirindex];

				if (santa_i > n || santa_j > n || santa_i < 1 || santa_j < 1) {
					arr[i][j] = 0;
					santalife[santanum] = max_int;
				}
				//튕겨나간 자리에 산타가 있다면, 상호작용 발생
				else if (arr[santa_i][santa_j] != 0) {
					int jumpsanta = arr[santa_i][santa_j];
					arr[i][j] = 0;
					interaction(santa_i, santa_j, santanum, santadirindex);
				}
				else {
					//빈자리였다면
					arr[i][j] = 0;
					arr[santa_i][santa_j] = santanum;
					santa[santanum] = { santa_i, santa_j };
				}
			}
			else {
				arr[i][j] = 0;
				arr[santa_i][santa_j] = santanum;
				santa[santanum] = { santa_i, santa_j };
			}
		}
		//추가점수
		int flag = 0;
		for (int tempsanta = 1; tempsanta <= p; tempsanta++) {
			if (santalife[tempsanta] == max_int) continue;
			santascore[tempsanta] += 1;
			flag = 1;
		}
		if (flag == 0) break;
	}
	//결과 출력
	for (int tempsanta = 1; tempsanta <= p; tempsanta++) {
		cout << santascore[tempsanta] << " ";
	}
	
}