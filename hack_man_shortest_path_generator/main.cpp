#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

bool wall[20][14];
int index[20][14];
vector<vector<int>> coordinates;
vector<bool> bugSpawns;

vector<vector<int>> bestPaths;
vector<vector<int>> pathLengths;
vector<vector<vector<int>>> bestPathsWithDirection;
vector<vector<vector<int>>> pathLengthsWithDirection;
vector<int> optimalPlacements;

int is_adjacent(int ax, int ay, int bx, int by) {
	/*
	-1=not adjacent
	0=up
	1=right
	2=down
	3=left
	*/
	if (ax == bx) {
		if (ay - by == 1)
			return 0;
		else if (ay - by == -1)
			return 2;
	}
	else if (ay == by) {
		if (ax - bx == 1)
			return 3;
		else if (ax - bx == -1)
			return 1;
	}
	return -1;
}

void main() {
	string walls = ".....................xxxxx.xxxxxx.xxxxx..x.....xxxxxx.....x..x.xxx...xx...xxx.x......xxx.xx.xxx......xxx.x........x.xxx....x.x.xxbbxx.x.x...xx.x...xbbbbx...x.xx...xxx.xxxxxx.xxx....x................x..xxx.xxxxxxxxxx.xxx..xxx............xxx..xxx.xxx.xx.xxx.xxx..........xx.........";
	char c;
	int i = 0;
	int spaceIndex = 0;
	for (int y=0; y<14; ++y)
	{
		for (int x = 0; x < 20; ++x)
		{
			c = walls[i++];
			if (c == 'x') {
				wall[x][y] = true;
				index[x][y] = -1;
			} else {
				wall[x][y] = false;
				coordinates.push_back(vector<int>(2));
				coordinates[spaceIndex][0] = x;
				coordinates[spaceIndex][1] = y;
				index[x][y] = spaceIndex++;
				if (c == 'b') {
					bugSpawns.push_back(true);
					wall[x][y] = true;
				} else {
					bugSpawns.push_back(false);
				}
			}
		}
	}
	bestPaths = vector<vector<int>>(spaceIndex);
	pathLengths = vector<vector<int>>(spaceIndex);
	for (int i = 0; i < spaceIndex; ++i) {
		bestPaths[i] = vector<int>(spaceIndex);
		pathLengths[i] = vector<int>(spaceIndex);
	}
	int dx[4] = { 0, 1, 0,-1 };
	int dy[4] = { -1, 0, 1, 0 };
	for (int startY = 0; startY < 14; ++startY) {
		for (int startX = 0; startX < 20; ++startX) {
			int startIndex = index[startX][startY];
			if (startIndex != -1){
				vector<int> visited;
				queue<int> Q;
				queue<int> length_Q;
				queue<int> startDir;
				visited.push_back(startIndex);
				bestPaths[startIndex][startIndex] = -1;
				pathLengths[startIndex][startIndex] = 0;
				for (int d = 0; d < 4; ++d) {
					if (startX + dx[d] < 0 || startX + dx[d] > 19 || startY + dy[d] < 0 || startY + dy[d] > 13)
						continue;
					int neighbor = index[startX + dx[d]][startY + dy[d]];
					if (neighbor != -1){
						if (bugSpawns[startIndex] && bugSpawns[neighbor] || !bugSpawns[neighbor])
						{
							bestPaths[startIndex][neighbor] = d;
							pathLengths[startIndex][neighbor] = 1;
							Q.push(neighbor);
							length_Q.push(1);
							startDir.push(d);
						}
					}
				}
				while (Q.size() > 0) {
					int curr = Q.front();
					int curr_length = length_Q.front();
					int curr_start_dir = startDir.front();
					Q.pop();
					length_Q.pop();
					startDir.pop();
					int currX = coordinates[curr][0];
					int currY = coordinates[curr][1];
					int dx[4] = { 0, 1, 0,-1 };
					int dy[4] = { -1, 0, 1, 0 };
					for (int d = 0; d < 4; ++d) {
						if (currX + dx[d] < 0 || currX + dx[d] > 19 || currY + dy[d] < 0 || currY + dy[d] > 13)
							continue;
						if (index[currX + dx[d]][currY + dy[d]] != -1) {
							int neighbor = index[currX + dx[d]][currY + dy[d]];
							if (neighbor != -1)
							{
								if (bugSpawns[startIndex] && bugSpawns[neighbor] || !bugSpawns[neighbor])
								{
									if (find(visited.begin(), visited.end(), neighbor) != visited.end())
										continue;
									bestPaths[startIndex][neighbor] = curr_start_dir;
									pathLengths[startIndex][neighbor] = curr_length + 1;
									Q.push(neighbor);
									length_Q.push(curr_length + 1);
									startDir.push(curr_start_dir);
								}
							}
						}
					}
					visited.push_back(curr);
				}
				/*
				vector<int> visited;
				queue<int> Q;
				queue<int> length_Q;
				Q.push(startIndex);
				length_Q.push(0);
				while (Q.size() > 0) {
					int curr = Q.front();
					int curr_length = length_Q.front();
					Q.pop();
					length_Q.pop();
					int currX = coordinates[curr][0];
					int currY = coordinates[curr][1];
					for (int d = 0; d < 4; ++d) {
						if (currX + dx[d] < 0 || currX + dx[d] > 19 || currY + dy[d] < 0 || currY + dy[d] > 13)
							continue;
						if (index[currX + dx[d]][currY + dy[d]] != -1) {
							int neighbor = index[currX + dx[d]][currY + dy[d]];
							if (neighbor != -1)
							{
								if (find(visited.begin(), visited.end(), neighbor) != visited.end())
									continue;
								bestPaths[neighbor][startIndex] = is_adjacent(currX+dx[d], currY+dy[d], currX, currY);
								pathLengths[neighbor][startIndex] = curr_length + 1;
								Q.push(neighbor);
								length_Q.push(curr_length + 1);
							}
						}
					}
					visited.push_back(curr);
				}
				*/
				/*
				for (int printY = 0; printY < 14; ++printY){
					for (int printX = 0; printX < 20; ++printX) {
						int i = index[printX][printY];
						if (i == -1) {
							cout << (char)219 << (char)219 << (char)219 << (char)219 << (char)219;
						}
						else {
							int dist = pathLengths[i][startIndex];
							
							if (dist < 100)
								cout << " ";
							if (dist < 10)
								cout << " ";
							cout << dist << " " << bestPaths[i][startIndex];
						}
					}
					cout << endl;
				}
				cout << endl;
				*/
			}
		}
	}
	bestPathsWithDirection = vector<vector<vector<int>>>(4);
	pathLengthsWithDirection = vector<vector<vector<int>>>(4);
	for (int dir = 0; dir < 4; ++dir) {
		bestPathsWithDirection[dir] = vector<vector<int>>(spaceIndex);
		pathLengthsWithDirection[dir] = vector<vector<int>>(spaceIndex);
		for (int i = 0; i < spaceIndex; ++i) {
			bestPathsWithDirection[dir][i] = vector<int>(spaceIndex);
			pathLengthsWithDirection[dir][i] = vector<int>(spaceIndex);
			for (int j = 0; j < spaceIndex; ++j) {
				bestPathsWithDirection[dir][i][j] = -1;
			}
		}
	}
	for (int startY = 0; startY < 14; ++startY) {
		for (int startX = 0; startX < 20; ++startX) {
			int startIndex = index[startX][startY];
			if (startIndex != -1) {
				for (int direction = 0; direction < 4; ++direction) {
					vector<int> visited;
					queue<int> Q;
					queue<int> length_Q;
					queue<int> startDir;
					visited.push_back(startIndex);
					bestPathsWithDirection[direction][startIndex][startIndex] = -1;
					pathLengthsWithDirection[direction][startIndex][startIndex] = 0;
					for (int d = 0; d < 4; ++d) {
						if (startX + dx[d] < 0 || startX + dx[d] > 19 || startY + dy[d] < 0 || startY + dy[d] > 13 || d == (direction+2)%4)
							continue;
						int neighbor = index[startX + dx[d]][startY + dy[d]];
						if (neighbor != -1)
						{
							if (bugSpawns[startIndex] && bugSpawns[neighbor] || !bugSpawns[neighbor])
							{
								bestPathsWithDirection[direction][startIndex][neighbor] = d;
								pathLengthsWithDirection[direction][startIndex][neighbor] = 1;
								Q.push(neighbor);
								length_Q.push(1);
								startDir.push(d);
							}
						}
					}
					while (Q.size() > 0) {
						int curr = Q.front();
						int curr_length = length_Q.front();
						int curr_start_dir = startDir.front();
						Q.pop();
						length_Q.pop();
						startDir.pop();
						int currX = coordinates[curr][0];
						int currY = coordinates[curr][1];
						int dx[4] = { 0, 1, 0,-1 };
						int dy[4] = { -1, 0, 1, 0 };
						for (int d = 0; d < 4; ++d) {
							if (currX + dx[d] < 0 || currX + dx[d] > 19 || currY + dy[d] < 0 || currY + dy[d] > 13)
								continue;
							if (index[currX + dx[d]][currY + dy[d]] != -1) {
								int neighbor = index[currX + dx[d]][currY + dy[d]];
								if (neighbor != -1)
								{
									if (bugSpawns[startIndex] && bugSpawns[neighbor] || !bugSpawns[neighbor])
									{
										if (find(visited.begin(), visited.end(), neighbor) != visited.end())
											continue;
										bestPathsWithDirection[direction][startIndex][neighbor] = curr_start_dir;
										pathLengthsWithDirection[direction][startIndex][neighbor] = curr_length + 1;
										Q.push(neighbor);
										length_Q.push(curr_length + 1);
										startDir.push(curr_start_dir);
									}
								}
							}
						}
						visited.push_back(curr);
					}
				}
			}
		}
	}
	/*
	for (int startY = 0; startY < 14; ++startY) {
		for (int startX = 0; startX < 20; ++startX) {
			if (index[startX][startY] != -1) {
				for (int y = 0; y < 14; ++y) {
					for (int dir = 0; dir < 4; ++dir) {
						for (int x = 0; x < 20; ++x) {
							if (index[x][y] == -1) {
								cout << (unsigned char)219 << (unsigned char)219;
							}
							else {
								switch (bestPathsWithDirection[dir][index[x][y]][index[startX][startY]]) {
								case 0:
									cout << " ^";
									break;
								case 1:
									cout << " >";
									break;
								case 2:
									cout << " v";
									break;
								case 3:
									cout << " <";
									break;
								default:
									cout << "  ";
									break;
								}
							}
						}
						cout << "    ";
					}
					cout << endl;
				}
				cout << endl;
			}
			cout << endl;
		}
	}
	*/
	optimalPlacements = vector<int>(spaceIndex);
	vector<int> optimalNearerCount = vector<int>(spaceIndex);
	for (int startY = 0; startY < 14; ++startY) {
		for (int startX = 0; startX < 20; ++startX) {
			int startIndex = index[startX][startY];
			if (startIndex != -1){
				int bestNearNodeCount = 0;
				int bestIndex = -1;
				for (int y = 0; y < 14; ++y) {
					for (int x = 0; x < 14; ++x) {
						int targetIndex = index[x][y];
						if (targetIndex != -1) {
							int closerToTargetCount = 0;
							for (int endY = 0; endY < 14; ++endY) {
								for (int endX = 0; endX < 20; ++endX) {
									int endIndex = index[endX][endY];
									if (endIndex != -1){
										if (pathLengths[endIndex][startIndex] > pathLengths[endIndex][targetIndex]) {
											closerToTargetCount++;
										}
									}
								}
							}
							if (closerToTargetCount > bestNearNodeCount) {
								bestNearNodeCount = closerToTargetCount;
								bestIndex = targetIndex;
							}
						}
					}
				}
				optimalPlacements[startIndex] = bestIndex;
				optimalNearerCount[startIndex] = bestNearNodeCount;
			}

		}
	}
	ofstream file;
	file.open("paths.txt");
	if (!file.is_open())
	{
		cout << "File could not be opened!" << endl;
	}
	file << "const int index[20][14] = " << endl;
	file << "{" << endl;
	for (int x = 0; x < 20; ++x) {
		file << "    {";
		for (int y = 0; y < 14; ++y) {
			file << index[x][y];
			if (y < 13)
				file << ",";
		}
		file << "}";
		if (x < 19)
			file << ",";
		file << endl;
	}
	file << "};" << endl << endl;

	file << "const int coordinate[" << spaceIndex << "][2] = " << endl;
	file << "{" << endl;
	for (int i=0; i<spaceIndex; ++i) {
		file << "    {" << coordinates[i][0] << "," << coordinates[i][1] << "}";
		if (i < spaceIndex - 1)
			file << ",";
		file << endl;
	}
	file << "};" << endl << endl;

	file << "const bool walls[20][14] = " << endl;
	file << "{" << endl;
	for (int x = 0; x < 20; ++x) {
		file << "    {";
		for (int y = 0; y < 14; ++y) {
			if (wall[x][y])
				file << "true ";
			else
				file << "false";
			if (y < 13)
				file << ",";
		}
		file << "}";
		if (x < 19)
			file << ",";
		file << endl;
	}
	file << "};" << endl << endl;

	file << "const int optimalPositions[" << spaceIndex << "] = {";
	for (int i = 0; i < spaceIndex; ++i){
		file << optimalPlacements[i];
		if (i != spaceIndex - 1)
			file << ",";
	}
	file << "};" << endl << endl;

	file << "const int paths[" << spaceIndex << "][" << spaceIndex << "] = " << endl;
	file << "{" << endl;
	for (int i = 0; i < spaceIndex; ++i) {
		file << "    {";
		for (int j = 0; j < spaceIndex; ++j) {
			file << bestPaths[i][j];
			if (j != spaceIndex - 1)
				file << ",";
		}
		file << "}";
		if (i < spaceIndex - 1)
			file << ",";
		file << endl;
	}
	file << "};" << endl << endl;

	file << "const int path_lengths[" << spaceIndex << "][" << spaceIndex << "] = " << endl;
	file << "{" << endl;
	for (int i = 0; i < spaceIndex; ++i) {
		file << "    {";
		for (int j = 0; j < spaceIndex; ++j) {
			file << pathLengths[i][j];
			if (j != spaceIndex - 1)
				file << ",";
		}
		file << "}";
		if (i < spaceIndex - 1)
			file << ",";
		file << endl;
	}
	file << "};" << endl << endl;

	file << "const int paths_with_direction[4][" << spaceIndex << "][" << spaceIndex << "] = " << endl;
	file << "{" << endl;
	for (int dir = 0; dir < 4; ++dir) {
		file << "    {" << endl;
		for (int i = 0; i < spaceIndex; ++i) {
			file << "        {";
			for (int j = 0; j < spaceIndex; ++j) {
				file << bestPathsWithDirection[dir][i][j];
				if (j != spaceIndex - 1)
					file << ",";
			}
			file << "}";
			if (i < spaceIndex-1)
				file << ",";
			file << endl;
		}
		file << "    }";
		if (dir < 3)
			file << ",";
		file << endl;
	}
	file << "};" << endl << endl;

	file << "const int path_lengths_with_direction[4][" << spaceIndex << "][" << spaceIndex << "] = " << endl;
	file << "{" << endl;
	for (int dir = 0; dir < 4; ++dir) {
		file << "    {" << endl;
		for (int i = 0; i < spaceIndex; ++i) {
			file << "        {";
			for (int j = 0; j < spaceIndex; ++j) {
				file << pathLengthsWithDirection[dir][i][j];
				if (j != spaceIndex - 1)
					file << ",";
			}
			file << "}";
			if (i < spaceIndex - 1)
				file << ",";
			file << endl;
		}
		file << "    }";
		if (dir < 3)
			file << ",";
		file << endl;
	}
	file << "};" << endl;
}