#include <bitops.hh>
#include "driver.hh"

using namespace std;

template <typename T>
class ShiftTest : public ::testing::Test {
};
TYPED_TEST_CASE_P(ShiftTest);

TYPED_TEST_P(ShiftTest, Lshl) {
  typedef TypeParam T;

  ASSERT_EQ(T(2), lshl(T(1), 1));
  ASSERT_EQ(T(4), lshl(T(2), 1));
  ASSERT_EQ(T(8), lshl(T(4), 1));
  ASSERT_EQ(T(16), lshl(T(8), 1));
  ASSERT_EQ(T(32), lshl(T(16), 1));
};

REGISTER_TYPED_TEST_CASE_P(ShiftTest, Lshl);
INSTANTIATE_TYPED_TEST_CASE_P(Ints, ShiftTest, IntTypes);

