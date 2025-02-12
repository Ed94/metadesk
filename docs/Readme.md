# docs

**NOTE: metagen has not been reviewed yet.**

## Notable Changes: Original -> RAD Debugger

Metadesk's original implementation recieved an overhaul with its use within the RAD debugger. 

`TODO`

## Notable Changes: RAD Debugger -> Standalone Library

For ergonomic usage and ability for the user to generate the library tailor made for their code base, the library has been heavily modified further while preserving the original indended design or interface for the user.

The metagen's base and os library's have been lifted to [code/base](../code/base/) & [code/os](../code/os/).  

* `base` was splintered further into more files.
* `os` was adjusted so that includes for the target platform are in separate headers.  

`MD_` and `md_` perfixes were removed, they are added back when generating variants of the library for user usage. The [gen_c11/c11.refactor](../gen_c11/c11.refactor) script has the refactoring mappings.

At the top of each head or source file in [code](../code/) the following pattern will be observed:

```c
#ifdef INTELLISENSE_DIRECTIVES
    ....
#endif
```

This provides a while for editors to provide syntatical or semantic information of the library's files without affecting how they are built. They are removed with [gencpp_c11.h](../third_party/gencpp_c11/gencpp_c11.h)'s `gen_scan_file` or manually while traversing the file's ast after `parse_global_body` via iterating through the tree during codegen.

Metadesk had a contrictive use of only one allocator scheme with Arenas, `memory_substrate.h` was introduced to absolve this.  
It defines a generic Allocator interface of the style seen in [GB Headers](https://github.com/gingerBill/gb/blob/52a3a542ef6d398d541d5083aa878598189425ef/gb.h#L1078) and [ZPL-C](https://github.com/zpl-c/zpl/blob/master/code/header/essentials/memory_custom.h) libraries, or the [Jai](https://github.com/Ivo-Balbaert/The_Way_to_Jai/blob/main/book/21A_Memory_Allocators_and_Temporary_Storage.md) and [Odin](https://github.com/odin-lang/Odin/blob/4c4b481ccc0d6ff98e91801517b23ef78c624bdb/base/runtime/core.odin#L355) programming languages:

```c
typedef void*(AllocatorProc)(
    void*         allocator_data, 
    AllocatorMode type, 
    SSIZE         size, 
    SSIZE         alignment, 
    void*         old_memory, 
    SSIZE         old_size, 
    U64           flags 
);

typedef struct AllocatorInfo AllocatorInfo;
struct AllocatorInfo
{
    AllocatorProc* proc;
    void*          data;
};
```

This allows the original `Arena` used by the RaD Debugger version to now be backed by an abitrary block/s of memory:

```c
typedef U32 ArenaFlags;
enum
{
    // Don't chain this arena
    ArenaFlag_NoChain        = (1 << 0), 
    // Only relevant if backing is virtual memory, will prevent allocating a new backing VArena when the current block exhausts
    // Will assume backing can chain multiple block_size arenas however. If there is an allocation failure it will assert.
    ArenaFlag_NoChainVirtual = (1 << 1),
    // Backing allocator identified as VArena during initialization
    ArenaFlag_Virtual        = (1 << 2),
};

typedef struct ArenaParams ArenaParams;
struct ArenaParams
{
    AllocatorInfo backing;
    ArenaFlags    flags;
    U64           block_size; // If chaining VArenas set this to the reserve size
};

#define ARENA_DEFAULT_BLOCK_SIZE VARENA_DEFAULT_RESERVE - align_pow2(size_of(VArena), MD_DEFAULT_MEMORY_ALIGNMENT)

typedef struct Arena Arena;
struct Arena
{
    Arena*        prev;    // Previous arena in chain
    Arena*        current; // Current arena in chain
    AllocatorInfo backing;
    SSIZE         base_pos; // Tracks how main arenas have been chained
    SSIZE         pos;
    SSIZE         block_size;
    ArenaFlags    flags;
};
```

`generic_macros.h` was addded for use of _Generic expressions from C11.  
`_Generic` is utilized mainly for creating a unified interface for procedures that take either the `Arena*` as they originally did or the new `AllocatorInfo` equivalents: 

```c
MD_API String8List debug_string_list_from_tree__arena(Arena*        arena, Node* root);
MD_API String8List debug_string_list_from_tree__ainfo(AllocatorInfo ainfo, Node* root);

#define debug_string_list_from_tree(allocator, string) _Generic( \
(allocator),                                                     \
    Arena*:        debug_string_list_from_tree__arena,           \
    AllocatorInfo: debug_string_list_from_tree__ainfo,           \
    default:       assert_generic_sel_fail                       \
) generic_call(allocator, string)

force_inline String8List
debug_string_list_from_tree__arena(Arena* arena, Node* root) { 
    return debug_string_list_from_tree__ainfo(arena_allocator(arena), root); 
}
```

As shown above, many of the `__arena` functions now collapse to the `__ainfo` equivalents as they do not do anything unique that the general allocator scheme cannot do.  
There are however exceptions if the thread scratch allocator or essentially the `Arena`'s ability to pop/rewind its position is utilized. In those cases the distinct code path is preserved:

```c
String8
os_string_from_file_range__arena(Arena* arena, OS_Handle file, Rng1U64 range)
{
    U64 pre_pos = arena_pos(arena);
    String8 result;
    result.size = dim_1u64(range);
    result.str  = push_array_no_zero(arena, U8, result.size);
    U64 actual_read_size = os_file_read(file, range, result.str);
    if(actual_read_size < result.size) {
        arena_pop_to(arena, pre_pos + actual_read_size);
        result.size = actual_read_size;
    }
    return result;
}
```

The generic allocator an technically handle it though just not as ergonomnically:

```c
String8
os_string_from_file_range__ainfo(AllocatorInfo ainfo, OS_Handle file, Rng1U64 range) {
    String8 result;
    result.size = dim_1u64(range);
    result.str  = alloc_array_no_zero(ainfo, U8, result.size);
    U64 actual_read_size = os_file_read(file, range, result.str);
    if ((allocator_query_support(ainfo) & AllocatorQuery_ResizeShrink) && actual_read_size < result.size) {
        resize(ainfo, result.str, result.size, actual_read_size);
        result.size = actual_read_size;
    }
    return result;
}
```

The above resize-shrink pattern could be used or the allocator made need to be wrapped in a nested arena allocator then have the resize shrink back to zero.  
Natrually the allocator passed must support ths and thus the need for the query, as such any allocator that doesn't support it would not be able to shrink but this deferred to the discretion of the user now.

The following was introduced into [os.h](../code/os/os.h):

```c
typedef struct OS_Context OS_Context;
struct OS_Context
{
    Arena* state_arena;
    Arena* entity_arena;
    B32    enable_large_pages;
};

// OS layer initialization
MD_API void os_init(OS_Context* ctx, TCTX* thread_ctx);
```

This allows for os platform functionality from the original entry_point abstraction (which is still available), to be lifted to a regular procedure that the user may manually call.

The following was introduced into [mdesk.h](../code/mdesk/mdesk.h):

```c
typedef struct Context Context;
struct Context
{
    TCTX thread_ctx;

    OS_Context os_ctx;

    B32 dont_init_os;
};

MD_API void init(Context* ctx);
MD_API void deinit(Context* ctx); // Does nothing for now.
```

This provides the user a way to control startup of the libary.

[code/metadesk.h](../code/metadesk.h) & [code/metadesk.c](../code//metadesk.c) outline the usual intended resolved outline of how the library should be constructed for use. Its tested for vaildity using [tests/code_sanity.c](../tests/code_sanity.c).

### Additional Info

* [Zig's Allocator Interface](https://github.com/ziglang/zig/blob/master/lib/std/mem/Allocator.zig) is similar but not exactly the same.

## Notes

[Handmade Network Project Page](https://metadesk.handmade.network)

[Original Repo](https://github.com/ryanfleury/metadesk)

[Dion Systems Page](https://web.archive.org/web/20231126220529/https://dion.systems/metadesk)  
[Dion Systems Reference](https://web.archive.org/web/20211205200037/https://dion.systems/metadesk_reference)
