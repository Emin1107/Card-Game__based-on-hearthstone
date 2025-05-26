#include <iostream>
#include <memory>

#include "Exeption.hpp"
#include "Init.hpp"
#include "Card.hpp"
#include "Player.hpp"

Init::Init(Player &player1, Player &player2, char *argv[])
    : player1_(player1), player2_(player2), max_rounds_(0)
{
  config_file_name_ = argv[1];
  message_file_name_ = argv[2];
  card_file_name_ = "data/card_codebook.txt";
}

Init::~Init() {}

//-----------------------------------------------------------------------------------------------------
///
/// Loads the config file, throws an error if the file is invalid
///
/// @return true = success, false = failure
bool Init::loadConfig()
{
  std::ifstream config_file(config_file_name_);
  if (!config_file.is_open())
  {
    throw file_error(config_file_name_);
  }

  std::string line;
  std::string deck1_line, deck2_line;

  std::getline(config_file, line);
  if (line != "GAME")
  {
    throw file_error(config_file_name_);
  }

  int player_health, deck_size, mana_pool_size;
  config_file >> player_health >> max_rounds_ >> deck_size >> mana_pool_size;

  std::getline(config_file, line);
  std::getline(config_file, line);

  std::getline(config_file, deck1_line);
  std::getline(config_file, deck2_line);

  player1_.setHealth(player_health);
  player2_.setHealth(player_health);
  player1_.setManaPool(mana_pool_size);
  player2_.setManaPool(mana_pool_size);
  player1_.setMana(mana_pool_size);
  player2_.setMana(mana_pool_size);

  parseDeckLines(deck1_line, player1_);
  parseDeckLines(deck2_line, player2_);

  config_file.close();

  return true;
}

//-----------------------------------------------------------------------------------------------------
///
/// Parses the deck lines and adds the cards to the player's deck
///
/// @param line line from the config file
/// @param player player to add the cards to
///
/// @return nothing
void Init::parseDeckLines(std::string line, Player& player)
{
  std::string deck_line = line;
  unsigned long pos = 0;
  while ((pos = deck_line.find(';')) != std::string::npos)
  {
    std::string ID = deck_line.substr(0, pos);
    deck_line.erase(0, pos + 1);

    std::shared_ptr<Card> card = Card::createCardFromID(ID);
    std::string effect = infos_["I_" + ID];
    card->changeEffect(effect);

    player.addCardToDeck(card);
  }

  if (!deck_line.empty())
  {
    std::shared_ptr<Card> card = Card::createCardFromID(deck_line);
    player.addCardToDeck(card);
  }
}

//-----------------------------------------------------------------------------------------------------
///
/// Parses the message lines from the message config file and adds them to the respective maps
///
/// @return nothing
void Init::parseMessageLines()
{
  std::ifstream config_file(message_file_name_);
  if (!config_file.is_open())
  {
    throw file_error(message_file_name_);
  }

  int line_number = 0;
  std::string line;
  while (std::getline(config_file, line))
  {
    line_number++;
    if (line_number == 1)
    {
      if (line != "MESSAGE")
      {
        throw file_error(message_file_name_);
      }
      continue;
    }

    size_t pos = line.find(':');
    std::string id = line.substr(0, pos);
    std::string message = line.substr(pos + 1);

    if (id[0] == 'E')
    {
      errors_[id] = message;
    }
    else if (id[0] == 'I')
    {
      infos_[id] = message;
    }
    else
    {
      descriptions_[id] = message;
    }
  }
  config_file.close();
}

//-----------------------------------------------------------------------------------------------------
///
/// Loads the creatures from the self defined card codebook file
///
/// @return nothing
void Init::loadCreatureCodes()
{
  std::ifstream config_file(card_file_name_);
  if (!config_file.is_open())
  {
    throw file_error(card_file_name_);
  }
  std::string line;
  bool in_creature_section = false;

  while (std::getline(config_file, line))
  {
    if (line.empty())
    {
      continue;
    }
    if (line == "creature")
    {
      in_creature_section = true;
      continue;
    }
    else if (line == "spell")
    {
      break;
    }

    if (in_creature_section)
    {
      auto creature = std::make_shared<Creature>();
      std::string deck_line = line;
      int pos = 0;
      pos = deck_line.find(';');
      creature->setManaCost(std::stoi(deck_line.substr(0, pos)));
      deck_line.erase(0, pos + 1);

      pos = deck_line.find(';');
      creature->setCardID(deck_line.substr(0, pos));
      deck_line.erase(0, pos + 1);

      pos = deck_line.find(';');
      creature->setCardName(deck_line.substr(0, pos));
      deck_line.erase(0, pos + 1);

      pos = deck_line.find(';');
      std::string traits_str = deck_line.substr(0, pos);
      deck_line.erase(0, pos + 1);

      std::vector<Trait> traits;
      for (char ch : traits_str)
      {
        traits.push_back(creature->traitFromChar(ch));
      }

      creature->setBaseTraits(traits);

      pos = deck_line.find(';');
      creature->setBaseAttack(std::stoi(deck_line.substr(0, pos)));
      deck_line.erase(0, pos + 1);

      creature->setBaseHealth(std::stoi(deck_line));

      creature_codebook_.push_back(creature);
    }
  }
  config_file.close();
}

//-----------------------------------------------------------------------------------------------------
///
/// Loads the spells from the self defined card codebook file
///
/// @return nothing
void Init::loadSpellCodes()
{
  std::ifstream config_file(card_file_name_);
  if (!config_file.is_open())
  {
    throw file_error(card_file_name_);
  }
  std::string line;
  bool in_spell_section = false;

  while (std::getline(config_file, line))
  {
    if (line.empty())
    {
      continue;
    }
    if (line == "spell")
    {
      in_spell_section = true;
      continue;
    }

    if (in_spell_section)
    {
      auto spell = std::make_shared<Spell>();
      std::string deck_line = line;
      int pos = 0;

      pos = deck_line.find(';');
      if (deck_line.substr(0, pos) == "x")
      {
        spell->setXXManaCost();
      }
      else
      {
        spell->setManaCost(std::stoi(deck_line.substr(0, pos)));
      }
      deck_line.erase(0, pos + 1);

      pos = deck_line.find(';');
      spell->setCardID(deck_line.substr(0, pos));
      deck_line.erase(0, pos + 1);

      pos = deck_line.find(';');
      spell->setCardName(deck_line.substr(0, pos));
      deck_line.erase(0, pos + 1);

      spell->setEffect(deck_line);

      spell_codebook_.push_back(spell);
    }
  }

  config_file.close();
}

const std::vector<std::shared_ptr<Creature>> Init::getCreatureCodebook() const
{
  return creature_codebook_;
}

const std::vector<std::shared_ptr<Spell>> Init::getSpellCodebook() const
{
  return spell_codebook_;
}

const std::map<std::string, std::string> Init::getErrors() const
{
  return errors_;
}

const std::map<std::string, std::string> Init::getInfos() const
{
  return infos_;
}

const std::map<std::string, std::string> Init::getDescriptions() const
{
  return descriptions_;
}

int Init::getMaxRounds() const
{
  return max_rounds_;
}
