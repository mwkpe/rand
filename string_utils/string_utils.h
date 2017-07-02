#ifndef MPXE_H
#define MPXE_H


#include <cstdint>
#include <string>
#include <string_view>
#include <locale>
#include <vector>
#include <algorithm>


// Implementation details
//
namespace mpxe::string::details
{


template<typename I> inline bool compare(I a, I b, I last)
{
  if (*a != *b)
    return false;

  while (b != last && *(++a) == *(++b));

  if (b == last)
    return true;

  return false;
}


}  // namepsace mpxe::string::details


// The following functions assume US ASCII (or single byte in some cases) encoding
//
namespace mpxe::string::ascii  
{


void to_upper(std::string& s)
{
  std::transform(std::begin(s), std::end(s), std::begin(s), ::toupper);
}


void to_lower(std::string& s)
{
  std::transform(std::begin(s), std::end(s), std::begin(s), ::tolower);
}


std::string as_upper(std::string_view sv)
{
  std::string s;
  s.reserve(sv.size());
  for (auto c : sv)
    s += ::toupper(c);
  return s;
}


std::string as_lower(std::string_view sv)
{
  std::string s;
  s.reserve(sv.size());
  for (auto c : sv)
    s += ::tolower(c);
  return s;
}


void trim(std::string& s)
{
  
}


std::string_view trimmed(std::string_view sv)
{
  return sv;
}


// Splits <sv> each <character_count> characters, skipping <skip> characters after each split
// E.g.: split(sv, 3)    : "abcdef123"   -> "abc", "def" and "123"
//       split(sv, 3, 1) : "abc,def,123" -> "abc", "def" and "123"
//
// Substantially faster (scales linear with <character_count>) for constant width splitting
// than using split(sv, ',') for "abc,def,123"
//
std::vector<std::string_view> split(std::string_view sv, std::size_t character_count,
    std::size_t skip = 0)
{
  if (character_count == 0)
    return {};

  std::vector<std::string_view> v;
  auto size = sv.size();
  std::size_t i = 0;
  while (i < size) {
    v.push_back(sv.substr(i, character_count));
    i += character_count + skip;
  }

  return v;
}


}  // namespace mpxe::string::ascii


namespace mpxe::string
{


bool starts_with(std::string_view sv, std::string_view test)
{
  if (sv.empty() || test.empty() || test.size() > sv.size())
    return false;
  return details::compare(std::begin(sv), std::begin(test), std::end(test));
}


bool ends_with(std::string_view sv, std::string_view test)
{
  if (sv.empty() || test.empty() || test.size() > sv.size())
    return false;
  return details::compare(std::rbegin(sv), std::rbegin(test), std::rend(test));
}


std::vector<std::string_view> split_keep_empty(std::string_view sv, char token)
{
  std::size_t start = 0;
  auto i = sv.find(token);
  std::vector<std::string_view> parts;

  while (i != sv.npos) {
    parts.push_back(sv.substr(start, i - start));
    start = i + 1;
    i = sv.find(token, start);
  }
  parts.push_back(sv.substr(start));

  return parts;
}


std::vector<std::string_view> split_ignore_empty(std::string_view sv, char token)
{
  std::size_t start = 0;
  auto i = sv.find(token);
  std::vector<std::string_view> parts;

  while (i != sv.npos) {
    if (auto len = i - start; len > 0)
      parts.push_back(sv.substr(start, len));
    start = i + 1;
    i = sv.find(token, start);
  }
  if (sv.size() - start > 0)
    parts.push_back(sv.substr(start));

  return parts;
}


std::vector<std::string_view> split(std::string_view sv, char token, bool keep_empty_parts = true)
{
  if (keep_empty_parts)
    return split_keep_empty(sv, token);
  return split_ignore_empty(sv, token);
}


std::tuple<std::string_view, std::string_view> split_first(std::string_view sv, char token)
{
  if (auto i = sv.find(token); i != sv.npos) {
    return {sv.substr(0, i), sv.substr(i+1)};
  }

  return {sv, {}};
}


}  // namespace mpxe::string


#endif  // MPXE_H
