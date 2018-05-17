#include <cassert>

#include "../program_options.h"

int main(int argc, char* argv[])
{
    knb::Group knobs("test", false);
    knobs.addKnob("version", "1.2.3", "Program version")
         .addKnob("max",         100, "max value of ...")
         .addKnob("min",          10, "min value of ...")
         .addKnob("feature-A",  true, "Enable feature A")
         .addKnob("feature-B", false, "Enable feature B")
    ;
    knobs.getGroup("feature-A")
        .addKnob("A-val1", 345, "Feature A value of ...")
        .getGroup("A-X")
            .addKnob("A-X-val2", 987, "Feature B value of ...")
    ;
    knobs.getGroup("feature-B")
        .addKnob("B-val4", 4, "Feature B val4 ...")
    ;
    knobs.getGroup("feature-C")
        .addKnob("C-val5", 21.87f, "C val5 ...")
    ;

    knb::printOptions(knobs, std::cout, 50);

    std::vector<std::string> options{
        "--max","200", "--version","10.20.30",
        "--unknown1", "unknown2", "--no-feature-A", "--feature-B",
        "--A-X-val2", "789", "--C-val5", "87.21"
    };

    std::cout << "================================" << std::endl;

    knb::parseOptions(options, knobs);

    knobs.finalize();

    knb::printOptions(knobs, std::cout, 50);

    bool ok; std::string path; const knb::Knob* k{nullptr};

    std::tie(ok,path,k) = knobs.findKnob("feature-A");
    assert(k->asBool() == false);

    std::tie(ok,path,k) = knobs.findKnob("feature-B");
    assert(k->asBool() == true);

    std::tie(ok,path,k) = knobs.findKnob("max");
    assert(k->asInt() == 200);

    std::tie(ok,path,k) = knobs.findKnob("C-val5");
    assert(k->asFloat() == 87.21f);

    std::tie(ok,path,k) = knobs.findKnob("A-X-val2");
    assert(k->asInt() == 789);

    std::tie(ok,path,k) = knobs.findKnob("version");
    assert(k->asString() == "10.20.30");

    return 0;
}
