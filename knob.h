/**
 * @file
 * @brief     Knob - constant with name 
 * @author    Igor Lesik
 * @copyright 2018 Igor Lesik
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
#ifndef KNOBCPP_KNOB_H_INCLUDED
#define KNOBCPP_KNOB_H_INCLUDED

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <algorithm>
#include <variant>
#include <functional>

#include "static_knob.h"

namespace knb {

using str = std::string;
using strv = std::string_view;
using cstr = const char*;

class Group;

class Knob final
{
    std::string name_;
    std::variant<bool,int,float,std::string> v;
    std::string desc_;
public:
    enum class T : std::size_t { Bool=0, Int, Float, String};
public:
    Knob(const str& nm, bool  b, const str& d=""):name_(nm),v(b),desc_(d){}
    Knob(const str& nm, int   i, const str& d=""):name_(nm),v(i),desc_(d){}
    Knob(const str& nm, float f, const str& d=""):name_(nm),v(f),desc_(d){}
    Knob(const str& nm, const str& s, const str& d=""):name_(nm),v(s),desc_(d){}
    //NOTE: next ctor is important, we need temp std::string, otherwise
    //Knob's copy/move ctor and op= do not work, exception -> variant looses
    //its value, gcc 7.3.
    Knob(const std::string& nm, cstr s, const str& d=""):
        name_(nm),v(std::string(s)),desc_(d){}
    Knob():Knob("",false){}

    //Compiler made defaults for us already.
    //Knob(Knob&& other) = default;
    //Knob(const Knob& other) = default;
    //Knob& operator=(const Knob&) = default;

    const std::string& name() const {return name_;}
    const std::string& desc() const {return desc_;}

    Knob::T type() const {return static_cast<Knob::T>(v.index());}
    std::size_t typeId() const {return v.index();}

public:
    bool  asBool()  const {return std::get<bool>(v);}
    int   asInt()   const {return std::get<int>(v);}
    float asFloat() const {return std::get<float>(v);}
    str asString() const {
        switch (type()){
        case T::Bool:   return (asBool()==true)? "true":"false";
        case T::Int:    return std::to_string(asInt());
        case T::Float:  return std::to_string(asFloat());
        case T::String: return std::get<str>(v); 
        }
        return std::get<str>(v);
    }

    explicit operator bool()  const { return asBool(); }
    explicit operator int()   const { return asInt(); }
    explicit operator float() const { return asFloat(); }

    bool operator< (const Knob& other)const {return this->v <  other.v;}
    bool operator<=(const Knob& other)const {return this->v <= other.v;}
    bool operator> (const Knob& other)const {return this->v >  other.v;}
    bool operator>=(const Knob& other)const {return this->v >= other.v;}
    bool operator==(const Knob& other)const {return this->v == other.v;}
    bool operator!=(const Knob& other)const {return this->v != other.v;}

    bool operator==(bool v) const {return asBool() == v;}
    bool operator!=(bool v) const {return asBool() != v;}

    friend class knb::Group;
};


/** Array of knobs.
 *
 * Facility to add a knob to the container and find a knob by its name.
 */
class Array
{
    std::string name_;
    std::vector<Knob> knobs_;
public:
    explicit Array(const std::string& nm):name_(nm){}

    void addKnob(const Knob& kb){knobs_.push_back(kb);}

    template< class... Args >
    void addKnob(Args&&... args){knobs_.emplace_back(args...);}


    std::tuple<bool,Knob> findKnob(std::string_view name) const
    {
        auto knob = std::find_if(
                       std::begin(knobs_), std::end(knobs_),
                       [name](const Knob& k)->bool{return k.name()==name;}
        );

        return (knob==std::end(knobs_))? std::make_tuple(false, Knob()):
                                         std::make_tuple(true, *knob);
    }

    const Knob& at(std::size_t pos) const {return knobs_.at(pos);}
	
};

/** Hierarchy of groups of knobs.
 *
 */
class Group
{
    std::string name_;
    std::map<std::string,Knob> knobs_;
    std::map<std::string,Group> groups_;

    bool immutable_;
public:
    explicit Group(const std::string& nm, bool immutable=true):
        name_(nm),immutable_(immutable){}

    Group& addKnob(const Knob& kb){
        knobs_.insert_or_assign(kb.name(), kb);
        return *this;
    }

    template< class... Args >
    Group& addKnob(const std::string& name, Args&&... args){
        knobs_.try_emplace(name,name,args...);
        return *this;
    }

    const Knob& at(const std::string& knobName) const {
        return knobs_.at(knobName);
    }

    const Group& gr(const std::string& groupName) const {
        return groups_.at(groupName);
    }

    Group& getGroup(const std::string& groupName) {
        auto g = groups_.find(groupName);
        if (g == std::end(groups_)) {
            auto [ig, inserted] = groups_.emplace(groupName,groupName);
            return (inserted)? ig->second : *this;
        }
        return g->second;
    }

    std::tuple<bool,std::string,const Knob*> findKnob(const std::string& name) const
    {
        if (const auto knob = knobs_.find(name); knob != std::end(knobs_)) {
            return std::make_tuple(true, name_+":"+name, &(knob->second));
        }

        for (const auto& kv : groups_) {
            if (auto [foundInSubgr, path, knob] = kv.second.findKnob(name); foundInSubgr) {
                return std::make_tuple(true, name_+":"+path, knob);
            }
        }

        return std::make_tuple(false, "", nullptr);
    }

    void visit(std::function<void(const Knob&)> visitor) const
    {
        for (const auto& name_knob : knobs_) visitor(name_knob.second);
        for (const auto& name_group : groups_) name_group.second.visit(visitor);
    }

    void finalize() { immutable_ = true; }

    void changeValue(const Knob* knob, const std::string& s){
        if (immutable_) return;
        Knob* mutant = const_cast<Knob*>(knob);
        switch (mutant->type()){
        case Knob::T::Bool:   break;
        case Knob::T::Int:    break;
        case Knob::T::Float:  break;
        case Knob::T::String: mutant->v = s; break; 
        }
    }
};

}

#endif
