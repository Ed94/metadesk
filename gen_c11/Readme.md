# gen_c11

[gen_c11.c](./gen_c11.c) generates both *segregated* and *single-header* varients of the metadesk library utilizing [gencpp_c11](../third_party/gencpp_c11/).

Generated content will be place in a subdirectory called `/gen`.

[gen_common.h](./gen_common.h) contains implementation that will eventually be utilized by [gen_cpp17](../gen_cpp17/).

The prefix `md_` or `MD_` (depending on type of symbol) will be prepended to the symbols of the library using [refactor.exe](../bin/refactor.exe) which is from the [refactor repo](https://github.com/Ed94/refactor).

[c11.refactor](./c11.refactor) is the script defining all the rename mappings

[.clang_format](../bin/.clang_format) is used to cleanup library's formatting on some files post-refactor.

The generation takes a few seconds due to how slow both clang-format and refactor.exe are. Eventually I wish to make refactor a c11 library to offset it that problem but I don't have a good solution to clang_format yet.

If using the powershell script [build.ps1](../bin/build.ps1); Use the following command:

```ps1
.\build gen_c11 <debug> <msvc>
```
