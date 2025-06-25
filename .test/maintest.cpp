/**
    \file : maintest.cpp
    \brief : Intergate tests for framework 'CLIF' based on 'Google Test'.
 */

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}