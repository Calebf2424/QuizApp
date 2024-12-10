#ifndef VERSIONCONTROLMALFORMEDTEST_HPP
#define VERSIONCONTROLMALFORMEDTEST_HPP

#include "oatpp-test/UnitTest.hpp"

class VersionControlMalformedTest : public oatpp::test::UnitTest {
 public:
  VersionControlMalformedTest() :
  UnitTest("TEST[VersionControlMalformedTest]") {}
  void onRun() override;
};

#endif  // VERSIONCONTROLMALFORMEDTEST_HPP
