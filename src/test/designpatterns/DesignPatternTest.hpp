#ifndef DESIGNPATTERNTEST_hpp
#define DESIGNPATTERNTEST_hpp

#include "oatpp-test/UnitTest.hpp"

class DesignPatternTest : public oatpp::test::UnitTest {
 public:
  DesignPatternTest()
      : UnitTest("TEST[DesignPatternTest]") {}

  void onRun() override;
};

#endif  // DESIGNPATTERNTEST_hpp
