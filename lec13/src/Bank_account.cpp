#include "Bank_account.h"

Bank_account::Bank_account(unsigned long id, std::string& owner)
    : id_{id}, owner_{owner}, balance_{0}
{ }

unsigned long Bank_account::id() const
{
    return id_;
}

std::string& Bank_account::owner() const
{
    return owner_;
}

unsigned int Bank_account::balance() const
{
    return balance_;
}

void Bank_account::change_owner(std::string& s)
{
    owner_ = s;
}

void Bank_account::deposit(unsigned int amount)
{
    balance_ += amount;
}

bool Bank_account::withdrawal(unsigned int amount)
{
    if (amount <= balance_) {
        balance_ -= amount;
        return true;
    } else {
        return false;
    }
}

