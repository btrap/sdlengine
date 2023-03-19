#pragma once

#define BEETRAP_MEMORY_POOLS

#ifndef BEETRAP_VARIABLE_DEFINES
#define BEETRAP_VARIABLE_DEFINES

typedef unsigned long long u64;
typedef u64 umax;

#endif

#include "system_defines.h"

#if COMPILER_MSVC
    #define VC_EXTRALEAN
    #define WIN32_LEAN_AND_MEAN
    //#include <memoryapi.h> // win32 VirtualAlloc
    #include <windows.h> // win32 VirtualAlloc
    #undef far
    #undef near
    #undef pascal
#endif

#ifdef DEBUG_ACTIVE
struct DEBUG_Memory_Data
{
    umax permanent_Used;
};
DEBUG_Memory_Data debug_Memory_Data = {0};
#endif

struct Memory_Pool
{
    umax storage_Size;
    void* storage_Current;
    void* storage_Start;
};
Memory_Pool temp_Memory = {0}; // temp data
Memory_Pool permanent_Memory = {0}; // static data

#ifndef TEMP_MEMORY_SIZE_IN_MB
    #define TEMP_MEMORY_SIZE_IN_MB 1
#endif

#ifndef PERMANENT_MEMORY_SIZE_IN_MB
    #define PERMANENT_MEMORY_SIZE_IN_MB 16
#endif

#ifndef BASE_PERMANENT_MEMORY_ADDRESS_IN_TB
    #define BASE_PERMANENT_MEMORY_ADDRESS_IN_TB 2
#endif

#ifndef BASE_TEMP_MEMORY_ADDRESS_IN_TB
    #define BASE_TEMP_MEMORY_ADDRESS_IN_TB 3
#endif

FUNC_INLINE void SetupMemoryPools()
{
    temp_Memory.storage_Size = MEGABYTES(TEMP_MEMORY_SIZE_IN_MB);
    permanent_Memory.storage_Size = MEGABYTES(PERMANENT_MEMORY_SIZE_IN_MB);

    LPVOID base_Address = 0;
#ifdef DEBUG_ACTIVE
    base_Address = (LPVOID)TERABYTES(BASE_PERMANENT_MEMORY_ADDRESS_IN_TB);
#endif
    permanent_Memory.storage_Start = VirtualAlloc(base_Address, permanent_Memory.storage_Size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    permanent_Memory.storage_Current = permanent_Memory.storage_Start;

#ifdef DEBUG_ACTIVE
    base_Address = (LPVOID)TERABYTES(BASE_TEMP_MEMORY_ADDRESS_IN_TB);
#endif
    temp_Memory.storage_Start = VirtualAlloc(base_Address, temp_Memory.storage_Size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    temp_Memory.storage_Current = temp_Memory.storage_Start;
}

FUNC_INLINE void ClearMemoryPools()
{
    VirtualFree(permanent_Memory.storage_Start, 0, MEM_RELEASE);
    VirtualFree(temp_Memory.storage_Start, 0, MEM_RELEASE);
}

FUNC_INLINE void StoreData_(void **data, umax sizeof_Data, void **memory)
{
    *data = *memory;
    *memory = (void*)((char*)*memory + sizeof_Data);
}
#define STORE_DATA(data, count, memory) StoreData_( (void **)(&data), ( (count) * sizeof(*data) ), (&memory) );

FUNC_INLINE void StorePermanentData_(void **data, umax sizeof_Data)
{
#ifdef DEBUG_ACTIVE
    ASSERT((debug_Memory_Data.permanent_Used + sizeof_Data) < permanent_Memory.storage_Size);
    debug_Memory_Data.permanent_Used += sizeof_Data;
#endif

    *data = permanent_Memory.storage_Current;
    permanent_Memory.storage_Current = (void*)((char*)permanent_Memory.storage_Current + sizeof_Data);
}
#define STORE_PERM_DATA(data, count) StorePermanentData_( (void **)(&data), ( (count) * sizeof(*data) ));

FUNC_INLINE void StoreTempData_(void **data, umax sizeof_Data)
{
    ASSERT(sizeof_Data < temp_Memory.storage_Size);

    *data = temp_Memory.storage_Current;
}
#define STORE_TEMP_DATA(data, count) StoreTempData_( (void **)(&data), ( (count) * sizeof(*data) ));

// dyamic data
#include <cstdlib> // malloc, realloc, calloc, free

FUNC_INLINE umax GetData_(void **data, umax sizeof_Data)
{
    umax result = 0;

    char* new_Data = (char*)malloc((size_t)(sizeof_Data));
    if(new_Data)
    {
        result = sizeof_Data;
        *data = new_Data;
    }
    else if(*data)
    {
        // TODO handle memory issues
        PRINT_CONSOLE("malloc failed in GetData_\n");
        ASSERT_NOW();
    }

    return result;
}
#define GET_DATA(data, count) GetData_( (void **)(&data), ( (count) * sizeof(*data) ) );

FUNC_INLINE umax GetClearData_(void **data, umax count, umax sizeof_Data)
{
    umax result = 0;

    char* new_Data = (char*)calloc(count, (size_t)(sizeof_Data));
    if(new_Data)
    {
        result = sizeof_Data;
        *data = new_Data;
    }
    else if(*data)
    {
        // TODO handle memory issues
        PRINT_CONSOLE("calloc failed in GetClearData_\n");
        ASSERT_NOW();
    }

    return result;
}
#define GET_CLEAR_DATA(data, count) GetClearData_( (void **)(&data), (count), ( (count) * sizeof(*data) ) );

FUNC_INLINE umax GrowData_(void **data, umax sizeof_Data)
{
    umax result = 0;

    char* new_Data = (char*)realloc(*data, (size_t)(sizeof_Data));
    if(new_Data)
    {
        result = sizeof_Data;
        *data = new_Data;
    }
    else if(*data)
    {
        // TODO handle memory issues
        PRINT_CONSOLE("realloc failed in GrowData_\n");
        ASSERT_NOW();
    }

    return result;
}
#define GROW_DATA(data, count) GrowData_( (void **)(&data), ( (count) * sizeof(*data) ) );


// Allocate size plus one for null terminator
FUNC_INLINE umax AllocNullTermString_(void** data, umax sizeof_Data)
{
    umax result = 0;

    char* new_Data = (char*)realloc(*data, (size_t)(sizeof_Data + 1));
    if (new_Data)
    {
        result = sizeof_Data + 1;
        *data = new_Data;
    }
    else if (*data)
    {
        // TODO handle memory issues
        PRINT_CONSOLE("realloc failed in AllocNullTermString_\n");
        //ASSERT_NOW();
    }

    return result;
}
#define ALLOC_NULLTERM_STRING(data, count) AllocNullTermString_( (void **)(&data), ( (count) * sizeof(*data) ) );

FUNC_INLINE void FreeData_(void **data)
{
    free(*data);
    *data = 0;
}
#define FREE_DATA(data) FreeData_( (void **)(&data) );
