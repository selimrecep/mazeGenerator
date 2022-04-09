include_directories(${mazeSolverRobotProject_SOURCE_DIR}/mazeGenerator/src/)
add_library(mazeGenerator TrueGrid.cpp random.cpp Maze2.cpp)