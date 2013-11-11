#include <bitops.hh>
#include "driver.hh"

using namespace std;

template <typename T>
class SatMathTest : public ::testing::Test {
};
TYPED_TEST_CASE_P(SatMathTest);

TYPED_TEST_P(SatMathTest, Add) {
  typedef typename TypeParam::first_type L;
  typedef typename TypeParam::second_type R;
  typedef decltype(L() + R()) LR;
  constexpr bool samesign = std::is_signed<L>::value == std::is_signed<R>::value;

  auto lmax = std::numeric_limits<L>::max();
  auto rmax = std::numeric_limits<R>::max();
  auto lrmax = std::numeric_limits<LR>::max();

  if(sizeof(L) == sizeof(LR) && samesign) {
    ASSERT_EQ(lrmax, satadd(lmax, R(1)));
  }
  if(sizeof(R) == sizeof(LR) && samesign) {
    ASSERT_EQ(lrmax, satadd(L(1), rmax));
  }
};

TYPED_TEST_P(SatMathTest, Sub) {
  typedef typename TypeParam::first_type L;
  typedef typename TypeParam::second_type R;
  typedef decltype(L() - R()) LR;

  //auto lrmax = std::numeric_limits<LR>::max();
  //auto lrmin = std::numeric_limits<LR>::min();

};

REGISTER_TYPED_TEST_CASE_P(SatMathTest, Add, Sub);
INSTANTIATE_TYPED_TEST_CASE_P(Ints, SatMathTest, IntPairTypes);

