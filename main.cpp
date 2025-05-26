//---------------------------------------------------------------------------------------------------------------------
//
// Magical OOPerations is a two player strategy card game, similar to Magic the Gathering, Legends of Runeterra 
//or Hearthstone.In Magical OOPerations, each player has health points and mana.Mana is the currency used to play
//cards, like creatures and spells.The goal of the game is to reduce the other player's health points to zero by 
//attacking them using creatures.Alternatively, a game is also won if the opponent cannot draw, because they have
//no cards left on their draw pile.
//There are two roles in the game(Attacker and Defender), which are filled depending on the round by Player 1 or
//Player 2. To protect themselves from attacks a Defender can set creatures to battle against the Attacker's creatures.
//
// Group: Division 007
//
// Authors: Amila Mujagic, Faris Cisija, Emin Hodzic
//---------------------------------------------------------------------------------------------------------------------
//

#include <iostream>

#include "Command.hpp"
#include "CommandLine.hpp"
#include "Init.hpp"
#include "Player.hpp"
#include "Game.hpp"
#include "Exeption.hpp"

#define MEM_ERROR_MESSAGE "[ERROR] Not enough memory!"
#define WRONG_PARAM_MESSAGE "[ERROR] Wrong number of parameters."
#define INVALID_FILE_MESSAGE "[ERROR] Invalid file "

enum Returns
{
  SUCCESSFUL = 0,
  INVALID_MEMORY = 1,
  WRONG_NUMBER_OF_PARAMETERS = 2,
  INVALID_FILE = 3
};


//---------------------------------------------------------------------------------------------------------------------
///
/// The main function
/// Connects all of the logic of the game together 
///
/// @param argc number of command line arguments
/// @param argv command line arguments
///
/// @return 0 = success, 1 = memory error, 2 = wrong num of params, 3 = invalid file
//
int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << WRONG_PARAM_MESSAGE << std::endl;
    return WRONG_NUMBER_OF_PARAMETERS;
  }

  Player p1(1, 0, 0, 0);
  Player p2(2, 0, 0, 0);

  Init init(p1, p2, argv);
  try
  {
    init.parseMessageLines();
    init.loadConfig();
    init.loadCreatureCodes();
    init.loadSpellCodes();
  }
  catch (const file_error &e)
  {
    std::cout << e.what() << std::endl;
    return INVALID_FILE;
  }
  catch (const MemoryEx &e)
  {
    std::cout << MEM_ERROR_MESSAGE << std::endl;
    return INVALID_MEMORY;
  }

  Game game{p1, p2, init.getErrors(), init.getInfos(), init.getDescriptions(),
            init.getMaxRounds(), init.getCreatureCodebook(), init.getSpellCodebook()};
  CommandLine commandLine;

  int game_status = 0;
  int battle_status = 0;
  try
  {
    while (!(game_status = game.startRound()))
    {
      while (true)
      {
        Command command = commandLine.readCommand(game.getAttackerNumber());
        if (command.isQuit())
          return SUCCESSFUL;
        else if (command.isDone())
        {
          game.setRedrawFalse(game.getAttacker());
          game.applyTraits(game.getAttackerNumber());
          game.printBoard();
          break;
        }

        game.processCommand(game.getAttacker(), command);
      }

      while (true)
      {
        Command command = commandLine.readCommand(game.getDefenderNumber());
        if (command.isQuit())
          return SUCCESSFUL;
        else if (command.isDone())
        {
          game.setRedrawFalse(game.getDefender());
          game.applyTraits(game.getDefenderNumber());
          game.printBoard();
          break;
        }

        game.processCommand(game.getDefender(), command);
      }

      battle_status = game.battlePhase();
      if (battle_status == 1)
      {
        game_status = 4;
        break;
      }
      else if (battle_status == 2)
      {
        game_status = 3;
        break;
      }
      else if (battle_status == 7)
      {
        game_status = 7;
        break;
      }
    }
  }
  catch (const MemoryEx &e)
  {
    std::cout << MEM_ERROR_MESSAGE << std::endl;
    return INVALID_MEMORY;
  }

  game.endGame(game_status, argv[1]);

  return SUCCESSFUL;
}
