# KnobCpp - configuration knobs for C++ program 
Knobs are program input parameters to control setup
and run time execution flow. 

![Online documentation](https://codedocs.xyz/curoles/knobcpp.svg)

Online documentation available at [CodeDocs](https://codedocs.xyz/curoles/knobcpp/)

Why? Many programs to run multi-parameter experiments require configuration knobs.
When there are many knobs and they all implemented as global variables,
the code becomes messy.

To address this problem:
 1. Use type `Knob` to explicitly indicate the purpose of variable.
 2. Use type `Knob` to uniformly deal with
    all configuration knobs.
 3. Keep knobs neatly organized in groups, use configuration trees.
 4. Avoid making knobs as C/C++ globals, have configuration tree owned
    by some object at the top of hierarchy.
 5. Treat knobs as **immutable** variables, do NOT ever change a knob
    during run time.

KnobCpp provides following facilities:
 1. [StaticKnob](@ref knb::StaticKnob) - Compile Time configuration constant
    to be used in '`if constexpr (...`' and '`static_assert`'.
 2. [Knob](@ref knb::Knob) - Run Time configuration constant.

