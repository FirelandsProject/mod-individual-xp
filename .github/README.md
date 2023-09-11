# Base Module template

[English](README.md) | [Español (Soon...)]()


## How to create your own module

- Before start, Please execute the script `create-module.sh` located in [`mods/`](https://github.com/FirelandsProject/firelands-cata/tree/master/mods).
- Edit the `README.md` and other files (`include.sh` etc...) to fit your module. Note: the README is automatically created from `README_example.md` when you use the script `create-module.sh`.


## How to test your module?

Disable PCH (precompiled headers) and try to compile. To disable PCH, set `-DNOPCH=1` with Cmake.

If you forgot some headers, it is time to add them!

## Licensing

The default license of the **base-module** template is the MIT but you can use a different license for your own modules.

So modules can also be kept private. However, if you need to add new hooks to the core, as well as improving existing ones, you have to share your improvements because the main core is released under the AGPL license. Please [Create a PR](https://github.com/FirelandsProject/firelands-cata) if that is the case.
