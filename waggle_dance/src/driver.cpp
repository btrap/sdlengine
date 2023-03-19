#include "../../src/ascii_string.h"
#include "../../src/test_framework.h"

#include <cstdio>

// Print Test
bool TestIfTrue(bool if_Check, const char* fail_Message)
{
    if (if_Check)
    {
        return true;
    }
    else
    {
        printf(fail_Message);
        return false;
    }
}

bool Tests_MakeFileString()
{
    bool result = true;
    int passed = 0;
    int failed = 0;

    const char* root_Path = "C:\\This\\Is\\A\\Path\\";
    umax root_Path_Length = Length(root_Path);
    const char* path_Extension = "foo";
    umax path_Extension_Length = Length(path_Extension);
    const char* test_Foo_Path_String = "C:\\This\\Is\\A\\Path\\FILE.FOO";

    // Test Resize and Length
    {
        Ascii_String string = {};
        umax new_Size = Resize(&string, 0);
        bool result = TestIfTrue(new_Size == 0, "The Resize() should return 0.");
        result &= TestIfTrue(string.length == 0, "The length should be 0.");
        result &= TestIfTrue(string.allocated_Bytes == 0, "The allocated_Bytes should be 0.");
        (result) ? ++passed : ++failed;

        new_Size = Resize(&string, 10);
        result = TestIfTrue(new_Size == 10, "The Resize() should return 10.");
        result &= TestIfTrue(string.length == 0, "The length should be 0.");
        result &= TestIfTrue(string.allocated_Bytes == 10, "The allocated_Bytes should be 0.");
        (result) ? ++passed : ++failed;
    }

    // Make Zero size File_String
    {
        File_String file_String = MakeFileString("", "", "");
        bool result = TestIfTrue(Compare(&file_String.full_Path, ""), "test_size_zero path must be empty.");
        (result) ? ++passed : ++failed;
    }

    // Make a test File_String
    File_String test_normal_string = MakeFileString(root_Path, "FILE", ".FOO");
    if (!Compare(&test_normal_string.full_Path, test_Foo_Path_String))
    {
        printf("test_normal_string must be %s not %s.", test_Foo_Path_String, test_normal_string.full_Path.chars);
        ++failed;
    }
    else
    {
        ++passed;
    }

    // Make directory string from file path.
    Ascii_String* test_normal_dir_string = MakeDirectoryString(test_Foo_Path_String);
    if (!Compare(test_normal_dir_string->chars, root_Path))
    {
        printf("test_normal_string must be %s not %s.", root_Path, test_normal_string.full_Path.chars);
        ++failed;
    }
    else
    {
        ++passed;
    }


    printf("Tests_MakeFileString passed %d tests and failed %d tests.", passed, failed);
    return result;
}

int RunTests(int argc, char** argv)
{
    const char* resource_Folder_Name = "resources\\";
    Ascii_String* current_Directory = MakeDirectoryString(argv[0]);
    printf("current directory -> %s\n", current_Directory->chars);

    Asset_Bundle asset_Bundle = {};
    Resize(&asset_Bundle.base_Path, current_Directory->length + Length(resource_Folder_Name) + 20);
    Copy(&asset_Bundle.base_Path, current_Directory);
    printf("asset directory -> %s\n", asset_Bundle.base_Path.chars);
    GoUpOneDirectoryLevel(&asset_Bundle.base_Path);

    printf("asset directory -> %s\n", asset_Bundle.base_Path.chars);
    Concatenate(&asset_Bundle.base_Path, resource_Folder_Name);
    printf("asset directory -> %s\n", asset_Bundle.base_Path.chars);

    asset_Bundle.local_Asset_File_Count = 3;
    asset_Bundle.local_Asset_Files = (Ascii_String*)malloc(sizeof(asset_Bundle.local_Asset_Files) * 5);
    for (umax i = 0; i < asset_Bundle.local_Asset_File_Count; ++i)
    {
        asset_Bundle.local_Asset_Files[i] = {};
    }
    Resize(asset_Bundle.local_Asset_Files, 20);
    Copy(asset_Bundle.local_Asset_Files, "arm.png");
    Resize(asset_Bundle.local_Asset_Files + 1, 20);
    Copy(asset_Bundle.local_Asset_Files + 1, "audio.mp3");
    Resize(asset_Bundle.local_Asset_Files + 2, 20);
    Copy(asset_Bundle.local_Asset_Files + 2, "temp.custom");

    PrintFiles(&asset_Bundle);

    if (!Tests_MakeFileString())
        return 1;
}

void Test_Cstr_Length()
{
    const char *test_String = "Test";
    ARE_EQUAL(Length(test_String), strlen(test_String));
    ARE_EQUAL(Length(""), strlen(""));
}

void Test_Ascii_Length()
{
    Ascii_String *test_Ascii1 = MakeAsciiString("Test");
    ARE_EQUAL(Length(test_Ascii1), strlen(test_Ascii1->chars));
}

void Test_Cstr_GetChar()
{
}

void Test_Ascii_GetChar()
{
}

void Test_Cstr_Duplicate()
{
}

void Test_Ascii_Duplicate()
{
}

void Test_Cstr_Concatenate()
{
}

void Test_Ascii_Concatenate()
{
}

void Test_Cstr_Copy()
{
    const char *test_String = "Test";
    umax test_Length = Length(test_String);
    char *new_Str = 0;
    ALLOC_NULLTERM_STRING(new_Str, test_Length);
    Copy(new_Str, test_String);
    IS_TRUE(Compare(test_String, new_Str));
    FREE_DATA(new_Str);

    const char *test_String2 = "Test this out";
    char *new_Str2 = 0;
    ALLOC_NULLTERM_STRING(new_Str2, test_Length);
    Copy(new_Str2, test_String2, test_Length);
    IS_TRUE(Compare(test_String, new_Str2));
    FREE_DATA(new_Str2);
}

void Test_Ascii_Copy()
{
    const char *test_String = "Test";
    umax test_Length = Length(test_String);
    Ascii_String *test_Ascii1 = MakeAsciiString(test_String);

    Ascii_String *test_Ascii2 = MakeAsciiString(test_Length);
    Copy(test_Ascii2, test_String);
    IS_TRUE(Compare(test_Ascii2, test_String));
    FreeAsciiString(test_Ascii2);  

    test_Ascii2 = MakeAsciiString(test_Length);

    Copy(test_Ascii2, test_Ascii1);
    IS_TRUE(Compare(test_Ascii1, test_String));
    FreeAsciiString(test_Ascii2);

    const char *test_String2 = "Test this out";
    test_Ascii2 = MakeAsciiString(test_Length);
    Copy(test_Ascii2, test_String2, test_Length);
    IS_TRUE(Compare(test_Ascii2, test_String));
    FreeAsciiString(test_Ascii2);
    FreeAsciiString(test_Ascii1);
}

void Test_Cstr_Compare()
{
    const char *test_String = "Test";
    const char *test_String2 = "Test";
    IS_TRUE(Compare(test_String, test_String));
    IS_TRUE(Compare(test_String2, test_String));
    IS_FALSE(Compare("test", test_String));
    IS_FALSE(Compare("Test ", test_String));
    IS_FALSE(Compare((const char *)0, (const char *)0));
}

void Test_Ascii_Compare()
{
    const char *test_String = "Test";
    const char *test_String2 = "Test";
    Ascii_String *test_Ascii1 = MakeAsciiString(test_String);
    Ascii_String *test_Ascii2 = MakeAsciiString(test_String2);
    IS_TRUE(Compare(test_Ascii2, test_Ascii1));
    IS_TRUE(Compare(test_Ascii1, test_String));
    IS_TRUE(Compare(test_Ascii1->chars, test_String));
    IS_FALSE(Compare(test_Ascii1, "test"));
    IS_FALSE(Compare(test_Ascii2, "Test "));
    IS_FALSE(Compare((Ascii_String *)0, (const char *)0));
    IS_FALSE(Compare((Ascii_String *)0, (Ascii_String *)0));

    FreeAsciiString(test_Ascii2);
    FreeAsciiString(test_Ascii1);
}

int main(int argc, char **argv)
{
    Test_Cstr_Length();
    Test_Ascii_Length();

    Test_Cstr_Compare();
    Test_Ascii_Compare();

    Test_Cstr_Copy();
    Test_Ascii_Copy();

    PRINT_CONSOLE("Total tests run %d, Success : (%d) Failed : (%d)\n", total_Tests, total_Tests - failed_Tests, failed_Tests);

    ASSERT(failed_Tests == 0);

    return 0;
}
