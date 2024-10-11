# ChessEngine

A fully functional Chess Engine implemented in C++ that allows users to play chess with a computer opponent. This engine includes a user-friendly interface, move generation, board evaluation, and more, following the rules of standard chess.

## Features

- **Move Generation**: Supports all legal chess moves, including castling, en passant, and pawn promotion.
- **Minimax Algorithm with Alpha-Beta Pruning**: Provides an AI opponent that can evaluate board positions and select the best possible moves.
- **Board Evaluation**: Evaluates positions based on material balance and other positional factors to make strategic decisions.
- **Two-Player Mode**: Allows users to play against the engine or a human opponent.
- **Graphical User Interface (GUI)**: Provides a simple interface for playing chess visually.
  
## Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/Itachi0906/ChessEngine.git
    cd Game1/src
    ```

2. Compile the project (assuming you have a C++ compiler installed):
    ```bash
    g++ -o chessengine main.cpp -std=c++17
    ```

3. Run the Chess Engine:
    ```bash
    ./chessengine
    ```

## How to Play

- Once the engine starts, you can choose to play against the AI or another player.
- Click the piece which you want to move then select the square where the piece should be placed.
- The AI will calculate and make its moves based on the current board state.

## Development

If you want to contribute to the development of the Chess Engine, here’s how you can set up the environment:

1. Fork the repository and clone it locally.
2. Create a new branch for your feature:
    ```bash
    git checkout -b feature-branch-name
    ```
3. Make your changes and test them locally.
4. Push the changes to your fork and open a pull request.

## To-Do List

- [ ] Add support for different difficulty levels.
- [ ] Implement additional board evaluation heuristics.
- [ ] Improve the GUI with more visual elements.
- [ ] Add unit tests for key components.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
