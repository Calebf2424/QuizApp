#ifndef POST_MATCHING_VC_HPP
#define POST_MATCHING_VC_HPP

#include "oatpp-test/UnitTest.hpp"

class PostMatchingVC : public oatpp::test::UnitTest {
 public:
  PostMatchingVC() : UnitTest("TEST[PostMatchingVC]") {}

  void onRun() override;
};

#endif  // POST_MATCHING_VC_HPP
