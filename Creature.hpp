#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <iostream>
#include <vector>
#include <string>

#include "Card.hpp"
#include "Command.hpp"

enum Trait
{
  NON,
  B,
  C,
  F,
  H,
  L,
  P,
  R,
  T,
  U,
  V
};

class Creature : public Card
{
protected:
  int base_health_;
  int current_health_;
  int base_attack_;
  int current_attack_;
  std::vector<Trait> traits_; // base_traits_
  std::vector<Trait> base_traits_;
  int mana_cost_;
  std::string effect;
  int placed_in_round_;

public:
  // Forward declarations
  Creature() {};
  Creature(std::string name, std::string ID, int base_health, int base_attack, int mana_cost_, std::vector<Trait> traits);
  ~Creature() = default;

  void printInfo(std::string border_info, std::string border_d) override;
  Trait traitFromChar(char c);
  std::string nameFromTrait(Trait t);
  std::vector<std::string> printCard() const override;

  void setManaCost(int mana_cost) { mana_cost_ = mana_cost; }
  void setBaseTraits(const std::vector<Trait> &traits) { traits_ = traits; }
  void setBaseAttack(int base_attack) { base_attack_ = base_attack; }
  void setBaseHealth(int base_health) { base_health_ = base_health; }
  void resetAttributes();
  int resetHealth();
  void removeUndying();

  int getCurrentAttack() const { return current_attack_; }
  int getCurrentHealth() const { return current_health_; }
  int getManaCost() const { return mana_cost_; }
  int getBaseAttack() const { return base_attack_; }
  int getBaseHealth() const { return base_health_; }
  const std::vector<Trait> &getTraits() const { return traits_; }
  void increaseCurrentAttack(int attack) { current_attack_ += attack; }
  void increaseCurrentHealth(int health) { current_health_ += health; }
  void removeTrait();
  void addTrait(Trait t);
  void damageCreature(int damage);
  void setCurrentAttack(int attack) { current_attack_ = attack; }
  void setCurrentHealth(int health) { current_health_ = health; }
  void setCurrentTraits(const std::vector<Trait> &traits) { traits_ = traits; }
  bool isDead() const { return current_health_ <= 0; }
  bool checkTrait(Trait t) const;

  int getRoundPlacement() const { return placed_in_round_; };
  void setRoundPlacement(int round_number);
  void sortTraits();
};

#endif
