#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>

class GameEx : public std::exception
{
protected:
  std::string message_;
public:
  // Forward declarations
  explicit GameEx(const std::string& message) : message_(message) {}
  const char* what() const noexcept override
  {
    return message_.c_str();
  }
};

//-----------------------------------------------------------------------------------------------------
///
/// File error exception
///
/// @param file_path path to the file
///
/// @return nothing
class file_error : public GameEx
{
public:
  explicit file_error(const std::string& file_path) :
    GameEx("[ERROR] Invalid file (" + file_path + ").") {}
};

//-----------------------------------------------------------------------------------------------------
///
/// Memory error exception
///
/// @return nothing
class MemoryEx : public GameEx
{
public:
  MemoryEx() : GameEx("") {}
};


#endif
