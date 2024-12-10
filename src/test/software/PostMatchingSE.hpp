#ifndef PostMatchingSE_hpp
#define PostMatchingSE_hpp

#include "oatpp-test/UnitTest.hpp"

class PostMatchingSE : public oatpp::test::UnitTest {
 public:
  PostMatchingSE()
      : UnitTest("TEST[PostMatchingSE]") {}

  void onRun() override;
};

#endif  // PostMatchingSE_hpp
