#pragma once
#include "CellPos.h"
#include <array>
#include <cassert>
#include <iostream>

/*
I got a suggestion about I shouldn't use std::array here because no real benefit
in this use case. template<size_t M> using columns_t = std::array<bool, M>;
template<size_t N, size_t M>
using grid_t = std::array<columns_t<M>, N>;*/

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

constexpr wchar_t FULL_CHAR[] = L"X ";
constexpr wchar_t EMPTY_CHAR[] = L"  ";
template <size_t N, size_t M> using grid_t = bool[N][M];

template <size_t N, size_t M> class Table {
private:
  grid_t<N, M> grid{};
  int rows;
  int columns;

public:
  Table() : rows(N), columns(M) {}
  Table(bool defaultValue) {
    for (size_t i{0}; i < N; ++i)
      for (size_t j{0}; j < M; ++j)
        grid[i][j] = defaultValue;
  }
  void printTable(std::wostream &stream = std::wcout) {
    stream << TL;
    for (size_t i{0}; i < M - 1; ++i) {
      stream << H << H << MT;
    }
    stream << H << H << TR;
    stream << '\n';

    std::wstring ch{};
    for (size_t i{0}; i < N - 1; ++i) {
      stream << V;
      for (size_t j{0}; j < M - 1; ++j) {
        ch = (!grid[i][j] ? FULL_CHAR : EMPTY_CHAR);
        stream << ch << V;
      }
      ch = (!grid[i][M - 1] ? FULL_CHAR : EMPTY_CHAR);
      stream << ch << V;
      stream << '\n';
      stream << ML;
      for (size_t j{0}; j < M - 1; ++j)
        stream << H << H << MC;
      stream << H << H << MR;
      stream << '\n';
    }

    stream << V;
    for (size_t j{0}; j < M - 1; ++j) {
      ch = (!grid[N - 1][j] ? FULL_CHAR : EMPTY_CHAR);
      stream << ch << V;
    }
    ch = (!grid[N - 1][M - 1] ? FULL_CHAR : EMPTY_CHAR);
    stream << ch << V;
    stream << '\n';

    stream << BL;

    for (size_t j{0}; j < M - 1; ++j) {
      stream << H << H << MB;
    }
    stream << H << H << BR;

    stream << '\n';
  }
  bool *operator[](int i) { return grid[i]; }

  bool &operator[](CellPos pos) {
    if (pos.row < 0 || pos.row >= rows || pos.column < 0 ||
        pos.column >= columns)
      assert(false && "invalid bounds");
    return grid[pos.row][pos.column];
  }

  grid_t<N, M> &getGrid() { return grid; }
};