
/// @file IgPartitionTest.cc
/// @brief IgPartitionTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "IgPartition.h"
#include "InputInfo.h"


BEGIN_NAMESPACE_YM_LOGIC

TEST(IgPartitionTest, base)
{
  InputInfo iinfo;

  // group#0: 0, 1
  iinfo.new_group(0, 0);
  iinfo.add_elem(0, 1);

  // group#1: 2, 3
  iinfo.new_group(2, 0);
  iinfo.add_elem(1, 3);

  // group#2: 4, 5: bisym
  iinfo.new_group(4, 0);
  iinfo.set_bisym(2);
  iinfo.add_elem(2, 5);

  IgPartition igpart(iinfo);

  EXPECT_EQ( 3, igpart.group_num() );
  EXPECT_EQ( 2, igpart.group_id(0) );
  EXPECT_EQ( 0, igpart.group_id(1) );
  EXPECT_EQ( 1, igpart.group_id(2) );

  EXPECT_EQ( 2, igpart.partition_num() );

  EXPECT_EQ( 0, igpart.partition_begin(0) );
  EXPECT_EQ( 1, igpart.partition_end(0) );

  EXPECT_EQ( 1, igpart.partition_begin(1) );
  EXPECT_EQ( 3, igpart.partition_end(1) );
}

TEST(IgPartitionTest, refine)
{
  InputInfo iinfo;

  iinfo.new_group(0, 0);
  iinfo.new_group(1, 0);
  iinfo.new_group(2, 0);
  iinfo.new_group(3, 0);

  iinfo.new_group(4, 0);
  iinfo.set_bisym(4);

  iinfo.new_group(5, 0);
  iinfo.set_bisym(5);

  iinfo.new_group(6, 0);
  iinfo.set_bisym(6);

  iinfo.new_group(7, 0);
  iinfo.set_bisym(7);

  IgPartition igpart(iinfo);

  EXPECT_EQ( 8, igpart.group_num() );
  EXPECT_EQ( 4, igpart.group_id(0) );
  EXPECT_EQ( 5, igpart.group_id(1) );
  EXPECT_EQ( 6, igpart.group_id(2) );
  EXPECT_EQ( 7, igpart.group_id(3) );
  EXPECT_EQ( 0, igpart.group_id(4) );
  EXPECT_EQ( 1, igpart.group_id(5) );
  EXPECT_EQ( 2, igpart.group_id(6) );
  EXPECT_EQ( 3, igpart.group_id(7) );

  EXPECT_EQ( 2, igpart.partition_num() );

  EXPECT_EQ( 0, igpart.partition_begin(0) );
  EXPECT_EQ( 4, igpart.partition_end(0) );

  EXPECT_EQ( 4, igpart.partition_begin(1) );
  EXPECT_EQ( 8, igpart.partition_end(1) );

  igpart._refine(0, 2);

  EXPECT_EQ( 3, igpart.partition_num() );

  EXPECT_EQ( 6, igpart.group_id(0) );
  EXPECT_EQ( 4, igpart.group_id(1) );
  EXPECT_EQ( 5, igpart.group_id(2) );
  EXPECT_EQ( 7, igpart.group_id(3) );

  EXPECT_EQ( 0, igpart.partition_begin(0) );
  EXPECT_EQ( 1, igpart.partition_end(0) );

  EXPECT_EQ( 1, igpart.partition_begin(1) );
  EXPECT_EQ( 4, igpart.partition_end(1) );

  EXPECT_EQ( 4, igpart.partition_begin(2) );
  EXPECT_EQ( 8, igpart.partition_end(2) );

}

END_NAMESPACE_YM_LOGIC
