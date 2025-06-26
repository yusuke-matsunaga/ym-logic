#ifndef PATGRAPH_H
#define PATGRAPH_H

/// @file PatGraph.h
/// @brief PatGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "Npn4.h"


BEGIN_NAMESPACE_YM_AIG

class PatNode;

//////////////////////////////////////////////////////////////////////
/// @class PatGraph PatGraph.h "PatGraph.h"
/// @brief 置き換え用のパタンを表す構造体
///
/// 実際には入力と出力に対するNPN変換も合わせて持つ．
//////////////////////////////////////////////////////////////////////
class PatGraph
{
public:

  /// @brief 空のコンストラクタ
  PatGraph() = default;

  /// @brief 個々のパラメータを指定したコンストラクタ
  PatGraph(
    const PatNode* root, ///< [in] 根のノード
    const Npn4& npn      ///< [in] NPN変換
  ) : mRoot{root},
      mNpn{npn}
  {
  }

  /// @brief デストラクタ
  ~PatGraph() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 根のノードを返す．
  const PatNode*
  root() const
  {
    return mRoot;
  }

  /// @brief 根の反転属性を返す．
  bool
  root_inv() const
  {
    return mNpn.oinv();
  }

  /// @brief NPN変換を返す．
  Npn4
  npn() const
  {
    return mNpn;
  }

  /// @brief NPN変換を自身に適用する．
  PatGraph&
  operator*=(
    Npn4 npn
  )
  {
    mNpn *= npn;
    return *this;
  }

  /// @brief NPN変換を行った結果を返す．
  PatGraph
  operator*(
    Npn4 npn
  ) const
  {
    return PatGraph(mRoot, mNpn * npn);
  }

  /// @brief 内容を出力する(主にデバッグ用)．
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のノード
  const PatNode* mRoot{nullptr};

  // 代表関数に対するNPN変換
  Npn4 mNpn;

};

inline
std::ostream&
operator<<(
  std::ostream& s,
  const PatGraph& pat
)
{
  pat.print(s);
  return s;
}

END_NAMESPACE_YM_AIG

#endif // PATMGR_H
