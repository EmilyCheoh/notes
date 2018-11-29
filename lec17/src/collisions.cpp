#include "Chain_hash_table.h"
#include "Open_hash_table.h"
#include "Various_hashes.h"
#include "hash_trial.h"

int main()
{
    {
        Chain_hash_table<size_t> ht(10000);
        hash_trial("First_char", ht);
    }

    {
        One_byte_hash_table<size_t> ht(10000);
        hash_trial("One_byte", ht);
    }

    {
        Identity_hash_table<size_t> ht(10000);
        hash_trial("Identity", ht);
    }

    {
        Eight_bytes_hash_table<size_t> ht(10000);
        hash_trial("Eight_bytes", ht);
    }

    {
        Simple_mix_hash_table<size_t> ht(10000);
        hash_trial("Simple_mix", ht);
    }

    {
        Sbox_hash_table<size_t> ht(10000);
        hash_trial("Sbox", ht);
    }

    {
        Open_hash_table<size_t> ht(10000);
        hash_trial("Open", ht);
    }
}

