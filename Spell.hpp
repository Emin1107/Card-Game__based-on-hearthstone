#ifndef SPELL_HPP
#define SPELL_HPP

#include <iostream>

#include "Card.hpp"
#include <string>
#include "Player.hpp"
#include "Board.hpp"

class Spell : public Card
{
protected:
  bool has_mana_;
  int mana_cost_;
  std::string effect_;

public:
  // Forward declarations
  Spell() {};
  Spell(std::string name, std::string ID, int mana_cost, std::string effect);
  ~Spell() = default;
  Spell(const Spell &) = delete;

  void printInfo(std::string border_info, std::string border_d) override;
  std::vector<std::string> printCard() const override;

  void setManaCost(int mana_cost) { mana_cost_ = mana_cost; }
  int getManaCost() { return mana_cost_; }
  void setXXManaCost() { mana_cost_ = 0; }

  void setEffect(std::string effect) { effect_ = effect; }
  std::string getEffect() { return effect_; }

  int getSpellCardType(std::string cardID);
  void processSpell(std::string card_id, Player &player, Player &opponent, Board &board,
                    std::shared_ptr<Creature> &affected_creature, int round);
  bool hasMana() { return has_mana_; }
};

#endif
