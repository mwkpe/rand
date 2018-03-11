#include <cstdint>
#include <tuple>
#include <vector>
#include <string>
#include <string_view>
#include <regex>
#include <cmath>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>

#include "cxxopts.hpp"
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include "nlohmann/json.hpp"


namespace fsys = std::experimental::filesystem;
using namespace fmt::literals;
using json = nlohmann::json;


namespace {


std::tuple<std::string, int, float> parse_args(int argc, char** argv)
{
  std::string filename;
  int offset;
  float framerate;
  try {
    cxxopts::Options options{"assconv", "ASS to JSON converter"};
    options.add_options()
        ("i,input", "input file", cxxopts::value<std::string>(filename))
        ("o,offset", "time offset in milliseconds", cxxopts::value<int>(offset)->default_value("0"))
        ("f,framerate", "framerate", cxxopts::value<float>(framerate)->default_value("23.975"))
        ;
    auto result = options.parse(argc, argv);
    if (result.count("input") == 0) {
      throw std::runtime_error{"File must be specified, use the -f or --file"};
    }
    return {filename, offset, framerate};
  }
  catch (const cxxopts::OptionException& e) {
    throw std::runtime_error{e.what()};
  }
}


std::vector<std::string> split(std::string_view sv, char token)
{
  std::size_t start = 0;
  auto i = sv.find(token);
  std::vector<std::string> parts;

  while (i != sv.npos) {
    parts.emplace_back(sv.substr(start, i - start));
    start = i + 1;
    i = sv.find(token, start);
  }
  parts.emplace_back(sv.substr(start));

  return parts;
}


std::uint64_t as_milliseconds(int hours, int minutes, int seconds, int milliseconds)
{
  return static_cast<std::uint64_t>(milliseconds) + seconds * 1000 +
      minutes * 60 * 1000 + hours * 60 * 60 * 1000;
}


std::tuple<int, int, int, int> as_clocktime(int milliseconds)
{
  int hours = milliseconds / (1000 * 60 * 60) % 24;
  int minutes = milliseconds / (1000 * 60) % 60;
  int seconds = milliseconds / 1000 % 60;
  milliseconds = milliseconds % 1000;
  return {hours, minutes, seconds, milliseconds};
}


std::string as_frametime(int hours, int minutes, int seconds, int milliseconds, int offset, float framerate)
{
  if (offset != 0) {
    auto ms = as_milliseconds(hours, minutes, seconds, milliseconds) + offset;
    std::tie(hours, minutes, seconds, milliseconds) = as_clocktime(ms);
  }
  
  // Round to closest frame
  auto frame = static_cast<int>(std::round(milliseconds * 0.001f * framerate));
  return "{:02}:{:02}:{:02}:{:02}"_format(hours, minutes, seconds, frame);
}


json build_json(std::vector<std::string>&& parts, int hours, int minutes, int seconds,
    int milliseconds, int offset, float framerate)
{
  json j;
  j["japanese"] = "";
  j["hiragana"] = "";
  j["romaji"] = "";
  j["english"] = "";
  switch (parts.size()) {
    case 4: j["english"] = std::move(parts[3]);
    [[fallthrough]];
    case 3: j["romaji"] = std::move(parts[2]);
    [[fallthrough]];
    case 2: j["hiragana"] = std::move(parts[1]);
    [[fallthrough]];
    case 1: j["japanese"] = std::move(parts[0]);
  }
  j["timestamp"] = as_milliseconds(hours, minutes, seconds, milliseconds) + offset;
  j["timecode"] = as_frametime(hours, minutes, seconds, milliseconds, offset, framerate);
  return j;
}


}  // namespace


int main(int argc, char** argv)
{
  std::string filename;
  int offset;
  float framerate;
  try {
    std::tie(filename, offset, framerate) = parse_args(argc, argv);
  }
  catch (const std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    return 0;
  }

  if (std::ifstream ass_file{filename}; ass_file.is_open()) {
    std::string line;
    // Dialogue: 0,0:00:01.31,0:00:01.68,Default,,0,0,0,,話;はなし;hanashi;story
    std::regex rx{R"(^\s*Dialogue:\s+\d+,(\d+):(\d+):(\d+).(\d+),.+,(.+)$)"};
    std::smatch m;
    json transcript;
    while (getline(ass_file, line)) {
      if (std::regex_match(line, m, rx)) {
        int hours = std::stoi(m[1]);
        int minutes = std::stoi(m[2]);
        int seconds = std::stoi(m[3]);
        int milliseconds = std::stoi(m[4]);
        transcript.push_back(build_json(split(m[5].str(), ';'), hours, minutes, seconds,
            milliseconds * 10, offset, framerate));
      }
    }

    filename = fsys::path{filename}.stem().string() + ".json";
    if (std::ofstream fs{filename, std::ios::binary}; fs.is_open()) {
      fs << transcript.dump(2);
    }
  }

  return 0;
}
