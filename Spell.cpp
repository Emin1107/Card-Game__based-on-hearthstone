#include <iostream>
#include <algorithm>

#include "Spell.hpp"

Spell::Spell(std::string name, std::string ID, int mana_cost, std::string effect)
    : Card(name, ID), mana_cost_(mana_cost), effect_(effect)
{
  has_mana_ = true;
}

//-----------------------------------------------------------------------------------------------------
///
/// Prints the spell card information
///
/// @param border_info border information
/// @param border_d border data
///
/// @return nothing
void Spell::printInfo(std::string border_info, std::string border_d)
{
  std::cout << border_info << std::endl;
  if (mana_cost_)
  {
    std::cout << getCardName() << " [" << getCardID() << "] " << "(" << getManaCost() << " mana" << ")" << std::endl;
  }
  else
  {
    std::cout << getCardName() << " [" << getCardID() << "] " << "(" << "XX mana" << ")" << std::endl;
  }
  std::cout << "Type: Spell" << std::endl
            << "Effect: " << getEffect() << std::endl
            << border_d << std::endl;
}

std::vector<std::string> Spell::printCard() const
{
  std::vector<std::string> card;

  // line 1
  std::string mana_str = std::to_string(mana_cost_);
  if (!mana_cost_)
  {
    mana_str = "XX";
  }
  else if (mana_str.length() == 1)
    mana_str = "0" + mana_str;
  else if (mana_cost_ > 99)
    mana_str = "**";

  card.push_back(" _____M" + mana_str);

  // line 2
  card.push_back("| " + ID_ + " |");

  // line 3
  card.push_back("|       |");

  // line 4
  card.push_back(" _______ ");

  return card;
}

//-----------------------------------------------------------------------------------------------------
///
/// Checks what type of spell it is
///
/// @param card_id card id
///
/// @return 0 = not a spell, 1 = General spell, 2 = Target spell, 3 = Graveyard spell
int Spell::getSpellCardType(std::string card_id)
{
  if (card_id == "BTLCY" || card_id == "METOR" || card_id == "FIRBL")
    return 1;
  else if (card_id == "CLONE" || card_id == "CURSE" || card_id == "SHOCK" ||
           card_id == "MOBLZ" || card_id == "RRUSH" || card_id == "SHILD" ||
           card_id == "AMPUT" || card_id == "FINAL" || card_id == "LYLTY" ||
           card_id == "ZMBFY" || card_id == "BLOOD")
    return 2;
  else if (card_id == "MEMRY" || card_id == "REVIV")
    return 3;

  return 0;
}

//-----------------------------------------------------------------------------------------------------
///
/// Processes the logic for all of the spells
///
/// @param card_id card id
/// @param player player
/// @param opponent opponent
/// @param board board
/// @param affected_creature affected creature
/// @param round current round number
///
/// @return nothing
void Spell::processSpell(std::string card_id, Player& player, Player& opponent,
                         Board &board, std::shared_ptr<Creature> &affected_creature, int round)
{
  if (card_id == "BTLCY")
  {
    std::shared_ptr<Creature> current_card = nullptr;
    for (int i = 0; i < 7; i++)
    {
      current_card = board.fetchFieldCard(player.getPlayerNumber(), i);
      if (current_card != nullptr)
      {
        current_card->increaseCurrentAttack(3);
        current_card->addTrait(H);
        current_card->addTrait(T);
      }

      current_card = board.fetchBattleCard(player.getPlayerNumber(), i);
      if (current_card != nullptr)
      {
        current_card->increaseCurrentAttack(3);
        current_card->addTrait(H);
        current_card->addTrait(T);
      }
    }
  }
  else if (card_id == "METOR")
  {
    for (int i = 0; i < 7; i++)
    {
      if (board.fetchBattleCard(1, i) != nullptr)
        board.fetchBattleCard(1, i)->damageCreature(3);
      if (board.fetchBattleCard(2, i) != nullptr)
        board.fetchBattleCard(2, i)->damageCreature(3);
    }
    for (int i = 0; i < 7; i++)
    {
      if (board.fetchFieldCard(1, i) != nullptr)
        board.fetchFieldCard(1, i)->damageCreature(3);
      if (board.fetchFieldCard(2, i) != nullptr)
        board.fetchFieldCard(2, i)->damageCreature(3);
    }
  }
  else if (card_id == "FIRBL")
  {
    std::shared_ptr<Creature> current_card = nullptr;
    for (int i = 0; i < 7; i++)
    {
      current_card = board.fetchBattleCard(opponent.getPlayerNumber(), i);
      if (current_card == nullptr)
        continue;
      current_card->damageCreature(2);
    }
    for (int i = 0; i < 7; i++)
    {
      current_card = board.fetchFieldCard(opponent.getPlayerNumber(), i);
      if (current_card == nullptr)
        continue;
      current_card->damageCreature(2);
    }
  }

  else if (card_id == "CLONE")
  {
    player.subtractMana((affected_creature->getManaCost() + 1) / 2);
    std::shared_ptr<Creature> cloned_creature = std::dynamic_pointer_cast<Creature>(createCardFromID(affected_creature->getCardID()));
    cloned_creature->setCurrentAttack(affected_creature->getCurrentAttack());
    cloned_creature->setCurrentHealth(affected_creature->getCurrentHealth());
    cloned_creature->setCurrentTraits(affected_creature->getTraits());
    cloned_creature->addTrait(H);
    cloned_creature->addTrait(T);
    cloned_creature->setRoundPlacement(round);
    board.placeCard(cloned_creature, player.getPlayerNumber() - 1, -1);
  }
  else if (card_id == "CURSE")
  {
    player.subtractMana(affected_creature->getManaCost() + 1);
    affected_creature->addTrait(T);
  }
  else if (card_id == "SHOCK")
  {
    affected_creature->damageCreature(1);
  }
  else if (card_id == "MOBLZ")
  {
    affected_creature->addTrait(H);

    affected_creature->increaseCurrentAttack(1);
  }
  else if (card_id == "RRUSH")
  {
    affected_creature->addTrait(F);
    affected_creature->addTrait(T);
    affected_creature->increaseCurrentAttack(2);
  }
  else if (card_id == "SHILD")
  {
    affected_creature->increaseCurrentHealth(2);
  }
  else if (card_id == "AMPUT")
  {
    affected_creature->removeTrait();
  }
  else if (card_id == "FINAL")
  {
    affected_creature->addTrait(B);
    affected_creature->addTrait(H);
    affected_creature->addTrait(T);
    affected_creature->increaseCurrentAttack(3);
  }
  else if (card_id == "LYLTY")
  {
    affected_creature->addTrait(H);
    affected_creature->increaseCurrentHealth(1);
  }
  else if (card_id == "ZMBFY")
  {
    affected_creature->addTrait(V);
    affected_creature->addTrait(U);
  }
  else if (card_id == "BLOOD")
  {
    affected_creature->addTrait(B);
    affected_creature->addTrait(L);
    affected_creature->setCurrentHealth((affected_creature->getCurrentHealth() + 1) / 2);
  }
  else if (card_id == "MEMRY")
  {
    if (!board.areAllFieldsFull(player.getPlayerNumber()))
    {
      player.removeFromGraveyard(affected_creature->getCardID());
      affected_creature->resetAttributes();
      affected_creature->addTrait(H);
      affected_creature->addTrait(T);
      affected_creature->setRoundPlacement(round);
      board.placeCard(affected_creature, player.getPlayerNumber() - 1, -1);
    }
    player.subtractMana(((affected_creature->getManaCost() + 1) / 2));
  }
  else if (card_id == "REVIV")
  {
    player.removeFromGraveyard(affected_creature->getCardID());
    affected_creature->resetAttributes();
    player.addCardToHand(affected_creature);
  }
}
