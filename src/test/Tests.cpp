#include <iostream>

#include "designpatterns/DesignPatternTest.hpp"
#include "designpatterns/DesignPatternMalformedTest.hpp"
#include "designpatterns/PostMatchingDP.hpp"
#include "versioncontrol/VersionControlTest.hpp"
#include "versioncontrol/PostMatchingVC.hpp"
#include "versioncontrol/VersionControlMalformedTest.hpp"
#include "testing/TestingTest.hpp"
#include "testing/PostMatchingTesting.hpp"
#include "testing/TestingMalformedTest.hpp"
#include "software/SoftwareTest.hpp"
#include "software/SoftwareMalformedTest.hpp"
#include "software/PostMatchingSE.hpp"
#include "optimusprime/optimusPrimeTest.hpp"

void RunTests() {
  OATPP_RUN_TEST(DesignPatternTest);
  OATPP_RUN_TEST(DesignPatternMalformedTest);
  OATPP_RUN_TEST(VersionControlTest);
  OATPP_RUN_TEST(VersionControlMalformedTest);
  OATPP_RUN_TEST(TestingTest);
  OATPP_RUN_TEST(TestingMalformedTest);
  OATPP_RUN_TEST(SoftwareTest);
  OATPP_RUN_TEST(SoftwareMalformedTest);
  OATPP_RUN_TEST(optimusPrimeTest);
  OATPP_RUN_TEST(PostMatchingDP);
  OATPP_RUN_TEST(PostMatchingSE);
  OATPP_RUN_TEST(PostMatchingTesting);
  OATPP_RUN_TEST(PostMatchingVC);
}

int main() {
  oatpp::base::Environment::init();

  RunTests();

  // Print the count of objects created during runtime and check for leaks.
  // Disable object counting for release builds using the
  // '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for performance.
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = "
            << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = "
            << oatpp::base::Environment::getObjectsCreated() << "\n\n";

  OATPP_ASSERT(oatpp::base::Environment::getObjectsCount() == 0);

  oatpp::base::Environment::destroy();

  return 0;
}
