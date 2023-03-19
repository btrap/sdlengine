#include "../../src/memory.h"
#include "../../src/test_framework.h"

void TestSetupMemoryPool()
{
    SetupMemoryPools();
    ARE_EQUAL(temp_Memory.storage_Size, MEGABYTES(TEMP_MEMORY_SIZE_IN_MB));
    ARE_EQUAL(permanent_Memory.storage_Size, MEGABYTES(PERMANENT_MEMORY_SIZE_IN_MB));

    ClearMemoryPools();
}

void TestPermanentMemoryPools()
{
    SetupMemoryPools();

    // Test store some data.
    int* test = 0;
    int test_Count = 12;
    {
        STORE_PERM_DATA(test, test_Count);
        int* endPos = test + test_Count;
        ARE_EQUAL(endPos, permanent_Memory.storage_Current);
        for (int i = 0; i < test_Count; ++i)
        {
            test[i] = i;
        }
        for (int i = 0; i < test_Count; ++i)
        {
            ARE_EQUAL(test[i], ((int*)permanent_Memory.storage_Start)[i]);
        }
    }

    // Test it palces it after what is stored.
    int* test2 = 0;
    int test_Count2 = 10;
    STORE_PERM_DATA(test2, test_Count2);
    int* endPos = test + test_Count + test_Count2;
    ARE_EQUAL(endPos, permanent_Memory.storage_Current);
    for (int i = 0; i < test_Count2; ++i)
    {
        test2[i] = i;
    }
    for (int i = 0; i < test_Count; ++i)
    {
        ARE_EQUAL(test2[i], ((int*)permanent_Memory.storage_Start)[i + test_Count]);
    }

    ClearMemoryPools();
}

void TestTempMemoryPools()
{
    SetupMemoryPools();

    // Test store some data.
    int* test = 0;
    int test_Count = 12;
    {
        STORE_TEMP_DATA(test, test_Count);
        int * endPos = test + test_Count;
        ARE_NOT_EQUAL(endPos, temp_Memory.storage_Current);
        for (int i = 0; i < test_Count; ++i)
        {
            test[i] = i;
        }
        for (int i = 0; i < test_Count; ++i)
        {
            ARE_EQUAL(test[i], ((int*)temp_Memory.storage_Start)[i]);
        }
    }

    // Test it overrites what is stored.
    STORE_TEMP_DATA(test, test_Count);
    int* endPos = test + test_Count;
    ARE_NOT_EQUAL(endPos, temp_Memory.storage_Current);
    for (int i = 0; i < test_Count; ++i)
    {
        test[i] = i + 9;
    }
    for (int i = 0; i < test_Count; ++i)
    {
        ARE_EQUAL(test[i], ((int*)temp_Memory.storage_Start)[i]);
    }

    ClearMemoryPools();
}

void TestGetDynamicData()
{
    int* test = 0;
    int test_Count = 12;
    int test_Count_Minus_One = test_Count - 1;
    int memory_Allocated = GET_DATA(test, test_Count);
    for(int i = 0; i < test_Count; ++i)
    {
        test[i] = i;
    }
    ARE_EQUAL(memory_Allocated, test_Count * sizeof(int));
    ARE_EQUAL(test[0], 0);
    ARE_EQUAL(test[test_Count_Minus_One], test_Count_Minus_One);

    FREE_DATA(test);
    ARE_EQUAL(test, 0);

    memory_Allocated = GET_CLEAR_DATA(test, test_Count);
    ARE_EQUAL(memory_Allocated, test_Count * sizeof(int));
    for(int i = 0; i < test_Count; ++i)
    {
        ARE_EQUAL(test[i], 0);
    }

    test_Count *= 2;
    test_Count_Minus_One = test_Count - 1;
    memory_Allocated = GROW_DATA(test, test_Count);
    for (int i = 0; i < test_Count; ++i)
    {
        test[i] = i;
    }
    ARE_EQUAL(memory_Allocated, test_Count * sizeof(int));
    ARE_EQUAL(test[0], 0);
    ARE_EQUAL(test[test_Count_Minus_One], test_Count_Minus_One);

    FREE_DATA(test);
}

int main(int argc, char** argv)
{
    TestSetupMemoryPool();

    TestPermanentMemoryPools();

    TestTempMemoryPools();

    TestGetDynamicData();
 
    PRINT_CONSOLE("Total tests run %d, Success : (%d) Failed : (%d)\n", total_Tests, total_Tests - failed_Tests, failed_Tests);

    ASSERT(failed_Tests == 0);

    return 0;
}
