#ifndef CARD_HPP
#define CARD_HPP

#include <iostream>
#include <vector>
#include <string>
#include <memory>

class Card
{
protected:
  std::string name_;
  std::string ID_;
  std::string effect = "";

public:
  // Forward declarations
  Card();
  Card(std::string name, std::string ID);
  virtual ~Card() = default;

  virtual void printInfo(std::string border_info, std::string border_d) = 0;
  static std::shared_ptr<Card> createCardFromID(const std::string &ID);
  virtual std::vector<std::string> printCard() const;

  void changeEffect(std::string new_effect);

  void setCardID(std::string ID) { ID_ = ID; }
  void setCardName(std::string name) { name_ = name; }

  std::string getCardID() const { return ID_; }
  std::string getCardName() const { return name_; }
};

#endif
