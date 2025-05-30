#include "Maze.h"
#include <iostream>
#include <cstdlib>
#include <random>
#include <vector>
#include <queue>

// Node ����ü ����
struct Node {
    int cost;   // ����ġ (wallX, wallY��)
    int x, y;   // ��ǥ
    int wallX, wallY; // �� ���� �����ϱ� ���� ������ ��ǥ 

    // �켱���� ť���� ����� �� ������ (����ġ ���� �������� ����)
    bool operator>(const Node& other) const {
        return cost > other.cost;
    }
};

Maze::Maze(int size) : size(size), grid(size, std::vector<PlaceType>(size, WALL)) {} // ������ ���� 

// �̷� ���� �Լ� (Prim �˰��� �̿�)
void Maze::GenerateRandomMaze() {
    // (Ȧ��, Ȧ��) ��ġ�� ��η� ����
    for (int x = 1; x < size; x += 2) {
        for (int y = 1; y < size; y += 2) {
            grid[x][y] = PATH; // PATH
        }
    }
    std::vector<std::vector<int>> cost(size, std::vector<int>(size, 0)); // (Ȧ, Ȧ)�� ������ ���� cost�� ���� 

    // ����ġ ���� (���� ������ ��� �Ҵ�)
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

    // �湮 ���θ� �����ϴ� �迭
    std::vector<std::vector<int>> visited(size, std::vector<int>(size, 0)); // visited = 1�̸� �湮 
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;

    // ������ ����
    visited[1][1] = 1;

    // ���������� ����� ����(��)�� ť�� �߰�
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

        // �̹� �湮�� ���� �ǳʶٱ�
        if (visited[current.x][current.y]) continue;

        // MST�� ���Ե� ���� �ձ�
        visited[current.x][current.y] = 1;
        grid[current.wallX][current.wallY] = PATH; // MST�� ���ԵǴ� ���� ���Ͽ�, ������ �ϴ� ���� PATH�� ����

        // ���� ������ ����� ���ο� ������ ť�� �߰�
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

    // �������� ���� ����
    grid[1][1] = START; // START
    grid[size - 2][size - 1] = END; // END
}

PlaceType Maze::GetPlaceType(int x, int y) const { return grid[x][y]; } // ĭ�� Type ��ȯ 
void Maze::SetPlaceType(int x, int y, PlaceType type) { grid[x][y] = type; } // ĭ�� Type ���� 
int Maze::GetSize() const { return size; } // size ���� 
