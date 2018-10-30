#include "Bank_account.h"
#include <catch.h>

TEST_CASE("NewAccount")
{
    Bank_account acct(1, "Robby");
    CHECK(1 == acct.id());
    CHECK("Robby" == acct.owner());
    CHECK(0 == acct.balance());
}

TEST_CASE("Deposit")
{
    Bank_account acct(2, "Jesse");
    acct.deposit(50);
    CHECK(50 == acct.balance());
    acct.deposit(25);
    CHECK(75 == acct.balance());
}

TEST_CASE("Withdrawal_success")
{
    Bank_account acct(2, "Jesse");
    acct.deposit(50);
    CHECK(50 == acct.balance());
    CHECK(true == acct.withdraw(20));
    CHECK(30 == acct.balance());
}

TEST_CASE("Withdrawal_failure")
{
    Bank_account acct(2, "Jesse");
    acct.deposit(50);
    CHECK(50 == acct.balance());
    CHECK(false == acct.withdraw(70));
    CHECK(50 == acct.balance());
}

TEST_CASE("Steal")
{
    Bank_account acct(3, "Robby");
    acct.deposit(10000);
    CHECK("Robby" == acct.owner());
    acct.change_owner("Jesse");
    CHECK("Jesse" == acct.owner());
}
