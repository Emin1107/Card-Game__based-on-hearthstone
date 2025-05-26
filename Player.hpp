#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "Card.hpp"
#include "Creature.hpp"

class Player
{
protected:
  int number_;
  int mana_;
  int mana_pool_;
  int health_;

  std::vector<std::shared_ptr<Card>> hand_cards_;
  std::vector<std::shared_ptr<Creature>> graveyard_;
  std::vector<std::shared_ptr<Card>> deck_;

  bool can_redraw_;

public:
  // Forward declarations
  Player(int number, int mana, int health, int mana_pool);
  ~Player();

  void setHealth(int health);
  void setManaPool(int mana_pool);
  void setMana(int mana) { mana_ = mana; }
  void addCardToDeck(std::shared_ptr<Card> card);
  void addCardToGraveyard(std::shared_ptr<Creature> card) { graveyard_.insert(graveyard_.begin(), card); }

  void returnHandToDeck();

  bool isDead() const;
  void drawCard();
  bool noCards() const;
  int getHandSize() const;

  void increaseManaPool();
  void printHand() const;
  void removeFromHand(std::string card_id);
  void removeFromGraveyard(std::string card_id);
  void removeUndyingFromGraveyard(std::vector<unsigned long> &graveyard_indexes);
  void addCardToHand(std::shared_ptr<Card> card) { hand_cards_.push_back(card); }

  friend std::ostream &operator<<(std::ostream &os, const Player &player);

  int getPlayerNumber() const;
  int getManaPool() const;
  int getMana() const;
  int getHealth() const;
  void drawInitialCards();
  void subtractMana(int mana);
  void damagePlayer(int damage) { health_ -= damage; }
  std::shared_ptr<Creature> getFromGraveyard(std::string card_id) const;

  const std::vector<std::shared_ptr<Card>> &getHand() const { return hand_cards_; }
  const std::vector<std::shared_ptr<Creature>> &getGraveyard() const { return graveyard_; }
  const std::vector<std::shared_ptr<Card>> &getDeck() const { return deck_; }

  void setRedrawToFalse();
  bool getRedrawStatus();
};

#endif
