#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

#include "Card.hpp"
#include "Creature.hpp"
#include "Creature.hpp"
#include "Player.hpp"
#include "Init.hpp"

Player::Player(int number, int mana, int health, int mana_pool) : number_(number),
                                                                  mana_(mana), mana_pool_(mana_pool), health_(health), can_redraw_(true) {}

//-----------------------------------------------------------------------------------------------------
///
/// Destructor, clears the hand, graveyard and deck vectors
///
/// @return nothing
Player::~Player()
{
  hand_cards_.clear();
  graveyard_.clear();
  deck_.clear();
}

//-----------------------------------------------------------------------------------------------------
///
/// Operator overloader for the << operator for printing the player information
///
/// @param os output stream
/// @param player player object
///
/// @return output stream
std::ostream& operator<<(std::ostream& os, const Player& player)
{
  os << "Health: " << player.health_ << std::endl
     << "Mana: " << player.mana_ << " / " << player.mana_pool_ << std::endl
     << "Remaining Deck: " << player.deck_.size() << " card(s)" << std::endl
     << "Graveyard Size: " << player.graveyard_.size() << " card(s)" << std::endl;
  return os;
}

void Player::setHealth(int health) { health_ = health; }

void Player::setManaPool(int mana_pool) { mana_pool_ = mana_pool; }

void Player::addCardToDeck(std::shared_ptr<Card> card)
{
  deck_.push_back(card);
}

//-----------------------------------------------------------------------------------------------------
///
/// Increases the mana pool by 1 and refills the mana to the new mana pool
///
/// @return nothing
void Player::increaseManaPool()
{
  mana_pool_++;
  setMana(mana_pool_);
}

//-----------------------------------------------------------------------------------------------------
///
/// Prints the hand of the player
///
/// @return nothing
void Player::printHand() const
{
  if (hand_cards_.empty())
  {
    return;
  }
  unsigned long card_row_size = hand_cards_.size() >= 7 ? 7 : hand_cards_.size();
  for (unsigned long card_row = 0; card_row < std::ceil(hand_cards_.size() / 7.); card_row++)
  {
    for (unsigned long row = 0; row < 4; row++)
    {
      std::cout << " ";
      for (unsigned long card_counter = 0 + card_row * 7; card_counter < card_row_size; card_counter++)
      {
        std::cout << "   ";
        std::cout << hand_cards_.at(card_counter)->printCard()[row];
      }
      std::cout << std::endl;
    }
    card_row_size += 7;
    if (card_row_size > hand_cards_.size())
    {
      card_row_size = hand_cards_.size();
    }
  }
}

int Player::getPlayerNumber() const
{
  return number_;
}

int Player::getManaPool() const { return mana_pool_; }

int Player::getHealth() const { return health_; }

//-----------------------------------------------------------------------------------------------------
///
/// Draws the initial 6 cards from the deck and adds them to the hand of the player
///
/// @return nothing
void Player::drawInitialCards()
{
  for (int i = 0; i < 6; i++)
  {
    hand_cards_.push_back(deck_.front());
    deck_.erase(deck_.begin());
  }
}

//-----------------------------------------------------------------------------------------------------
///
/// Checks if the player is dead
/// @return true = player is dead, false = player is alive
bool Player::isDead() const
{
  if (health_ <= 0)
    return true;
  return false;
}

//-----------------------------------------------------------------------------------------------------
///
/// Draws a single card from the deck and adds it to the hand of the player
///
/// @return nothing
void Player::drawCard()
{
  hand_cards_.push_back(deck_.front());
  deck_.erase(deck_.begin());
}

//-----------------------------------------------------------------------------------------------------
///
/// Checks if the player has no cards left in the deck
///
/// @return true = no cards left, false = cards left
bool Player::noCards() const
{
  if (deck_.empty())
    return true;

  return false;
}

int Player::getHandSize() const
{
  return hand_cards_.size();
}

//-----------------------------------------------------------------------------------------------------
///
/// Returns the whole hand to the deck
///
/// @return nothing
void Player::returnHandToDeck()
{
  for (auto &card : hand_cards_)
  {
    deck_.push_back(card);
  }
  hand_cards_.clear();
}

//-----------------------------------------------------------------------------------------------------
///
/// Removes a card from the hand
///
/// @param card_id card id
///
/// @return nothing
void Player::removeFromHand(std::string card_id)
{
  for (unsigned long i = 0; i < hand_cards_.size(); i++)
  {
    if (card_id == hand_cards_.at(i)->getCardID())
    {
      hand_cards_.erase(hand_cards_.begin() + i);
      return;
    }
  }
}

int Player::getMana() const
{
  return mana_;
}

void Player::subtractMana(int mana)
{
  mana_ -= mana;
}

void Player::setRedrawToFalse()
{
  can_redraw_ = false;
}

bool Player::getRedrawStatus()
{
  return can_redraw_;
}

//-----------------------------------------------------------------------------------------------------
///
/// Gets the card from the graveyard
///
/// @param card_id card id
///
/// @return card object, nullptr = card not in graveyard
std::shared_ptr<Creature> Player::getFromGraveyard(std::string card_id) const
{
  for (unsigned long i = 0; i < graveyard_.size(); i++)
  {
    if (card_id == graveyard_.at(i)->getCardID())
    {
      return graveyard_.at(i);
    }
  }
  return nullptr;
}

//-----------------------------------------------------------------------------------------------------
///
/// Removes a card to the graveyard
///
/// @param card_id card id
///
/// @return nothing
void Player::removeFromGraveyard(std::string card_id)
{
  for (unsigned long i = 0; i < graveyard_.size(); i++)
  {
    if (card_id == graveyard_.at(i)->getCardID())
    {
      graveyard_.erase(graveyard_.begin() + i);
      return;
    }
  }
}

//-----------------------------------------------------------------------------------------------------
///
/// Removes creatures with the Trait::U from the graveyard
///
/// @param graveyard_indexes indexes of the creatures to be removed
///
/// @return nothing
void Player::removeUndyingFromGraveyard(std::vector<unsigned long>& graveyard_indexes)
{
  std::sort(graveyard_indexes.begin(), graveyard_indexes.end(), std::greater<unsigned long>());
  for (unsigned long i = 0; i < graveyard_indexes.size(); i++)
  {
    if (graveyard_.size() >= graveyard_indexes[i])
    {
      graveyard_.erase(graveyard_.begin() + graveyard_indexes[i]);
    }
  }
}
