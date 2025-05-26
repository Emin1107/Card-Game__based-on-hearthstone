#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <vector>
#include <string>
#include "Player.hpp"
#include "Creature.hpp"

class Board
{
protected:
  std::vector<std::shared_ptr<Creature>> field_zone_[2];
  std::vector<std::shared_ptr<Creature>> battle_zone_[2];
  bool is_active_;

public:
  // Forward declarations
  Board();
  ~Board();

  void printBoard(int defender, std::string border_A, std::string border_B) const;
  void placeCardInBattle(std::shared_ptr<Creature> card, int player, int battle_slot, int field_pos);
  bool isFieldSlotOccupied(int player, int slot) const;
  bool isBattleSlotOccupied(int player, int slot) const;
  bool areAllFieldsFull(int player);
  void damagePoisonedCreatures(int player, std::string message);
  void regenerateCreatures(int player, std::string message);
  void placeCard(std::shared_ptr<Creature> card, int player, int fieldSlot);
  void toggleActive() { is_active_ = !is_active_; };
  bool isActive() const { return is_active_; };
  std::shared_ptr<Creature> fetchBattleCard(int player, int slot);
  std::shared_ptr<Creature> fetchFieldCard(int player, int slot);
  void removeCardFromBattle(int player, int slot);
  void removeCardFromField(int player, int slot);
};

#endif
