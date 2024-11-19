#ifndef DDINFOMGR_H
#define DDINFOMGR_H

/// @file DdInfoMgr.h
/// @brief DdInfoMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "dd/DdInfo.h"


BEGIN_NAMESPACE_YM_DD

class DdNodeMgr;
class DdEdge;

//////////////////////////////////////////////////////////////////////
/// @class DdInfoMgr DdInfoMgr.h "DdInfoMgr.h"
/// @brief DdInfo 用のマネージャクラス
//////////////////////////////////////////////////////////////////////
class DdInfoMgr
{
public:

  /// @brief コンストラクタ
  DdInfoMgr(
    const vector<DdEdge>& root_list, ///< [in] 根の枝のリスト
    DdNodeMgr* node_mgr              ///< [in] DdNodeMgr
  );

  /// @brief デストラクタ
  ~DdInfoMgr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief レベルの最大値(+1)を返す．
  SizeType
  max_level() const
  {
    return mIdListArray.size();
  }

  /// @brief レベルを変数番号に変換する．
  SizeType
  level_to_varid(
    SizeType level ///< [in] レベル ( 0 <= level < max_level() )
  ) const
  {
    _check_level(level);
    return mVarIdArray[level];
  }

  /// @brief 根の枝を表す整数のリストを返す．
  const vector<SizeType>&
  root_list() const
  {
    return mRootList;
  }

  /// @brief ノード数を返す．
  SizeType
  node_num() const
  {
    return mNodeList.size();
  }

  /// @brief ノード情報のリストを返す．
  const vector<DdInfo>&
  node_list() const
  {
    return mNodeList;
  }

  /// @brief 指定されたレベルのノード番号のリストを返す．
  const vector<SizeType>&
  id_list(
    SizeType level ///< [in] レベル ( 0 <= level < max_level() )
  ) const
  {
    _check_level(level);
    return mIdListArray[level];
  }

  /// @brief 内容を見やすい形式で出力する．
  void
  display(
    ostream& s ///< [in] 出力ストリーム
  ) const;

  // @brief 構造を表す整数配列を作る．
  vector<SizeType>
  rep_data() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief レベルの値をチェックする．
  void
  _check_level(
    SizeType level
  ) const
  {
    if ( level >= max_level() ) {
      throw std::out_of_range{"level is out of range"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // レベルをキーにして変数番号を保持する配列
  vector<SizeType> mVarIdArray;

  // 根の枝のリスト
  vector<SizeType> mRootList;

  // ノードリスト
  vector<DdInfo> mNodeList;

  // レベルごとのノード番号のリスト
  vector<vector<SizeType>> mIdListArray;

};

END_NAMESPACE_YM_DD

#endif // DDINFOMGR_H
