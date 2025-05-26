# Card Game

[![C++](https://img.shields.io/badge/language-C++-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B)
[![OOP](https://img.shields.io/badge/paradigm-OOP-brightgreen)]()
[![Status](https://img.shields.io/badge/status-completed-success)]()
[![Team](https://img.shields.io/badge/team-3%20members-blueviolet)]()
[![License](https://img.shields.io/badge/license-MIT-lightgrey.svg)]()

## Overview

This C++ project implements a complex card game simulator with dynamic creature/spell interactions and AI opponents. Developed as a university assignment by a team of three students, it features sophisticated game mechanics, board management, and both rule-based and Minimax AI strategies. Players can battle using creatures with special traits and spells with unique effects through a console interface or predefined AI matches.

## Features

- **Creature Management**: Play creatures with traits (Brutal, Lifesteal, Venomous, etc.)
- **Spell System**: 15+ spell types with dynamic effects (Clone, Curse, Meteor, etc.)
- **Battle Phase**: Automated combat resolution with trait interactions
- **AI Opponent**: Rule-based logic with Minimax algorithm support (tba.)
- **Deck Building**: Load custom card sets from configuration files

## Learning Objectives

- Object-oriented design patterns
- Smart pointer memory management
- AI decision-making algorithms (tba.)

## How to Run

Compile with:
```bash
g++ -std=c++17 -o cardgame *.cpp -lstdc++fs
```
Run with:
```bash
./cardgame data/m2_game_config.txt data/message_config.txt
```

## Command Summary

| Command                        | Description |
|-------------------------------|-------------|
| `help`                        | Show help message |
| `graveyard`                   | Prints a list of all cards that are in the current player's graveyard |
| `board`                       | Activates / deactivates the board printing |
| `hand`                        | Prints a list of all cards that are in the current player's hand |
| `info <CARD_ID>`              | Prints the card informations based on the card ID |
| `redraw`                      | Redraws the hand if its not good |
| `status`                      | Prints general information about the current status of the game |
| `done`                        | Finishes the turn of a player and starts the next phase |
| `battle <FIELD_SLOT> <BATTLE_SLOT>` | Adds a card from the field to the battle |
| `creature <HAND_CARD_ID> <FIELD_SLOT>` | Places a creature card from the hand to the field |
| `spell <HAND_CARD_ID> <OPTIONAL_ADDITIONAL_PARAMETER>` | Plays a spell card from the player's hand |
| `quit`                        | Exit the program |

## Technical Highlights

- **Technical Depth**: Highlighting OOP and (AI aspects tba.)
- **Visual Consistency**: Same badges/formatting patterns
- **Game-Specific Elements**: Added card traits, spell types, (AI architecture tba.)

## Return Codes

| Code | Meaning |
|------|---------|
| 0    | Success |
| 1    | Memory allocation error |
| 2    | Wrong number of command line parameters |
| 3    | Config file could not be opened for reading, or does not start with correct magic number |

## Project Structure

```text
.
├── Game.hpp/cpp         # Core game logic
├── Player.hpp/cpp       # Player state and deck
├── Card.hpp/cpp         # Base card system
├── Creature.hpp/cpp     # Creature implementations
├── Spell.hpp/cpp        # Spell implementations  
├── Board.hpp/cpp        # Battle/field management
└── main.cpp             # All logic combined
```

## License

This project is for academic purposes. Use freely under the MIT License.
