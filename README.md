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


## StaticKnob

Use StaticKnob instead of `constexpr` variable when:
 1. You want to say explicitly that it is "knob".
 2. When you have .h file with many parameters that serve
    similar purpose.
 3. You want to combine knobs in logical groups. See example below.

```cpp
bool some_function()
{
    enum {enabled, version, value};
    constexpr StaticKnob featureX[]={{true},{"1.2.3"},{1234}};

    if constexpr (featureX[enabled] and featureX[version] == "1.2.3"sv) {
        // use featureX[value]
    }
```

## Knob

The power of Knob is in its ability to form logical groups.

```cpp
    Group knobs("root");
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

    if (auto [ok, path, val] = knobs.findKnob("A-X-val2"); ok) {
        std::cout << "path=" << path << " ,val=" << val.asString() << std::endl;
        assert(path == "root:feature-A:A-X:A-X-val2");
        assert((int)val == 987);
    }
```


