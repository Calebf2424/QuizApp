#ifndef POST_MATCHING_TESTING_HPP
#define POST_MATCHING_TESTING_HPP

#include "oatpp-test/UnitTest.hpp"

class PostMatchingTesting : public oatpp::test::UnitTest {
 public:
  PostMatchingTesting() : UnitTest("TEST[PostMatchingTesting]") {}

  void onRun() override;
};

#endif  // POST_MATCHING_TESTING_HPP
