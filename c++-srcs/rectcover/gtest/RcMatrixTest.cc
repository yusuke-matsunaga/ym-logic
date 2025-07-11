
/// @file RcMatrixText.cc
/// @brief RcMatrixText の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "RcMatrix.h"


BEGIN_NAMESPACE_YM_RC

TEST(RcMatrixTest, empty1)
{
  auto mtx = RcMatrix();

  EXPECT_EQ( 0, mtx.val_size() );
  EXPECT_EQ( 0, mtx.row_size() );
  EXPECT_EQ( 0, mtx.col_size() );
  EXPECT_THROW( mtx.value(0), std::out_of_range );
  EXPECT_THROW( mtx.row_list(0), std::out_of_range );
  EXPECT_THROW( mtx.row_num(0), std::out_of_range );
  EXPECT_THROW( mtx.row_cost(0), std::out_of_range );
  EXPECT_THROW( mtx.col_list(0), std::out_of_range );
  EXPECT_THROW( mtx.col_num(0), std::out_of_range );
  EXPECT_THROW( mtx.col_cost(0), std::out_of_range );
}

TEST(RcMatrixTest, add_value)
{
  auto mtx = RcMatrix();

  SizeType val0 = 1;
  auto id0 = mtx.add_value(val0);
  EXPECT_EQ( 0, id0 );
  EXPECT_EQ( val0, mtx.value(id0) );

  SizeType val1 = 3;
  auto id1 = mtx.add_value(val1);
  EXPECT_EQ( 1, id1 );
  EXPECT_EQ( val1, mtx.value(id1) );

  EXPECT_THROW( mtx.value(2), std::out_of_range );
}

TEST(RcMatrixTest, insert_row)
{
  auto mtx = RcMatrix();

  SizeType cost0 = 2;
  mtx.insert_row(cost0);

  EXPECT_EQ( 1, mtx.row_size() );
  EXPECT_EQ( 0, mtx.row_num(0) );
  EXPECT_EQ( cost0, mtx.row_cost(0) );
  EXPECT_THROW( mtx.row_num(1), std::out_of_range );
  EXPECT_THROW( mtx.row_cost(1), std::out_of_range );

  EXPECT_EQ( 0, mtx.col_size() );
  EXPECT_THROW( mtx.col_num(0), std::out_of_range );
  EXPECT_THROW( mtx.col_cost(0), std::out_of_range );

  auto row_list = mtx.row_list(0);
  EXPECT_EQ( 0, row_list.size() );
  EXPECT_EQ( row_list.end(), row_list.begin() );
}

TEST(RcMatrixTest, insert_col)
{
  auto mtx = RcMatrix();

  SizeType cost0 = 3;
  mtx.insert_col(cost0);

  EXPECT_EQ( 1, mtx.col_size() );
  EXPECT_EQ( 0, mtx.col_num(0) );
  EXPECT_EQ( cost0, mtx.col_cost(0) );
  EXPECT_THROW( mtx.col_num(1), std::out_of_range );
  EXPECT_THROW( mtx.col_cost(1), std::out_of_range );

  EXPECT_EQ( 0, mtx.row_size() );
  EXPECT_THROW( mtx.row_num(0), std::out_of_range );
  EXPECT_THROW( mtx.row_cost(0), std::out_of_range );

  auto col_list = mtx.col_list(0);
  EXPECT_EQ( 0, col_list.size() );
  EXPECT_EQ( col_list.end(), col_list.end() );
}

TEST(RcMatrixTest, costr1)
{
  std::vector<SizeType> row_costs{1, 2, 3};
  std::vector<SizeType> col_costs{4, 5, 6, 7};

  auto mtx = RcMatrix(row_costs, col_costs);

  EXPECT_EQ( row_costs.size(), mtx.row_size() );
  for ( SizeType i = 0; i < mtx.row_size(); ++ i ) {
    EXPECT_EQ( 0, mtx.row_num(i) );
    EXPECT_EQ( row_costs[i], mtx.row_cost(i) );
    auto row_list = mtx.row_list(i);
    EXPECT_EQ( row_list.end(), row_list.begin() );
  }
  EXPECT_EQ( col_costs.size(), mtx.col_size() );
  for ( SizeType i = 0; i < mtx.col_size(); ++ i ) {
    EXPECT_EQ( 0, mtx.col_num(i) );
    EXPECT_EQ( col_costs[i], mtx.col_cost(i) );
    auto col_list = mtx.col_list(i);
    EXPECT_EQ( col_list.end(), col_list.begin() );
  }
}

TEST(RcMatrixTest, add_elem1)
{
  std::vector<SizeType> row_costs{1, 2, 3};
  std::vector<SizeType> col_costs{4, 5, 6, 7};

  auto mtx = RcMatrix(row_costs, col_costs);

  SizeType val0 = 10;
  auto vid0 = mtx.add_value(val0);

  SizeType r0 = 1;
  SizeType c0 = 3;
  mtx.add_elem(vid0, r0, c0);

  EXPECT_EQ( 1, mtx.row_num(r0) );
  EXPECT_EQ( 1, mtx.col_num(c0) );

  auto row_list = mtx.row_list(r0);
  EXPECT_EQ( 1, row_list.size() );
  auto iter1 = row_list.begin();
  auto elem1 = *iter1;
  EXPECT_EQ( r0, elem1->row() );
  EXPECT_EQ( c0, elem1->col() );
  EXPECT_EQ( vid0, elem1->val_id() );
  auto iter2 = iter1;
  ++ iter2;
  EXPECT_EQ( row_list.end(), iter2 );

  auto col_list = mtx.col_list(c0);
  EXPECT_EQ( 1, col_list.size() );
  auto iter3 = col_list.begin();
  auto elem2 = *iter3;
  EXPECT_EQ( elem1, elem2 );
  EXPECT_EQ( r0, elem2->row() );
  EXPECT_EQ( c0, elem2->col() );
  EXPECT_EQ( vid0, elem2->val_id() );
  auto iter4 = iter3;
  ++ iter3;
  EXPECT_EQ( col_list.end(), iter3 );
}

TEST(RcMatrixTest, add_elem2)
{
  std::vector<SizeType> row_costs{1, 2, 3};
  std::vector<SizeType> col_costs{4, 5, 6, 7};

  auto mtx = RcMatrix(row_costs, col_costs);

  //    0  1  2  3
  // 0  .  .  .  .
  // 1  1  .  .  0
  // 2  .  .  .  2

  SizeType val0 = 10;
  auto vid0 = mtx.add_value(val0);

  SizeType r0 = 1;
  SizeType c0 = 3;
  mtx.add_elem(vid0, r0, c0);

  SizeType val1 = 5;
  auto vid1 = mtx.add_value(val1);

  SizeType r1 = 1;
  SizeType c1 = 0;
  mtx.add_elem(vid1, r1, c1);

  SizeType val2 = 2;
  auto vid2 = mtx.add_value(val2);

  SizeType r2 = 2;
  SizeType c2 = 3;
  mtx.add_elem(vid2, r2, c2);

  EXPECT_EQ( 0, mtx.row_num(0) );
  EXPECT_EQ( 2, mtx.row_num(1) );
  EXPECT_EQ( 1, mtx.row_num(2) );

  EXPECT_EQ( 1, mtx.col_num(0) );
  EXPECT_EQ( 0, mtx.col_num(1) );
  EXPECT_EQ( 0, mtx.col_num(2) );
  EXPECT_EQ( 2, mtx.col_num(3) );

  auto row_list0 = mtx.row_list(0);
  EXPECT_EQ( 0, row_list0.size() );

  auto row_list1 = mtx.row_list(1);
  ASSERT_EQ( 2, row_list1.size() );
  auto iter1 = row_list1.begin();
  auto elem1 = *iter1; ++ iter1;
  auto elem2 = *iter1; ++ iter1;
  EXPECT_EQ( row_list1.end(), iter1 );

  EXPECT_EQ( 1, elem1->row() );
  EXPECT_EQ( 0, elem1->col() );
  EXPECT_EQ( vid1, elem1->val_id() );

  EXPECT_EQ( 1, elem2->row() );
  EXPECT_EQ( 3, elem2->col() );
  EXPECT_EQ( vid0, elem2->val_id() );

  auto row_list2 = mtx.row_list(2);
  ASSERT_EQ( 1, row_list2.size() );
  auto iter2 = row_list2.begin();
  auto elem3 = *iter2; ++ iter2;
  EXPECT_EQ( row_list2.end(), iter2 );

  EXPECT_EQ( 2, elem3->row() );
  EXPECT_EQ( 3, elem3->col() );
  EXPECT_EQ( vid2, elem3->val_id() );

  auto col_list0 = mtx.col_list(0);
  ASSERT_EQ( 1, col_list0.size() );
  auto iter4 = col_list0.begin();
  auto elem4 = *iter4; ++ iter4;
  EXPECT_EQ( col_list0.end(), iter4 );

  EXPECT_EQ( 1, elem4->row() );
  EXPECT_EQ( 0, elem4->col() );
  EXPECT_EQ( vid1, elem4->val_id() );

  auto col_list1 = mtx.col_list(1);
  EXPECT_EQ( 0, col_list1.size() );

  auto col_list2 = mtx.col_list(2);
  EXPECT_EQ( 0, col_list2.size() );

  auto col_list3 = mtx.col_list(3);
  ASSERT_EQ( 2, col_list3.size() );
  auto iter5 = col_list3.begin();
  auto elem5 = *iter5; ++ iter5;
  auto elem6 = *iter5; ++ iter5;
  EXPECT_EQ( col_list3.end(), iter5 );

  EXPECT_EQ( 1, elem5->row() );
  EXPECT_EQ( 3, elem5->col() );
  EXPECT_EQ( vid0, elem5->val_id() );

  EXPECT_EQ( 2, elem6->row() );
  EXPECT_EQ( 3, elem6->col() );
  EXPECT_EQ( vid2, elem6->val_id() );
}

END_NAMESPACE_YM_RC
