#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Command.hpp"
#include "CommandLine.hpp"
#include "Game.hpp"

void CommandLine::removeTrailingWhitespaces(std::string &string)
{
  size_t end = string.find_last_not_of(' ');
  string = end == std::string::npos ? "" : string.substr(0, end + 1);
}

void CommandLine::removeLeadingWhitespace(std::string &string)
{
  auto start = string.find_first_not_of(' ');
  string = start == std::string::npos ? "" : string.substr(start);
}

void CommandLine::removeWhitespacesAtEnds(std::string &string)
{
  removeLeadingWhitespace(string);
  removeTrailingWhitespaces(string);
}

void CommandLine::stringToVector(const std::string &string,
                                 std::vector<std::string> &vector, char delimiter)
{
  size_t pos = 0;
  size_t pos2;
  std::string split_string;
  while ((pos2 = string.find(delimiter, pos)) != std::string::npos)
  {
    split_string = string.substr(pos, pos2 - pos);
    pos = pos2 + 1;
    removeWhitespacesAtEnds(split_string);

    if (!split_string.empty())
    {
      vector.push_back(split_string);
    }
  }
  vector.push_back(string.substr(pos));
}

//-----------------------------------------------------------------------------------------------------
///
/// Reads a command from the command line
///
/// @param player player number
/// 
/// @return command object
Command CommandLine::readCommand(int player)
{
  std::cout << std::endl
            << "P" << player << "> ";
  std::string input;
  if (!std::getline(std::cin, input))
    return Command(CommandType::QUIT);
  removeWhitespacesAtEnds(input);
  std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c)
                 { return std::tolower(c); });

  if (input.empty())
    return Command(CommandType::INVALID);

  std::vector<std::string> vector;
  stringToVector(input, vector, ' ');
  Command command(vector);
  switch (command.getType())
  {
  case CommandType::QUIT:
    if (!command.getParameters().empty())
    {
      command.setType(CommandType::WRONG_PARAM);
    }
    break;
  case CommandType::INVALID:
  case CommandType::WRONG_PARAM:
    break;
  case CommandType::HELP:
  case CommandType::GRAVEYARD:
  case CommandType::BOARD:
  case CommandType::HAND:
  case CommandType::REDRAW:
  case CommandType::STATUS:
  case CommandType::DONE:
    if (!command.getParameters().empty())
    {
      command.setType(CommandType::WRONG_PARAM);
    }
    break;
  case CommandType::INFO:
    if (command.getParameters().size() != 1)
    {
      command.setType(CommandType::WRONG_PARAM);
    }
    break;
  case CommandType::BATTLE:
  case CommandType::CREATURE:
    if (command.getParameters().size() != 2)
    {
      command.setType(CommandType::WRONG_PARAM);
    }
    break;
  case CommandType::SPELL:
    break;
  }

  return command;
}
