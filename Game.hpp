#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <string>
#include <vector>
#include <regex>

#include "Command.hpp"
#include "CommandLine.hpp"
#include "Player.hpp"
#include "Board.hpp"
#include "Card.hpp"
#include "Init.hpp"
#include "Creature.hpp"
#include "Spell.hpp"
#include "Exeption.hpp"

#define HELP_TEXT "=== Commands ============================================================================\n" \
                  "- help\n"                                                                                    \
                  "    Prints this help text.\n"                                                                \
                  "\n"                                                                                          \
                  "- quit\n"                                                                                    \
                  "    Terminates the game.\n"                                                                  \
                  "\n"                                                                                          \
                  "- battle <FIELD_SLOT> <BATTLE_SLOT>\n"                                                       \
                  "    Moves a creature from a Field Zone slot into a Battle Zone slot.\n"                      \
                  "    <FIELD_SLOT>: Current slot of the creature (F1, ..., F7)\n"                              \
                  "    <BATTLE_SLOT>: Battle slot for the creature (B1, ..., B7)\n"                             \
                  "\n"                                                                                          \
                  "- board\n"                                                                                   \
                  "    Toggles the board printing.\n"                                                           \
                  "\n"                                                                                          \
                  "- creature <HAND_CARD_ID> <FIELD_SLOT>\n"                                                    \
                  "    Places a creature from your hand into a Field Zone slot.\n"                              \
                  "    <HAND_CARD_ID>: The ID of the creature card in your hand\n"                              \
                  "    <FIELD_SLOT>: The Field Zone slot to place the creature in\n"                            \
                  "\n"                                                                                          \
                  "- done\n"                                                                                    \
                  "    Ends your turn for this round.\n"                                                        \
                  "\n"                                                                                          \
                  "- graveyard\n"                                                                               \
                  "    Prints all the cards in your graveyard.\n"                                               \
                  "\n"                                                                                          \
                  "- hand\n"                                                                                    \
                  "    Prints your hand cards.\n"                                                               \
                  "\n"                                                                                          \
                  "- info <CARD_ID>\n"                                                                          \
                  "    Prints card information.\n"                                                              \
                  "    <CARD_ID>: The ID of the card to be inspected\n"                                         \
                  "\n"                                                                                          \
                  "- redraw\n"                                                                                  \
                  "    Discards all hand cards and draws the same amount minus one from your deck.\n"           \
                  "\n"                                                                                          \
                  "- spell <HAND_CARD_ID> [<TARGET_SLOT>|<GRAVEYARD_CARD_ID>]\n"                                \
                  "    Casts a spell from your hand.\n"                                                         \
                  "    <HAND_CARD_ID>: The ID of the spell in your hand\n"                                      \
                  "    <TARGET_SLOT>: The slot to target with a target spell\n"                                 \
                  "    <GRAVEYARD_CARD_ID>: The ID of a card in the graveyard to cast a graveyard spell on\n"   \
                  "\n"                                                                                          \
                  "- status\n"                                                                                  \
                  "    Prints general information about both players.\n"                                        \
                  "\n"                                                                                          \
                  "========================================================================================="

class Game
{
protected:
  std::vector<Player> players_;
  int attacker_;
  int defender_;
  int max_rounds_;
  int round_;
  Board board_;
  const std::map<std::string, std::string> errors_;
  const std::map<std::string, std::string> infos_;
  const std::map<std::string, std::string> descriptions_;

  std::vector<std::shared_ptr<Creature>> creature_codebook_;
  std::vector<std::shared_ptr<Spell>> spell_codebook_;

public:
  // Forward declarations
  Game(Player& player1, Player& player2, const std::map<std::string, std::string>& errors,
       const std::map<std::string, std::string> &infos,
       const std::map<std::string, std::string> &descriptions,
       int max_rounds,
       std::vector<std::shared_ptr<Creature>> creature_codebook,
       std::vector<std::shared_ptr<Spell>> spell_codebook);

  ~Game() = default;

  void endGame(int game_status, std::string config_file_name);

  int startRound();
  int battlePhase();
  void resolvingFight(std::shared_ptr<Creature> attacking_card, std::shared_ptr<Creature> defending_card);
  void resolveFightTraits(std::shared_ptr<Creature> attacking_card, std::shared_ptr<Creature> defending_card, int defender);

  void handleTemporaryCards();
  void offsetCreaturesOnBoard();
  Player &getAttacker();
  Player &getDefender();
  int getAttackerNumber() const { return attacker_; }
  int getDefenderNumber() const { return defender_; }

  void processCommand(Player &player, Command command);
  void setRedrawFalse(Player &player);

  std::string getErrorWithId(const std::string id) const;
  std::string getDescWithId(const std::string id) const;
  std::string getInfoWithId(const std::string &id) const;

  void helpWrapper();
  void graveyardWrapper(Player &player);
  void boardWrapper();
  void handWrapper(Player &player);
  void infoWrapper(std::vector<std::string> &parameters);
  void redrawWrapper(Player &player);
  void statusWrapper();
  void battleWrapper(Player &player, std::vector<std::string> &parameters);
  bool checkFieldSlot(std::string fieldSlot);
  bool checkBattleSlot(std::string battleSlot);
  bool isCreatureTraitHaste(std::shared_ptr<Creature> card);
  bool isCreatureTraitChallenger(std::shared_ptr<Creature> card);
  void challengeTheOpponent(int opponent, int battle_pos);
  bool checkOpponentsField(int battle_pos, int opponent);
  bool checkOpponentsBattleField(int battle_pos, int opponent);
  void checkCreatureDeaths();

  void creatureWrapper(Player &player, std::vector<std::string> &parameters);
  void spellWrapper(Player &player, Player &opponent, std::vector<std::string> &parameters);

  void printBoard()
  {
    board_.printBoard(defender_, descriptions_.at("D_BORDER_A"), descriptions_.at("D_BORDER_B"));
  }

  bool isValidFieldSlot(std::string slotString);
  bool doesCardExist(std::string cardID);
  bool isInHand(Player &player, std::string cardID);
  std::shared_ptr<Card> getFromHand(std::string cardID, Player &player);

  bool cardIsCreature(std::string card_id);
  bool cardIsSpell(std::string card_id);
  bool isEnoughMana(Player &player, int mana_cost);
  void applyTraits(int player);
  void handleUndyingCards();

  std::string printRole(int player) const;
};

#endif
