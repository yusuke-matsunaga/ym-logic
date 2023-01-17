
/// @file NpnMapM.cc
/// @brief NpnMapM の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/NpnMapM.h"
#include "ym/NpnMap.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
// クラス NpnMapM
//////////////////////////////////////////////////////////////////////

// @brief 入力数と出力数を指定したコンストラクタ
// @param[in] ni 入力数
// @param[in] no 出力数
// @note 各変数の変換内容は不正な値になっている．
NpnMapM::NpnMapM(
  SizeType ni,
  SizeType no
)
{
  resize(ni, no);
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
NpnMapM::NpnMapM(
  const NpnMapM& src
)
{
  copy(src);
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 自分自身への定数参照を返す．
NpnMapM&
NpnMapM::operator=(
  const NpnMapM& src
)
{
  if ( this != &src ) {
    copy(src);
  }
  return *this;
}

// @brief コピーする．
void
NpnMapM::copy(
  const NpnMapM& src
)
{
  SizeType ni = src.mInputNum;
  SizeType no = src.mOutputNum;
  resize(ni, no);
  SizeType n = ni + no;
  for ( SizeType i = 0; i < n; ++ i ) {
    mMapArray[i] = src.mMapArray[i];
  }
}

// @brief NpnMap からの変換コンストラクタ
// @note 出力数が1となる．
NpnMapM::NpnMapM(
  const NpnMap& src
)
{
  resize(src.input_num(), 1);
  for ( SizeType i = 0; i < mInputNum; ++ i ) {
    mMapArray[i] = src.imap(i);
  }
  set_omap(0, 0, src.oinv());
}

// @brief デストラクタ
NpnMapM::~NpnMapM()
{
  delete [] mMapArray;
}

// @brief 内容をクリアする．
// @note 各変数の変換内容は不正な値になる．
void
NpnMapM::clear()
{
  SizeType n = mInputNum + mOutputNum;
  for ( SizeType i = 0; i < n; ++ i ) {
    mMapArray[i] = NpnVmap::invalid();
  }
}

// @brief 入力数と出力数を再設定する．
void
NpnMapM::resize(
  SizeType ni,
  SizeType no
)
{
  SizeType n = ni + no;
  if ( mInputNum + mOutputNum != n ) {
    delete [] mMapArray;
    mMapArray = new NpnVmap[n];
  }
  mInputNum = ni;
  mOutputNum = no;
  clear();
}

// @brief 恒等変換を表すように設定する．
void
NpnMapM::set_identity(
  SizeType ni,
  SizeType no
)
{
  resize(ni, no);
  for ( SizeType i = 0; i < ni; ++ i ) {
    set_imap(i, i, false);
  }
  for ( SizeType i = 0; i < no; ++ i ) {
    set_omap(i, i, false);
  }
}

// @brief 入力の変換内容の設定
void
NpnMapM::set_imap(
  SizeType var,
  NpnVmap imap
)
{
  if ( var < input_num() ) {
    mMapArray[var] = imap;
  }
}

// @brief 出力の変換内容の設定
void
NpnMapM::set_omap(
  SizeType var,
  NpnVmap omap
)
{
  if ( var < output_num() ) {
    mMapArray[var + mInputNum] = omap;
  }
}

// @brief 内容が等しいか調べる．
bool
NpnMapM::operator==(
  const NpnMapM& src
) const
{
  if ( src.mInputNum != mInputNum || src.mOutputNum != mOutputNum ) {
    return false;
  }
  SizeType n = mInputNum + mOutputNum;
  for ( SizeType i = 0; i < n; ++ i ) {
    if ( mMapArray[i] != src.mMapArray[i] ) {
      return false;
    }
  }
  return true;
}

// @brief 逆写像を求める．
NpnMapM
inverse(
  const NpnMapM& src
)
{
  SizeType ni = src.input_num();
  SizeType no = src.output_num();

  NpnMapM ans(ni, no);

  for ( SizeType src_var = 0; src_var < ni; ++ src_var ) {
    auto imap = src.imap(src_var);
    if ( imap.is_invalid() ) {
      // 不正な値を返す．
      return NpnMapM(ni, no);
    }
    auto dst_var = imap.var();
    bool inv = imap.inv();
    ans.set_imap(dst_var, src_var, inv);
  }
  for (SizeType src_var = 0; src_var < no; ++ src_var) {
    auto omap = src.omap(src_var);
    if ( omap.is_invalid() ) {
      // 不正な値を返す．
      return NpnMapM(ni, no);
    }
    auto dst_var = omap.var();
    bool inv = omap.inv();
    ans.set_omap(dst_var, src_var, inv);
  }

  return ans;
}

// @brief 合成を求める．
NpnMapM
operator*(
  const NpnMapM& src1,
  const NpnMapM& src2
)
{
  SizeType ni = src1.input_num();
  SizeType no = src1.output_num();
  if ( ni != src2.input_num() || no != src2.output_num() ) {
    // 不正な値を返す．
    return NpnMapM(ni, no);
  }

  NpnMapM ans(ni, no);

  for (SizeType var1 = 0; var1 < ni; ++ var1) {
    auto imap1 = src1.imap(var1);
    if ( imap1.is_invalid() ) {
      return NpnMapM(ni, no);
    }
    auto var2 = imap1.var();
    bool inv2 = imap1.inv();
    auto imap2 = src2.imap(var2);
    if ( imap2.is_invalid() ) {
      return NpnMapM(ni, no);
    }
    auto var3 = imap2.var();
    bool inv3 = imap2.inv();
    ans.set_imap(var1, var3, inv2 ^ inv3);
  }

  for (SizeType var1 = 0; var1 < no; ++ var1) {
    auto omap1 = src1.omap(var1);
    if ( omap1.is_invalid() ) {
      return NpnMapM(ni, no);
    }
    auto var2 = omap1.var();
    bool inv2 = omap1.inv();
    auto omap2 = src2.omap(var2);
    if ( omap2.is_invalid() ) {
      return NpnMapM(ni, no);
    }
    auto var3 = omap2.var();
    bool inv3 = omap2.inv();
    ans.set_omap(var1, var3, inv2 ^ inv3);
  }

  return ans;
}

// @brief 内容を表示する(主にデバッグ用)．
ostream&
operator<<(
  ostream& s,
  const NpnMapM& map
)
{
  const char* comma = "";

  s << "INPUT(";
  for (SizeType i = 0; i < map.input_num(); ++ i) {
    s << comma;
    comma = ", ";
    s << i << " ==> ";
    auto imap = map.imap(i);
    if ( imap.is_invalid() ) {
      s << "--";
    }
    else {
      auto dst_var = imap.var();
      bool inv = imap.inv();
      if ( inv ) {
	s << "~";
      }
      s << dst_var;
    }
  }
  s << ")";

  comma = "";
  s << " OUTPUT(";
  for (SizeType i = 0; i < map.output_num(); ++ i) {
    s << comma;
    comma = ", ";
    s << i << " ==> ";
    auto omap = map.omap(i);
    if ( omap.is_invalid() ) {
      s << "--";
    }
    else {
      auto dst_var = omap.var();
      bool inv = omap.inv();
      if ( inv ) {
	s << "~";
      }
      s << dst_var;
    }
  }
  s << ")";

  return s;
}

// バイナリ出力
void
NpnMapM::dump(
  ostream& bos
) const
{
  SizeType ni = input_num();
  bos << ni;

  SizeType no = output_num();
  bos << no;

  for ( SizeType i = 0; i < ni; ++ i ) {
    auto vmap = imap(i);
    vmap.dump(bos);
  }

  for ( SizeType i = 0; i < no; ++ i ) {
    auto vmap = omap(i);
    vmap.dump(bos);
  }
}

// バイナリ入力
void
NpnMapM::restore(
  istream& bis
)
{
  SizeType ni;
  SizeType no;
  bis >> ni >> no;
  resize(ni, no);

  for ( SizeType i = 0; i < ni; ++ i ) {
    NpnVmap vmap;
    vmap.restore(bis);
    set_imap(i, vmap);
  }

  for ( SizeType i = 0; i < no; ++ i ) {
    NpnVmap vmap;
    vmap.restore(bis);
    set_omap(i, vmap);
  }
}

END_NAMESPACE_YM_LOGIC
