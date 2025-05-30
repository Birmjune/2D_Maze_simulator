#include "Maze.h"
#include <iostream>
#include <cstdlib>
#include <random>
#include <vector>
#include <queue>

// Node 구조체 정의
struct Node {
    int cost;   // 가중치 (wallX, wallY의)
    int x, y;   // 좌표
    int wallX, wallY; // 그 점에 도달하기 위한 간선의 좌표 

    // 우선순위 큐에서 사용할 비교 연산자 (가중치 기준 오름차순 정렬)
    bool operator>(const Node& other) const {
        return cost > other.cost;
    }
};

Maze::Maze(int size) : size(size), grid(size, std::vector<PlaceType>(size, WALL)) {} // 생성자 구현 

// 미로 생성 함수 (Prim 알고리즘 이용)
void Maze::GenerateRandomMaze() {
    // (홀수, 홀수) 위치를 경로로 설정
    for (int x = 1; x < size; x += 2) {
        for (int y = 1; y < size; y += 2) {
            grid[x][y] = PATH; // PATH
        }
    }
    std::vector<std::vector<int>> cost(size, std::vector<int>(size, 0)); // (홀, 홀)과 인접한 벽은 cost를 설정 

    // 가중치 설정 (벽에 무작위 비용 할당)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 100);

    for (int x = 1; x < size; x++) {
        for (int y = 1; y < size; y++) {
            if ((x % 2 == 0 && y % 2 != 0) || (x % 2 != 0 && y % 2 == 0)) {
                cost[x][y] = dist(gen);
            }
        }
    }

    // 방문 여부를 저장하는 배열
    std::vector<std::vector<int>> visited(size, std::vector<int>(size, 0)); // visited = 1이면 방문 
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;

    // 시작점 설정
    visited[1][1] = 1;

    // 시작점에서 연결된 간선(벽)을 큐에 추가
    std::vector<std::pair<int, int>> moves = { {-2, 0}, {2, 0}, {0, 2}, {0, -2} };
    for (const auto& move : moves) {
        int nx = 1 + move.first;
        int ny = 1 + move.second;
        if (nx > 0 && nx < size && ny > 0 && ny < size) {
            int wallX = 1 + move.first / 2;
            int wallY = 1 + move.second / 2;
            pq.push({ cost[wallX][wallY], nx, ny, wallX, wallY });
        }
    }

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        // 이미 방문한 셀은 건너뛰기
        if (visited[current.x][current.y]) continue;

        // MST에 포함된 벽만 뚫기
        visited[current.x][current.y] = 1;
        grid[current.wallX][current.wallY] = PATH; // MST에 포함되는 점에 대하여, 지나야 하는 벽을 PATH로 설정

        // 현재 셀에서 연결된 새로운 간선을 큐에 추가
        for (const auto& move : moves) {
            int nx = current.x + move.first;
            int ny = current.y + move.second;
            if (nx > 0 && nx < size && ny > 0 && ny < size && !visited[nx][ny]) {
                int newWallX = current.x + move.first / 2;
                int newWallY = current.y + move.second / 2;
                pq.push({ cost[newWallX][newWallY], nx, ny, newWallX, newWallY });
            }
        }
    }

    // 시작점과 끝점 설정
    grid[1][1] = START; // START
    grid[size - 2][size - 1] = END; // END
}

PlaceType Maze::GetPlaceType(int x, int y) const { return grid[x][y]; } // 칸의 Type 반환 
void Maze::SetPlaceType(int x, int y, PlaceType type) { grid[x][y] = type; } // 칸의 Type 설정 
int Maze::GetSize() const { return size; } // size 리턴 
