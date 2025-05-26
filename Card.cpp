#include <iostream>
#include <memory>

#include "Card.hpp"
#include "Creature.hpp"
#include "Spell.hpp"
#include "Exeption.hpp"
#include "Command.hpp"

Card::Card() : name_(""), ID_("") {}

Card::Card(std::string name, std::string ID) : name_(name), ID_(ID) {}

//---------------------------------------------------------------------------------------------------------------------
///
/// Virtual function to print the card
///
///
/// @return nothing
std::vector<std::string> Card::printCard() const
{
  return {};
}

void Card::changeEffect(std::string new_effect)
{
  effect = new_effect;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Creates a card from the given ID
///
/// @param ID card ID
/// 
/// @return pointer to the created card, nullptr = ID invalid
std::shared_ptr<Card> Card::createCardFromID(const std::string& ID)
{
  try
  {
    if (ID == "AGRAT")
      return std::make_shared<Creature>("Angry Rat", "AGRAT", 1, 1, 1, std::initializer_list<Trait>{H});
    if (ID == "CADET")
      return std::make_shared<Creature>("Cadet", "CADET", 1, 2, 1, std::initializer_list<Trait>{});
    if (ID == "FARMR")
      return std::make_shared<Creature>("Farmer", "FARMR", 2, 1, 1, std::initializer_list<Trait>{H});
    if (ID == "SQIRL")
      return std::make_shared<Creature>("Squirrel Soldier", "SQIRL", 1, 1, 1, std::initializer_list<Trait>{F});
    if (ID == "FSHLD")
      return std::make_shared<Creature>("Floating Shield", "FSHLD", 8, 0, 2, std::initializer_list<Trait>{});
    if (ID == "NITMR")
      return std::make_shared<Creature>("Nightmare", "NITMR", 1, 5, 2, std::initializer_list<Trait>{H, T});
    if (ID == "SOLDR")
      return std::make_shared<Creature>("Soldier", "SOLDR", 4, 3, 2, std::initializer_list<Trait>{});
    if (ID == "SNAKE")
      return std::make_shared<Creature>("Snake", "SNAKE", 1, 2, 2, std::initializer_list<Trait>{V});
    if (ID == "HWOLF")
      return std::make_shared<Creature>("Hungry Wolf", "HWOLF", 2, 3, 2, std::initializer_list<Trait>{B});
    if (ID == "ZOMBI")
      return std::make_shared<Creature>("Zombie", "ZOMBI", 2, 2, 2, std::initializer_list<Trait>{U});
    if (ID == "ASASN")
      return std::make_shared<Creature>("Assassin", "ASASN", 2, 5, 3, std::initializer_list<Trait>{F});
    if (ID == "CVLRY")
      return std::make_shared<Creature>("Cavalry", "CVLRY", 4, 4, 3, std::initializer_list<Trait>{H});
    if (ID == "GLDTR")
      return std::make_shared<Creature>("Gladiator", "GLDTR", 3, 5, 3, std::initializer_list<Trait>{C});
    if (ID == "KNGHT")
      return std::make_shared<Creature>("Knight", "KNGHT", 6, 4, 3, std::initializer_list<Trait>{H});
    if (ID == "VAMPS")
      return std::make_shared<Creature>("Vampire Soldier", "VAMPS", 3, 4, 3, std::initializer_list<Trait>{L});
    if (ID == "ALCHM")
      return std::make_shared<Creature>("Alchemist", "ALCHM", 6, 4, 4, std::initializer_list<Trait>{V});
    if (ID == "TUTOR")
      return std::make_shared<Creature>("Evil Tutor", "TUTOR", 4, 5, 4, std::initializer_list<Trait>{C, L});
    if (ID == "TURTL")
      return std::make_shared<Creature>("Giant Turtle", "TURTL", 11, 3, 4, std::initializer_list<Trait>{});
    if (ID == "NINJA")
      return std::make_shared<Creature>("Ninja", "NINJA", 4, 6, 4, std::initializer_list<Trait>{F, H});
    if (ID == "GUARD")
      return std::make_shared<Creature>("Eternal Guardian", "GUARD", 5, 5, 5, std::initializer_list<Trait>{H, U});
    if (ID == "RAPTR")
      return std::make_shared<Creature>("Raptor", "RAPTR", 4, 7, 5, std::initializer_list<Trait>{B, F});
    if (ID == "WRLCK")
      return std::make_shared<Creature>("Warlock", "WRLCK", 7, 4, 5, std::initializer_list<Trait>{L, V});
    if (ID == "GOLEM")
      return std::make_shared<Creature>("Golem", "GOLEM", 12, 5, 6, std::initializer_list<Trait>{R});
    if (ID == "HYDRA")
      return std::make_shared<Creature>("Hydra", "HYDRA", 6, 7, 6, std::initializer_list<Trait>{R, U});
    if (ID == "KINGV")
      return std::make_shared<Creature>("King V", "KINGV", 11, 6, 6, std::initializer_list<Trait>{C, H});
    if (ID == "LLICH")
      return std::make_shared<Creature>("Likeable Lich", "LLICH", 6, 9, 7, std::initializer_list<Trait>{L, U});
    if (ID == "T_REX")
      return std::make_shared<Creature>("T-Rex", "T_REX", 9, 13, 7, std::initializer_list<Trait>{B});
    if (ID == "VAMPL")
      return std::make_shared<Creature>("Vampire Lord", "VAMPL", 7, 10, 7, std::initializer_list<Trait>{C, L});
    if (ID == "ANGEL")
      return std::make_shared<Creature>("Angel", "ANGEL", 14, 9, 8, std::initializer_list<Trait>{H});
    if (ID == "DRAGN")
      return std::make_shared<Creature>("Dragon", "DRAGN", 10, 13, 8, std::initializer_list<Trait>{B, C});
    if (ID == "SLAYR")
      return std::make_shared<Creature>("Slayer", "SLAYR", 6, 15, 8, std::initializer_list<Trait>{F, H});
    if (ID == "D_GOD")
      return std::make_shared<Creature>("Demi-God", "D_GOD", 15, 15, 9, std::initializer_list<Trait>{R, U});
    if (ID == "DEVIL")
      return std::make_shared<Creature>("Devil", "DEVIL", 7, 16, 9, std::initializer_list<Trait>{B, F});

    // SPELLS
    if (ID == "BTLCY")
      return std::make_shared<Spell>("Battle Cry", "BTLCY", 3, "");
    if (ID == "METOR")
      return std::make_shared<Spell>("Meteor", "METOR", 4, "");
    if (ID == "FIRBL")
      return std::make_shared<Spell>("Fireball", "FIRBL", 5, "");

    if (ID == "CLONE")
      return std::make_shared<Spell>("Clone", "CLONE", 0, ""); // Cost: dynamic
    if (ID == "CURSE")
      return std::make_shared<Spell>("Death Curse", "CURSE", 0, "");
    if (ID == "SHOCK")
      return std::make_shared<Spell>("Shock", "SHOCK", 1, "");
    if (ID == "MOBLZ")
      return std::make_shared<Spell>("Mobilize", "MOBLZ", 2, "");
    if (ID == "RRUSH")
      return std::make_shared<Spell>("Rapid Rush", "RRUSH", 2, "");
    if (ID == "SHILD")
      return std::make_shared<Spell>("Shield", "SHILD", 2, "");
    if (ID == "AMPUT")
      return std::make_shared<Spell>("Amputate", "AMPUT", 3, "");
    if (ID == "FINAL")
      return std::make_shared<Spell>("Final Act", "FINAL", 3, "");
    if (ID == "LYLTY")
      return std::make_shared<Spell>("Loyalty", "LYLTY", 3, "");
    if (ID == "ZMBFY")
      return std::make_shared<Spell>("Zombify", "ZMBFY", 4, "");
    if (ID == "BLOOD")
      return std::make_shared<Spell>("Bloodlust", "BLOOD", 5, "");

    if (ID == "MEMRY")
      return std::make_shared<Spell>("Heroic Memory", "MEMRY", 0, "");
    if (ID == "REVIV")
      return std::make_shared<Spell>("Revive", "REVIV", 2, "");
  }
  catch (const std::bad_alloc &)
  {
    throw MemoryEx();
  }

  return nullptr;
}
