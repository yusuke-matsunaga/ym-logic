
/// @file CalcMerit.cc
/// @brief CalcMerit の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CalcMerit.h"
#include "CutMgr.h"
#include "Npn4.h"


BEGIN_NAMESPACE_YM_AIG

// @brief コンストラクタ
CalcMerit::CalcMerit(
  const Cut* cut,
  Cut::Tv4Type tv,
  bool debug
) : mDebug{debug}
{
  // 大まかには cut の葉になっているノードよりも根のノード側にある
  // MFFC(Maximal Fanout Free Cone)のサイズを求めればよい．
  // めんどくさいのは見かけ上は葉のノードが実際には使われていない
  // 場合があるということ．
  // 例えば a & a' という式に対応するノードがあったら，実際には
  // 定数0なので a のノードは不要となる．

  auto root = cut->root();
  if ( !root->is_and() ) {
    // 根が定数や入力なら削除できない．
    return;
  }

  if ( mDebug ) {
    cout << "CalcMerit" << endl;
    cut->print(cout);
    cout << endl;
  }

  // サポートになっている葉のノードに印を付ける．
  auto sup = Npn4::get_support(tv);
  for ( SizeType i = 0; i < cut->leaf_size(); ++ i ) {
    if ( sup & (1 << i) ) {
      auto node = cut->leaf(i);
      mCountDict.emplace(node->id(), -1);
    }
  }

  // root は削除されるはず．
  delete_node(root);

  if ( mDebug ) {
    cout << endl;
  }
}

void
CalcMerit::calc_sub(
  AigNode* node
)
{
  if ( mCountDict.count(node->id()) == 0 ) {
    mCountDict.emplace(node->id(), 0);
  }
  auto& count = mCountDict.at(node->id());
  ++ count;
  if ( mDebug ) {
    cout << "calc_sub(Node#" << node->id() << ")" << endl
	 << "  count = " << count
	 << " / " << node->ref_count() << endl;
  }
  if ( count == node->ref_count() ) {
    if ( node->is_and() ) {
      // node は削除される．
      delete_node(node);
    }
  }
}

void
CalcMerit::delete_node(
  AigNode* node
)
{
  mDeleteMark.emplace(node->id());
  calc_sub(node->fanin0_node());
  calc_sub(node->fanin1_node());
}

END_NAMESPACE_YM_AIG
