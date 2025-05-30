#pragma once
#include <vector>

enum PlaceType { WALL, PATH, START, END }; // ��, �̵��������, ����, �� 

class Maze {
public:
    Maze(int size); // ������ 
    void GenerateRandomMaze(); // ���̵� ���� �̷� ����
    PlaceType GetPlaceType(int x, int y) const; // grid[x][y]�� Ÿ�� ��ȯ 
    void SetPlaceType(int x, int y, PlaceType type); // grid[x][y] Ÿ�� ���� 
    int GetSize() const; // get size 

private:
    int size;
    std::vector<std::vector<PlaceType>> grid; // �̷θ� ǥ���ϴ� 2���� vector
};