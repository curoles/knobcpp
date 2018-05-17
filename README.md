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
 3. [Group](@ref knb::Group) - holds group of knobs and sub-groups,
    forms hierarchical configuration tree. 
 4. Print group as if it holds program options
    and parse program options with changing values of knobs inside the group.


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

## Knob and Group

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
        std::cout << "path=" << path << " ,val=" << val->asString() << std::endl;
        assert(path == "root:feature-A:A-X:A-X-val2");
        assert(val->asInt() == 987);
    }
```

## Program Options

Program options are conceptually knobs. It is easy to apply Knob and Group
do implement Program Options.
Function `knb::printOptions(knobs)` prints all knobs as program options
prepending each knob name with `--`.
Function `knb::parseOptions(options, knobs)` parses array of input strings
and saves options into knobs.

Create knobs:

```cpp
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
        .addKnob("B-val4", 4, "Feature B val4 ...")
    ;
    knobs.getGroup("feature-C")
        .addKnob("C-val5", 21.87f, "C val5 ...")
    ;

    knb::printOptions(knobs, std::cout, 50);
```

The output looks like:

```terminal
--feature-A
  Enable feature A
--no-feature-B
  Enable feature B
--max [100]
  max value of ...
--min [10]
  min value of ...
--version ["1.2.3"]
  Program version
--A-val1 [345]
  Feature A value of ...
--A-X-val2 [987]
  Feature B value of ...
--B-val4 [4]
  Feature B val4 ...
--C-val5 [21.870001]
  C val5 ...
```

Call `parseOptions()`, finalize knobs to make them immutable.
```cpp
    std::vector<std::string> options{
        "--max","200", "--version","10.20.30",
        "--unknown1", "unknown2", "--no-feature-A", "--feature-B",
        "--A-X-val2", "789", "--C-val5", "87.21"
    };

    knb::parseOptions(options, knobs);

    knobs.finalize();
```


