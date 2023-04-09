#include <random>
#include <chrono>
#include "item.h"

Item::Item(std::string name) : name{name} {
    std::default_random_engine gen(
        (long) std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<unsigned long long> distribution(1, INT64_MAX);
    id = distribution(gen);
}

Item::Item(std::string name, unsigned long long id) : name{name}, id{id} {
}

void Item::set_name(std::string other) {
    name = other;
}

std::string Item::get_name() const {
    return name;
}

std::string Item::fetch_xml() const {
    return "\t\t<item name=\"" + name + "\">";
}

bool Item::operator==(const Item& item) const {
    return item.get_name() == name && item.id == id;
}

unsigned long long Item::get_id() const {
    return id;
}