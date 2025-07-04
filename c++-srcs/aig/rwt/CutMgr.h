#ifndef CUTMGR_H
#define CUTMGR_H

/// @file CutMgr.h
/// @brief CutMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "../AigMgrImpl.h"
#include "../AigNode.h"


BEGIN_NAMESPACE_YM_AIG

class ReplaceMgr;

//////////////////////////////////////////////////////////////////////
/// @class Cut CutMgr.h "CutMgr.h"
/// @brief カットを表すクラス
///
/// - 根のノード
/// - 葉のノードのリスト
/// - 内部に含まれるノードのリスト(根のノードも含む)
/// を持つ．
///
/// 実は構造的にはつながっていても論理関数的にはサポートになっていないノードが
/// 存在する．
/// そこで，一旦構造的なカットを求めたら論理関数を計算してサポートをチェックする
/// 必要がある．
//////////////////////////////////////////////////////////////////////
class Cut
{
public:

  /// @brief 4入力論理関数を表す型
  using Tv4Type = std::uint16_t;

public:

  /// @brief 自身を葉とするコンストラクタ
  Cut(
    AigNode* node
  ) : mLeafList{node}
  {
  }

  /// @brief 通常のコンストラクタ
  Cut(
    const std::vector<AigNode*>& leaf_list, ///< [in] 葉のリスト
    const std::vector<AigNode*>& node_list  ///< [in] 内部ノードのリスト
  ) : mLeafList{leaf_list},
      mNodeList{node_list}
  {
  }

  /// @brief デストラクタ
  ~Cut() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 根のノードを返す．
  AigNode*
  root() const
  {
    if ( mNodeList.empty() ) {
      return mLeafList.front();
    }
    return mNodeList.back();
  }

  /// @brief 葉のサイズを返す．
  SizeType
  leaf_size() const
  {
    return mLeafList.size();
  }

  /// @brief 葉のノードのリストを返す．
  const std::vector<AigNode*>&
  leaf_list() const
  {
    return mLeafList;
  }

  /// @brief 葉のノードを返す．
  AigNode*
  leaf(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < leaf_size() )
  ) const
  {
    if ( pos >= mLeafList.size() ) {
      throw std::out_of_range{"pos is out of range"};
    }
    return mLeafList[pos];
  }

  /// @brief 内部のノードのリストを返す．
  const std::vector<AigNode*>&
  node_list() const
  {
    return mNodeList;
  }

  /// @brief 論理関数を計算する．
  Tv4Type
  calc_tv() const;

  /// @brief 内容が正しいかチェックする．
  bool
  check() const;

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 葉のノードのリスト
  std::vector<AigNode*> mLeafList;

  // 内部のノードのリスト
  // 葉からのトポロジカル順になっている．
  // 最後のノードが根のノードとなる．
  std::vector<AigNode*> mNodeList;

};

inline
std::ostream&
operator<<(
  std::ostream& s,
  const Cut& cut
)
{
  cut.print(s);
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @class CutMgr CutMgr.h "CutMgr.h"
/// @brief カットの列挙を行うクラス
///
/// カットの列挙はファンインのカットの直積を用いる．
/// その際に重複した計算を防ぐために入力側からのトポロジカル順で行う．
/// ただし，途中で AIG の構造が変わっている場合があるので注意が必要．
///
/// 生成されたカットの参照は CutMgr が所有する．
/// CutMgr が破棄される時にすべて破棄される．
//////////////////////////////////////////////////////////////////////
class CutMgr :
  public EventMgr
{
public:

  /// @brief コンストラクタ
  CutMgr(
    AigMgrImpl* mgr,  ///< [in] 親の AigMgr
    SizeType cut_size ///< [in] カットの入力数の最大値
  );

  /// @brief デストラクタ
  ///
  /// ここで生成された全てのカットを開放する．
  ~CutMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カットを列挙する．
  const std::vector<Cut*>&
  enum_cuts(
    AigNode* node ///< [in] 根のノード
  )
  {
    return _enum_cuts(node);
  }

  /// @brief カットの情報をクリアする．
  void
  clear_cuts(
    AigNode* node,            ///< [in] 対象のノード
    const ReplaceMgr& rep_mgr ///< [in] ファンアウト情報を持つマネージャ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief enum_cuts() の下請け関数
  const std::vector<Cut*>&
  _enum_cuts(
    AigNode* node ///< [in] 根のノード
  );

  /// @brief 2つのカットをマージする．
  /// @return 結果のカットを返す．
  ///
  /// 不正なカットの組み合わせの場合は nullptr を返す．
  Cut*
  _merge_cuts(
    AigNode* root,                              ///< [in] 根のノード
    const Cut* cut0,                            ///< [in] fanin0 のカット
    const Cut* cut1,                            ///< [in] fanin1 のカット
    const std::unordered_set<SizeType>& fp_mark ///< [in] フットプリントのマーク
  );

  /// @brief フットプリントをマージする．
  std::unordered_set<SizeType>
  _merge_footprint(
    AigNode* node0,
    AigNode* node1
  );

  /// @brief カットのリストを削除する．
  void
  _remove_cuts(
    const std::vector<Cut*>& cut_list
  )
  {
    for ( auto cut: cut_list ) {
      delete cut;
    }
  }

  /// @brief カットの情報が正しいかチェックする．
  void
  _sanity_check() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // カットサイズ
  SizeType mCutSize{0};

  // ノード番号をキーにしてカットのリストを持つハッシュ表
  std::unordered_map<SizeType, std::vector<Cut*>> mCutDict;

  // ノード番号をキーにしてカットリストのフットプリントを持つハッシュ表
  std::unordered_map<SizeType, std::vector<AigNode*>> mFootPrintDict;

};

END_NAMESPACE_YM_AIG

#endif // CUTMGR_H
