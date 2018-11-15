#include "mvc.h"
#include <algorithm>

using namespace ge211;

namespace motion
{

// Model constants
Dimensions const player_dims{25, 25};

// View constants
Color const room_color = Color::medium_red().darken(.5);
Color const player_color = Color::white();

// Controller constants
int const speed = 5;

//
// Model implementation
//

Model::Model(Model::Rectangle room)
        : room_(room)
        , player_(Rectangle::from_center(room.center(), player_dims))
{ }

Rectangle Model::get_room() const
{
    return room_;
}

Rectangle Model::get_player() const
{
    return player_;
}

void Model::move_player(int dx, int dy)
{
    if (dx > 0) {
        player_.x = std::min(player_.x + dx,
                             room_.x + room_.width - player_.width);
    } else {
        player_.x = std::max(player_.x + dx, room_.x);
    }

    if (dy > 0) {
        player_.y = std::min(player_.y + dy,
                             room_.y + room_.height - player_.height);
    } else {
        player_.y = std::max(player_.y + dy, room_.y);
    }
}

//
// View implementation
//

View::View(Model const& model)
        : model_(model)
        , room_(model.get_room().dimensions(), room_color)
        , player_(model.get_player().dimensions(), player_color)
{ }

void View::draw(ge211::Sprite_set& sprites) const
{
    sprites.add_sprite(room_, model_.get_room().top_left(), 0);
    sprites.add_sprite(player_, model_.get_player().top_left(), 1);
}

//
// Controller
//

static Rectangle room_with_margin(Dimensions margin)
{
    Dimensions screen = Abstract_game::default_window_dimensions;
    return Rectangle::from_center({screen.width / 2, screen.height / 2},
                                  screen - margin);
}

Game::Game(Dimensions margin)
        : model_(room_with_margin(margin))
        , view_(model_)
{ }

void Game::draw(Sprite_set& sprites)
{
    view_.draw(sprites);
}

void Game::on_key(Key key)
{
    if (key == Key::right())
        model_.move_player(speed, 0);
    else if (key == Key::left())
        model_.move_player(-speed, 0);
    else if (key == Key::down())
        model_.move_player(0, speed);
    else if (key == Key::up())
        model_.move_player(0, -speed);
    else if (key == Key::code('q'))
        quit();
}

} // end namespace motion
