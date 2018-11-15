#pragma once

#include <ge211.h>

namespace motion
{

// Represents a rectangular player moving around a rectangular
// room.
class Model
{
public:
    // We want to use doubles as coordinates instead of ints.
    using Coordinate = double;

    // We'll represent rectangles using ge211's Basic_rectangle type,
    // with four double fields: x, y, width, and height.
    using Rectangle = ge211::Basic_rectangle<Coordinate>;

    // Constructs a new model with the given room rectangle.
    explicit Model(Rectangle room);

    // Gets the dimensions of the room.
    Rectangle get_room() const;

    // Gets the rectangle of the player.
    Rectangle get_player() const;

    // Moves the player by the given x and y displacements. If
    // the motion would move the player outside the room, the
    // player moves only to the boundary instead.
    void move_player(Coordinate dx, Coordinate dy);

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

// Stores objects needed for rendering the game to the screen, and
// contains logic for doing said rendering.
class View
{
public:
    // When we construct a view, it needs const access to the model.
    explicit View(Model const&);

    // Renders the room and player at the appropriate positions by
    // adding them to the given Sprite_set.
    void draw(ge211::Sprite_set&) const;

private:
    Model const& model_;
    ge211::Rectangle_sprite room_;      // image of room
    ge211::Rectangle_sprite player_;    // image of player
};

// Stores control state and encapsulates control logic.
class Controller
{
public:
    Controller(Model&, View&);

    // Record that the mouse position has changed.
    void update_mouse_position(ge211::Position);

    // Update the model for the passage of time.
    void pass_time(double seconds);

private:
    bool started_ = false;          // is `mouse_` valid?
    ge211::Position mouse_{0, 0};   // last known mouse position
    Model& model_;
    View& view_;
};

// Packages the components together and routes control events
// to the controller.
class Game : public ge211::Abstract_game
{
public:
    explicit Game(ge211::Dimensions margin);

protected:
    void draw(ge211::Sprite_set&) override;
    void on_mouse_move(ge211::Position) override;
    void on_frame(double last_frame_seconds) override;

private:
    Model model_;
    View view_;
    Controller controller_;
};

} // end namespace motion
