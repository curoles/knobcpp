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
        .addKnob("B-val4", 4)
    ;
    knobs.getGroup("feature-C")
        .addKnob("C-val5", 21.87f)
    ;

    knb::printOptions(knobs, std::cout, 50);

    std::vector<std::string> options{
        "--max","200", "--version","10.20.30"};

    knb::parseOptions(options, knobs);

    knobs.finalize();

    knb::printOptions(knobs, std::cout, 50);

    return 0;
}
