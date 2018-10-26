#include <string>
#include <vector>
#include <set>
#include <random>
#include <iostream>


struct Cell
{
  std::int32_t index = -1;
  std::int32_t row = -1;
  std::int32_t column = -1;
  Cell* north = nullptr;
  Cell* south = nullptr;
  Cell* east = nullptr;
  Cell* west = nullptr;
  std::set<Cell> links;
};


void link(Cell& a, Cell& b)
{
  a.links.insert(b);
  b.links.insert(a);
}


void unlink(Cell& a, Cell& b)
{
  a.links.erase(b);
  b.links.erase(a);
}


auto neighbours(Cell& cell)
{
  std::vector<Cell*> cells;

  if (cell.north)
    cells.push_back(cell.north);
  if (cell.south)
    cells.push_back(cell.south);
  if (cell.east)
    cells.push_back(cell.east);
  if (cell.west)
    cells.push_back(cell.west);

  return cells;
}


class Grid
{
public:
  Grid(std::int32_t rows, std::int32_t columns) : rows_{rows}, columns_{columns}
  {
    cells_ = std::vector<Cell>(rows_ * columns_, Cell{});
    int index = 0;
    for (auto& cell : cells_) {
      cell.index = index++;
      cell.row = cell.index / columns_;
      cell.column = cell.index % columns_;
      cell.north = at(cell.row - 1, cell.column);
      cell.south = at(cell.row + 1, cell.column);
      cell.east = at(cell.row, cell.column + 1);
      cell.west = at(cell.row, cell.column - 1);
    }
  }

  Cell* at(std::int32_t row, std::int32_t column)
  {
    if (row >= 0 && row < rows_ && column >= 0 && column < columns_)
      return &cells_[row * columns_ + column];
    return nullptr;
  }

  void apply_binary_tree(std::int32_t seed)
  {
    std::mt19937 rng{seed};
    std::uniform_int_distribution<std::int32_t> dist{0, 1};
    for (auto& cell : cells_) {
      if (cell.north && cell.east) {
        link(cell, dist(rng) == 0 ? *cell.north : *cell.east);
      }
      else if (cell.north) {
        link(cell, *cell.north);
      }
      else if (cell.east) {
        link(cell, *cell.east);
      }
    }
  }

  std::vector<Cell>::const_iterator begin() const { return std::cbegin(cells_); }
  std::vector<Cell>::const_iterator end() const { return std::cend(cells_); }
  auto rows() const { return rows_; }
  auto columns() const { return columns_; }

private:
  std::int32_t rows_ = -1;
  std::int32_t columns_ = -1;
  std::vector<Cell> cells_;
};


bool operator<(const Cell& lhs, const Cell& rhs)
{
  return lhs.index < rhs.index;
}


std::ostream& operator<<(std::ostream& os, const Grid& grid)
{
  os << "+";
  for (int i=0; i<grid.columns(); ++i)
    os << "---+";
  os << "\n";
  std::int32_t current_row = 0;
  std::string center = "|";
  std::string bottom = "+";
  for (const auto& cell : grid) {
    if (cell.row != current_row) {
      os << center << "\n";
      os << bottom << "\n";
      current_row = cell.row;
      center = "|";
      bottom = "+";
    }
    center += "   ";
    center += cell.east && cell.links.count(*cell.east) == 1 ? " " : "|";
    bottom += cell.south && cell.links.count(*cell.south) == 1 ? "   " : "---";
    bottom += "+";
  }

  os << center << "\n";
  os << bottom << "\n";
  return os;
}


int main()
{
  Grid grid{8, 8};
  grid.apply_binary_tree(0x931dead3);
  std::cout << grid;
}
