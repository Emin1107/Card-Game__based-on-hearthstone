#include <iostream>

#include "Game.hpp"
#include "Command.hpp"
#include "Init.hpp"
#include <fstream>

Game::Game(Player &player1, Player &player2, const std::map<std::string, std::string> &errors,
           const std::map<std::string, std::string> &infos, const std::map<std::string, std::string> &descriptions, int max_rounds, std::vector<std::shared_ptr<Creature>> creature_codebook,
           std::vector<std::shared_ptr<Spell>> spell_codebook)
    : players_{player1, player2}, attacker_(1), defender_(2), max_rounds_(max_rounds), round_(0), board_(), errors_(errors),
      infos_(infos), descriptions_(descriptions), creature_codebook_(creature_codebook), spell_codebook_(spell_codebook)
{
  std::cout << getDescWithId("D_BORDER_D") << std::endl;
  std::cout << getDescWithId("D_WELCOME") << std::endl;
  std::cout << getDescWithId("D_BORDER_D") << std::endl;
  players_[0].drawInitialCards();
  players_[1].drawInitialCards();
}

//-----------------------------------------------------------------------------------------------------
///
/// Final phase of the game
///
/// @param game_status status of the game
/// @param config_file_name name of the configuration file
///
/// @return nothing
void Game::endGame(int game_status, std::string config_file_name)
{
  std::cout << std::endl
            << getDescWithId("D_BORDER_GAME_END") << std::endl;
  if (game_status == 1 || game_status == 2)
  {
    std::cout << getDescWithId("D_END_DRAW_CARD") << std::endl;
  }
  else if (game_status == 3 || game_status == 4 || game_status == 7)
  {
    std::cout << getDescWithId("D_END_PLAYER_DEFEATED") << std::endl;
  }
  else if (game_status == 5 || game_status == 6 || game_status == 8)
  {
    std::cout << getDescWithId("D_END_MAX_ROUNDS") << std::endl;
  }

  std::string winner_str;
  if (game_status == 7 || game_status == 8)
  {
    winner_str = getDescWithId("D_TIE") + "\n";
  }
  else
  {
    int winner = (game_status == 1 || game_status == 3 || game_status == 5) ? 1 : 2;
    winner_str = "Player " + std::to_string(winner) + " has won! Congratulations!\n";
  }

  std::cout << winner_str;
  std::cout << getDescWithId("D_BORDER_D") << std::endl;

  std::ofstream config_file(config_file_name, std::ios::app);
  if (config_file.is_open())
  {
    config_file << std::endl
                << winner_str;
    config_file.close();
  }
  else
  {
    std::cout << getInfoWithId("I_FILE_WRITE_FAILED") << std::endl;
  }
}

//-----------------------------------------------------------------------------------------------------
///
/// Starts a new round
///
/// @return 0 = game continues, 1 = 1 wins, 2 = 2 wins, 3 = 1 wins by death,
///         4 = 2 wins by death, 5 = 1 wins by max rounds, 6 = 2 wins by max rounds,
///         7 = tie by death, 8 = tie by max rounds
int Game::startRound()
{

  // cause: death of player
  // 3 is player 1 wins
  // 4 is player 2 wins
  // 7 is tie by death
  if (players_[0].isDead() && players_[1].isDead())
    return 7;
  if (players_[0].isDead())
    return 4;
  if (players_[1].isDead())
    return 3;

  // cause: max rounds exceeded
  // 5 is player 1 wins
  // 6 is player 2 wins
  // 8 is tie by max rounds
  if (round_ >= max_rounds_)
  {
    if (players_[0].getHealth() > players_[1].getHealth())
    {
      return 5;
    }
    else if (players_[0].getHealth() < players_[1].getHealth())
    {
      return 6;
    }
    else
    {
      return 8;
    }
  }

  round_++;

  // swap defender and attacker
  if (round_ % 2 == 0)
  {
    int temp = attacker_;
    attacker_ = defender_;
    defender_ = temp;
  }

  std::cout << std::endl
            << getDescWithId("D_BORDER_D") << std::endl;
  std::cout << "                                         ROUND " << round_ << std::endl;
  std::cout << getDescWithId("D_BORDER_D") << std::endl;

  // increase mana pool
  if (round_ % 2)
  {
    players_[0].increaseManaPool();
    players_[1].increaseManaPool();
  }

  // print board
  board_.printBoard(defender_, getDescWithId("D_BORDER_A"), getDescWithId("D_BORDER_B"));

  // cause: deck empty cant draw card
  //  1 is player 1 wins
  //  2 is player 2 wins
  if (players_[attacker_ - 1].noCards())
    return defender_;
  if (players_[defender_ - 1].noCards())
    return attacker_;

  players_[attacker_ - 1].drawCard();
  players_[defender_ - 1].drawCard();

  return 0;
}

//-----------------------------------------------------------------------------------------------------
///
/// Battle phase of the game
///
/// @return 0 = game continues, 7 = tie by death, getAttackerNumber() = attacker loses,
///         getDefenderNumber() = defender loses
int Game::battlePhase()
{
  std::cout << "\n"
            << getDescWithId("D_BORDER_BATTLE_PHASE") << "\n";
  std::shared_ptr<Creature> attacking_card = nullptr;
  std::shared_ptr<Creature> defending_card = nullptr;
  for (int slot = 0; slot < 7; slot++)
  {
    std::cout << "---------------------------------------- SLOT " << slot + 1 << " -----------------------------------------\n";
    attacking_card = board_.fetchBattleCard(attacker_, slot);
    defending_card = board_.fetchBattleCard(defender_, slot);

    if (attacking_card != nullptr && defending_card == nullptr)
    {
      std::cout << getInfoWithId("I_DIRECT") << std::endl;
      getDefender().damagePlayer(attacking_card->getCurrentAttack());
    }
    else if (attacking_card != nullptr && defending_card != nullptr)
    {
      resolvingFight(attacking_card, defending_card);
      checkCreatureDeaths();
    }

    if (getAttacker().isDead() && getDefender().isDead())
    {
      return 7;
    }
    else if (getAttacker().isDead())
    {
      return getAttackerNumber();
    }
    if (getDefender().isDead())
    {
      return getDefenderNumber();
    }
  }
  std::cout << getDescWithId("D_BORDER_BATTLE_END") << std::endl;
  handleTemporaryCards();
  offsetCreaturesOnBoard();
  handleUndyingCards();
  return 0;
}

//-----------------------------------------------------------------------------------------------------
///
/// Resolves the fight between two creatures
///
/// @param attacking_card attacking card
/// @param defending_card defending card
///
/// @return nothing
void Game::resolvingFight(std::shared_ptr<Creature> attacking_card, std::shared_ptr<Creature> defending_card)
{
  std::cout << getInfoWithId("I_FIGHT") << std::endl;

  std::cout << getDescWithId("D_ATTACK_1") << std::endl;

  // FIRST STRIKE CHECK =====================================
  if (attacking_card->checkTrait(Trait::F) && !defending_card->checkTrait(Trait::F))
  {
    std::cout << getInfoWithId("I_FIRST_STRIKE") << std::endl;
    resolveFightTraits(attacking_card, defending_card, defender_);
    if (!defending_card->isDead())
    {
      std::cout << getDescWithId("D_ATTACK_2") << std::endl;
      resolveFightTraits(defending_card, attacking_card, attacker_);
    }
  }
  else if (!attacking_card->checkTrait(Trait::F) && defending_card->checkTrait(Trait::F))
  {
    std::cout << getInfoWithId("I_FIRST_STRIKE") << std::endl;
    resolveFightTraits(defending_card, attacking_card, attacker_);
    if (!attacking_card->isDead())
    {
      std::cout << getDescWithId("D_ATTACK_2") << std::endl;
      resolveFightTraits(attacking_card, defending_card, defender_);
    }
  }
  else
  {
    resolveFightTraits(attacking_card, defending_card, defender_);
    std::cout << getDescWithId("D_ATTACK_2") << std::endl;
    resolveFightTraits(defending_card, attacking_card, attacker_);
  }
}

//-----------------------------------------------------------------------------------------------------
///
/// Resolves the fight traits
///
/// @param attacking_card attacking card
/// @param defending_card defending card
/// @param defender defending player
///
/// @return nothing
void Game::resolveFightTraits(std::shared_ptr<Creature> attacking_card, std::shared_ptr<Creature> defending_card, int defender)
{
  int excess_damage = attacking_card->getCurrentAttack() - defending_card->getCurrentHealth(); // brutal
  defending_card->damageCreature(attacking_card->getCurrentAttack());

  // BRUTAL
  if (attacking_card->checkTrait(Trait::B) && defending_card->isDead())
  {
    std::cout << getInfoWithId("I_BRUTAL") << std::endl;
    players_[defender - 1].damagePlayer(excess_damage < 0 ? 0 : excess_damage);
  }
  // LIFESTEAL
  if (attacking_card->checkTrait(Trait::L) && attacking_card->getCurrentAttack() > 0)
  {
    std::cout << getInfoWithId("I_LIFESTEAL") << std::endl;
    attacking_card->increaseCurrentHealth(2);
  }
  // VENOMOUS
  if (attacking_card->checkTrait(Trait::V) && attacking_card->getCurrentAttack() > 0)
  {
    std::cout << getInfoWithId("I_VENOMOUS") << std::endl;
    defending_card->addTrait(Trait::P);
  }
}

//-----------------------------------------------------------------------------------------------------
///
/// Checks if a creature has Trait::U, if so, it is placed back on the field
///
/// @return nothing
void Game::handleUndyingCards()
{
  std::shared_ptr<Creature> card = nullptr;
  for (int player = 0; player < 2; player++)
  {
    std::vector<unsigned long> creatures_to_remove_indexes;
    for (unsigned long index = 0; index < players_[player].getGraveyard().size(); index++)
    {
      card = players_[player].getGraveyard().at(index);
      if (card->checkTrait(Trait::U))
      {
        if (!board_.areAllFieldsFull(player + 1))
        {
          card->resetAttributes();
          card->removeUndying();
          card->setRoundPlacement(round_);
          board_.placeCard(card, player, -1);
          creatures_to_remove_indexes.push_back(index);
          std::cout << getInfoWithId("I_UNDYING") << std::endl;
        }
      }
    }
    players_[player].removeUndyingFromGraveyard(creatures_to_remove_indexes);
  }
}

//-----------------------------------------------------------------------------------------------------
///
/// Checks if a creature has Trait::T, if so, it is removed from the board
///
/// @return nothing
void Game::handleTemporaryCards()
{
  std::shared_ptr<Creature> card = nullptr;
  for (int player = 0; player < 2; player++)
  {
    for (int slot = 0; slot < 7; slot++)
    {
      card = board_.fetchBattleCard(player + 1, slot);
      if (card == nullptr)
        continue;
      if (card->checkTrait(Trait::T))
      {
        board_.removeCardFromBattle(player + 1, slot);
        players_[player].addCardToGraveyard(card);
        std::cout << getInfoWithId("I_TEMPORARY") << std::endl;
      }
    }
  }

  for (int player = 0; player < 2; player++)
  {
    for (int slot = 0; slot < 7; slot++)
    {
      card = board_.fetchFieldCard(player + 1, slot);
      if (card == nullptr)
        continue;
      if (card->checkTrait(Trait::T))
      {
        board_.removeCardFromField(player + 1, slot);
        players_[player].addCardToGraveyard(card);
        std::cout << getInfoWithId("I_TEMPORARY") << std::endl;
      }
    }
  }
}

//-----------------------------------------------------------------------------------------------------
///
/// Removes creatures from the battle and places them back on the field if possible
///
/// @return nothing
void Game::offsetCreaturesOnBoard()
{
  std::shared_ptr<Creature> card = nullptr;
  for (int player = 0; player < 2; player++)
  {
    for (int slot = 0; slot < 7; slot++)
    {
      card = board_.fetchBattleCard(player + 1, slot);
      if (card == nullptr)
        continue;
      if (!board_.areAllFieldsFull(player + 1))
        board_.placeCard(card, player, -1);
      else
        players_[player].addCardToGraveyard(card);
      board_.removeCardFromBattle(player + 1, slot);
    }
  }
}

Player &Game::getAttacker()
{
  return players_[attacker_ - 1];
}

Player &Game::getDefender()
{
  return players_[defender_ - 1];
}

//-----------------------------------------------------------------------------------------------------
///
/// Processes the commands passed from the input
///
/// @param player player
/// @param command command
///
/// @return nothing
void Game::processCommand(Player& player, Command command)
{
  if (command.getType() == CommandType::HELP)
  {
    helpWrapper();
  }
  else if (command.getType() == CommandType::GRAVEYARD)
  {
    graveyardWrapper(player);
  }
  else if (command.getType() == CommandType::BOARD)
  {
    boardWrapper();
  }
  else if (command.getType() == CommandType::HAND)
  {
    handWrapper(player);
  }
  else if (command.getType() == CommandType::INFO)
  {
    infoWrapper(command.getParameters());
  }
  else if (command.getType() == CommandType::REDRAW)
  {
    redrawWrapper(player);
  }
  else if (command.getType() == CommandType::STATUS)
  {
    statusWrapper();
  }
  else if (command.getType() == CommandType::BATTLE)
  {
    battleWrapper(player, command.getParameters());
  }
  else if (command.getType() == CommandType::CREATURE)
  {
    creatureWrapper(player, command.getParameters());
  }
  else if (command.getType() == CommandType::SPELL)
  {
    spellWrapper(player, players_[player.getPlayerNumber() == 1 ? 1 : 0], command.getParameters());
  }
  else if (command.getType() == CommandType::WRONG_PARAM)
  {
    std::cout << getErrorWithId("E_INVALID_PARAM_COUNT") << std::endl;
  }
  else
  {
    std::cout << getErrorWithId("E_UNKNOWN_COMMAND") << std::endl;
  }
}

//-----------------------------------------------------------------------------------------------------
///
/// Prints the help text from Command::HELP
///
/// @return nothing
void Game::helpWrapper()
{
  std::cout << HELP_TEXT << std::endl;
  return;
}

//-----------------------------------------------------------------------------------------------------
///
/// Processes the logic for the Command::GRAVEYARD 
///
/// @param player player
/// 
/// @return nothing
void Game::graveyardWrapper(Player& player)
{
  std::cout << getDescWithId("D_BORDER_GRAVEYARD") << std::endl;
  const auto &graveyard = player.getGraveyard();
  if (!graveyard.empty())
  {
    for (const auto &it : player.getGraveyard())
    {
      std::cout << it->getCardID() << " | " << it->getCardName() << std::endl;
    }
  }
  std::cout << getDescWithId("D_BORDER_D") << std::endl;
}

//-----------------------------------------------------------------------------------------------------
///
/// Processes the logic for the Command::BOARD
///
/// @return nothing
void Game::boardWrapper()
{
  board_.toggleActive();
  if (board_.isActive())
  {
    board_.printBoard(defender_, getDescWithId("D_BORDER_A"), getDescWithId("D_BORDER_B"));
  }
}

//-----------------------------------------------------------------------------------------------------
///
/// Processes the logic for the Command::HAND
///
/// @param player player
///
/// @return nothing
void Game::handWrapper(Player& player)
{
  std::cout << getDescWithId("D_BORDER_HAND") << std::endl;
  players_[player.getPlayerNumber() - 1].printHand();
  std::cout << getDescWithId("D_BORDER_D") << std::endl;
}

//-----------------------------------------------------------------------------------------------------
///
/// Help function that converts a string to upper case
///
/// @param lowercased string to convert
///
/// @return uppercased string
std::string stringToUpper(std::string lowercased)
{
  for (unsigned long i = 0; i < lowercased.length(); i++)
  {
    lowercased.at(i) = toupper(lowercased.at(i));
  }
  return lowercased;
}

//-----------------------------------------------------------------------------------------------------
///
/// Processes the logic for the Command::INFO
///
/// @param parameters parameters
///
/// @return nothing
void Game::infoWrapper(std::vector<std::string>& parameters)
{
  std::string card_id = parameters[0];
  card_id = stringToUpper(card_id);
  std::shared_ptr<Card> card = nullptr;

  if (!doesCardExist(card_id))
  {
    std::cout << getErrorWithId("E_INVALID_CARD") << std::endl;
    return;
  }

  for (auto &it : creature_codebook_)
  {
    if (it->getCardID() == card_id)
    {
      card = it;
      break;
    }
  }
  if (!card)
  {
    for (const auto &it : spell_codebook_)
    {
      if (it->getCardID() == card_id)
      {
        card = it;
        break;
      }
    }
  }

  card->printInfo(getDescWithId("D_BORDER_INFO"), getDescWithId("D_BORDER_D"));
}

//-----------------------------------------------------------------------------------------------------
///
/// Processes the logic for the Command::REDRAW
///
/// @param player player
///
/// @return nothing
void Game::redrawWrapper(Player& player)
{
  if (!player.getRedrawStatus())
  {
    std::cout << getErrorWithId("E_REDRAW_DISABLED") << std::endl;
    return;
  }

  int hand_size = player.getHandSize();

  if (hand_size < 2)
  {
    std::cout << getErrorWithId("E_REDRAW_NOT_ENOUGH_CARDS") << std::endl;
    return;
  }

  player.returnHandToDeck();

  --hand_size;
  for (int i = 0; i < hand_size; i++)
  {
    player.drawCard();
  }
}

//-----------------------------------------------------------------------------------------------------
///
/// Sets the redraw status to false
///
/// @param player player
///
/// @return nothing
void Game::setRedrawFalse(Player& player)
{
  player.setRedrawToFalse();
}

//-----------------------------------------------------------------------------------------------------
///
/// Prints the role of the passed player
///
/// @param player player number
///
/// @return string with the role of the player
std::string Game::printRole(int player) const
{
  if (player == attacker_)
  {
    return "Role: Attacker";
  }
  else
  {
    return "Role: Defender";
  }
}

//-----------------------------------------------------------------------------------------------------
///
/// Processes the logic for the Command::STATUS
///
/// @return nothing
void Game::statusWrapper()
{
  std::cout << getDescWithId("D_BORDER_STATUS") << std::endl;
  std::cout << "Player 1" << std::endl;
  std::cout << printRole(1) << std::endl;
  std::cout << players_[0]; // Use the overloaded operator
  std::cout << getDescWithId("D_BORDER_C") << std::endl;
  std::cout << "Player 2" << std::endl;
  std::cout << printRole(2) << std::endl;
  std::cout << players_[1]; // Use the overloaded operator
  std::cout << getDescWithId("D_BORDER_D") << std::endl;
}

//-----------------------------------------------------------------------------------------------------
///
/// Processes the logic for the Command::BATTLE
///
/// @param player player
/// @param parameters parameters
///
/// @return nothing
void Game::battleWrapper(Player& player, std::vector<std::string>& parameters)
{
  std::string fieldSlot = parameters[0];
  std::string battleSlot = parameters[1];

  if (!isValidFieldSlot(fieldSlot) || !isValidFieldSlot(battleSlot))
  {
    std::cout << getErrorWithId("E_INVALID_SLOT") << std::endl;
    return;
  }

  if (!checkFieldSlot(fieldSlot))
  {
    std::cout << getErrorWithId("E_NOT_IN_FIELD") << std::endl;
    return;
  }

  if (!board_.isFieldSlotOccupied(player.getPlayerNumber() - 1, fieldSlot[1] - '0' - 1))
  {
    std::cout << getErrorWithId("E_FIELD_EMPTY") << std::endl;
    return;
  }

  int field_pos = fieldSlot[1] - '0';
  int battle_pos = battleSlot[1] - '0';
  --field_pos;
  --battle_pos;
  std::shared_ptr<Creature> current_card = board_.fetchFieldCard(player.getPlayerNumber(), field_pos);

  bool has_haste = false;
  if (isCreatureTraitHaste(current_card) && (current_card->getRoundPlacement() == round_))
  {
    has_haste = true;
  }
  else if (!isCreatureTraitHaste(current_card) && (current_card->getRoundPlacement() == round_))
  {
    std::cout << getErrorWithId("E_CREATURE_CANNOT_BATTLE") << std::endl;
    return;
  }

  if (!checkBattleSlot(battleSlot))
  {
    std::cout << getErrorWithId("E_NOT_IN_BATTLE") << std::endl;
    return;
  }
  if (board_.isBattleSlotOccupied(player.getPlayerNumber() - 1, battleSlot[1] - '0' - 1))
  {
    std::cout << getErrorWithId("E_BATTLE_OCCUPIED") << std::endl;
    return;
  }

  board_.placeCardInBattle(current_card, player.getPlayerNumber(), battle_pos, field_pos);
  if (has_haste)
  {
    std::cout << getInfoWithId("I_HASTE") << std::endl;
  }

  if (isCreatureTraitChallenger(current_card))
  {
    int opponent = (player.getPlayerNumber() - 1) == 0 ? 1 : 0;
    challengeTheOpponent(opponent, battle_pos);
  }
  player.setRedrawToFalse();
}

//-----------------------------------------------------------------------------------------------------
///
/// Checks if the field slot is valid
///
/// @param fieldSlot field slot
///
/// @return true = valid, false = invalid
bool Game::checkFieldSlot(std::string fieldSlot)
{
  if (fieldSlot[0] != 'f' && fieldSlot[0] != 'F')
    return false;

  return true;
}

//------------------------------------------------------------------------------------------------------
///
/// Checks if the battle slot is valid
///
/// @param battleSlot battle slot
///
/// @return true = valid, false = invalid
bool Game::checkBattleSlot(std::string battleSlot)
{
  if (battleSlot[0] != 'b' && battleSlot[0] != 'B')
    return false;

  return true;
}

//-----------------------------------------------------------------------------------------------------
///
/// Checks if the creature has Trait::H
///
/// @param card card
///
/// @return true = has Haste, false = does not have Haste
bool Game::isCreatureTraitHaste(std::shared_ptr<Creature> card)
{
  std::vector<Trait> traits = card->getTraits();

  for (const auto &trait : traits)
  {
    if (trait == Trait::H)
    {
      return true;
    }
  }

  return false;
}

//-----------------------------------------------------------------------------------------------------
///
/// Checks if the creature has Trait::C
///
/// @param card card
///
/// @return true = has Challenger, false = does not have Challenger
bool Game::isCreatureTraitChallenger(std::shared_ptr<Creature> card)
{
  std::vector<Trait> traits = card->getTraits();

  for (const auto &trait : traits)
  {
    if (trait == Trait::C)
    {
      return true;
    }
  }

  return false;
}

//-----------------------------------------------------------------------------------------------------
///
/// Processes the logic for the Challenger trait
///
/// @param opponent opponent
/// @param battle_pos battle position
///
/// @return nothing
void Game::challengeTheOpponent(int opponent, int battle_pos)
{
  if (checkOpponentsField(battle_pos, opponent) && !checkOpponentsBattleField(battle_pos, opponent))
  {
    std::shared_ptr<Creature> current_card = board_.fetchFieldCard(opponent + 1, battle_pos);

    board_.placeCardInBattle(current_card, opponent + 1, battle_pos, battle_pos);
    std::cout << getInfoWithId("I_CHALLENGER") << std::endl;
  }
}

//-----------------------------------------------------------------------------------------------------
///
/// Checks if the opponent has a creature in the field zone
///
/// @param battle_pos battle position
/// @param opponent opponent
///
/// @return true = has a creature, false = does not have a creature
bool Game::checkOpponentsField(int battle_pos, int opponent)
{
  if (board_.isFieldSlotOccupied(opponent, battle_pos))
  {
    return true;
  }

  return false;
}

//-----------------------------------------------------------------------------------------------------
///
/// Checks if the opponent has a creature in the battle zone
///
/// @param battle_pos battle position
/// @param opponent opponent
///
/// @return true = has a creature, false = does not have a creature
bool Game::checkOpponentsBattleField(int battle_pos, int opponent)
{
  if (board_.isBattleSlotOccupied(opponent, battle_pos))
  {
    return true;
  }
  return false;
}

//-----------------------------------------------------------------------------------------------------
///
/// Checks if the input matches the regex
///
/// @param slotString slot string
///
/// @return true = valid, false = invalid
bool Game::isValidFieldSlot(std::string slotString)
{
  std::regex r("[oO]?([fF]|[bB])[1-7]");
  return regex_match(slotString, r);
}

//-----------------------------------------------------------------------------------------------------
///
/// Checks if the card exists in the codebooks
///
/// @param card_id card id
///
/// @return true = exists, false = does not exist
bool Game::doesCardExist(std::string card_id)
{
  for (const auto &it : creature_codebook_)
  {
    if (it->getCardID() == card_id)
    {
      return true;
    }
  }

  for (const auto &it : spell_codebook_)
  {
    if (it->getCardID() == card_id)
    {
      return true;
    }
  }
  return false;
}

//-----------------------------------------------------------------------------------------------------
///
/// Checks if the card is in the player's hand
///
/// @param player player
/// @param card_id card id
///
/// @return true = in hand, false = not in hand
bool Game::isInHand(Player& player, std::string card_id)
{
  for (auto &c : player.getHand())
  {
    if (c->getCardID() == card_id)
      return true;
  }
  return false;
}

//-----------------------------------------------------------------------------------------------------
///
/// Gets the card from the player's hand
///
/// @param card_id card id
/// @param player player
///
/// @return card
std::shared_ptr<Card> Game::getFromHand(std::string card_id, Player& player)
{
  std::vector<std::shared_ptr<Card>> p_hand = player.getHand();
  for (unsigned long i = 0; i < p_hand.size(); i++)
  {
    if (card_id == p_hand.at(i)->getCardID())
    {
      return p_hand.at(i);
    }
  }
  return nullptr;
}

//-----------------------------------------------------------------------------------------------------
///
/// Checks if the card is a creature
///
/// @param card_id card id
///
/// @return true = is a creature, false = is not a creature
bool Game::cardIsCreature(std::string card_id)
{
  for (const auto &it : creature_codebook_)
  {
    if (it->getCardID() == card_id)
    {
      return true;
    }
  }
  return false;
}

//-----------------------------------------------------------------------------------------------------
///
/// Checks if the card is a spell
///
/// @param card_id card id
///
/// @return true = is a spell, false = is not a spell
bool Game::cardIsSpell(std::string card_id)
{
  for (const auto &it : spell_codebook_)
  {
    if (it->getCardID() == card_id)
    {
      return true;
    }
  }
  return false;
}

//-----------------------------------------------------------------------------------------------------
///
/// Checks if the player has enough mana to play the card
///
/// @param player player
/// @param mana_cost mana cost
///
/// @return true = enough mana, false = not enough mana
bool Game::isEnoughMana(Player& player, int mana_cost)
{
  int total_mana = player.getMana();
  if (total_mana - mana_cost < 0)
    return false;

  return true;
}

//-----------------------------------------------------------------------------------------------------
///
/// Processes the logic for the Command::CREATURE
///
/// @param player player
/// @param parameters parameters
///
/// @return nothing
void Game::creatureWrapper(Player& player, std::vector<std::string>& parameters)
{
  std::string card_id = parameters[0];
  std::string card_id_uppercase = stringToUpper(card_id);
  auto fieldSlot = parameters[1];
  int field_position = fieldSlot[1] - '0';

  if (!doesCardExist(card_id_uppercase))
  {
    std::cout << getErrorWithId("E_INVALID_CARD") << std::endl;
    return;
  }
  else if (!isValidFieldSlot(fieldSlot))
  {
    std::cout << getErrorWithId("E_INVALID_SLOT") << std::endl;
    return;
  }
  else if (!isInHand(player, card_id_uppercase))
  {
    std::cout << getErrorWithId("E_NOT_IN_HAND") << std::endl;
    return;
  }
  else if (!cardIsCreature(card_id_uppercase))
  {
    std::cout << getErrorWithId("E_NOT_CREATURE") << std::endl;
    return;
  }
  else if (!checkFieldSlot(fieldSlot))
  {
    std::cout << getErrorWithId("E_NOT_IN_FIELD") << std::endl;
    return;
  }
  else if (board_.fetchFieldCard(player.getPlayerNumber(), field_position - 1) != nullptr)
  {
    std::cout << getErrorWithId("E_FIELD_OCCUPIED") << std::endl;
    return;
  }

  std::shared_ptr<Creature> card_from_hand = std::dynamic_pointer_cast<Creature>(getFromHand(card_id_uppercase, player));
  if (!isEnoughMana(player, card_from_hand->getManaCost()))
  {
    std::cout << getErrorWithId("E_NOT_ENOUGH_MANA") << std::endl;
    return;
  }

  int playerID = player.getPlayerNumber();
  board_.placeCard(card_from_hand, playerID - 1, field_position - 1);
  player.subtractMana(card_from_hand->getManaCost());
  card_from_hand->setRoundPlacement(round_);
  std::string card_ID = "I_" + card_id_uppercase;
  std::cout << getInfoWithId(card_ID) << std::endl;
  player.removeFromHand(card_id_uppercase);
  player.setRedrawToFalse();
}

//-----------------------------------------------------------------------------------------------------
///
/// Processes the logic for the Command::SPELL
///
/// @param player player
/// @param opponent opponent
/// @param parameters parameters
///
/// @return nothing
void Game::spellWrapper(Player& player, Player& opponent, std::vector<std::string>& parameters)
{
  if (parameters.empty())
  {
    std::cout << getErrorWithId("E_MISSING_CARD") << std::endl;
    return;
  }

  std::string card_id = parameters[0];
  card_id = stringToUpper(card_id);

  if (!doesCardExist(card_id))
  {
    std::cout << getErrorWithId("E_INVALID_CARD") << std::endl;
    return;
  }
  std::shared_ptr<Card> testing_card_from_hand = getFromHand(card_id, player);
  if (testing_card_from_hand == nullptr)
  {
    std::cout << getErrorWithId("E_NOT_IN_HAND") << std::endl;
    return;
  }
  std::shared_ptr<Spell> card_from_hand = std::dynamic_pointer_cast<Spell>(testing_card_from_hand);
  if (!cardIsSpell(card_id))
  {
    std::cout << getErrorWithId("E_NOT_SPELL") << std::endl;
    return;
  }
  int spell_type = card_from_hand->getSpellCardType(card_id);
  if (spell_type == 1 && parameters.size() != 1)
  {
    std::cout << getErrorWithId("E_INVALID_PARAM_COUNT_SPELL") << std::endl;
    return;
  }
  else if (spell_type == 2 && parameters.size() != 2)
  {
    std::cout << getErrorWithId("E_INVALID_PARAM_COUNT_SPELL") << std::endl;
    return;
  }
  else if (spell_type == 3 && parameters.size() != 2)
  {
    std::cout << getErrorWithId("E_INVALID_PARAM_COUNT_SPELL") << std::endl;
    return;
  }
  std::shared_ptr<Creature> affected_creature = nullptr;
  bool on_opponent_side = false;
  if (spell_type == 2)
  {
    std::string slot_str = parameters[1];
    if (!isValidFieldSlot(slot_str))
    {
      std::cout << getErrorWithId("E_INVALID_SLOT_SPELL") << std::endl;
      return;
    }

    int slot_number = 0;
    char slot_char = slot_str[0];
    if (slot_char == 'o' || slot_char == 'O')
    {
      on_opponent_side = true;
      slot_number = slot_str[2] - '0' - 1;
      slot_char = slot_str[1];
    }
    else
    {
      slot_number = slot_str[1] - '0' - 1;
    }

    if (slot_char == 'f' || slot_char == 'F')
    {
      if (on_opponent_side)
      {
        affected_creature = board_.fetchFieldCard(opponent.getPlayerNumber(), slot_number);
      }
      else
      {
        affected_creature = board_.fetchFieldCard(player.getPlayerNumber(), slot_number);
      }
    }
    else if (slot_char == 'b' || slot_char == 'B')
    {
      if (on_opponent_side)
      {
        affected_creature = board_.fetchBattleCard(opponent.getPlayerNumber(), slot_number);
      }
      else
      {
        affected_creature = board_.fetchBattleCard(player.getPlayerNumber(), slot_number);
      }
    }

    if (affected_creature == nullptr)
    {
      std::cout << getErrorWithId("E_TARGET_EMPTY") << std::endl;
      return;
    }
  }
  else if (spell_type == 3)
  {
    std::string card_id = stringToUpper(parameters[1]);
    affected_creature = player.getFromGraveyard(card_id);
    if (!affected_creature)
    {
      std::cout << getErrorWithId("E_NOT_IN_GRAVEYARD") << std::endl;
      return;
    }
  }
  int mana_cost = 0;
  if (!card_from_hand->hasMana())
  {
    if (card_id == "CLONE" || card_id == "MEMRY")
    {
      mana_cost = (affected_creature->getManaCost() + 1) / 2;
    }
    else if (card_id == "CURSE")
    {
      mana_cost = affected_creature->getManaCost() + 1;
    }
  }
  else
  {
    mana_cost = card_from_hand->getManaCost();
  }

  if (mana_cost > player.getMana())
  {
    std::cout << getErrorWithId("E_NOT_ENOUGH_MANA") << std::endl;
    return;
  }

  card_from_hand->processSpell(card_id, player, opponent, board_, affected_creature, round_);
  checkCreatureDeaths();
  std::cout << getInfoWithId("I_" + card_id) << std::endl;
  if (card_from_hand->hasMana())
  {
    player.subtractMana(card_from_hand->getManaCost());
  }
  player.removeFromHand(card_id);
  player.setRedrawToFalse();
}

//-----------------------------------------------------------------------------------------------------
///
/// Gets the description message from the passed id
///
/// @param id id
///
/// @return description message
std::string Game::getDescWithId(const std::string id) const
{
  auto it = descriptions_.find(id);
  return it->second;
}

//-----------------------------------------------------------------------------------------------------
///
/// Gets the error message from the passed id
///
/// @param id id
///
/// @return error message
std::string Game::getErrorWithId(const std::string id) const
{
  auto it = errors_.find(id);
  return "[ERROR] " + it->second;
}

//-----------------------------------------------------------------------------------------------------
///
/// Gets the info message from the passed id
///
/// @param id id
///
/// @return info message
std::string Game::getInfoWithId(const std::string& id) const
{
  auto it = infos_.find(id);
  return "[INFO] " + it->second;
}

//-----------------------------------------------------------------------------------------------------
///
/// Checks if the creatures are dead and moves them to the graveyard
///
/// @return nothing
void Game::checkCreatureDeaths()
{
  std::shared_ptr<Creature> card = nullptr;

  for (int player = 0; player < 2; player++)
  {
    for (int slot = 0; slot < 7; slot++)
    {
      card = board_.fetchBattleCard(player + 1, slot);
      if (card != nullptr)
      {
        if (card->isDead())
        {
          players_[player].addCardToGraveyard(card);
          board_.removeCardFromBattle(player + 1, slot);
        }
      }
    }
  }
  for (int player = 0; player < 2; player++)
  {
    for (int slot = 0; slot < 7; slot++)
    {
      card = board_.fetchFieldCard(player + 1, slot);
      if (card != nullptr)
      {
        if (card->isDead())
        {
          players_[player].addCardToGraveyard(card);
          board_.removeCardFromField(player + 1, slot);
        }
      }
    }
  }
}

//-----------------------------------------------------------------------------------------------------
///
/// Applies the effects of the Trait::R and Trait::P
///
/// @param player player
///
/// @return nothing
void Game::applyTraits(int player)
{
  if (round_ % 2 == 1)
  {
    board_.regenerateCreatures(player, getInfoWithId("I_REGENERATE"));
  }
  board_.damagePoisonedCreatures(player, getInfoWithId("I_POISONED"));
  checkCreatureDeaths();
}
