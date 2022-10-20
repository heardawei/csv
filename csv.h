#ifndef CSV_H
#define CSV_H

#include <fstream>
#include <string>
#include <type_traits>
#include <vector>

#include "stringutils.h"

namespace completion_tool
{
class CSV
{
 public:
  using cell_type = std::string;
  using size_type = std::size_t;
  static constexpr size_type npos = (size_type)(-1);
  using row_type = std::vector<cell_type>;
  using col_type = std::vector<cell_type>;
  using iterator = std::vector<row_type>::iterator;
  using const_iterator = std::vector<row_type>::const_iterator;

  inline operator bool() const { return cells_.size(); }
  inline bool operator!() const { return cells_.empty(); }
  inline iterator begin() { return cells_.begin(); }
  inline const_iterator cbegin() const { return cells_.cbegin(); }
  inline iterator end() { return cells_.end(); }
  inline const_iterator cend() const { return cells_.cend(); }
  inline CSV &operator<<(const std::string &line)
  {
    cells_.push_back(utils::StringUtils::split(line, ','));
    return *this;
  }
  inline CSV &operator<<(const row_type &line)
  {
    cells_.push_back(line);
    return *this;
  }
  inline row_type &operator[](size_type i) { return cells_[i]; }
  const row_type &operator[](size_type i) const { return cells_[i]; }
  inline size_type Rows() const { return cells_.size(); }
  inline size_type Columns() const { return Headers().size(); }
  inline row_type &Row(size_type i) { return cells_[i]; }
  inline const row_type &Row(size_type i) const { return cells_[i]; }
  col_type Column(size_type i) const
  {
    col_type cols;
    std::transform(
        cbegin(), cend(), std::back_inserter(cols), [i](const row_type &row) {
          return row[i];
        });
    return cols;
  }
  template <
      typename T,
      typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
  inline T Cell(size_type row, size_type column) const
  {
    return utils::StringUtils::parse<T>(cells_[row][column]);
  }
  inline cell_type &Cell(size_type row, size_type column)
  {
    return cells_[row][column];
  }
  inline const cell_type &Cell(size_type row, size_type column) const
  {
    return cells_[row][column];
  }
  inline row_type &Headers() { return cells_[0]; }
  inline const row_type &Headers() const { return cells_[0]; }
  inline cell_type &Header(size_type i) { return Headers()[i]; }
  inline size_type Header(const std::string &name) const
  {
    const auto &hdrs = Headers();
    auto itr = std::find(hdrs.cbegin(), hdrs.cend(), name);
    return (itr == hdrs.cend()) ? npos : std::distance(hdrs.cbegin(), itr);
  }
  inline bool HasHeader(const std::string &name) const
  {
    return Header(name) != npos;
  }
  inline const cell_type &Header(size_type i) const { return Headers()[i]; }
  inline void PushBack(const row_type &row) { cells_.push_back(row); }
  static CSV ParseFile(const std::string &filename);
  std::string Dump() const;

 private:
  std::vector<row_type> cells_;
};

}  // namespace completion_tool

#endif  // CSV_H