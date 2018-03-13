<h1>C++ Programming and Style Guidelines</h1>

# 1. References
These guidelines are loosely based on the [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md), [Bjarne Stroustrup's C++ Style and Technique FAQ](http://www.stroustrup.com/bs_faq2.html) and the [PPP Style Guide](http://www.stroustrup.com/Programming/PPP-style.pdf) (PDF). These documents, especially the Core Guidelines, are recommended reading and should be considered for anything not specifically mentioned here.

# 2. Naming conventions
## 2.1 General
* Don't use `camelCase` or `PascalCase`. Use `snake_case` as done in Standard C++ and the Standard Library.
* Don't add type information to names, e.g. Hungarian notation `f_velocity`. [<sub><sup>*(NL.5)*</sup></sub>](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#nl5-avoid-encoding-type-information-in-names)
* Don't use a leading underscore `_abc` or double underscores `ab__c`. These are generally reserved for library implementers or compiler vendors. <sub><sup>*(N4659 5.10.3)*</sup></sub>
* Avoid using unnecessarily or excessively long names.
```C++
int remaining_free_slots_in_symbol_table;  // Please no
int free_table_slots;  // Better
```

## 2.2 Variables
* Variable names should be written using only lower case characters, numbers and underscores to separate words.
```C++
int received_bytes;  // Yes
int receivedBytes;  // No
int ReCe1ved_ByT3s;  // You're fired!

string html_header;  // This also applies to acronyms
string HTML_header;  // No
```
* Variable names should clearly reflect the content of the variable.
* Don't remove vowels or needlessly shorten words, e.g. `rcvd_bytes`.
* Class member variables should be suffixed with an underscore.
```C++
class Vehicle
{
  ...
  float velocity_;
  float heading_;
};
```
* The length of a name should be roughly proportional to the size of its scope, i.e. use `n` instead of `received_bytes` for short-lived stack variables, especially if content can be inferred by context. [<sub><sup>*(NL.7)*</sup></sub>](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#nl7-make-the-length-of-a-name-roughly-proportional-to-the-length-of-its-scope)
```C++
if (receiver.wait_for_data()) {
  int n = receiver.bytes_available();
  if (n > 512)
    process_bytes(buffer, n);
}
```

## 2.3 Functions
Function names should be written using only lower case characters, numbers and underscores to separate words.
```C++
void send_message();  // Yes
void sendmessage();  // No
void sendMessage();  // No
```

## 2.4 Types
User defined types should start with a single capital letter followed by only lower case characters, numbers and underscores to separate words (Stroustrup style).
```C++
// Yes
class Fusion_object;
{
  ...
};

// No
class FusionObject;
{
  ...
};
```

## 2.5 Constants
* Don't capitalize all constants, e.g. `MAX_ITERATIONS`. Exceptions are preprocessor defines, global constants and plain `enum`.
* Prefer using `enum class` since these don't leak names to the surrounding scope.
```C++
enum COLOR { RED, GREEN, BLUE };  // OK
enum class Color { Red, Green, Blue };  // Better
```

## 2.6 Library usage
The rules in this guide should be followed independent of the frameworks and libraries used, i.e. do not adapt, or partly adapt, to the conventions of a specific library. User code should be kept consistent.
```C++
class TreeModel : public QAbstractItemModel  // No
class Tree_model : public QAbstractItemModel  // Yes

class file_error : public std::runtime_error  // No
class File_error : public std::runtime_error  // Yes
```

## 2.7 Files
* Header files should use `.h` and source files `.cpp`.
* File names should be written only using lower case characters, numbers and underscores to separate words.
* Files definining types should have the same name as the type.

Example `fusion_object.h`
```C++
class Fusion_object
{
  ...
};
```

# 3. Style conventions
## 3.1 Indentation
* Tabs should be automatically replaced with spaces.
* One tab should be two spaces wide.
```C++
// Yes
if (x < 0) {
  x = 0;
}

// No
if (x < 0) {
    x = 0;
}
```

## 3.2 Line length
* Lines should be around 100 characters long and not exceed 120 characters.
* Long lines should be continued on a new line with 4 spaces indentation with respect to the first line, i.e. any additional lines should not be indented further.
```C++
// Yes
auto time = to_timestamp(int hours, int minutes, int seconds, int milliseconds,
    int microseconds, int nanoseconds, int picoseconds);

// Yes
auto time = to_timestamp(int hours, int minutes, int seconds, int milliseconds,
    int microseconds, int nanoseconds, int picoseconds, int femtoseconds,
    int attoseconds, int zeptoseconds, int yoctoseconds);

// No
auto time = to_timestamp(int hours, int minutes, int seconds, int milliseconds,
                         int microseconds, int nanoseconds, int picoseconds);  
```

## 3.3 Functions
* The opening brace should be put on a new line (Allman style).
* Parameters should be separated by a single space after the comma.
* Indent contents
```C++
void run(int mode, float delta_time)
{
  int x;
}
```

## 3.4 Types
* The opening brace should be put on a new line (Allman style).
* Access specifiers should not be indented (Stroustrup style).
* Indent contents
```C++
class Vehicle
{
public:
  void start();

private:
  float velocity_;
};
```

## 3.5 Conditionals
* The opening brace should be put at the end of the line (K&R style, Stroustrup variant).
* Put a space after the keyword and before the opening brace.
* Don't put spaces after the opening bracket or before the closing bracket.
* Indent contents
```C++
// Yes
if (i < 10) {
  ...
}
else {
  ...
}

// No
if( i < 10 )
{
  ...
}
```
* Switch cases and contents within cases should be indented.
* Use braces when creating local variables not needed in subsequent cases.
* Denote intentional fallthrough with a comment or the C++17 `[[fallthrough]]` attribute.
```C++
// Yes
switch (condition) {
  case 0:
      parse_signal(std::string_view s);
    break;
  case 1: {
    int a = 10;
  } break;
  case 2:
    // Fallthrough or...
    [[fallthrough]];  // Starting with C++17
  case 3:
    break;
  default:
    break;
}
```

## 3.6 Const notation
* The const qualifier should be put before the type. [<sub><sup>*(NL.26)*</sup></sub>](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#nl26-use-conventional-const-notation)

```C++
const int i = 3;  // Yes
int const i = 3;  // No
```

## 3.7 Getter & setter
* Getter should not be prefixed and setter should be prefixed with `set_`.
```C++
class Vehicle
{
public:
  float velocity() const { return velocity_; };
  void set_velocity(float v) { velocity_ = v; };

private:
  float velocity_;
}
```

## 3.8 Comments
* Don't use C-style comments. `/* Comment */`
* Comments following code should be separated by 2 spaces.
```C++
int apples;  // Number of bananas
```

## 3.9 Whitespace
* Don't paint "pretty" pictures with whitespace.
```C++
// No
int i         = 0;
std::string s = "Maintaining pointless alignment isn't fun";
double d      = 13.37;

// Yes
int i = 0;
std::string s = "Each line is an individual, yay";
double d = 13.37;

// No, don't build staircases
auto time = to_timestamp(hours,
                         minutes,
                         seconds,
                         milliseconds,
                         to_microseconds(microseconds,
                                         nanoseconds,
                                         picoseconds));

// Yes
auto time = to_timestamp(hours, minutes, seconds, milliseconds,
    to_microseconds(microseconds, nanoseconds, picoseconds));
```
* Exception: Vector and matrix initialization
```C++
Matrix3f m;
m << 1,  2,  3,
     4,  5,  6,
     9, 10, 11;
```

# 4. Programming conventions
## 4.1 Header includes
* Explicitly include all headers you use.
* Prefer including headers in the source file instead of the header file.
* Prefer forward declaration whenever possible.
* Always include the C++ version, e.g. include `<cmath>` and not `<math.h>`
* Include in the following order:
    1. Current source file's header
    2. C libraries
    3. C++ Standard libraries
    4. C++ libraries
    5. Project header files

Example `vehicle.cpp`
```C++
#include "vehicle.h"

#include <stdio.h>
#extern "C" {
  #include "non-std-c-header.h"
}

#include <tuple>
#include <vector>

#include <fmt/format.h>
#include "pugixml.hpp"

#include "header_from_your_project.h"
```

## 4.2 Include guards
* Add include guards to each header file.
* The name must be the same as the file name and be written using only capital letters.
```C++
#ifndef CAN_SOCKET_H
#define CAN_SOCKET_H
   ...
#endif  // CAN_SOCKET_H
```

##  4.3 Preprocessor directives
* Avoid macro definitions and prefer proper C++ constants and constexpr expressions whenever possible.
```C++
#define PI 3.14159  // No
const float PI = 3.14159f;  // Yes
```
* Always write macros using only capital letters and numbers.

## 4.4 Namespaces
* Never import a namespace in a header file or at global scope.
* Always explicitly qualifiy the std namespace, e.g. `std::vector`. This applies to all types and functions included from C++ headers, e.g. `std::memcpy`, `std::abs` or `std::uint64_t`.
* Prefer declaring namespaces aliases in source files.
```C++
using namespace std;  // No, never
using namespace asio::ip::udp;  // Allowed at function level
namespace fsys = std::experimental::filesystem;  // Allowed in source files
```
* Put related classes into a common namespace.

## 4.6 Const correctness
* Pointers and references should be const whenever possible.
* Always mark a member function `const` when it doesn't modify any member variables.
```C++
class Vehicle
{
public:
  float velocity() const { return velocity_; };

private:
  float velocity_;
}
```

## 4.5 Fixed-width integers
* Use the integer types defined in `<cstdint>` when a fixed-width integer is necessary.
```C++
std::uint8_t crc;  // Yes
uint8_t crc;  // No
unsigned char crc;  // No
```

## 4.6 Casting
Don't use C-style casts.
```C++
auto x = (int)x_position;  // No
auto x = static_cast<int>(x_position);  // Yes
```