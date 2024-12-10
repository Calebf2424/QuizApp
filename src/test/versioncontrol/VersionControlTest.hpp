#ifndef VERSIONTEST_HPP
#define VERSIONTEST_HPP

#include "oatpp-test/UnitTest.hpp"

class VersionControlTest : public oatpp::test::UnitTest {
 public:
  VersionControlTest() : UnitTest("TEST[VersionControl]") {}

  void onRun() override;
};

#endif  // VERSIONTEST_HPP
