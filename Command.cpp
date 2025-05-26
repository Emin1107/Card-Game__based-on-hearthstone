#include <iostream>
#include <string>
#include <vector>

#include "Command.hpp"

//---------------------------------------------------------------------------------------------------------------------
///
/// Creates a command from the given input and adds the parameters
///
/// @param input command line input
///
/// @return nothing
Command::Command(std::vector<std::string>& input)
{
  std::string check = input[0];

  for (auto &letter : check)
  {
    letter = toupper(letter);
  }

  if (check == "HELP")
    type_ = CommandType::HELP;
  else if (check == "GRAVEYARD")
    type_ = CommandType::GRAVEYARD;
  else if (check == "BOARD")
    type_ = CommandType::BOARD;
  else if (check == "HAND")
    type_ = CommandType::HAND;
  else if (check == "INFO")
    type_ = CommandType::INFO;
  else if (check == "REDRAW")
    type_ = CommandType::REDRAW;
  else if (check == "STATUS")
    type_ = CommandType::STATUS;
  else if (check == "DONE")
    type_ = CommandType::DONE;
  else if (check == "QUIT")
    type_ = CommandType::QUIT;
  else if (check == "BATTLE")
    type_ = CommandType::BATTLE;
  else if (check == "CREATURE")
    type_ = CommandType::CREATURE;
  else if (check == "SPELL")
    type_ = CommandType::SPELL;
  else
  {
    type_ = CommandType::INVALID;
    return;
  }

  if (input.size() > 1)
  {
    for (unsigned long position = 1; position < input.size(); position++)
    {
      parameters_.push_back(input[position]);
    }
  }
}

Command::Command(CommandType type) : type_(type) {};

bool Command::isQuit() const { return type_ == CommandType::QUIT; }

bool Command::isDone() const { return type_ == CommandType::DONE; }

CommandType Command::getType() const { return type_; }

std::vector<std::string> &Command::getParameters() { return parameters_; }

void Command::setType(CommandType type) { type_ = type; }
