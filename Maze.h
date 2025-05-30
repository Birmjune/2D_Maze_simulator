#pragma once
#include <vector>

enum PlaceType { WALL, PATH, START, END }; // 벽, 이동가능통로, 시작, 끝 

class Maze {
public:
    Maze(int size); // 생성자 
    void GenerateRandomMaze(); // 난이도 따라 미로 생성
    PlaceType GetPlaceType(int x, int y) const; // grid[x][y]의 타입 반환 
    void SetPlaceType(int x, int y, PlaceType type); // grid[x][y] 타입 설정 
    int GetSize() const; // get size 

private:
    int size;
    std::vector<std::vector<PlaceType>> grid; // 미로를 표현하는 2차원 vector
};