#pragma once
#ifndef KNOBCPP_PROGRAM_OPTIONS_H_INCLUDED
#define KNOBCPP_PROGRAM_OPTIONS_H_INCLUDED

#include <iostream>

#include "knob.h"

namespace knb {

inline
void printOptions(
    const knb::Group& knobs,
    std::ostream& o = std::cout,
    std::size_t width = 50 // terminal window size
)
{
    auto pstr = [](std::ostream& o, const Knob& k) {
        o << k.name() << " [\"" << k.asString() << "\"]"; };
    auto pb = [](std::ostream& o, const Knob& k) {
        if (!k.asBool()) {o << "no-";}; o << k.name(); };
    auto pn = [](std::ostream& o, const Knob& k) {
        o << k.name() << " [" << k.asString() << "]"; };

    auto printer = [&](const Knob& knob) {
        o << "--";
        if (Knob::T t=knob.type(); t == Knob::T::String) {pstr(o,knob);}
        else if (t == Knob::T::Bool) {pb(o,knob);}
        else {pn(o,knob);}
        o << std::endl;
        for (std::size_t ln = 0, numln=(knob.desc().size()+width)/width; ln < numln; ++ln) {
            o << "  " << knob.desc().substr(ln*width, width) << std::endl;
        }
    };
    knobs.visit(printer);
}

/**
 *
 * @return tuple<ok,non-consumed-options>
 */
inline
std::tuple<bool,std::vector<std::string> >
parseOptions(
    const std::vector<std::string>& options, ///< array of options
    knb::Group& knobs ///< [in,out] changes per option
)
{
    constexpr std::size_t prfx_sz = sizeof("--");
    std::vector<std::string> non_consumed;
    auto is_op = [](const std::string& s){
        return s.size() > prfx_sz && s[0]=='-' && s[1]=='-';
    };
    bool waiting_for_val(false); const Knob* knob{nullptr};
    for (const auto& op : options) {
        if (waiting_for_val) { waiting_for_val = false;
            if (knob) knobs.changeValue(knob, op);
        } else if (is_op(op)) { auto knob_name = op.substr(prfx_sz-1);
            if (auto [found,p,k] = knobs.findKnob(knob_name); found){
                waiting_for_val = true; knob = k; knob_name = p;
            } else { non_consumed.push_back(op); }
        } else { non_consumed.push_back(op); }
    }
    return std::make_tuple(true, non_consumed);
}

inline
std::tuple<bool,std::vector<std::string> >
parseOptions(int argc, char* argv[], knb::Group& knobs)
{
    std::vector<std::string> options;
    for (int i = 0; i < argc and argv[i] != nullptr; ++i) {
        options.push_back(argv[i]);
    }
    return parseOptions(options, knobs);
}

}

#endif
