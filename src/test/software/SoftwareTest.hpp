#ifndef SOFTWARETEST_HPP
#define SOFTWARETEST_HPP

#include "oatpp-test/UnitTest.hpp"

class SoftwareTest : public oatpp::test::UnitTest {
 public:
  SoftwareTest() : UnitTest("TEST[SoftwareTest]") {}

  void onRun() override;
};

#endif  // SOFTWARETEST_HPP
