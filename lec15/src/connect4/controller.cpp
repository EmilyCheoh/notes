#include "controller.h"

using namespace connect4;
using namespace ge211;

namespace connect4 {

Controller::Controller(int k)
        : model_(k, k + 3, k + 2)
        , view_(model_)
        , mouse_column_(-1)
{ }

void Controller::draw(Sprite_set& set)
{
    view_.draw(set, mouse_column_);
}

void Controller::on_mouse_move(Position screen_pos)
{
    mouse_column_ = view_.screen_to_board(screen_pos).x;
}

Dimensions Controller::initial_window_dimensions() const
{
    return view_.screen_dimensions();
}

std::string Controller::initial_window_title() const
{
    return "Connect Four";
}

void Controller::on_mouse_down(Mouse_button btn, Position screen_posn)
{
    if (model_.get_turn() == Player::neither) return;
    if (btn != Mouse_button::left) return;

    int col_no = view_.screen_to_board(screen_posn).x;
    if (!model_.is_good_col(col_no)) return;

    if (model_.get_column(col_no).size() < model_.n()) {
        model_.place_token(col_no);
    }
}

} // end namespace connect4