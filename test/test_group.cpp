#include <iostream>
#include <cassert>
#include <type_traits>

#include "../knob.h"

using namespace knb;
using namespace std::string_view_literals;

bool test_Knob_array()
{
    knb::Knob k1{"k1",true};
    knb::Knob k3{"k3",123};

    knb::Array ar("top");
    ar.addKnob(k1);
    ar.addKnob("k7","knob #7");
    ar.addKnob(k3);
    if (auto [ok, k] = ar.findKnob("k7"); ok) {
        std::cout << k.name() << ":" << k.typeId()
                  << "=" << k.asString() << std::endl;
        assert(k.asString() == "knob #7");
    }
    if (auto [ok, k] = ar.findKnob("k3"); ok) {
        std::cout << k.name() << ":" << k.typeId()
                  << "=" << k.asString() << std::endl;
        assert(k.asInt() == 123);
    }

    return true;
}

bool test_Knob_group()
{
    Group knobs("root");
    knobs.addKnob("version","1.2.3")
         .addKnob("max", 100)
         .addKnob("min",  10)
         .addKnob("feature-A", true)
         .addKnob("feature-B", true)
    ;
    knobs.getGroup("feature-A")
        .addKnob("A-val1", 345)
        .getGroup("A-X")
            .addKnob("A-X-val2", 987)
    ;
    knobs.getGroup("feature-B")
        .addKnob("B-val4", 4)
    ;
    knobs.getGroup("feature-C");

    if (auto [ok, path, val] = knobs.findKnob("A-X-val2"); ok) {
        std::cout << "path=" << path << " ,val=" << val->asString() << std::endl;
        assert(path == "root:feature-A:A-X:A-X-val2");
        assert(val->asInt() == 987);
    }

    return true;
}


int main(int argc, char* argv[])
{
    if (auto ok=test_Knob_array();   !ok) return 1;
    if (auto ok=test_Knob_group();   !ok) return 1;

    return 0;
}
