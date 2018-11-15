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
double const speed = 1.5;

//
// Model implementation
//

Model::Model(Model::Rectangle room)
        : room_(room)
        , player_(Rectangle::from_center(room.center(),
                                         player_dims.into<Coordinate>()))
{ }

Model::Rectangle Model::get_room() const
{
    return room_;
}

Model::Rectangle Model::get_player() const
{
    return player_;
}

void Model::move_player(Coordinate dx, Coordinate dy)
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
        , room_(model.get_room().dimensions().into<int>(), room_color)
        , player_(model.get_player().dimensions().into<int>(), player_color)
{ }

void View::draw(ge211::Sprite_set& sprites) const
{
    sprites.add_sprite(room_, model_.get_room().top_left().into<int>(), 0);
    sprites.add_sprite(player_, model_.get_player().top_left().into<int>(), 1);
}

//
// Controller
//

Controller::Controller(Model& model, View& view)
        : model_(model)
        , view_(view)
{ }

void Controller::update_mouse_position(ge211::Position position)
{
    started_ = true;
    mouse_ = position;
}

void Controller::pass_time(double seconds)
{
    if (!started_) return;

    auto displacement = mouse_.into<double>() - model_.get_player().center();
    auto change = speed * seconds * displacement;
    model_.move_player(change.width, change.height);
}

//
// Putting it all together
//

static Model::Rectangle room_with_margin(Dimensions margin)
{
    Dimensions screen = Abstract_game::default_window_dimensions;
    return Rectangle::from_center({screen.width / 2, screen.height / 2},
                                  screen - margin)
            .into<Model::Coordinate>();
}

Game::Game(Dimensions margin)
        : model_(room_with_margin(margin))
        , view_(model_)
        , controller_(model_, view_)
{ }

void Game::draw(Sprite_set& sprites)
{
    view_.draw(sprites);
}

void Game::on_mouse_move(Position position)
{
    controller_.update_mouse_position(position);
}

void Game::on_frame(double last_frame_seconds)
{
    controller_.pass_time(last_frame_seconds);
}

} // end namespace motion
