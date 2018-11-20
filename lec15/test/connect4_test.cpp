#include "connect4/model.h"
#include <catch.h>
#include <ge211.h>

using namespace connect4;

TEST_CASE("can create default Connect Four")
{
    Model connect_four;
    CHECK( connect_four.k() == 4 );
    CHECK( connect_four.m() == 7 );
    CHECK( connect_four.n() == 6 );
}

TEST_CASE("can create other sizes")
{
    Model c2(2, 5, 4);
    CHECK( c2.k() == 2 );
    CHECK( c2.m() == 5 );
    CHECK( c2.n() == 4 );

    Model c3(3, 6, 5);
    CHECK( c3.k() == 3 );
    CHECK( c3.m() == 6 );
    CHECK( c3.n() == 5 );

    Model c5(5, 8, 7);
    CHECK( c5.k() == 5 );
    CHECK( c5.m() == 8 );
    CHECK( c5.n() == 7 );
}

TEST_CASE("can move")
{
    using Column = std::vector<Player>;

    Model c2(2, 5, 4);

    CHECK( c2.get_turn() == Player::first );
    CHECK( c2.get_winner() == Player::neither );
    c2.place_token(0);
    CHECK( c2.get_column(0) == Column{Player::first} );

    CHECK( c2.get_turn() == Player::second );
    c2.place_token(1);
    CHECK( c2.get_column(1) == Column{Player::second} );

    CHECK( c2.get_turn() == Player::first );
    c2.place_token(2);
    CHECK( c2.get_column(2) == Column{Player::first} );

    CHECK( c2.get_turn() == Player::second );
    c2.place_token(0);
    CHECK( c2.get_column(0) == Column{Player::first, Player::second} );

    CHECK( c2.get_turn() == Player::neither );
    CHECK( c2.get_winner() == Player::second );

    CHECK_THROWS_AS(c2.place_token(0), ge211::Client_logic_error);
}

TEST_CASE("full column throws")
{
    Model c2(2, 5, 4);

    c2.place_token(0);
    c2.place_token(0);
    c2.place_token(0);
    c2.place_token(0);
    CHECK_THROWS_AS(c2.place_token(0), ge211::Client_logic_error);

    c2.place_token(2);
    c2.place_token(2);
    c2.place_token(2);
    c2.place_token(2);
    CHECK_THROWS_AS(c2.place_token(2), ge211::Client_logic_error);

    c2.place_token(1);
    CHECK_THROWS_AS(c2.place_token(1), ge211::Client_logic_error);
}
