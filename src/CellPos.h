#pragma once

struct CellPos {
  int row;
  int column;

  CellPos& operator=(const CellPos& pos) {
    row = pos.row;
    column = pos.column;
    return *this;
  }

  bool operator==(const CellPos& pos) const {
    return (row == pos.row && column == pos.column);
  }

  bool operator!=(const CellPos& pos) const {
    return (row != pos.row || column != pos.column);
  }
  // Hmm, const, ...
  CellPos operator+(const CellPos& pos) const {
    return CellPos{ row + pos.row, column + pos.column };
  }

  // Hmm, const, ...
  CellPos operator-(const CellPos& pos) const {
    return CellPos{ row - pos.row, column - pos.column };
  }

};

