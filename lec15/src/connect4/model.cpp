#include "model.h"
#include <ge211.h>
#include <cassert>

namespace connect4 {

Player other_player(Player p)
{
    switch (p) {
        case Player::first: return Player::second;
        case Player::second: return Player::first;
        default:
            throw ge211::Client_logic_error("other_player: not a player");
    }
}

Model::Model(int k, int m, int n)
    : k_(k), m_(m), n_(n)
{
    assert(k > 1);
    assert(m > 1);
    assert(n > 1);

    while (columns_.size() < m) {
        columns_.emplace_back();
    }
}

void Model::place_token(int col_no)
{
    if (turn_ == Player::neither)
        throw ge211::Client_logic_error("place_token: game is over");

    auto& col = get_column_(col_no);

    if (col.size() == n_)
        throw ge211::Client_logic_error("place_token: column is full");

    col.push_back(turn_);

    update_winner_and_turn_(col_no);
}

void Model::update_winner_and_turn_(int const col_no)
{
    int const row_no = columns_[col_no].size() - 1;

    auto const count = [&](int dcol, int drow) {
        return count_from_by_(turn_, col_no, row_no, dcol, drow);
    };

    int below       = count( 0, -1);
    int left        = count(-1,  0);
    int right       = count( 1,  0);
    int above_left  = count(-1,  1);
    int above_right = count( 1,  1);
    int below_left  = count(-1, -1);
    int below_right = count( 1, -1);

    if (below + 1 >= k_ ||
            left + 1 + right >= k_ ||
            above_left + 1 + below_right >= k_ ||
            above_right + 1 + below_left >= k_) {
        winner_ = turn_;
    } else {
        for (const auto& column : columns_) {
            if (column.size() < n_) {
                turn_ = other_player(turn_);
                return;
            }
        }
    }

    turn_ = Player::neither;
}

const std::vector<Player>& Model::get_column(int col_no) const
{
    assert(is_good_col(col_no));
    return columns_[col_no];
}

std::vector<Player>& Model::get_column_(int col_no)
{
    assert(is_good_col(col_no));
    return columns_[col_no];
}

int Model::count_from_by_(Player goal,
                          int start_col, int start_row,
                          int change_col, int change_row) const
{
    int count = 0;

    for (;;) {
        start_col += change_col;
        start_row += change_row;

        if (!is_good_col_row_(start_col, start_row))
            return count;

        if (columns_[start_col][start_row] != goal)
            return count;

        ++count;
    }
}

bool Model::is_good_col(int col_no) const
{
    return 0 <= col_no && col_no < m_;
}

bool Model::is_good_col_row_(int col_no, int row_no) const
{
    return 0 <= col_no && col_no < m_ &&
            0 <= row_no && row_no < columns_[col_no].size();
}

} // end namespace connect4
