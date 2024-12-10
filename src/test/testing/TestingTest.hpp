#ifndef TESTINGTEST_HPP
#define TESTINGTEST_HPP

#include "oatpp-test/UnitTest.hpp"

class TestingTest : public oatpp::test::UnitTest {
 public:
  TestingTest() : UnitTest("TEST[Testing]") {}

  void onRun() override;
};

#endif  // TESTINGTEST_HPP
