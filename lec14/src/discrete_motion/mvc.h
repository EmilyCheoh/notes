#pragma once

#include <ge211.h>

namespace motion
{

// Represents a rectangular player moving around a rectangular
// room.
class Model
{
public:
    // We'll represent rectangles using ge211's standard Rectangle type,
    // which is struct with four int fields: x, y, width, and height.
    using Rectangle = ge211::Rectangle;

    // Constructs a new model with the given room rectangle.
    explicit Model(Rectangle room);

    // Gets the dimensions of the room.
    Rectangle get_room() const;

    // Gets the rectangle of the player.
    Rectangle get_player() const;

    // Moves the player by the given x and y displacements. If
    // the motion would move the player outside the room, the
    // player moves only to the boundary instead.
    void move_player(int dx, int dy);

private:
    Rectangle player_;
    Rectangle const room_;
    // INVARIANT:
    //  - player_.x >= room_.x
    //  - player_.y >= room_.y
    //  - player_.x + player_.width <= room_.x + room_.width
    //  - player_.y + player_.height <= room_.y + room_.height
    // In other words, room_ contains player_.
};

class View
{
public:
    explicit View(Model const&);

    void draw(ge211::Sprite_set&) const;

private:
    Model const& model_;
    ge211::Rectangle_sprite room_;
    ge211::Rectangle_sprite player_;
};

class Game : public ge211::Abstract_game
{
public:
    explicit Game(ge211::Dimensions margin);

protected:
    // Controller:
    void draw(ge211::Sprite_set&) override;
    void on_key(ge211::Key) override;

private:
    Model model_;
    View view_;
};

} // end namespace motion
