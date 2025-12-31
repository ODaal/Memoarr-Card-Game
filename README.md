# Memoarr-Card-Game
This project is a console-based C++ version of the Memoarr! memory game. It was developed for CSI2772 at the University of Ottawa. The game can be played by 2 to 4 players and uses an object-oriented design that represents all main parts of the game, including players, cards, the game board, decks, and rules.

The implementation includes the basic game and advanced expert modes. These feature an expert display mode and extra rule mechanics related to specific animals. The focus was on making the code easy to maintain and extend, with clean structures. It uses C++ features like templates, inheritance, const-correctness, operator overloading, and the standard library.

## Project Structure
Card.h / Card.cpp → Defines the base card abstraction (type, visibility, pairing logic).

CardDeck.h / CardDeck.cpp → Manages card collections, shuffling, and draw logic.

DeckFactory.h → Generic factory for creating and iterating through decks.

Board.h / Board.cpp → Represents the game board, handles card placement and state updates.

Player.h / Player.cpp → Stores player state (score, turn status, powers).

GameRules.h / GameRules.cpp → Implements Memoarr-specific rules (pair validation, power activation, turn handling).

Game.h / Game.cpp → High-level game controller coordinating players, board, and rules.

ExpertRules.cpp → Implements card power rules after a reveal (Crab repeat turn, Octopus swap adjacent, Turtle skip next, Walrus ban a cell, Penguin flip a visible card).

main.cpp → Program entry point; initializes the game and starts the main loop.
