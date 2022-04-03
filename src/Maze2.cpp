#include "Maze2.h"
#include "CellPos.h"
#include "random.h"
#include <cassert>
#include <iostream>

Maze2::Maze2(int n, int m) : grid(n, m) {}
Maze2::Maze2(int n, int m, CellPos enterPoint, CellPos leavePoint)
    : grid(n, m, GridCellState::NOTFILLED, enterPoint, leavePoint) {}
void Maze2::printMaze(std::wostream& stream) { grid.printTable(stream); }
void Maze2::printAsMatrix(std::wostream& stream) { grid.printAsMatrix(stream); }
void Maze2::generateRecursive(CellPos originPoint) {
  adjacent_walls_t walls;
  adjacent_points_t points;

  grid.visitedPoint(originPoint);
  int n{grid.getAdjacentWallsAndCells(originPoint, walls, points)};
  // assert(n != 0);

  while (n > 0) {
    int random{getRandom(0, n - 1)};
    CellInfo neighbour{points[random]};
    ptr_t<WallCellState> wallState{walls[random]};

    if (!grid.hasVisitedPoint(neighbour.pos)) {
      (*wallState) = WallCellState::OPENED;
      // printMaze();
      generateRecursive(neighbour.pos);
    }

    removeWallCellEntry(points, walls, random);
    --n;
  }
}
void Maze2::removeWallCellEntry(adjacent_points_t& points,
                                adjacent_walls_t& walls, int index) {
  int i{index};
  while (i < maxNeighbourCount - 1) {
    points[i] = points[i + 1];
    walls[i] = walls[i + 1];
    ++i;
  }
}

void Maze2::generate() {
  CellPos cellpos{getRandomPoint()};
  // grid.visitedPoint(cellpos);
  generateRecursive(cellpos);
}

CellPos Maze2::getRandomPoint() {
  return {getRandom(0, grid.rows - 1), getRandom(0, grid.columns - 1)};
}