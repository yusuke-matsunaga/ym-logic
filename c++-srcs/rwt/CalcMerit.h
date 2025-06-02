#ifndef CALCMERIT_H
#define CALCMERIT_H

/// @file CalcMerit.h
/// @brief CalcMerit のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "aig/AigNode.h"
#include "CutMgr.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class CalcMerit CalcMerit.h "CalcMerit.h"
/// @brief 削除されるノード数を計算するクラス
///
/// と言っても削除されるノードが新規で追加されるノードと重複する
/// 可能性があるのでここでは削除されるノードに印を付けるだけ．
//////////////////////////////////////////////////////////////////////
class CalcMerit
{
public:

  /// @brief コンストラクタ
  CalcMerit(
    const Cut* cut, ///< [in] カット
    Cut::Tv4Type tv ///< [in] カットに対応する論理関数
  );

  /// @brief デストラクタ
  ~CalcMerit() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 削除されるノード数を返す．
  int
  merit() const
  {
    return mDeleteMark.size();
  }

  /// @brief 削除されるノードかどうか調べる．
  /// @return 削除されるノードの時 true を返す．
  bool
  check(
    const AigNode* node ///< [in] 対象のノード
  ) const
  {
    return mDeleteMark.count(node->id()) > 0;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  void
  calc_sub(
    AigNode* node
  );

  void
  delete_node(
    AigNode* node
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号をキーにしてカウントを保持する辞書
  std::unordered_map<SizeType, int> mCountDict;

  // 削除されるノードの印
  std::unordered_set<SizeType> mDeleteMark;

};

END_NAMESPACE_YM_AIG

#endif // CALCMERIT_H
