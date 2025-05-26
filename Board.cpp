#include "Board.hpp"
#include "Game.hpp"

Board::Board() : is_active_(true)
{
  field_zone_[0].resize(7, nullptr);
  field_zone_[1].resize(7, nullptr);
  battle_zone_[0].resize(7, nullptr);
  battle_zone_[1].resize(7, nullptr);
}

Board::~Board()
{
  // TODO
}


//---------------------------------------------------------------------------------------------------------------------
///
/// Prints the board
///
/// @param defender defending player of the current round
/// @param border_A border A
/// @param border_B border B
///
/// @return nothing
//
void Board::printBoard(int defender, std::string border_A, std::string border_B) const
{
  if (!is_active_)
    return;

  int attacker = (defender == 1) ? 2 : 1;
  std::cout << "================================== DEFENDER: PLAYER " << defender << " ===================================\n";

  for (int row = 0; row < 4; row++)
  {
    std::cout << "F  ";
    for (int card_per_row_count = 0; card_per_row_count < 7; card_per_row_count++)
    {
      std::cout << " ";
      if (field_zone_[defender - 1][card_per_row_count] != nullptr)
      {
        std::cout << field_zone_[defender - 1][card_per_row_count]->printCard()[row];
      }
      else
      {
        std::cout << "         ";
      }
      std::cout << "  ";
    }
    std::cout << " F\n";
  }

  std::cout << border_A << std::endl;

  for (int row = 0; row < 4; row++)
  {
    std::cout << "B  ";
    for (int card_per_row_count = 0; card_per_row_count < 7; card_per_row_count++)
    {
      std::cout << " ";
      if (battle_zone_[defender - 1][card_per_row_count] != nullptr)
      {
        std::cout << battle_zone_[defender - 1][card_per_row_count]->printCard()[row];
      }
      else
      {
        std::cout << "         ";
      }
      std::cout << "  ";
    }
    std::cout << " B\n";
  }

  std::cout << border_B << std::endl;

  for (int row = 0; row < 4; row++)
  {
    std::cout << "B  ";
    for (int card_per_row_count = 0; card_per_row_count < 7; card_per_row_count++)
    {
      std::cout << " ";
      if (battle_zone_[attacker - 1][card_per_row_count] != nullptr)
      {
        std::cout << battle_zone_[attacker - 1][card_per_row_count]->printCard()[row];
      }
      else
      {
        std::cout << "         ";
      }
      std::cout << "  ";
    }
    std::cout << " B\n";
  }

  std::cout << border_A << std::endl;

  for (int row = 0; row < 4; row++)
  {
    std::cout << "F  ";
    for (int card_per_row_count = 0; card_per_row_count < 7; card_per_row_count++)
    {
      std::cout << " ";
      if (field_zone_[attacker - 1][card_per_row_count] != nullptr)
      {
        std::cout << field_zone_[attacker - 1][card_per_row_count]->printCard()[row];
      }
      else
      {
        std::cout << "         ";
      }
      std::cout << "  ";
    }
    std::cout << " F\n";
  }

  std::cout << "================================== ATTACKER: PLAYER " << attacker << " ===================================\n";
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Places a card in the field zone
///
/// @param card card to be placed
/// @param player player number
/// @param fieldSlot field slot to check if the zone is free (-1 put in the first free position)
///
/// @return nothing
void Board::placeCard(std::shared_ptr<Creature> card, int player, int fieldSlot)
{
  if (fieldSlot == -1)
  {
    for (int slot = 0; slot < 7; slot++)
    {
      if (field_zone_[player][slot] == nullptr)
      {
        field_zone_[player][slot] = card;
        return;
      }
    }
  }
  else
    field_zone_[player][fieldSlot] = card;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Fetches a card from the battle zone
///
/// @param player player number
/// @param slot slot number
///
/// @return card from the battle zone, nullptr = slot empty
std::shared_ptr<Creature> Board::fetchBattleCard(int player, int slot)
{
  return battle_zone_[player - 1][slot];
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Fetches a card from the field zone
///
/// @param player player number
/// @param slot slot number
///
/// @return card from the field zone, nullptr = slot empty
std::shared_ptr<Creature> Board::fetchFieldCard(int player, int slot)
{
  return field_zone_[player - 1][slot];
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Places a card in the battle zone, and removes it from the field zone
///
/// @param card card to be placed
/// @param player player number
/// @param battle_pos battle position
/// @param field_pos field position
///
/// @return nothing
void Board::placeCardInBattle(std::shared_ptr<Creature> card, int player, int battle_pos, int field_pos)
{
  battle_zone_[player - 1][battle_pos] = card;
  field_zone_[player - 1][field_pos] = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Checks if a field slot is occupied
///
/// @param player player number
/// @param slot slot number
///
/// @return true = occupied, false = not occupied
bool Board::isFieldSlotOccupied(int player, int slot) const
{
  return field_zone_[player][slot] != nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Checks if a battle slot is occupied
///
/// @param player player number
/// @param slot slot number
///
/// @return true = occupied, false = not occupied
bool Board::isBattleSlotOccupied(int player, int slot) const
{
  return battle_zone_[player][slot] != nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Removes a card from the battle zone
///
/// @param player player number
/// @param slot slot number
///
/// @return nothing
void Board::removeCardFromBattle(int player, int slot)
{
  battle_zone_[player - 1][slot] = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Removes a card from the field zone
///
/// @param player player number
/// @param slot slot number
///
/// @return nothing
void Board::removeCardFromField(int player, int slot)
{
  field_zone_[player - 1][slot] = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Checks if all fields are full
///
/// @param player player number
///
/// @return true = all fields full, false = some field is empty
bool Board::areAllFieldsFull(int player)
{
  for (int i = 0; i < 7; i++)
  {
    if (field_zone_[player - 1][i] == nullptr)
    {
      return false;
    }
  }
  return true;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Damage dealt to creatures with Trait::P
///
/// @param player player number
/// @param message according message
///
/// @return nothing
void Board::damagePoisonedCreatures(int player, std::string message)
{
  for (int slot = 0; slot < 7; slot++)
  {
    if (field_zone_[player - 1][slot] != nullptr)
    {
      if (field_zone_[player - 1][slot]->checkTrait(Trait::P))
      {
        field_zone_[player - 1][slot]->damageCreature(1);
        std::cout << message << std::endl;
      }
    }
    if (battle_zone_[player - 1][slot] != nullptr)
    {
      if (battle_zone_[player - 1][slot]->checkTrait(Trait::P))
      {
        battle_zone_[player - 1][slot]->damageCreature(1);
        std::cout << message << std::endl;
      }
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Regenerate creatures with Trait::R
///
/// @param player player number
/// @param message according message
///
/// @return nothing
void Board::regenerateCreatures(int player, std::string message)
{
  int status;
  for (int slot = 0; slot < 7; slot++)
  {
    if (field_zone_[player - 1][slot] != nullptr)
    {
      if (field_zone_[player - 1][slot]->checkTrait(Trait::R))
      {
        status = field_zone_[player - 1][slot]->resetHealth();
        if (status)
          std::cout << message << std::endl;
      }
    }
    if (battle_zone_[player - 1][slot] != nullptr)
    {
      if (battle_zone_[player - 1][slot]->checkTrait(Trait::R))
      {
        status = battle_zone_[player - 1][slot]->resetHealth();
        if (status)
          std::cout << message << std::endl;
      }
    }
  }
}
