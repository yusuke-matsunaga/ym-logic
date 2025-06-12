#ifndef FRAIGENC_H
#define FRAIGENC_H

/// @file FraigEnc.h
/// @brief FraigEnc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "FraigMgr.h"
#include "ym/BnNode.h"
#include "ym/Expr.h"
#include "ym/TvFunc.h"
#include "ym/Bdd.h"


BEGIN_NAMESPACE_EQUIV

//////////////////////////////////////////////////////////////////////
/// @class FraigEnc FraigEnc.h "FraigEnc.h"
/// @brief BnModel の論理関数を表す AIG を作るクラス
//////////////////////////////////////////////////////////////////////
class FraigEnc
{
public:

  /// @brief コンストラクタ
  FraigEnc(
    FraigMgr& mgr ///< [in] FraigMgr
  ) : mMgr{mgr}
  {
  }

  /// @brief デストラクタ
  ~FraigEnc() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークの出力の論理関数 を AIG に変換する．
  vector<FraigHandle>
  operator()(
    const BnModel& network,           ///< [in] ネットワーク
    const vector<FraigHandle>& inputs ///< [in] 入力のハンドルのリスト
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BnNode の出力の論理関数を Fraig に変換する．
  FraigHandle
  node2aig(
    const BnNode& node ///< [in] ノード
  );

  /// @brief Expr を Fraig に変換する．
  FraigHandle
  expr2aig(
    const Expr& expr ///< [in] 論理式
  );

  /// @brief TvFunc を Fraig に変換する．
  FraigHandle
  tv2aig(
    const TvFunc& func,  ///< [in] 関数
    SizeType var         ///< [in] 次に分解すべき変数
  );

  /// @brief BDDをAIGに変換する．
  FraigHandle
  bdd2aig(
    const Bdd& bdd ///< [in] BDD
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // FraigMgr
  FraigMgr& mMgr;

  // ノード番号をキーにして Fraig を格納する辞書
  std::unordered_map<SizeType, FraigHandle> mNodeDict;

  // 入力ハンドルのリスト
  vector<FraigHandle> mInputList;

  // TvFunc をキーにして Fraig を格納する辞書
  std::unordered_map<TvFunc, FraigHandle> mTvDict;

  // Bdd をキーにして Fraig を格納する辞書
  std::unordered_map<Bdd, FraigHandle> mBddDict;

};

END_NAMESPACE_EQUIV

#endif // FRAIGENC_H
