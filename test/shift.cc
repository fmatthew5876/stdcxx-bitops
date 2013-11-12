#include <bitops.hh>
#include "driver.hh"

using namespace std;

template <typename T>
class ShiftTest : public ::testing::Test {
};
TYPED_TEST_CASE_P(ShiftTest);

TYPED_TEST_P(ShiftTest, Shll) {
  typedef TypeParam T;

  ASSERT_EQ(T(2), shll(T(1), 1));
  ASSERT_EQ(T(4), shll(T(2), 1));
  ASSERT_EQ(T(8), shll(T(4), 1));
  ASSERT_EQ(T(16), shll(T(8), 1));
  ASSERT_EQ(T(32), shll(T(16), 1));
};

REGISTER_TYPED_TEST_CASE_P(ShiftTest, Shll);
INSTANTIATE_TYPED_TEST_CASE_P(Ints, ShiftTest, IntTypes);

