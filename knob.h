/**
 * @file
 * @author    Igor Lesik
 * @copyright 2018 Igor Lesik
 * @brief     Knob - constant with name 
 *
 * Some programs (simulators) need many input "knobs" to set
 * desired configuration. Usually knobs are of very few types:
 * {boolean|numeric|string}. Usualy knobs in C/C++ are implemented
 * as `union`, not as different types of knobs.
 *
 * Benifits of implementing knob as union/variant:
 *  1. Single type makes it easy to place knobs in containers.
 *  2. Type of initializing value "defines" knob type,
 *     as a result shorter almost typeless code.
 */
#pragma once
#ifndef KNOB_H_INCLUDED
#define KNOB_H_INCLUDED

#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <variant>

namespace knb {

using str = std::string;
using strv = std::string_view;
using cstr = const char*;

class Knob final
{
    std::string name_;
    std::variant<bool,int,float,std::string> v;
    enum class T : std::size_t { Bool=0, Int, Float, String};
public:
    Knob(const std::string& nm, bool  b):name_(nm),v(b){}
    Knob(const std::string& nm, int   i):name_(nm),v(i){}
    Knob(const std::string& nm, float f):name_(nm),v(f){}
    Knob(const std::string& nm, const str& s):name_(nm),v(s){}
    //NOTE: next ctor is important, we need temp std::string, otherwise
    //Knob's copy/move ctor and op= do not work, exception -> variant looses
    //its value, gcc 7.3.
    Knob(const std::string& nm, cstr  s):name_(nm),v(std::string(s)){}
    Knob():Knob("",false){}

    //Compiler made defaults for us already.
    //Knob(Knob&& other) = default;
    //Knob(const Knob& other) = default;
    //Knob& operator=(const Knob&) = default;

    const std::string& name() const {return name_;}

    Knob::T type() const {return static_cast<Knob::T>(v.index());}
    std::size_t typeId() const {return v.index();}

public:
    bool  asBool()  const {return std::get<bool>(v);}
    int   asInt()   const {return std::get<int>(v);}
    float asFloat() const {return std::get<float>(v);}
    str asString() const {
        switch (type()){
        case T::Bool:  return (asBool()==true)? "true":"false";
        case T::Int:   return std::to_string(asInt());
        case T::Float: return std::to_string(asFloat()); 
        }
        return std::get<str>(v);
    }

    bool operator<(const Knob& other)const{return this->v < other.v;}
    bool operator<=(const Knob& other)const{return this->v <= other.v;}
    bool operator>(const Knob& other)const{return this->v > other.v;}
    bool operator>=(const Knob& other)const{return this->v >= other.v;}
    bool operator==(const Knob& other)const{return this->v == other.v;}
    bool operator!=(const Knob& other)const{return this->v != other.v;}

};


/** Group/containers of knobs, has a name.
 *
 * Facility to add a knob to the container and find a knob by its name.
 */
class Group
{
    std::string name_;
    std::vector<Knob> knobs_;
public:
    explicit Group(const std::string& nm):name_(nm){}

    void addKnob(const Knob& kb){knobs_.push_back(kb);}

    template< class... Args >
    void addKnob(Args&&... args){knobs_.emplace_back(args...);}


    std::tuple<bool,Knob> findKnob(std::string_view name){
        auto knob = std::find_if(
                       std::begin(knobs_), std::end(knobs_),
                       [name](const Knob& k)->bool{return k.name()==name;}
        );

        return (knob==std::end(knobs_))? std::make_tuple(false, Knob()):
                                         std::make_tuple(true, *knob);
    }

};

class StaticKnob final
{
    std::variant<bool,int,float,cstr> v;
    enum class T : std::size_t { Bool=0, Int, Float, String };

    constexpr std::size_t typeId() const {return v.index();}

public:
    constexpr StaticKnob(bool  b):v(b){}
    constexpr StaticKnob(int   i):v(i){}
    constexpr StaticKnob(float f):v(f){}
    constexpr StaticKnob(cstr  s):v(s){}

public:
    constexpr bool  asBool()   const {return std::get<bool>(v);}
    constexpr int   asInt()    const {return std::get<int>(v);}
    constexpr float asFloat()  const {return std::get<float>(v);}
    constexpr cstr  asString() const {return std::get<cstr>(v);}

    constexpr explicit operator bool() const { return asBool(); }
    constexpr explicit operator int() const { return asInt(); }
    constexpr explicit operator float() const { return asFloat(); }
};

}

#endif
