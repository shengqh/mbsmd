//============================================================================
// Name        : mbsmd.cpp
// Author      : Quanhu Sheng, Qi Liu, Shyr Yu
// Version     :
// Copyright   : CQS/VUMC
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <gtest/gtest.h>
using namespace std;

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	cin.get();
}
