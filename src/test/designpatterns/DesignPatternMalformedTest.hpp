#ifndef DESIGNPATTERNMALFORMEDTEST_HPP
#define DESIGNPATTERNMALFORMEDTEST_HPP

#include "oatpp-test/UnitTest.hpp"

class DesignPatternMalformedTest : public oatpp::test::UnitTest {
 public:
  DesignPatternMalformedTest() : UnitTest("DesignPatternMalformedTest") {}
  void onRun() override;
};

#endif  // DESIGNPATTERNMALFORMEDTEST_HPP
