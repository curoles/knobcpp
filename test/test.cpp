#include <iostream>
#include <cassert>
#include <type_traits>

#include "../knob.h"

using namespace knb;
using namespace std::string_view_literals;

constexpr StaticKnob featureXxxEnabled{true};
constexpr StaticKnob pressureLimit{20.0f};
constexpr StaticKnob versionString{"0.1.1"};
constexpr StaticKnob luckyNumber{777};

bool test_StaticKnob()
{
    static_assert(featureXxxEnabled == true);
    if constexpr (featureXxxEnabled) {
    }
    else {
        assert(1==2);
    }

    static_assert(pressureLimit.asFloat() < 20.1f);
    if constexpr (pressureLimit > 10.0f) {
    }

    static_assert(versionString == "0.1.1"sv);
    static_assert(versionString != "2.3.4"sv);

    if constexpr (versionString.asString() == "0.1.1"sv) {
        std::cout << versionString.asString() << "=0.0.1" << std::endl;
    }

    static_assert(luckyNumber == 777);

    constexpr StaticKnob devilNumber{666};
    static_assert(devilNumber == 666);

    return true;
}

bool test_KnobBool()
{
    Knob k1{"k1",true};
    assert(k1.asBool() == true);
    assert(k1.name() == "k1");
    assert(k1.asString() == "true");

    return true;
}

//test_KnobAssign
//k2=k1

int main(int argc, char* argv[])
{
    static_assert(std::is_copy_constructible<std::string>::value == true);
    static_assert(std::is_copy_constructible<knb::Knob>::value == true);

    if (auto ok=test_StaticKnob(); !ok) return 1;
    if (auto ok=test_KnobBool();   !ok) return 1;

    knb::Knob k1{"k1",true};
    knb::Knob k2{"k2",777};
    knb::Knob k3{"k3",3.14f};
    knb::Knob k4{"k4","I'm Knob!"};
    Knob k5; k5=k3;
    Knob k6(k4);

    if (k1 > k2) {}

    std::cout << k1.name() << ":" << k1.typeId()
              << "=" << k1.asBool() << ", str="<< k1.asString() << std::endl
              << k2.name() << ":" << k2.typeId()
              << "=" << k2.asInt() << std::endl
              << k5.name() << ":" << k5.typeId()
              << "=" << k5.asFloat() << std::endl
              << k6.name() << ":" << k6.typeId()
              << "=" << k6.asString() << std::endl;

    knb::Group top("top");
    top.addKnob(k1);
    top.addKnob("k7","knob #7");
    top.addKnob(k3);
    if (auto [ok, k] = top.findKnob("k7"); ok) {
        std::cout << k.name() << ":" << k.typeId()
                  << "=" << k.asString() << std::endl;
    }
    if (auto [ok, k] = top.findKnob("k3"); ok) {
        std::cout << k.name() << ":" << k.typeId()
                  << "=" << k.asString() << std::endl;
    }

    return 0;
}
