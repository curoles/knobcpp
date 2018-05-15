#include "../program_options.h"

int main(int argc, char* argv[])
{
    knb::Group knobs("test");
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

    knb::printOptions(knobs);

    return 0;
}
