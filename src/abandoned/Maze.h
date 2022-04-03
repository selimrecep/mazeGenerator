#pragma once
#include "../CellPos.h"
#include "../random.h"
#include "Table.h"
#include <array>
#include <cassert>


constexpr CellPos invalidPoint{-1, -1};

using neighbours_t = std::array<CellPos, 4>;

template <size_t N, size_t M> class Maze {
private:
  CellPos begin;
  CellPos finish;
  Table<N, M> grid;
  size_t rows;
  size_t columns;

public:
  // Probably should been used std::move but I
  // haven't properly learnt it yet.
  Maze(CellPos begin, CellPos finish) : begin(begin), finish(finish) {
    grid[begin.row][begin.column] = true;
    grid[finish.row][finish.column] = true;

    rows = N;
    columns = M;
  }
  // Dummy
  void generateLevel1Algorithm() {
    CellPos currPos{begin};

    while (currPos != finish) {
      currPos = getNearestPoint(currPos, finish);
      grid[currPos] = true;
      printMaze();
    }

    int branchCount = 15;
    int minDepth = 3;
    int maxDepth = 8;

    for (int i{0}; i < branchCount; ++i) {
      CellPos closedPoint{getRandomClosedPoint()};
      int depth{getRandom(minDepth, maxDepth)};
      CellPos curr = closedPoint;
      for (int j{0}; j < depth; ++j) {
        curr = getRandomClosedNextPoint(curr);
        if (curr == invalidPoint)
          break;
        grid[curr] = true;
        // printMaze();
      }
    }
  }
  // Still dummy but less dummier

  void generateLevel2Algorithm() {
    CellPos currPos{begin};

    while (currPos != finish) {
      currPos = getNearestPoint(currPos, finish);
      grid[currPos] = true;
      // printMaze();
    }

    int branchCount = sqrt(rows * columns) * 2;
    int minDepth = 3;
    int maxDepth = 5;
    CellPos curr{getRandomClosedPoint()};

    for (int i{0}; i < branchCount; ++i) {
      CellPos closedPoint{};
      if (curr == invalidPoint)
        curr = getRandomClosedPoint();
      int random{getRandom(0, 7)};
      if (random == 0)
        closedPoint = curr;
      else if (random == 1 || random == 2)
        closedPoint = getRandomPoint();
      else
        closedPoint = getRandomClosedPoint();
      int depth{getRandom(minDepth, maxDepth)};
      for (int j{0}; j < depth; ++j) {
        curr = getRandomClosedNextPoint(curr, true, false);
        if (curr == invalidPoint)
          break;
        grid[curr] = true;
        printMaze();
      }
    }
  }
  CellPos getRandomClosedNextPoint(CellPos currentPos,
                                   bool ignoreNeighbours = false,
                                   bool ignoreOpenPath = false) {
    neighbours_t randomPoints{};
    size_t index{0};
    CellPos nextPos{};

    nextPos = {currentPos.row + 1, currentPos.column};
    if (validPoint(nextPos) &&
        (ignoreNeighbours || !hasOpenNeighbor(nextPos, currentPos)) &&
        (ignoreOpenPath || grid[nextPos] == false))
      randomPoints[index++] = nextPos;
    nextPos = {currentPos.row - 1, currentPos.column};
    if (validPoint(nextPos) &&
        (ignoreNeighbours || !hasOpenNeighbor(nextPos, currentPos)) &&
        (ignoreOpenPath || grid[nextPos] == false))
      randomPoints[index++] = nextPos;
    nextPos = {currentPos.row, currentPos.column + 1};
    if (validPoint(nextPos) &&
        (ignoreNeighbours || !hasOpenNeighbor(nextPos, currentPos)) &&
        (ignoreOpenPath || grid[nextPos] == false))
      randomPoints[index++] = nextPos;
    nextPos = {currentPos.row, currentPos.column - 1};
    if (validPoint(nextPos) &&
        (ignoreNeighbours || !hasOpenNeighbor(nextPos, currentPos)) &&
        (ignoreOpenPath || grid[nextPos] == false))
      randomPoints[index++] = nextPos;

    if (index == 0) {
      return CellPos{-1, -1};
      // assert(false && "RIP");
    }
    int random{getRandom(0, index - 1)};

    return randomPoints[random];
  }
  CellPos getRandomClosedPoint() {
    CellPos pos;
    do {
      pos = getRandomPoint();
    } while (grid[pos] == true);

    return pos;
  }
  CellPos getRandomPoint() {
    CellPos pos{getRandom(0, rows - 1), getRandom(0, columns - 1)};
    return pos;
  }

  CellPos getNearestPoint(CellPos current, CellPos target) {
    CellPos firstPoint{current};
    CellPos secondPoint{current};
    bool rowNeedsChange = false;
    bool columnNeedsChange = false;

    if (target.row != current.row) {
      rowNeedsChange = true;
      firstPoint.row += (target.row - current.row > 0) ? 1 : -1;
      if (!validPoint(firstPoint))
        rowNeedsChange = false;
    }
    if (target.column != current.column) {
      columnNeedsChange = true;
      secondPoint.column += (target.column - current.column > 0) ? 1 : -1;
      if (!validPoint(secondPoint))
        columnNeedsChange = false;
    }

    if (rowNeedsChange && columnNeedsChange) {
      int random{getRandom(0, 1)};
      if (random == 0)
        return firstPoint;
      else
        return secondPoint;
    } else if (rowNeedsChange) {
      return firstPoint;
    } else if (columnNeedsChange) {
      return secondPoint;
    } else {
      assert(false && "RIP");
    }

    return current;
  }

  int validPathsToOpen(CellPos pos, CellPos prevPos, neighbours_t& positions) {
    int index{0};
    CellPos curr{};

    curr = pos + CellPos{0, 1};
    if (validPoint(curr) && pos != prevPos && !hasOpenNeighbor(curr, pos))
      positions[index++] = curr;
    curr = pos + CellPos{0, -1};
    if (validPoint(curr) && pos != prevPos && !hasOpenNeighbor(curr, pos))
      positions[index++] = curr;
    curr = pos + CellPos{1, 0};
    if (validPoint(curr) && pos != prevPos && !hasOpenNeighbor(curr, pos))
      positions[index++] = curr;
    curr = pos + CellPos{-1, 0};
    if (validPoint(curr) && pos != prevPos && !hasOpenNeighbor(curr, pos))
      positions[index++] = curr;
    return index;
  }

  bool hasOpenNeighbor(CellPos nextPos, CellPos currPos) {
    CellPos curr{};

    curr = {nextPos.row + 0, nextPos.column + 1};
    if (validPoint(curr) && curr != currPos && grid[curr] == true)
      return true;
    curr = {nextPos.row + 0, nextPos.column + -1};
    if (validPoint(curr) && curr != currPos && grid[curr] == true)
      return true;
    curr = {nextPos.row + 1, nextPos.column + 0};
    if (validPoint(curr) && curr != currPos && grid[curr] == true)
      return true;
    curr = {nextPos.row + -1, nextPos.column + 0};
    if (validPoint(curr) && curr != currPos && grid[curr] == true)
      return true;

    return false;
  }

  bool validPoint(CellPos pos) const {
    if (pos.row <= 0 || pos.row >= static_cast<int>(rows))
      return false;
    if (pos.column <= 0 || pos.column >= static_cast<int>(columns))
      return false;
    return true;
  }

  void printMaze() { grid.printTable(); }
};