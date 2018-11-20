#pragma once

#include <vector>

namespace connect4 {

/// How we represent players or the absence thereof.
enum class Player
{
    first, second, neither
};

/// Returns the other player, if given Player::first or Player::second;
/// throws ge211::Client_logic_error if given Player::neither.
Player other_player(Player);

/// Models a Connect Four game. The parameters are `k` (the length of the
/// needed line), `m` (the width of the board), and `n` (the height of the
/// board).
class Model
{
public:
    /// Constructs a Connect-K-M-N game model. Default arguments give
    /// standard Connect Four. Otherwise:
    ///
    /// \param k the number of tokens to connect
    /// \param m the width of the grid
    /// \param n the height of the grid
    explicit Model(int k = 4, int m = 7, int n = 6);

    int k() const { return k_; }
    int m() const { return m_; }
    int n() const { return n_; }

    /// Gets a view of the given column.
    ///
    /// **PRECONDITION:** `0 <= col_no < m()`
    const std::vector<Player>& get_column(int col_no) const;

    /// Places the token for the given player in the given column, provided
    /// that column is not full.
    ///
    /// **PRECONDITIONS**:
    ///  - `0 <= col_no < m()`
    ///  - `get_column(col_no).size() < n()`
    void place_token(int col_no);

    /// Is the column number within bounds?
    bool is_good_col(int col_no) const;

    /// Returns whose turn it is, or Player::neither for game over.
    Player get_turn() const { return turn_; };

    /// Returns the winner, or Player::neither for stalemates or when
    /// the game is not yet over.
    Player get_winner() const { return winner_; };

private:
    // The game parameters.
    int k_;
    int m_;
    int n_;

    // The current turn.
    Player turn_   = Player::first;

    // The winning player, if any.
    Player winner_ = Player::neither;

    // The board itself.
    std::vector<std::vector<Player>> columns_;
    // INVARIANT:
    //  - columns_.size() == m_
    //  - for (const auto& column : columns) column.size() <= n_
    //  - none of the Player values is Player::neither

    //
    // Private helpers:
    //

    /// Gets a mutable view of the given column.
    ///
    /// **PRECONDITION:** `col_no < m()`
    std::vector<Player>& get_column_(int col_no);

    /// **PRECONDITION** `col_no` is valid and where the last move
    /// was played.
    void update_winner_and_turn_(int col_no);

    /// Counts the number of instances of `goal`, moving in direction
    /// {change_col, change_row} starting at {start_col, start_row}, and
    /// not counting the Player at the starting position.
    int count_from_by_(Player goal,
                       int start_col, int start_row,
                       int change_col, int change_row) const;

    /// Returns whether there is a token at the given position.
    bool is_good_col_row_(int col_no, int row_no) const;
};

} // end namespace connect4