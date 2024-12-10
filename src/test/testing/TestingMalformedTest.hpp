#ifndef TESTING_MALFORMED_TEST_HPP
#define TESTING_MALFORMED_TEST_HPP

#include "oatpp-test/UnitTest.hpp"

class TestingMalformedTest : public oatpp::test::UnitTest {
 public:
  TestingMalformedTest() : UnitTest("TEST[TestingMalformedTest]") {}
  void onRun() override;
};

#endif  // TESTING_MALFORMED_TEST_HPP
