#pragma once
#include "TrueGrid.h"
#include <iostream>

constexpr int maxNeighbourCount = 4;

class Maze2 {
private:
  TrueGrid grid;

  CellPos getRandomPoint();

  void generateRecursive(CellPos point);
  static void removeWallCellEntry(adjacent_points_t& points,
                                  adjacent_walls_t& walls, int index);

public:
  Maze2(int n, int m);
  Maze2(int n, int m, CellPos enterPoint, CellPos leavePoint);
  void printMaze(std::wostream& stream = std::wcout);
  void printAsMatrix(std::wostream& stream = std::wcout);
  void generate();
};