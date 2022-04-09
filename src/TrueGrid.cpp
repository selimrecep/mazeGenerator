#include "TrueGrid.h"
#include "CellPos.h"
#include <array>
#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>

bool TrueGrid::hasVisitedPoint(CellPos pos) {
  return hasVisited[pos.row][pos.column] == HasVisited::HAS_VISITED;
}
void TrueGrid::visitedPoint(CellPos pos) {
  hasVisited[pos.row][pos.column] = HasVisited::HAS_VISITED;
}
void TrueGrid::resizeGrid(int n, int m) {
  rows = n;
  columns = m;

  hasVisited.resize(rows);
  for (int i{0}; i < n; ++i)
    hasVisited[i].resize(m);

  grid.resize(n);
  for (int i{0}; i < n; ++i)
    grid[i].resize(m);

  wallRowsCount = n - 1;
  wallColumnsCount = m - 1;

  wallsRows.resize(wallRowsCount);
  for (int i{0}; i < wallRowsCount; ++i) {
    wallsRows[i].resize(wallColumnsCount + 1);
    std::fill(wallsRows[i].begin(), wallsRows[i].end(), WallCellState::CLOSED);
  }

  wallsColumns.resize(wallRowsCount + 1);
  for (int i{0}; i < wallRowsCount + 1; ++i) {
    wallsColumns[i].resize(wallColumnsCount);
    std::fill(wallsColumns[i].begin(), wallsColumns[i].end(),
              WallCellState::CLOSED);
  }
}
TrueGrid::TrueGrid(int n, int m, GridCellState defaultValue, CellPos enterPoint,
                   CellPos leavePoint)
    : enterPoint(enterPoint), leavePoint(leavePoint) {

  hasEnterOutPoint = true;
  resizeGrid(n, m);
  for (int i{0}; i < n; ++i)
    for (int j{0}; j < m; ++j)
      grid[i][j] = defaultValue;
}
TrueGrid::TrueGrid(int n, int m) { resizeGrid(n, m); }
TrueGrid::TrueGrid(int n, int m, GridCellState defaultValue) {
  resizeGrid(n, m);
  for (int i{0}; i < n; ++i)
    for (int j{0}; j < m; ++j)
      grid[i][j] = defaultValue;
}
void TrueGrid::printTable(std::wostream& stream) {
  bool writtenEntryPoint{!hasEnterOutPoint};
  bool writtenExitPoint{!hasEnterOutPoint};

  stream << TL;
  for (int i{0}; i < columns - 1; ++i) {
    stream << (isSpace(writtenEntryPoint, writtenExitPoint, 0, i) ? L"  "
                                                                  : doubleH)
           << MT;
  }
  stream << (isSpace(writtenEntryPoint, writtenExitPoint, 0, columns - 1)
                 ? L"  "
                 : doubleH)
         << TR;
  stream << '\n';

  std::wstring ch{};
  for (int i{0}; i < rows - 1; ++i) {
    stream << (isSpace(writtenEntryPoint, writtenExitPoint, i, 0) ? L' ' : V);
    for (int j{0}; j < columns - 1; ++j) {
      ch = printRepresentingChar(grid[i][j]);
      stream << ch << (wallsColumns[i][j] == WallCellState::CLOSED ? V : L' ');
    }
    ch = printRepresentingChar(grid[i][columns - 1]);
    stream << ch
           << (isSpace(writtenEntryPoint, writtenExitPoint, i, columns - 1)
                   ? L' '
                   : V);
    stream << '\n';
    stream << ML;
    for (int j{0}; j < columns - 1; ++j)
      stream << (wallsRows[i][j] == WallCellState::CLOSED ? doubleH : L"  ")
             << MC;
    stream << (wallsRows[i][columns - 1] == WallCellState::CLOSED ? doubleH
                                                                  : L"  ")
           << MR;
    stream << '\n';
  }

  stream << V;
  for (int j{0}; j < columns - 1; ++j) {
    ch = printRepresentingChar(grid[rows - 1][j]);
    stream << ch
           << (wallsColumns[rows - 1][j] == WallCellState::CLOSED ? V : L' ');
  }
  ch = printRepresentingChar(grid[rows - 1][columns - 1]);
  stream << ch << V;
  stream << '\n';

  stream << BL;

  for (int j{0}; j < columns - 1; ++j) {
    stream << (isSpace(writtenEntryPoint, writtenExitPoint, rows - 1, j)
                   ? L"  "
                   : doubleH)
           << MB;
  }
  stream << (isSpace(writtenEntryPoint, writtenExitPoint, rows - 1, columns - 1)
                 ? L"  "
                 : doubleH)
         << BR;

  stream << '\n';
}
columns_grid_t& TrueGrid::operator[](int i) { return grid[i]; }

GridCellState& TrueGrid::operator[](CellPos pos) {
  if (pos.row < 0 || pos.row >= rows || pos.column < 0 || pos.column >= columns)
    assert(false && "invalid bounds");
  return grid[pos.row][pos.column];
}

grid_t& TrueGrid::getGrid() { return grid; }

int TrueGrid::getAdjacentWallsAndCells(CellPos pos, adjacent_walls_t& walls,
                                       adjacent_points_t& points) {
  int n{0};
  walls.fill(nullptr);
  points.fill({false, invalidPoint, GridCellState::NOTFILLED});

  if (pos.row > 0) {
    walls[n] = &wallsRows[pos.row - 1][pos.column];
    points[n].valid = true;
    points[n].pos = {pos.row - 1, pos.column};
    points[n].cellState = grid[pos.row][pos.column];
    ++n;
  }

  if (pos.column < columns - 1) {
    walls[n] = &wallsColumns[pos.row][pos.column];
    points[n].valid = true;
    points[n].pos = {pos.row, pos.column + 1};
    points[n].cellState = grid[pos.row][pos.column];
    ++n;
  }

  if (pos.row < rows - 1) {
    walls[n] = &wallsRows[pos.row][pos.column];
    points[n].valid = true;
    points[n].pos = {pos.row + 1, pos.column};
    points[n].cellState = grid[pos.row][pos.column];
    ++n;
  }

  if (pos.column > 0) {
    walls[n] = &wallsColumns[pos.row][pos.column - 1];
    points[n].valid = true;
    points[n].pos = {pos.row, pos.column - 1};
    points[n].cellState = grid[pos.row][pos.column];
    ++n;
  }

  return n;
}

int TrueGrid::getAdjacentWallsAndCellsMarked(CellPos pos,
                                             adjacent_walls_t& walls,
                                             adjacent_points_t& points) {
  int n{0};
  walls.fill(nullptr);
  points.fill({false, invalidPoint, GridCellState::NOTFILLED});

  if (pos.row > 0) {
    walls[n] = &wallsRows[pos.row - 1][pos.column];
    points[n].valid = true;
    points[n].pos = {pos.row - 1, pos.column};
    points[n].cellState = grid[pos.row][pos.column];
  }
  ++n;
  if (pos.column < columns - 1) {
    walls[n] = &wallsColumns[pos.row][pos.column];
    points[n].valid = true;
    points[n].pos = {pos.row, pos.column + 1};
    points[n].cellState = grid[pos.row][pos.column];
  }
  ++n;
  if (pos.row < rows - 1) {
    walls[n] = &wallsRows[pos.row][pos.column];
    points[n].valid = true;
    points[n].pos = {pos.row + 1, pos.column};
    points[n].cellState = grid[pos.row][pos.column];
  }
  ++n;
  if (pos.column > 0) {
    walls[n] = &wallsColumns[pos.row][pos.column - 1];
    points[n].valid = true;
    points[n].pos = {pos.row, pos.column - 1};
    points[n].cellState = grid[pos.row][pos.column];
  }
  ++n;
  return n;
}

bool TrueGrid::isEntryPoint(bool haveWrittenEntry, int row, int column) {
  return !haveWrittenEntry &&
         (enterPoint.row == row && enterPoint.column == column);
}
bool TrueGrid::isExitPoint(bool haveWrittenExit, int row, int column) {
  return !haveWrittenExit &&
         (leavePoint.row == row && leavePoint.column == column);
}

bool TrueGrid::isSpace(bool& haveWrittenEntry, bool& haveWrittenExit, int row,
                       int column) {
  if (isEntryPoint(haveWrittenEntry, row, column)) {
    haveWrittenEntry = true;
    return true;
  }

  if (isExitPoint(haveWrittenExit, row, column)) {
    haveWrittenExit = true;
    return true;
  }

  return false;
}

std::wstring TrueGrid::printRepresentingChar(GridCellState state) {
  switch (state) {
  case GridCellState::FILLED:
    return FULL_CHAR;
  case GridCellState::NOTFILLED:
    return EMPTY_CHAR;
  case GridCellState::UP_LOOKING:
    return L"^ ";
  case GridCellState::RIGHT_LOOKING:
    return L"> ";
    break;
  case GridCellState::DOWN_LOOKING:
    return L"v ";
  case GridCellState::LEFT_LOOKING:
    return L"< ";
  case GridCellState::BEEN_VISITED:
    return L"+ ";
  case GridCellState::BEEN_LEFT:
    return L"- ";
  default:
    return L"--";
  }
}

wchar_t TrueGrid::printRepresentingBinaryChar(GridCellState state) {
  switch (state) {
  case GridCellState::FILLED:
    return L'0';
  case GridCellState::NOTFILLED:
    return L'1';
  case GridCellState::UP_LOOKING:
    return L'^';
  case GridCellState::RIGHT_LOOKING:
    return L'>';
    break;
  case GridCellState::DOWN_LOOKING:
    return L'v';
  case GridCellState::LEFT_LOOKING:
    return L'<';
  case GridCellState::BEEN_VISITED:
    return L'+';
  case GridCellState::BEEN_LEFT:
    return L'-';
  default:
    return L'*';
  }
}

void TrueGrid::printTheMatrix(bool_grid_t& gridBinary, std::wostream& stream) {

  int gridRowCount(gridBinary.size());
  int gridColumnCount(gridBinary[0].size());

  for (int i{0}; i < gridRowCount; ++i) {
    for (int j{0}; j < gridColumnCount; ++j) {
      stream << (printRepresentingBinaryChar(gridBinary[i][j])) << " ";
    }
    stream << "\n";
  }
  stream << "\n";
}
void TrueGrid::printAsMatrix(std::wostream& stream) {
  bool_grid_t gridBinary;

  getAsMatrix(gridBinary);
  printTheMatrix(gridBinary, stream);
}
void TrueGrid::getAsMatrix(bool_grid_t& gridBinary) {
  int gridRowCount{2 * rows + 1};
  int gridColumnCount{2 * columns + 1};

  gridBinary.resize(gridRowCount);
  for (auto& row : gridBinary) {
    row.resize(gridColumnCount);
    std::fill(row.begin(), row.end(), GridCellState::FILLED);
  }
  CellPos tempEnter{};
  CellPos tempExit{};

  if (enterPoint.row == 0) {
    tempEnter.row = 0;
    tempEnter.column = enterPoint.column * 2 + 1;
  } else if (enterPoint.row == rows - 1) {
    tempEnter.row = gridRowCount - 1;
    tempEnter.column = enterPoint.column * 2 + 1;
  } else if (enterPoint.column == 0) {
    tempEnter.column = 0;
    tempEnter.row = enterPoint.row * 2 + 1;
  } else if (enterPoint.column == columns - 1) {
    tempEnter.column = gridColumnCount - 1;
    tempEnter.row = enterPoint.row * 2 + 1;
  } else {
    std::cout << "ouch";
    assert(false);
  }

  if (leavePoint.row == 0) {
    tempExit.row = 0;
    tempExit.column = leavePoint.column * 2 + 1;
  } else if (leavePoint.row == rows - 1) {
    tempExit.row = gridRowCount - 1;
    tempExit.column = leavePoint.column * 2 + 1;
  } else if (leavePoint.column == 0) {
    tempExit.column = 0;
    tempExit.row = leavePoint.row * 2 + 1;
  } else if (leavePoint.column == columns - 1) {
    tempExit.column = gridColumnCount - 1;
    tempExit.row = leavePoint.row * 2 + 1;
  } else {
    std::cout << "ouch";
    assert(false);
  }

  gridBinary[tempEnter.row][tempEnter.column] = GridCellState::NOTFILLED;
  gridBinary[tempExit.row][tempExit.column] = GridCellState::NOTFILLED;

  for (int i{0}; i < rows; ++i) {
    for (int j{0}; j < columns; ++j) {
      gridBinary[2 * i + 1][2 * j + 1] = grid[i][j];
    }
  }

  for (int i{0}; i < rows - 1; ++i) {
    for (int j{0}; j < columns; ++j) {
      gridBinary[2 * i + 2][2 * j + 1] =
          (wallsRows[i][j] == WallCellState::OPENED) ? GridCellState::NOTFILLED
                                                     : GridCellState::FILLED;
    }
  }

  for (int i{0}; i < rows; ++i) {
    for (int j{0}; j < columns - 1; ++j) {
      gridBinary[2 * i + 1][2 * j + 2] =
          (wallsColumns[i][j] == WallCellState::OPENED)
              ? GridCellState::NOTFILLED
              : GridCellState::FILLED;
    }
  }
}

bool TrueGrid::hasEnterAndExit() { return hasEnterOutPoint; }

walls_t& TrueGrid::getRowWalls() { return wallsRows; }
walls_t& TrueGrid::getColumnWalls() { return wallsColumns; }

CellPos TrueGrid::convertPosToBinaryPos(CellPos pos) {
  return {pos.row * 2 + 1, pos.column * 2 + 1};
}