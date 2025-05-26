#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "Creature.hpp"

Creature::Creature(std::string name, std::string ID, int base_health, int base_attack,
                   int mana_str_cost, std::vector<Trait> traits)
    : Card(name, ID),
      base_health_(base_health),
      current_health_(base_health),
      base_attack_(base_attack),
      current_attack_(base_attack),
      traits_(traits),
      base_traits_(traits),
      mana_cost_(mana_str_cost) {}

//-----------------------------------------------------------------------------------------------------
///
/// Prints the card information
///
/// @param border_info border information
/// @param border_d border data
///
/// @return nothing
void Creature::printInfo(std::string border_info, std::string border_d)
{
  std::cout << border_info << std::endl;
  if (mana_cost_)
  {
    std::cout << name_ << " [" << ID_ << "] " << "(" << mana_cost_ << " mana" << ")" << std::endl;
  }
  else
  {
    std::cout << name_ << " [" << ID_ << "] " << "(" << "XX mana" << ")" << std::endl;
  }
  std::cout << "Type: Creature" << std::endl
            << "Base Attack: " << base_attack_ << std::endl
            << "Base Health: " << base_health_ << std::endl
            << "Base Traits: ";
  for (auto it : traits_)
  {
    std::cout << nameFromTrait(it);
    if (it != Trait::NON && it != traits_.back())
    {
      std::cout << ", ";
    }
  }
  std::cout << "\n";
  std::cout << border_d << std::endl;
}

//-----------------------------------------------------------------------------------------------------
///
/// Converts a Trait to a string
///
/// @param trait trait letter
///
/// @return string representation of the trait
std::string Creature::nameFromTrait(Trait trait)
{
  switch (trait)
  {
  case Trait::B:
    return "Brutal";
  case Trait::C:
    return "Challenger";
  case Trait::F:
    return "First Strike";
  case Trait::H:
    return "Haste";
  case Trait::L:
    return "Lifesteal";
  case Trait::P:
    return "Poisoned";
  case Trait::R:
    return "Regenerate";
  case Trait::T:
    return "Temporary";
  case Trait::U:
    return "Undying";
  case Trait::V:
    return "Venomous";
  case Trait::NON:
    return "-";
  }
}

//-----------------------------------------------------------------------------------------------------
///
/// Converts a character to a Trait
///
/// @param c character representation of the trait
///
/// @return Trait enum
Trait Creature::traitFromChar(char c)
{
  switch (c)
  {
  case 'B':
    return Trait::B;
  case 'C':
    return Trait::C;
  case 'F':
    return Trait::F;
  case 'H':
    return Trait::H;
  case 'L':
    return Trait::L;
  case 'P':
    return Trait::P;
  case 'R':
    return Trait::R;
  case 'T':
    return Trait::T;
  case 'U':
    return Trait::U;
  case 'V':
    return Trait::V;
  default:
    return Trait::NON;
  }
}

//-----------------------------------------------------------------------------------------------------
///
///  Prints the card
///
/// @return vector of strings representing the card
std::vector<std::string> Creature::printCard() const
{
  std::vector<std::string> card;

  // line 1
  std::string mana_str = std::to_string(mana_cost_);
  if (mana_str.length() == 1)
    mana_str = "0" + mana_str;
  else if (mana_cost_ > 99)
    mana_str = "**";
  card.push_back(" _____M" + mana_str);

  // line 2
  card.push_back("| " + ID_ + " |");

  // line 3
  std::string traits_str = "| ";
  for (Trait t : traits_)
  {
    switch (t)
    {
    case B:
      traits_str += "B";
      break;
    case C:
      traits_str += "C";
      break;
    case F:
      traits_str += "F";
      break;
    case H:
      traits_str += "H";
      break;
    case L:
      traits_str += "L";
      break;
    case P:
      traits_str += "P";
      break;
    case R:
      traits_str += "R";
      break;
    case T:
      traits_str += "T";
      break;
    case U:
      traits_str += "U";
      break;
    case V:
      traits_str += "V";
      break;
    case NON:
      break;
    }
  }
  if (traits_str.size() > 7) // because of the "| " on line 120
  {
    traits_str = traits_str.substr(0, 6) + "+";
  }
  else
  {
    for (unsigned long spaces = 0; spaces < 5 - traits_.size(); spaces++)
    {
      traits_str += " ";
    }
  }
  traits_str += " |";
  card.push_back(traits_str);

  // line 4
  std::string attack_str = std::to_string(current_attack_);
  if (attack_str.length() == 1)
    attack_str = "0" + attack_str;
  else if (current_attack_ > 99)
    attack_str = "**";

  std::string health_str = std::to_string(current_health_);
  if (health_str.length() == 1)
    health_str = "0" + health_str;
  else if (current_health_ > 99)
    health_str = "**";

  card.push_back("A" + attack_str + "___H" + health_str);
  return card;
}

//-----------------------------------------------------------------------------------------------------
///
/// Adds a trait to the creature
///
/// @param t trait
///
/// @return nothing
void Creature::addTrait(Trait t)
{
  for (unsigned long i = 0; i < traits_.size(); i++)
  {
    if (traits_[i] == t)
    {
      return;
    }
  }
  traits_.push_back(t);
  sortTraits();
}

void Creature::damageCreature(int damage)
{
  current_health_ -= damage;
}

void Creature::setRoundPlacement(int round_number)
{
  placed_in_round_ = round_number;
}

//-----------------------------------------------------------------------------------------------------
///
/// Removes a trait from the creature
///
/// @return nothing
void Creature::removeTrait()
{
  if (traits_.empty())
  {
    return;
  }
  sortTraits();
  traits_.erase(traits_.begin());
}

//-----------------------------------------------------------------------------------------------------
///
/// Checks if the creature has a specific trait
///
/// @param t trait
///
/// @return true = trait found, false = trait not found
bool Creature::checkTrait(Trait t) const
{
  for (unsigned long i = 0; i < traits_.size(); i++)
  {
    if (traits_[i] == t)
    {
      return true;
    }
  }
  return false;
}

//-----------------------------------------------------------------------------------------------------
///
/// Resets the creature's attributes to their base values
///
/// @return nothing
void Creature::resetAttributes()
{
  current_health_ = base_health_;
  current_attack_ = base_attack_;
  traits_ = base_traits_;
}

//-----------------------------------------------------------------------------------------------------
///
/// Resets the creature's health to its base value
///
/// @return 0 = doesnt reset the health, 1 = health reset
int Creature::resetHealth()
{
  if (current_health_ >= base_health_)
    return 0;
  current_health_ = base_health_;
  return 1;
}

//-----------------------------------------------------------------------------------------------------
///
/// Removes the Undying trait from the creature
///
/// @return nothing
void Creature::removeUndying()
{
  auto it = std::find(traits_.begin(), traits_.end(), Trait::U);
  if (it != traits_.end())
  {
    traits_.erase(it);
  }
}

//-----------------------------------------------------------------------------------------------------
///
/// Sorts the traits of the creature
///
/// @return nothing
void Creature::sortTraits()
{
  std::sort(traits_.begin(), traits_.end());
}
