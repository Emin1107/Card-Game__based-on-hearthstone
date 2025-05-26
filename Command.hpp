#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <string>
#include <vector>

enum class CommandType
{
  HELP,
  GRAVEYARD,
  BOARD,
  HAND,
  INFO,
  REDRAW,
  STATUS,
  DONE,
  QUIT,
  BATTLE,
  CREATURE,
  SPELL,
  INVALID,
  WRONG_PARAM
};

class Command
{
protected:
  CommandType type_;
  std::vector<std::string> parameters_;

public:
  // Forward declarations
  Command(std::vector<std::string>& input);

  Command(CommandType type);

  Command(const Command& other) = default;
  ~Command() = default;

  bool isQuit() const;
  bool isDone() const;
  CommandType getType() const;
  std::vector<std::string>& getParameters();
  void setType(CommandType type);
};

#endif