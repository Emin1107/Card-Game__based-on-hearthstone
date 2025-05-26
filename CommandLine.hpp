#ifndef COMMANDLINE_HPP
#define COMMANDLINE_HPP

#include <string>
#include <map>
#include "Command.hpp"

class CommandLine
{
public:
  // Forward declarations
  CommandLine() = default;
  CommandLine(const CommandLine&) = delete;
  ~CommandLine() = default;

  Command readCommand(int player);

private:
  void removeTrailingWhitespaces(std::string &string);
  void removeLeadingWhitespace(std::string &string);
  void removeWhitespacesAtEnds(std::string &string);
  void stringToVector(const std::string &string, std::vector<std::string> &vector, char delimiter);
};

#endif
