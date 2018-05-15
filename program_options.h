#pragma once
#ifndef KNOBCPP_PROGRAM_OPTIONS_H_INCLUDED
#define KNOBCPP_PROGRAM_OPTIONS_H_INCLUDED

#include <iostream>

#include "knob.h"

namespace knb {

inline void printOptions(const knb::Group& knobs, std::ostream& o = std::cout)
{
    auto pstr = [](std::ostream& o, const Knob& k) {
        o << k.name() << " [\"" << k.asString() << "\"]"; };
    auto pb = [](std::ostream& o, const Knob& k) {
        if (!k.asBool()) {o << "no-";}; o << k.name(); };
    auto pn = [](std::ostream& o, const Knob& k) {
        o << k.name() << " [" << k.asString() << "]"; };

    auto printer = [&](const Knob& knob) {
        o << "--";
        if (auto t=knob.type(); t == Knob::T::String) pstr(o,knob);
        else if (t == Knob::T::Bool) pb(o,knob);
        else pn(o,knob);
        o << std::endl;
    };
    knobs.visit(printer);
}

/**
 *
 * @param[in,out] options - consumed strings will be removed.
 */
inline bool parseOptions(std::vector<std::string>& options, knb::Group& knobs)
{

    return true;
}

inline bool parseOptions(int argc, char* argv[], knb::Group& knobs)
{
    std::vector<std::string> options;
    for (int i = 0; i < argc and argv[i] != nullptr; ++i) {
        options.push_back(argv[i]);
    }
    return parseOptions(options, knobs);
}

}

#endif
