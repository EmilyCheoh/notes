#include <memory>
#include <iostream>

struct fish
{
    double weight;
};

struct aquarium
{
    fish first;
    std::shared_ptr<aquarium> rest;

    aquarium(fish const& f, std::shared_ptr<aquarium> const& r)
            : first(f)
            , rest(r)
    { }
};

double sum_aquarium(std::shared_ptr<aquarium> aq)
{
    double result = 0;

    while (aq != nullptr) {
        result += aq->first.weight;
        aq = aq->rest;
    }

    return result;
}

int main()
{
    fish one_fish{1}, two_fish{2}, red_fish{3}, blue_fish{4};

    std::shared_ptr<aquarium> a = std::make_shared<aquarium>(one_fish, nullptr);
    std::shared_ptr<aquarium> b = std::make_shared<aquarium>(two_fish, a);
    std::shared_ptr<aquarium> c = std::make_shared<aquarium>(red_fish, b);
    std::shared_ptr<aquarium> d = std::make_shared<aquarium>(blue_fish, c);

    std::cout << sum_aquarium(d) << "\n";
}
