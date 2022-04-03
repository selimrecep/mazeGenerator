#include <codecvt>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <locale>
#include <string>

#include "CellPos.h"
//#include "Maze.h"
//#include "Table.h"

#include "Maze2.h"
#include "TrueGrid.h"

bool getNumbers(int& rows, int& columns, CellPos& entryPoint,
                CellPos& exitPoint) {
  // std::cout doesn't work for some reason when using wcout
  // could be that I am using that magic "_setmode" function in main.
  std::wcout << "Enter height of the maze: ";
  std::cin >> rows;
  if (rows < 1)
    return false;
  std::wcout << "Enter width of the maze: ";
  std::cin >> columns;
  if (columns < 1)
    return false;

  std::wcout << "Enter entry point's row coordinate(based on 1 not zero): ";
  std::cin >> entryPoint.row;
  --entryPoint.row;
  if (entryPoint.row < 0 || entryPoint.row > rows - 1)
    return false;

  std::wcout << "Enter entry point's column coordinate(based on 1 not zero): ";
  std::cin >> entryPoint.column;
  --entryPoint.column;
  if (entryPoint.column < 0 || entryPoint.column > columns - 1)
    return false;

  std::wcout << "Enter exit point's row coordinate(based on 1 not zero): ";
  std::cin >> exitPoint.row;
  --exitPoint.row;
  if (exitPoint.row < 0 || exitPoint.row > rows - 1)
    return false;

  std::wcout << "Enter exit point's column coordinate(based on 1 not zero): ";
  std::cin >> exitPoint.column;
  --exitPoint.column;
  if (exitPoint.column < 0 || exitPoint.column > columns - 1)
    return false;
  return true;
}
int main() {
  _setmode(_fileno(stdout), _O_U16TEXT);

  int rows{}, columns{};
  CellPos entryPoint{}, exitPoint{};
  bool flag{};
  do {
    flag = getNumbers(rows, columns, entryPoint, exitPoint);
    if (!flag) {
      std::cout << "You've entered an invalid value.\n";
    }
  } while (!flag);

  Maze2 maze{rows, columns, entryPoint, exitPoint};

  maze.printMaze();
  maze.generate();
  maze.printMaze();
  maze.printAsMatrix();

  std::wcout << "\n";
  char ch{};
  std::cin >> ch;
}