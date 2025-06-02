
/// @file Cut.cc
/// @brief Cut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CutMgr.h"
#include "aig/AigNode.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class CalcTv CalcTv.h "CalcTv.h"
/// @brief Cut の論理関数を計算するためのクラス
///
/// ここでは４入力関数のみを扱うので16ビットの符号なし整数(std::uint16_t)
/// で論理関数を表す．
//////////////////////////////////////////////////////////////////////
class CalcTv
{
public:

  using Tv4Type = Cut::Tv4Type;

public:

  /// @brief コンストラクタ
  CalcTv() = default;

  /// @brief デストラクタ
  ~CalcTv() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数をセットする．
  void
  set(
    AigNode* node, ///< [in] 対象のノード
    Tv4Type tv     ///< [in] 論理関数
  )
  {
    mTvDict.emplace(node->id(), tv);
  }

  /// @brief 関数を返す．
  Tv4Type
  tv(
    AigNode* node
  ) const
  {
    if ( mTvDict.count(node->id()) == 0 ) {
      std::ostringstream buf;
      buf << "Node#" << node->id() << " is not registered";
      throw std::logic_error{buf.str()};
    }
    return mTvDict.at(node->id());
  }

  /// @brief 関数を返す．
  Tv4Type
  tv(
    AigEdge edge ///< [in] 対象の枝
  )
  {
    auto node = edge.node();
    auto tv = this->tv(node);
    if ( edge.inv() ) {
      tv = ~tv;
    }
    return tv;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号をキーにして論理関数を保持する辞書
  std::unordered_map<SizeType, SizeType> mTvDict;

};


//////////////////////////////////////////////////////////////////////
// クラス Cut
//////////////////////////////////////////////////////////////////////

// @brief 論理関数を計算する．
Cut::Tv4Type
Cut::calc_tv() const
{
  // 入力の論理関数を登録する．
  // このパタンは PatMgr と一致させる必要がある．
  static Tv4Type pat[] = {
    0xAAAA,
    0xCCCC,
    0xF0F0,
    0xFF00
  };
  // ノード番号をキーにして論理関数を保持する辞書
  CalcTv calc_tv;
  for ( SizeType i = 0; i < leaf_size(); ++ i ) {
    auto node = leaf(i);
    calc_tv.set(node, pat[i]);
  }
  try {
    for ( auto node: mNodeList ) {
      auto tv0 = calc_tv.tv(node->fanin0());
      auto tv1 = calc_tv.tv(node->fanin1());
      auto tv = tv0 & tv1;
      calc_tv.set(node, tv);
    }
    return calc_tv.tv(mRoot);
  }
  catch ( std::logic_error err ) {
    print(cout);
    throw err;
  }
}

// @brief 内容を出力する．
void
Cut::print(
  std::ostream& s
) const
{
  s << "Root: Node#" << mRoot->id() << endl;
  s << "Leaves:";
  for ( auto node: mLeafList ) {
    s << " Node#" << node->id();
  }
  s << endl;
  for ( auto node: mNodeList ) {
    s << "  Node#" << node->id() << ": ";
    if ( node->is_input() ) {
      s << "Input#" << node->input_id();
    }
    else {
      s << "And(" << node->fanin0()
	<< ", " << node->fanin1()
	<< ")";
    }
    s << endl;
  }
}

END_NAMESPACE_YM_AIG
