#include "view.h"

namespace connect4 {

View::View(const Model& model)
        : model_(model)
{ }

void View::draw(ge211::Sprite_set& sprites, int mouse_column) const
{
    for (int col_no = 0; col_no < model_.m(); ++col_no) {
        auto const& column = model_.get_column(col_no);
        for (int row_no = 0; row_no < column.size(); ++row_no) {
            Player player = column[row_no];
            auto const& sprite =
                    player == Player::first ? player1_token_ : player2_token_;
            sprites.add_sprite(sprite, board_to_screen({col_no, row_no}));
        }
    }

    if (model_.is_good_col(mouse_column)) {
        auto col_no        = mouse_column;
        auto const& column = model_.get_column(col_no);
        auto row_no        = int(column.size());
        Player player      = model_.get_turn();
        if (row_no < model_.n() && player != Player::neither) {
            auto const& sprite =
                    player == Player::first ? move1_token_ : move2_token_;
            sprites.add_sprite(sprite, board_to_screen({col_no, row_no}));
        }
    }
}

ge211::Position View::board_to_screen(ge211::Position board_pos) const
{
    int x = 2 * token_radius * board_pos.x;
    int y = 2 * token_radius * (model_.n() - board_pos.y - 1);
    return {x, y};
}

ge211::Position View::screen_to_board(ge211::Position screen_pos) const
{
    int col_no = screen_pos.x / (2 * token_radius);
    int row_no = model_.n() - screen_pos.y / (2 * token_radius) - 1;
    return {col_no, row_no};
}

ge211::Dimensions View::screen_dimensions() const
{
    return {2 * token_radius * model_.m(),
            2 * token_radius * model_.n()};
}

} // end namespace connect4
