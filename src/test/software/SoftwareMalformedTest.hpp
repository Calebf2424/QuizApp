#ifndef SOFTWAREMALFORMEDTEST_hpp
#define SOFTWAREMALFORMEDTEST_hpp

#include "oatpp-test/UnitTest.hpp"

class SoftwareMalformedTest : public oatpp::test::UnitTest {
 public:
  SoftwareMalformedTest()
      : UnitTest("TEST[SoftwareMalformedTest]") {}

  void onRun() override;
};

#endif  // SOFTWAREMALFORMEDTEST_hpp
