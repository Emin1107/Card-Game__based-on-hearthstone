#ifndef INIT_HPP
#define INIT_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>

#include "Player.hpp"
#include "Creature.hpp"
#include "Spell.hpp"

class Init
{
protected:
  Player &player1_;
  Player &player2_;
  std::vector<std::shared_ptr<Creature>> creature_codebook_;
  std::vector<std::shared_ptr<Spell>> spell_codebook_;

  std::map<std::string, std::string> errors_;
  std::map<std::string, std::string> infos_;
  std::map<std::string, std::string> descriptions_;
  std::string config_file_name_;
  std::string message_file_name_;
  std::string card_file_name_;
  int max_rounds_;

public:
  // Forward declarations
  Init(Player& player1, Player& player2, char* argv[]);
  ~Init();

  bool loadConfig();
  void parseDeckLines(std::string line, Player &player);
  void parseMessageLines();

  void loadCreatureCodes();
  void loadSpellCodes();

  bool loadCards();

  const std::vector<std::shared_ptr<Creature>> getCreatureCodebook() const;

  const std::vector<std::shared_ptr<Spell>> getSpellCodebook() const;

  const std::map<std::string, std::string> getErrors() const;
  const std::map<std::string, std::string> getInfos() const;
  const std::map<std::string, std::string> getDescriptions() const;

  int getMaxRounds() const;
};

#endif
