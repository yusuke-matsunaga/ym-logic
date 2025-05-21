#ifndef CUTMGR_H
#define CUTMGR_H

/// @file CutMgr.h
/// @brief CutMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "aig/AigNode.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class Cut CutMgr.h "CutMgr.h"
/// @brief カットを表すクラス
///
/// - 葉のノードのリスト
/// - 内部に含まれるノードのリスト
/// を持つ．
/// 根のノードはコンテキストから明らかなのでここでは保持しない．
//////////////////////////////////////////////////////////////////////
class Cut
{
public:

  /// @brief 4入力論理関数を表す型
  using Tv4Type = std::uint16_t;

public:

  /// @brief 自身を葉とするコンストラクタ
  Cut(
    const AigNode* node
  ) : mLeafList{node}
  {
  }

  /// @brief コンストラクタ
  Cut(
    const std::vector<const AigNode*>& leaf_list, ///< [in] 葉のリスト
    const std::vector<const AigNode*>& node_list  ///< [in] 内部ノードのリスト
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

  /// @brief 葉のサイズを返す．
  SizeType
  leaf_size() const
  {
    return mLeafList.size();
  }

  /// @brief 葉のノードのリストを返す．
  const std::vector<const AigNode*>&
  leaf_list() const
  {
    return mLeafList;
  }

  /// @brief 内部のノードのリストを返す．
  const std::vector<const AigNode*>&
  node_list() const
  {
    return mNodeList;
  }

  /// @brief 論理関数を計算する．
  Tv4Type
  calc_tv(
    const AigNode* root ///< [in] 根のノード
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 葉のノードのリスト
  std::vector<const AigNode*> mLeafList;

  // 内部のノードのリスト
  std::vector<const AigNode*> mNodeList;

};


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
class CutMgr
{
public:

  /// @brief コンストラクタ
  CutMgr(
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
    const AigNode* node ///< [in] 根のノード
  )
  {
    return _enum_cuts(node);
  }

  /// @brief ノードの削除に伴ってカットを削除する．
  void
  erase_cuts(
    const AigNode* node ///< [in] 対象のノード
  )
  {
    if ( mCutHash.count(node->id()) > 0 ) {
      auto& cut_list = mCutHash.at(node->id());
      remove_cuts(cut_list);
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief enum_cuts() の下請け関数
  const std::vector<Cut*>&
  _enum_cuts(
    const AigNode* node ///< [in] 根のノード
  );

  /// @brief 2つのカットをマージする．
  /// @return 結果のカットを返す．
  ///
  /// 不正なカットの組み合わせの場合は nullptr を返す．
  Cut*
  _merge_cuts(
    const AigNode* root, ///< [in] 根のノード
    const Cut* cut0,     ///< [in] fanin0 のカット
    const Cut* cut1      ///< [in] fanin1 のカット
  );

  /// @brief カットのリストを削除する．
  void
  remove_cuts(
    const std::vector<Cut*>& cut_list
  )
  {
    for ( auto cut: cut_list ) {
      delete cut;
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // カットサイズ
  SizeType mCutSize{0};

  // ノード番号をキーにしてカットのリストを持つハッシュ表
  std::unordered_map<SizeType, std::vector<Cut*>> mCutHash;

};

END_NAMESPACE_YM_AIG

#endif // CUTMGR_H
