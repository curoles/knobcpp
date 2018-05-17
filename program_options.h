/**
 * @file
 * @brief     Program options with Knob 
 * @author    Igor Lesik
 * @copyright 2018 Igor Lesik
 *
 * Program Options can be naturally implemented with Knob and Group, 
 * conceptually program options are knobs.
 *
 * Knob has:
 *  1. name and description
 *  2. value type information
 *  3. value
 *
 * Knob is ready to be used as an individual Program Option.
 * Then Group can naturally be used as a container for Program Options.
 */
#pragma once
#ifndef KNOBCPP_PROGRAM_OPTIONS_H_INCLUDED
#define KNOBCPP_PROGRAM_OPTIONS_H_INCLUDED

#include <iostream>

#include "knob.h"

namespace knb {

/** Prints knobs prepending name with `--`.
 *
 * Prints:
 *  1. `--knob-name`
 *  2. default knob value that also suggests the type
 *  3. decsription string
 */
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

/** Parse program options.
 *
 * Format: `--op1 op1val --op2 op2val --feature-a-bool --no-feature-b`
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
    std::vector<std::string> non_consumed;
    auto is_op = [](const std::string& s){
        return s.find("--") == 0;
    };
    bool waiting_for_val(false); const Knob* knob{nullptr};
    for (const auto& op : options) {
        if (waiting_for_val) { waiting_for_val = false;
            if (knob) knobs.changeValue(knob, op);
        } else if (is_op(op)) {
            auto knob_name = op.substr((op.find("--no-")==0)? 5:2);
            if (auto [found,p,k] = knobs.findKnob(knob_name); found){
                knob = k; knob_name = p;
                if (waiting_for_val = (k->type()!=Knob::T::Bool); !waiting_for_val) {
                    knobs.changeValue(knob, (op.find("--no-")==0)? "":"true");
                }
            } else { non_consumed.push_back(op); }
        } else { non_consumed.push_back(op); }
    }
    return std::make_tuple(true, non_consumed);
}

/** Parse program options `(int argc, char** argv)`
 *
 */
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
