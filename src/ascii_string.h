#pragma once

#define BEETRAP_ASCII_STRING

#ifndef BEETRAP_VARIABLE_DEFINES
#define BEETRAP_VARIABLE_DEFINES

typedef unsigned long long u64;
typedef u64 umax;

#endif

#include "system_defines.h"
#include "memory.h"

#ifndef BEETRAP_UTILITY

    FUNC_INLINE void MemoryCopy(void* destination, const void* source, u64 size)
    {
        if(((uintptr_t)destination % sizeof(size_t) == 0) &&
            ((uintptr_t)source % sizeof(size_t) == 0) &&
            (size % sizeof(size_t) == 0))
        {
            size_t* size_t_destination = (size_t*)destination;
            const size_t* size_t_source = (const size_t*)source;
            for(u64 i = 0; i < size; ++i)
                size_t_destination[i] = size_t_source[i];
        }
        else
        {
            char* char_destination = (char*)destination;
            const char* char_source = (const char*)source;
            for(u64 i = 0; i < size; ++i)
                char_destination[i] = char_source[i];
        }
    }

    FUNC_INLINE void MemoryMove(void* destination, const void* source, u64 size)
    {
        char* result = (char*)malloc((sizeof(char) * (size_t)size));
        if(!result)
            return;

        const char* char_source = (const char*)source;
        for(u64 i = 0; i < size; ++i)
            result[i] = char_source[i];

        char* char_destination = (char*)destination;
        for(u64 i = 0; i < size; ++i)
            char_destination[i] = result[i];

        free(result);
    }
#endif

// c string_terminater
#define NULL_TERM '\0'

#define FILE_EXTENSION_SEPARATOR '.'
#define DRIVE_SEPERATOR ':'

#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
#define DIRECTORY_SEPARATOR '\\'
#else
#define DIRECTORY_SEPARATOR '/'
#endif

struct Ascii_String
{
    umax allocated_Bytes;
    umax length;
    char *chars;
};

struct Asset_Bundle
{
    Ascii_String base_Path;
    // ring buffers
    umax local_Asset_File_Count;
    Ascii_String *local_Asset_Files;
    Ascii_String *stray_Asset_Files;

};

FUNC_INLINE void SetPathToFile(Asset_Bundle* asset_Bundle, umax file_Index)
{
    for(umax i = 0; i < asset_Bundle->local_Asset_Files[file_Index].length; ++i)
    {
        asset_Bundle->base_Path.chars[asset_Bundle->base_Path.length + i];
        ++asset_Bundle->base_Path.length;
    }
    asset_Bundle->base_Path.chars[asset_Bundle->base_Path.length] = 0;
}

FUNC_INLINE void ClearPathToFile(Asset_Bundle* asset_Bundle)
{
    umax path_Length = 0;
    umax step = 0;
    for(umax i = 0; asset_Bundle->base_Path.chars[asset_Bundle->base_Path.length]; ++i)
    {
        ++step;
        if(asset_Bundle->base_Path.chars[asset_Bundle->base_Path.length] == DIRECTORY_SEPARATOR)
        {
            path_Length += step;
            step = 0;
        }
    }

    asset_Bundle->base_Path.length = path_Length;
    asset_Bundle->base_Path.chars[asset_Bundle->base_Path.length] = 0;
}

#include <cstdio>
FUNC_INLINE void PrintFiles(Asset_Bundle * asset_Bundles)
{
    for(umax i = 0; i < asset_Bundles->local_Asset_File_Count; ++i)
    {
        printf("%s\n", asset_Bundles->local_Asset_Files[i].chars);
        SetPathToFile(asset_Bundles, i);
        printf("%s\n", asset_Bundles->base_Path.chars);
        ClearPathToFile(asset_Bundles);
        printf("%s\n", asset_Bundles->base_Path.chars);
    }
}

FUNC_INLINE umax Resize(Ascii_String *string, umax new_Length);
FUNC_INLINE umax Length(Ascii_String *string);
FUNC_INLINE umax Length(const char *c_Str);
FUNC_INLINE char GetChar(Ascii_String *string, umax index);
FUNC_INLINE char GetChar(const char *c_Str, umax index);

FUNC_INLINE Ascii_String Duplicate(Ascii_String *new_Str, const char *c_Str);
FUNC_INLINE Ascii_String Duplicate(const char *c_Str, umax length);
FUNC_INLINE umax ConcatenateNew(Ascii_String *destination, Ascii_String *sourceA, Ascii_String *sourceB);

FUNC_INLINE umax Concatenate(char *destination, const char *source, umax length);
FUNC_INLINE umax Concatenate(char *destination, const char *source);
FUNC_INLINE umax Concatenate(Ascii_String *destination, const char *source);
FUNC_INLINE umax Concatenate(Ascii_String *destination, Ascii_String *source);
FUNC_INLINE umax Concatenate(Ascii_String* destination, char source);

FUNC_INLINE void Copy(char *destination, const char *source, umax length);
FUNC_INLINE void Copy(char *destination, const char *source);
FUNC_INLINE void Copy(Ascii_String *destination, const char *source, umax length);
FUNC_INLINE void Copy(Ascii_String *destination, const char *source);
FUNC_INLINE void Copy(Ascii_String *destination, Ascii_String *source);

FUNC_INLINE bool Compare(const char *string_A, const char *string_B);
FUNC_INLINE bool Compare(Ascii_String *string_A, Ascii_String *string_B);

struct File_String
{
    umax path_Length;
    umax file_Length;
    umax extenstion_Length;

    Ascii_String full_Path;
};

FUNC_INLINE File_String MakeFileString(const char *path, const char *file, const char *extension);

FUNC_INLINE File_String MakeFileString(const char *path, const char *file, const char *extension)
{
    ASSERT(path);
    ASSERT(file);
    ASSERT(extension);

    File_String result = {};
    result.path_Length = Length(path);
    result.file_Length = Length(file);
    result.extenstion_Length = Length(extension);

    // Plus one for FILE_EXTENSION_SEPARATOR
    umax full_PathLength = result.path_Length + result.file_Length + result.extenstion_Length + 1;
    Resize(&result.full_Path, full_PathLength);

    Concatenate(&result.full_Path, path);
    Concatenate(&result.full_Path, file);
    Concatenate(&result.full_Path, FILE_EXTENSION_SEPARATOR);
    Concatenate(&result.full_Path, extension);

    return result;
}

FUNC_INLINE Ascii_String *MakeAsciiString(umax length)
{
    Ascii_String *temp = 0;
    umax allocated_Bytes = GET_DATA(temp, 1);

    if( allocated_Bytes == 0 )
    {
        // Handle issue
        return 0;
    }

    temp->chars = 0;
    temp->allocated_Bytes = 0;
    temp->length = length;
    temp->allocated_Bytes = ALLOC_NULLTERM_STRING(temp->chars, temp->length);

    if(allocated_Bytes == 0)
    {
        // Handle issue
        FREE_DATA(temp);
        return 0;
    }

    return temp;
}

FUNC_INLINE Ascii_String *MakeAsciiString(const char *c_String)
{
    Ascii_String* temp = MakeAsciiString(Length(c_String));
    if( !temp )
    {
        // Handle issue
        return 0;
    }

    Copy(temp, c_String);
    return temp;
}

void FreeAsciiString(Ascii_String *ascii_Str)
{
    FREE_DATA(ascii_Str->chars);
    FREE_DATA(ascii_Str);
}

FUNC_INLINE Ascii_String* MakeDirectoryString(const char *path)
{
    umax path_Length = 0;
    umax step = 0;
    for(umax i = 0; path[i]; ++i)
    {
        ++step;
        if(path[i] == DIRECTORY_SEPARATOR)
        {
            path_Length += step;
            step = 0;
        }
    }

    Ascii_String* directory_String = 0;
    Ascii_String* temp = (Ascii_String*)realloc(directory_String, sizeof(*directory_String));
    if(temp == 0)
    {
        return 0;
    }
    directory_String = temp;

    directory_String->allocated_Bytes = 0;
    directory_String->chars = 0;
    directory_String->length = 0;
    Resize(directory_String, path_Length);
    Copy(directory_String, path, path_Length);
    return directory_String;
}

FUNC_INLINE umax GoUpOneDirectoryLevel(Ascii_String *path)
{
    umax chars_Removed = 0;

    umax i = path->length - 1;
    for(; i > 0; --i)
    {
        if(path->chars[i] == DIRECTORY_SEPARATOR)
        {
            path->chars[i] = 0;
            ++chars_Removed;
            continue;
        }
        break;
    }

    for(; i > 0; --i)
    {
        if(path->chars[i] != DIRECTORY_SEPARATOR)
        {
            path->chars[i] = 0;
            ++chars_Removed;
            continue;
        }
        break;
    }

    return ++chars_Removed;
}

// Returns new size in bytes
FUNC_INLINE umax Resize(Ascii_String *string, umax new_Length)
{
    ASSERT(string);

    if(new_Length == 0)
    {
        string->length = 0;
    }

    if(new_Length < string->length)
    {
        string->chars[new_Length] = NULL_TERM;
        string->chars[string->length] = NULL_TERM;
        return new_Length;
    }

    umax allocated_Length = ALLOC_NULLTERM_STRING(string->chars, new_Length);
    // TODO test for new_Length != allocated_Length
    if(allocated_Length)
    {
        string->allocated_Bytes = allocated_Length;
        string->chars[string->length] = NULL_TERM;
        string->chars[allocated_Length] = NULL_TERM;
    }

    return allocated_Length;
}

// Asserts c_Str not null
FUNC_INLINE umax Length(const char *c_Str)
{
    ASSERT(c_Str);

    umax length = 0;
    while(c_Str[length] != NULL_TERM)
        ++length;
    return length;
}

// Asserts string not null
FUNC_INLINE umax Length(Ascii_String *string)
{
    ASSERT(string);

    return string->length;
}

FUNC_INLINE char GetChar(Ascii_String *string, umax index)
{
    ASSERT(string);
    ASSERT(index < string->length);

    char result = string->chars[index];
    return result;
}

FUNC_INLINE char GetChar(const char *c_Str, umax index)
{
    ASSERT(c_Str);
    ASSERT(index < Length(c_Str));

    char result = c_Str[index];
    return result; 
}

FUNC_INLINE Ascii_String Duplicate(const char* c_Str, umax length)
{
    ASSERT(c_Str);

    Ascii_String result = {};

    Resize(&result, length);

    MemoryCopy(result.chars, c_Str, length);
    result.chars[length] = NULL_TERM;

    return result;
}

FUNC_INLINE Ascii_String Duplicate(const char* c_Str)
{
    ASSERT(c_Str);

    umax length = Length(c_Str);

    return Duplicate(c_Str, length);
}

FUNC_INLINE umax Concatenate(char *destination, const char *source, umax length)
{
    ASSERT(destination);
    ASSERT(source);
    ASSERT(length <= Length(source));

    umax length_Dest = Length(destination);

    for(umax i = 0; i < length; ++i)
    {
        destination[length_Dest + i] = source[i];
    }

    destination[length_Dest + length] = NULL_TERM;

    return length + length_Dest;
}

FUNC_INLINE umax Concatenate(char *destination, const char *source)
{
    ASSERT(destination);
    ASSERT(source);

    umax length = Length(source);
    length = Concatenate(destination, source, length);

    return length;
}

FUNC_INLINE umax Concatenate(Ascii_String *destination, const char *source)
{
    ASSERT(destination);
    ASSERT(source);
    ASSERT(destination->allocated_Bytes >= (Length(source) + destination->length));

    umax length = Concatenate(destination->chars, source);
    destination->length = length;
    return length;
}

FUNC_INLINE umax Concatenate(Ascii_String *destination, Ascii_String *source)
{
    ASSERT(destination);
    ASSERT(source);
    ASSERT(destination->allocated_Bytes >= (source->length + destination->length));

    umax length = Concatenate(destination->chars, source->chars, source->length);
    destination->length = length;
    return length;
}

FUNC_INLINE umax Concatenate(Ascii_String *destination, char source)
{
    ASSERT(destination);
    ASSERT(destination->allocated_Bytes >= (1 + destination->length));

    umax length = 1 + destination->length;
    destination->length = length;
    return length;
}

FUNC_INLINE void Copy(char *destination, const char *source, umax length)
{
    ASSERT(source);
    ASSERT(destination);
    ASSERT(length <= Length(source));

    for(umax i = 0; i < length; ++i)
    {
        destination[i] = source[i];
    }
    destination[length] = NULL_TERM;
}

FUNC_INLINE void Copy(char *destination, const char *source)
{
    ASSERT(source);
    ASSERT(destination);

    umax i = 0;
    while(source[i] != NULL_TERM)
    {
        destination[i] = source[i];
        ++i;
    }
    destination[i] = NULL_TERM;
}

FUNC_INLINE void Copy(Ascii_String *destination, const char *source, umax length)
{
    ASSERT(destination);
    ASSERT(source);
    ASSERT(destination->allocated_Bytes >= length);

    Copy(destination->chars, source, length);
    destination->length = length;
}

FUNC_INLINE void Copy(Ascii_String *destination, const char *source)
{
    ASSERT(destination);
    ASSERT(source);

    umax string_Length = Length(source);
    ASSERT(destination->allocated_Bytes >= string_Length);

    Copy(destination, source, string_Length);
}

FUNC_INLINE void Copy(Ascii_String *destination, Ascii_String *source)
{
    ASSERT(destination);
    ASSERT(source);
    ASSERT(destination->allocated_Bytes >= source->length);

    Copy(destination, source->chars);
}

FUNC_INLINE bool Compare(const char *string_A, const char *string_B)
{
    if(!string_A || !string_B)
        return false;

    if( Length(string_A) != Length(string_B) )
        return false;

    if(string_A == string_B)
        return true;

    umax i = 0;
    while((string_A[i] != NULL_TERM) && (string_B[i] != NULL_TERM))
    {
        if(string_A[i] != string_B[i])
            return false;
        ++i;
    }

    return true;
}

FUNC_INLINE bool Compare(Ascii_String *string_A, const char *string_B)
{
    if(!string_A || !string_B)
        return false;

    bool result = Compare(string_A->chars, string_B);

    return result;
}

FUNC_INLINE bool Compare(Ascii_String *string_A, Ascii_String *string_B)
{
    if(!string_A || !string_B)
        return false;

    if(string_A == string_B)
        return true;

    bool result = Compare(string_A->chars, string_B->chars);

    return result;
}

//bool RemoveFront(Ascii_String *Ascii_String)
//{
//    if(char_Array->char_Length > 0)
//    {
//        --char_Array->char_Length;
//        return true;
//    }
//    return false;
//}
//
//bool IsWhiteSpace(CharArray *char_Array)
//{
//    char test = char_Array->chars[0];
//    bool result = (test == ' ') || (test == '\r') || (test == '\n') || (test == '\t') || (test == '\f') || (test == '\v');
//    return result;
//}
//
//bool AddChar(CharArray *char_Array, char token)
//{
//    if(char_Array->allocated_Bytes > char_Array->char_Length)
//    {
//        char_Array->chars[char_Array->char_Length] = token;
//        ++char_Array->char_Length;
//        return true;
//    }
//
//    return false;
//}
