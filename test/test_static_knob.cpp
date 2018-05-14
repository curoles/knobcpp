#include <iostream>
#include <cassert>
#include <type_traits>

#include "../static_knob.h"

using namespace knb;
using namespace std::string_view_literals;

bool test_StaticKnob_libraryVersion()
{
    static_assert(knb::knobCppLibraryVersion == "1.0"sv);

    return true;
}

bool test_StaticKnob_buildType()
{
    if constexpr (knb::debugBuild == true) {
        std::cout << "debug build" << std::endl;
    } else {
        std::cout << "non debug build" << std::endl;
    }

    return true;
}

constexpr StaticKnob featureXxxEnabled{true};
constexpr StaticKnob pressureLimit{20.0f};
constexpr StaticKnob versionString{"0.1.1"};
constexpr StaticKnob luckyNumber{777};

bool test_StaticKnob_bool()
{
    static_assert(featureXxxEnabled == true);
    if constexpr (featureXxxEnabled) {
    }
    else {
        assert(1==2);
    }

    return true;
}

bool test_StaticKnob_int()
{
    static_assert(luckyNumber == 777);

    constexpr StaticKnob devilNumber{666};
    static_assert(devilNumber == 666);
    static_assert(devilNumber != luckyNumber);
    static_assert(devilNumber < luckyNumber);
    static_assert(devilNumber <= luckyNumber);

    constexpr StaticKnob luckyOne{luckyNumber.asInt()+1};

    return true;
}

bool test_StaticKnob_float()
{
    static_assert(pressureLimit.asFloat() < 20.1f);
    if constexpr (pressureLimit > 10.0f) {
    }

    return true;
}

bool test_StaticKnob_string()
{
    static_assert(versionString == "0.1.1"sv);
    static_assert(versionString != "2.3.4"sv);

    if constexpr (versionString.asString() == "0.1.1"sv) {
        std::cout << versionString.asString() << "=0.0.1" << std::endl;
    }

    return true;
}

bool test_StaticKnob_array()
{
    constexpr StaticKnob knobs[]={{1},{3.14f},{false},{"be true"}};
    static_assert(knobs[0] == 1);
    static_assert(knobs[3] == "be true"sv);

    return true;
}

bool test_StaticKnob_group()
{
    enum {enabled=0, version, value};
    constexpr StaticKnob featureX[]={{true},{"1.2.3"},{1234}};

    if constexpr (featureX[enabled] and featureX[version] == "1.2.3"sv) {
        static_assert(featureX[value] == 1234);
    } else {
        assert(false);//should not be here
    }

    return true;
}

int main(int argc, char* argv[])
{
    if (auto ok=test_StaticKnob_libraryVersion(); !ok) return 1;
    if (auto ok=test_StaticKnob_buildType(); !ok) return 1;
    if (auto ok=test_StaticKnob_bool(); !ok) return 1;
    if (auto ok=test_StaticKnob_int(); !ok) return 1;
    if (auto ok=test_StaticKnob_float(); !ok) return 1;
    if (auto ok=test_StaticKnob_string(); !ok) return 1;
    if (auto ok=test_StaticKnob_array(); !ok) return 1;
    if (auto ok=test_StaticKnob_group(); !ok) return 1;

    return 0;
}
