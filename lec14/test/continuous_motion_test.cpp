#include "continuous_motion/mvc.h"
#include <catch.h>

using Rectangle = motion::Model::Rectangle;

Rectangle const room1{0, 0, 300, 200};
Rectangle const room2{10, 10, 280, 180};

TEST_CASE("Model(Rectangle)")
{
    motion::Model model1(room1);
    motion::Model model2(room2);
}

TEST_CASE("Model::get_room()")
{
    CHECK(motion::Model(room1).get_room() == room1);
    CHECK(motion::Model(room2).get_room() == room2);
}

bool inside(Rectangle const& inner, Rectangle const&
outer)
{
    return inner.x >= outer.x &&
            inner.y >= outer.y &&
            inner.x + inner.width <= outer.x + outer.width &&
            inner.y + inner.height <= outer.y + outer.height;
}

TEST_CASE("Model::get_player()")
{
    motion::Model model1(room1);
    CHECK(inside(model1.get_player(), room1));

    motion::Model model2(room2);
    CHECK(inside(model2.get_player(), room2));
}

void move_right(Rectangle room)
{
    motion::Model model(room);
    auto player = model.get_player();

    model.move_player(1, 0);
    player.x += 1;
    CHECK(model.get_player() == player);

    model.move_player(2, 0);
    player.x += 2;
    CHECK(model.get_player() == player);
}

TEST_CASE("Model::move_player([right])")
{
    move_right(room1);
    move_right(room2);
}

void move_left(Rectangle room)
{
    motion::Model model(room);
    auto player = model.get_player();

    model.move_player(-1, 0);
    player.x -= 1;
    CHECK(model.get_player() == player);

    model.move_player(-2, 0);
    player.x -= 2;
    CHECK(model.get_player() == player);
}

TEST_CASE("Model::move_player([left])")
{
    move_left(room1);
    move_left(room2);
}

void move_down(Rectangle room)
{
    motion::Model model(room);
    auto player = model.get_player();

    model.move_player(0, 1);
    player.y += 1;
    CHECK(model.get_player() == player);

    model.move_player(0, 2);
    player.y += 2;
    CHECK(model.get_player() == player);
}

TEST_CASE("Model::move_player([down])")
{
    move_down(room1);
    move_down(room2);
}

void move_up(Rectangle room)
{
    motion::Model model(room);
    auto player = model.get_player();

    model.move_player(0, -1);
    player.y -= 1;
    CHECK(model.get_player() == player);

    model.move_player(0, -2);
    player.y -= 2;
    CHECK(model.get_player() == player);
}

TEST_CASE("Model::move_player([up])")
{
    move_up(room1);
    move_up(room2);
}

void move_out_of_bounds(Rectangle room)
{
    motion::Model model(room);
    auto player = model.get_player();

    model.move_player(2 * room1.width, 0);
    player.x = room.x + room.width - player.width;
    CHECK(model.get_player() == player);

    model.move_player(-2 * room1.width, 0);
    player.x = room.x;
    CHECK(model.get_player() == player);

    model.move_player(0, room.height);
    player.y = room.y + room.height - player.height;
    CHECK(model.get_player() == player);
}

TEST_CASE("Model::move_player out of bounds")
{
    move_out_of_bounds(room1);
    move_out_of_bounds(room2);
}

