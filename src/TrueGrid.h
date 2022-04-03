#pragma once
#include "CellPos.h"
#include <array>
#include <cassert>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

constexpr wchar_t TL = L'┌';
constexpr wchar_t TR = L'┐';
constexpr wchar_t BL = L'└';
constexpr wchar_t BR = L'┘';
constexpr wchar_t ML = L'├';
constexpr wchar_t MR = L'┤';
constexpr wchar_t MT = L'┬';
constexpr wchar_t MB = L'┴';
constexpr wchar_t MC = L'┼';
constexpr wchar_t V = L'│';
constexpr wchar_t H = L'─';

constexpr wchar_t doubleH[] = L"──";

constexpr wchar_t FULL_CHAR[] = L"X ";
constexpr wchar_t EMPTY_CHAR[] = L"  ";

constexpr CellPos invalidPoint{-1, -1};

template <typename T> using ptr_t = T*;

enum class GridCellState { NOTFILLED, FILLED };

enum class HasVisited { NOT_VISITED, HAS_VISITED };
enum class WallType { top, right, bottom, left };

enum class WallCellState { CLOSED, OPENED, INVALID };

struct CellInfo {
  bool valid{false};
  CellPos pos;
  GridCellState cellState;
};

// std::vector<bool> is a joke
// enum Bool { bfalse, btrue };

using bool_grid_t = std::vector<std::vector<int>>;

using columns_wall_t = std::vector<WallCellState>;
using walls_t = std::vector<columns_wall_t>;

using columns_grid_t = std::vector<GridCellState>;
using grid_t = std::vector<columns_grid_t>;

using hasvisited_t = std::vector<std::vector<HasVisited>>;
using adjacent_walls_t = std::array<ptr_t<WallCellState>, 4>;
using adjacent_points_t = std::array<CellInfo, 4>;

class TrueGrid {
private:
  grid_t grid{};
  walls_t wallsRows{};
  walls_t wallsColumns{};
  int wallRowsCount{0};
  int wallColumnsCount{0};

  bool hasEnterOutPoint{false};

  CellPos enterPoint{};
  CellPos leavePoint{};

  void resizeGrid(int n, int m);
  bool isEntryPoint(bool haveWrittenEntry, int row, int column);
  bool isExitPoint(bool haveWrittenExit, int row, int column);

  bool isSpace(bool& haveWrittenEntry, bool& haveWrittenExit, int row,
               int column);

public:
  int rows;
  int columns;
  hasvisited_t hasVisited{false};

  TrueGrid(int n, int m);
  TrueGrid(int n, int m, GridCellState defaultValue);
  TrueGrid(int n, int m, GridCellState defaultValue, CellPos enterPoint,
           CellPos leavePoint);
  void printTable(std::wostream& stream = std::wcout);
  void printAsMatrix(std::wostream& stream = std::wcout);
  columns_grid_t& operator[](int i);

  GridCellState& operator[](CellPos pos);

  grid_t& getGrid();

  int getAdjacentWallsAndCells(CellPos pos, adjacent_walls_t& walls,
                               adjacent_points_t& points);
  void visitedPoint(CellPos pos);
  bool hasVisitedPoint(CellPos pos);
};