#ifndef REPLACEMGR_H
#define REPLACEMGR_H

/// @file ReplaceMgr.h
/// @brief ReplaceMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "AigNode.h"


BEGIN_NAMESPACE_YM_AIG

class AigMgrImpl;
class AigHandle;

//////////////////////////////////////////////////////////////////////
/// @class FoInfo FoInfo.h "FoInfo.h"
/// @brief ファンアウト情報をあらわす構造体
//////////////////////////////////////////////////////////////////////
struct FoInfo
{
  /// @brief ファンイン0のファンアウトノードのリスト
  std::vector<AigNode*> fanin0_list;

  /// @brief ファンイン1のファンアウトノードのリスト
  std::vector<AigNode*> fanin1_list;

  /// @brief ハンドルのリスト
  std::vector<AigHandle*> handle_list;

};


//////////////////////////////////////////////////////////////////////
/// @class ReplaceMgr ReplaceMgr.h "ReplaceMgr.h"
/// @brief ノード(枝)の置き換えを行うクラス
///
/// 実際の処理は AigMgrImpl が行う．
/// このクラスはスコープを用いて生存期間をコントロールするためだけのもの
//////////////////////////////////////////////////////////////////////
class ReplaceMgr
{
  friend class AigMgrImpl;

public:

  /// @brief コンストラクタ
  ReplaceMgr(
    AigMgrImpl* mgr
  );

  /// @brief デストラクタ
  ~ReplaceMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードのファンアウト情報を持つ時 true を返す．
  bool
  has_fo_info(
    AigNode* node ///< [in] 対象のノード
  ) const
  {
    return mFoDict.count(node->id()) > 0;
  }

  /// @brief ノードのファンアウト情報を得る．
  FoInfo&
  fo_info(
    AigNode* node ///< [in] 対象のノード
  )
  {
    auto id = node->id();
    if ( mFoDict.count(id) == 0 ) {
      mFoDict.emplace(id, FoInfo{});
    }
    return mFoDict.at(id);
  }

  /// @brief ノードのファンアウト情報を得る．
  const FoInfo&
  fo_info(
    AigNode* node ///< [in] 対象のノード
  ) const
  {
    auto id = node->id();
    if ( mFoDict.count(id) == 0 ) {
      std::ostringstream buf;
      buf << "Node#" << id << " has no FoInfo";
      throw std::logic_error{buf.str()};
    }
    return mFoDict.at(id);
  }

  /// @brief ノードのファンアウト情報を削除する．
  void
  erase(
    AigNode* node ///< [in] 対象のノード
  )
  {
    mFoDict.erase(node->id());
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // AigMgr
  AigMgrImpl* mMgr{nullptr};

  // ノード番号をキーにしてファンアウト情報を持つ辞書
  std::unordered_map<SizeType, FoInfo> mFoDict;

};

END_NAMESPACE_YM_AIG

#endif // REPLACEMGR_H
