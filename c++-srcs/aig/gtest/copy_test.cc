
/// @file copy_test.cc
/// @brief copy_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/SopCover.h"
#include "ym/SopCube.h"
#include "ym/AigMgr.h"


BEGIN_NAMESPACE_YM

class CopyTest:
public ::testing::Test
{
public:

  bool
  check(
    const AigHandle& handle1,
    const AigHandle& handle2
  );


private:

  std::unordered_map<AigHandle, AigHandle> mPairMap;

};

bool
CopyTest::check(
  const AigHandle& handle1,
  const AigHandle& handle2
)
{
  if ( handle1.is_zero() ) {
    return handle2.is_zero();
  }
  if ( handle1.is_one() ) {
    return handle2.is_one();
  }
  if ( handle1.is_input() ) {
    if ( handle2.is_input() && handle1.input_id() == handle2.input_id() ) {
      return true;
    }
    return false;
  }
  if ( handle2.is_input() ) {
    return false;
  }
  if ( handle1.inv() != handle2.inv() ) {
    return false;
  }
  if ( mPairMap.count(handle1) > 0 ) {
    return mPairMap.at(handle1) == handle2;
  }
  // handle1 と handle2 は共に AND
  if ( check(handle1.fanin0(), handle2.fanin0()) &&
       check(handle1.fanin1(), handle2.fanin1()) ) {
    mPairMap.emplace(handle1, handle2);
    return true;
  }
  return false;
}

TEST_F(CopyTest, empty)
{
  AigMgr mgr;

  auto src_list = std::vector<AigHandle>{};
  auto handle_list = mgr.copy(src_list);
  EXPECT_TRUE( handle_list.empty() );
}

TEST_F(CopyTest, zero)
{
  AigMgr mgr;

  auto src_handle = AigHandle::zero();
  auto handle = mgr.copy(src_handle);
  EXPECT_TRUE( check(src_handle, handle) );
}

TEST_F(CopyTest, one)
{
  AigMgr mgr;

  auto src_handle = AigHandle::one();
  auto handle = mgr.copy(src_handle);
  EXPECT_TRUE( check(src_handle, handle) );
}

TEST_F(CopyTest, input)
{
  AigMgr mgr;

  auto src_handle = mgr.input(0);
  auto handle = mgr.copy(src_handle);
  EXPECT_TRUE( check(src_handle, handle) );
}

TEST_F(CopyTest, and2)
{
  AigMgr mgr;

  auto i0 = mgr.input(0);
  auto i1 = mgr.input(1);
  auto src_handle = i0 & i1;
  auto handle = mgr.copy(src_handle);
  EXPECT_TRUE( check(src_handle, handle) );
}

END_NAMESPACE_YM
