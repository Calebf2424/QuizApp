#ifndef OPService_hpp
#define OPService_hpp

#include "oatpp-test/UnitTest.hpp"

class optimusPrimeTest : public oatpp::test::UnitTest {
 public:
  optimusPrimeTest()
      : UnitTest("TEST[optimusPrimeTest]") {}

  void onRun() override;
};

#endif  // OPService_hpp
