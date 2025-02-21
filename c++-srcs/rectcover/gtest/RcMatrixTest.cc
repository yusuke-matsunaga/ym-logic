
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
  EXPECT_THROW( mtx.row_head(0), std::out_of_range );
  EXPECT_THROW( mtx.row_num(0), std::out_of_range );
  EXPECT_THROW( mtx.row_cost(0), std::out_of_range );
  EXPECT_THROW( mtx.col_head(0), std::out_of_range );
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

  auto head = mtx.row_head(0);
  EXPECT_EQ( head, head->right() );
  EXPECT_EQ( head, head->left() );
  EXPECT_EQ( 0, head->row() );
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

  auto head = mtx.col_head(0);
  EXPECT_EQ( head, head->down() );
  EXPECT_EQ( head, head->up() );
  EXPECT_EQ( 0, head->col() );
}

TEST(RcMatrixTest, costr1)
{
  vector<SizeType> row_costs{1, 2, 3};
  vector<SizeType> col_costs{4, 5, 6, 7};

  auto mtx = RcMatrix(row_costs, col_costs);

  EXPECT_EQ( row_costs.size(), mtx.row_size() );
  for ( SizeType i = 0; i < mtx.row_size(); ++ i ) {
    EXPECT_EQ( 0, mtx.row_num(i) );
    EXPECT_EQ( row_costs[i], mtx.row_cost(i) );
    auto head = mtx.row_head(i);
    EXPECT_EQ( head, head->right() );
    EXPECT_EQ( head, head->left() );
    EXPECT_EQ( i, head->row() );
  }
  EXPECT_EQ( col_costs.size(), mtx.col_size() );
  for ( SizeType i = 0; i < mtx.col_size(); ++ i ) {
    EXPECT_EQ( 0, mtx.col_num(i) );
    EXPECT_EQ( col_costs[i], mtx.col_cost(i) );
    auto head = mtx.col_head(i);
    EXPECT_EQ( head, head->down() );
    EXPECT_EQ( head, head->up() );
    EXPECT_EQ( i, head->col() );
  }
}

TEST(RcMatrixTest, add_elem1)
{
  vector<SizeType> row_costs{1, 2, 3};
  vector<SizeType> col_costs{4, 5, 6, 7};

  auto mtx = RcMatrix(row_costs, col_costs);

  SizeType val0 = 10;
  auto vid0 = mtx.add_value(val0);

  SizeType r0 = 1;
  SizeType c0 = 3;
  mtx.add_elem(vid0, r0, c0);

  EXPECT_EQ( 1, mtx.row_num(r0) );
  EXPECT_EQ( 1, mtx.col_num(c0) );

  auto row_head = mtx.row_head(r0);
  auto elem1 = row_head->right();
  EXPECT_EQ( row_head, elem1->right() );
  EXPECT_EQ( row_head, elem1->left() );
  EXPECT_EQ( elem1, row_head->left() );
  EXPECT_EQ( r0, elem1->row() );
  EXPECT_EQ( c0, elem1->col() );
  EXPECT_EQ( vid0, elem1->val_id() );

  auto col_head = mtx.col_head(c0);
  auto elem2 = col_head->down();
  EXPECT_EQ( elem1, elem2 );
  EXPECT_EQ( col_head, elem2->down() );
  EXPECT_EQ( col_head, elem2->up() );
  EXPECT_EQ( elem2, col_head->up() );
  EXPECT_EQ( r0, elem2->row() );
  EXPECT_EQ( c0, elem2->col() );
  EXPECT_EQ( vid0, elem2->val_id() );
}

TEST(RcMatrixTest, add_elem2)
{
  vector<SizeType> row_costs{1, 2, 3};
  vector<SizeType> col_costs{4, 5, 6, 7};

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

  auto row_head0 = mtx.row_head(0);
  EXPECT_EQ( row_head0, row_head0->right() );
  EXPECT_EQ( row_head0, row_head0->left() );

  auto row_head1 = mtx.row_head(1);
  auto elem1 = row_head1->right();
  auto elem2 = elem1->right();
  EXPECT_EQ( row_head1, elem1->left() );
  EXPECT_EQ( elem1, elem2->left() );
  EXPECT_EQ( row_head1, elem2->right() );
  EXPECT_EQ( elem2, row_head1->left() );

  EXPECT_EQ( 1, elem1->row() );
  EXPECT_EQ( 0, elem1->col() );
  EXPECT_EQ( vid1, elem1->val_id() );

  EXPECT_EQ( 1, elem2->row() );
  EXPECT_EQ( 3, elem2->col() );
  EXPECT_EQ( vid0, elem2->val_id() );

  auto row_head2 = mtx.row_head(2);
  auto elem3 = row_head2->right();
  EXPECT_EQ( row_head2, elem3->left() );
  EXPECT_EQ( row_head2, elem3->right() );
  EXPECT_EQ( elem3, row_head2->left() );

  EXPECT_EQ( 2, elem3->row() );
  EXPECT_EQ( 3, elem3->col() );
  EXPECT_EQ( vid2, elem3->val_id() );

  auto col_head0 = mtx.col_head(0);
  auto elem4 = col_head0->down();
  EXPECT_EQ( col_head0, elem4->down() );
  EXPECT_EQ( col_head0, elem4->up() );
  EXPECT_EQ( elem4, col_head0->up() );

  EXPECT_EQ( 1, elem4->row() );
  EXPECT_EQ( 0, elem4->col() );
  EXPECT_EQ( vid1, elem4->val_id() );

  auto col_head1 = mtx.col_head(1);
  EXPECT_EQ( col_head1, col_head1->down() );
  EXPECT_EQ( col_head1, col_head1->up() );

  auto col_head2 = mtx.col_head(2);
  EXPECT_EQ( col_head2, col_head2->down() );
  EXPECT_EQ( col_head2, col_head2->up() );

  auto col_head3 = mtx.col_head(3);
  auto elem5 = col_head3->down();
  auto elem6 = elem5->down();
  EXPECT_EQ( col_head3, elem5->up() );
  EXPECT_EQ( elem5, elem6->up() );
  EXPECT_EQ( col_head3, elem6->down() );
  EXPECT_EQ( elem6, col_head3->up() );

  EXPECT_EQ( 1, elem5->row() );
  EXPECT_EQ( 3, elem5->col() );
  EXPECT_EQ( vid0, elem5->val_id() );

  EXPECT_EQ( 2, elem6->row() );
  EXPECT_EQ( 3, elem6->col() );
  EXPECT_EQ( vid2, elem6->val_id() );
}

END_NAMESPACE_YM_RC
