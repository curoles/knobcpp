/**
 * @file
 * @brief     Static Knob - Compile Time configuration value 
 * @author    Igor Lesik
 * @copyright 2018 Igor Lesik
 *
 * Having a special type StaticKnob as type of a configuration parameter
 * makes it explicit what `constexpr StaticKnob` is used for,
 * that is for configuration.
 *
 * It is possible to group static knobs as array:
 * ~~~{.cpp}
 * constexpr StaticKnob knobs[]={{1},{3.14f},{false},{"be true"}};
 * ~~~
 *
 */
#pragma once
#ifndef STATIC_KNOB_H_INCLUDED
#define STATIC_KNOB_H_INCLUDED

#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <variant>

/// KnobCpp library namespace
namespace knb {

using str = std::string;
using strv = std::string_view;
using cstr = const char*;


/** StaticKnob is Compile Time configuration parameter.
 *
 * Most of the time only few configuration types
 * are practically needed for Compile Time parameters: 
 *  1. Boolean - feature enabled or disabled.
 *  2. Integer - max/min limits, ranges, static size.
 *  3. Float - math/engineering constants like Pi
 *  4. String - fixed names, non numeric parameters.
 *
 * Once knob is constructed its type and value can't change.
 *
 * When getting value of a knob, use asXxxType() function,
 * it is your responsibility to know type of the knob.
 * Good new is that compiler will complain if you use wrong getter.
 *
 * Since most common usage case is too use knob for a comparison
 * in if-else statement, for all types custom comparison operands
 * are defined, so calling asXxx() usually is not required.
 *
 * Usage cases
 * -----------
 *
 * Prefer using StaticKnob and `if constexpr` instead of C-style
 * `#if-else`; in case of `if constexpr` compiler checks
 * code in all if-else branches.
 *
 * ~~~{.cpp}
 *   if constexpr (knb::debugBuild == true) {
 *       std::cout << "debug build" << std::endl;
 *   } else {
 *       std::cout << "non debug build" << std::endl;
 *   }
 * ~~~
 *
 * It is possible to group static knobs as array:
 * ~~~{.cpp}
 * constexpr StaticKnob knobs[]={{1},{3.14f},{false},{"be true"}};
 * ~~~
 *
 */
class StaticKnob final
{
    std::variant<bool,int,float,cstr> v; ///< Holds value
    enum class T : std::size_t { Bool=0, Int, Float, String };

public:
    constexpr std::size_t typeId() const {return v.index();}
    constexpr StaticKnob::T type() const {return static_cast<StaticKnob::T>(v.index());}

public:
    // Overloaded ctor defines type and value of the knob.
    constexpr StaticKnob(bool  b):v(b){} ///< Construct `bool` knob
    constexpr StaticKnob(int   i):v(i){} ///< Construct `int` knob
    constexpr StaticKnob(float f):v(f){} ///< Construct `float` knob
    constexpr StaticKnob(cstr  s):v(s){} ///< Construct `string` knob

public:
    constexpr bool  asBool()   const {return std::get<bool>(v);}
    constexpr int   asInt()    const {return std::get<int>(v);}
    constexpr float asFloat()  const {return std::get<float>(v);}
    constexpr cstr  asString() const {return std::get<cstr>(v);}

    constexpr explicit operator bool()  const { return asBool(); }
    constexpr explicit operator int()   const { return asInt(); }
    constexpr explicit operator float() const { return asFloat(); }


    constexpr bool operator==(bool v) const {return asBool() == v;}
    constexpr bool operator!=(bool v) const {return asBool() != v;}

    constexpr bool operator==(int v) const {return asInt() == v;}
    constexpr bool operator!=(int v) const {return asInt() != v;}
    constexpr bool operator>=(int v) const {return asInt() >= v;}
    constexpr bool operator<=(int v) const {return asInt() <= v;}
    constexpr bool operator> (int v) const {return asInt() >  v;}
    constexpr bool operator< (int v) const {return asInt() <  v;}

    constexpr bool operator==(float v) const {return asFloat() == v;}
    constexpr bool operator!=(float v) const {return asFloat() != v;}
    constexpr bool operator>=(float v) const {return asFloat() >= v;}
    constexpr bool operator<=(float v) const {return asFloat() <= v;}
    constexpr bool operator> (float v) const {return asFloat() >  v;}
    constexpr bool operator< (float v) const {return asFloat() <  v;}

    constexpr bool operator==(strv v) const {return asString() == v;}
    constexpr bool operator!=(strv v) const {return asString() != v;}

    friend constexpr bool operator==(const StaticKnob& k1, const StaticKnob& k2);
    friend constexpr bool operator!=(const StaticKnob& k1, const StaticKnob& k2);
    friend constexpr bool operator<=(const StaticKnob& k1, const StaticKnob& k2);
    friend constexpr bool operator< (const StaticKnob& k1, const StaticKnob& k2);
    friend constexpr bool operator>=(const StaticKnob& k1, const StaticKnob& k2);
    friend constexpr bool operator> (const StaticKnob& k1, const StaticKnob& k2);
};

constexpr bool operator==(const StaticKnob& k1, const StaticKnob& k2){return k1.v == k2.v;}
constexpr bool operator!=(const StaticKnob& k1, const StaticKnob& k2){return k1.v != k2.v;}
constexpr bool operator<=(const StaticKnob& k1, const StaticKnob& k2){return k1.v <= k2.v;}
constexpr bool operator< (const StaticKnob& k1, const StaticKnob& k2){return k1.v <  k2.v;}
constexpr bool operator>=(const StaticKnob& k1, const StaticKnob& k2){return k1.v >= k2.v;}
constexpr bool operator> (const StaticKnob& k1, const StaticKnob& k2){return k1.v <  k2.v;}

/// KnobCpp library version defined as StaticKnob.
constexpr StaticKnob knobCppLibraryVersion{"1.0"};

/** Debug build flag as StaticKnob.
 *
 * Use it instead of `#ifdef DEBUG` and compiler will check
 * code for all `if constexpr` branches.
 */
constexpr StaticKnob debugBuild{
#if (defined NDEBUG) && !(defined DEBUG)
false
#else
true
#endif
};

}

#endif
