#pragma once

#include "model.h"
#include <ge211.h>

namespace connect4 {

// The radius of each rendered token.
int const token_radius = 50;

// Colors of rendered tokens.
ge211::Color const first_color       = ge211::Color::medium_red();
ge211::Color const second_color      = ge211::Color::medium_blue();
ge211::Color const first_move_color  = first_color.lighten(0.3);
ge211::Color const second_move_color = second_color.lighten(0.3);

// The view class, which encapsulates rendering state and knowledge.
class View
{
public:
    /// The view has const& access to the model--it can see it but cannot
    /// change it.
    explicit View(Model const&);

    /// Renders the current state of the model.
    void draw(ge211::Sprite_set&, int mouse_column) const;

    /// The actual screen dimensions of the required game window.
    ge211::Dimensions screen_dimensions() const;

    /// Converts a logical board position to a physical screen position.
    ge211::Position board_to_screen(ge211::Position) const;

    /// Converts a physical screen position to a logical board position.
    ge211::Position screen_to_board(ge211::Position) const;

private:
    /// The model.
    Model const& model_;

    /// The sprites.
    ge211::Circle_sprite const player1_token_{token_radius, first_color};
    ge211::Circle_sprite const player2_token_{token_radius, second_color};
    ge211::Circle_sprite const move1_token_{token_radius, first_move_color};
    ge211::Circle_sprite const move2_token_{token_radius, second_move_color};
};

} // end namespace connect4

