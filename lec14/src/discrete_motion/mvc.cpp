#include "mvc.h"
#include <algorithm>

using namespace ge211;

namespace motion
{

Dimensions const player_dims{10, 10};

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

} // end namespace motion
