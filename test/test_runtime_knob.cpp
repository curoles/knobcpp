#include <iostream>
#include <cassert>
#include <type_traits>

#include "../knob.h"

using namespace knb;
using namespace std::string_view_literals;

bool test_Knob_construct()
{
    knb::Knob k1{"k1",true};
    knb::Knob k2{"k2",777};
    knb::Knob k3{"k3",3.14f};
    knb::Knob k4{"k4","I'm Knob!"};

    Knob k5; k5=k3;
    assert(k5.name() == k3.name());
    assert(k5.type() == k3.type());
    assert(k5 == k3);

    Knob k6(k4);
    assert(k4.name() == k6.name());
    assert(k4.type() == k6.type());
    assert(k4 == k6);

    Knob k7 = k4;
    assert(k7.name() == k6.name());
    assert(k7.type() == k6.type());
    assert(k7 == k6);

    return true;
}

bool test_Knob_Bool()
{
    Knob k1{"k1",true};
    assert(k1.asBool() == true);
    assert(k1.name() == "k1");
    assert(k1.asString() == "true");

    return true;
}

bool test_Knob_compare()
{
    const Knob k1{"k1",1};
    const Knob k2{"k2",2};

    assert(k1 < k2 && k1 <= k2);
    assert(k2 > k1 && k2 >= k1);
    assert(k1 != k2);

    assert((int)k1 < 2 && (int)k1 <= 2);
    assert(2 > k1.asInt() && 2 >= k1.asInt());
    assert(k1.asInt() != 2 && k1.asInt() == 1);

    return true;
}


int main(int argc, char* argv[])
{
    static_assert(std::is_copy_constructible<std::string>::value == true);
    static_assert(std::is_copy_constructible<knb::Knob>::value == true);

    if (auto ok=test_Knob_construct();   !ok) return 1;
    if (auto ok=test_Knob_Bool();   !ok) return 1;
    if (auto ok=test_Knob_compare();   !ok) return 1;

    knb::Knob k1{"k1",true};
    knb::Knob k2{"k2",777};
    knb::Knob k3{"k3",3.14f};
    knb::Knob k4{"k4","I'm Knob!"};
    Knob k5; k5=k3;
    Knob k6(k4);

    std::cout << k1.name() << ":" << k1.typeId()
              << "=" << k1.asBool() << ", str="<< k1.asString() << std::endl
              << k2.name() << ":" << k2.typeId()
              << "=" << k2.asInt() << std::endl
              << k5.name() << ":" << k5.typeId()
              << "=" << k5.asFloat() << std::endl
              << k6.name() << ":" << k6.typeId()
              << "=" << k6.asString() << std::endl;


    return 0;
}
