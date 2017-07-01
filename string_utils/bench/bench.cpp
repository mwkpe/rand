#include "../string_utils.h"
#include "hayai/hayai.hpp"


BENCHMARK(string, split, 1000, 100000)
{
  mpxe::string::split("hello, world!,,,1,2,abcdefghijklmnopqrstuvwxyz,3,test,19293439,10.24", ',');
}
 

int main()
{ 
  hayai::ConsoleOutputter o;
  hayai::Benchmarker::AddOutputter(o);
  hayai::Benchmarker::RunAllTests();

  return 0;
}
