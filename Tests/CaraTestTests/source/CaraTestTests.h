#pragma once

#include <CaraTest/TestSuite.h>

CaraTest::TestSuite CaraTestTestsSuiteSimple();
CaraTest::TestSuite CaraTestTestsSuiteParameterized();
CaraTest::TestSuite CaraTestSnapshotTestsSuite();

QList<CaraTest::TestSuite> CaraTestTestsSuite();
