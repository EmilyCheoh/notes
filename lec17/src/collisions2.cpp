#include "hash_trial.h"
#include "Various_hashes.h"

#include <sstream>

int main()
{
    for (uint64_t mixer = 1; mixer <= 128; ++mixer) {
        std::ostringstream name;
        name << "Simple_mix(" << mixer << ")";

        Simple_mix_hash_table<size_t> ht(10000, mixer);
        hash_trial(name.str(), ht);
    }
}
