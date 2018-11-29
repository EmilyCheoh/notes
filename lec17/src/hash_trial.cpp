#include "hash_trial.h"
#include "hamlet.h"

#include <iomanip>
#include <iostream>

void hash_trial(std::string const& name, Hash_table_base<size_t>& h)
{
    size_t i = 0;

    for (auto const& s : get_hamlet()) {
        h.insert(s, i++);
    }

    std::cout << "size: " << h.nbuckets()
              << "    collisions: " << std::setw(6) << h.collisions()
              << "    " << name << "\n";
}

