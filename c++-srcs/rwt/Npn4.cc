
/// @file Npn4.cc
/// @brief Npn4 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "Npn4.h"


BEGIN_NAMESPACE_YM_AIG

// memo
//
// mChunk の内訳
// - [0:4] : 入力の順列を表す．
//           実際には 0(00000) から 23(11001) までしか使わない．
// - [5:8] : 入力の反転属性を表す．
//           [5] が入力０ [8] が入力3 の反転属性に対応する．
// - [9]   : 出力の反転属性を表す．
//
// perm_list は順列番号をインデックスとして順列を保持する
//

BEGIN_NONAMESPACE

// 順列のリスト
static
std::uint8_t perm_list[][4] = {
  {0, 1, 2, 3},
  {0, 1, 3, 2},
  {0, 2, 1, 3},
  {0, 2, 3, 1},
  {0, 3, 1, 2},
  {0, 3, 2, 1},
  {1, 0, 2, 3},
  {1, 0, 3, 2},
  {1, 2, 0, 3},
  {1, 2, 3, 0},
  {1, 3, 0, 2},
  {1, 3, 2, 0},
  {2, 0, 1, 3},
  {2, 0, 3, 1},
  {2, 1, 0, 3},
  {2, 1, 3, 0},
  {2, 3, 0, 1},
  {2, 3, 1, 0},
  {3, 0, 1, 2},
  {3, 0, 2, 1},
  {3, 1, 0, 2},
  {3, 1, 2, 0},
  {3, 2, 0, 1},
  {3, 2, 1, 0}
};

// 逆変換用のテーブル
static
std::uint8_t inv_tbl[] = {
#include "rwt/inv_tbl.h"
};

// 入力順(iperm)をシグネチャに変換する．
// 具体的には各々の値を2ビットずつシフトして足し合わせる．
SizeType
perm2sig(
  const vector<SizeType>& iperm
)
{
  SizeType sig = 0;
  for ( SizeType i = 0; i < 4; ++ i ) {
    auto val = iperm[i] & 3;
    sig |= val << (i * 2);
  }
  return sig;
}

// 入力順(iperm)をインデックス番号に変換する．
// iperm が不正な時は std::invalid_argument 例外を送出する．
std::uint16_t
perm2index(
  const vector<SizeType>& iperm
)
{
  static std::uint8_t index_tbl[] = {
#include "rwt/index_tbl.h"
  };

  if ( iperm.size() != 4 ) {
    throw std::invalid_argument{"iperm.size() != 4"};
  }
  auto sig = perm2sig(iperm);
  std::uint16_t index = index_tbl[sig];
  if ( index == 255 ) {
    throw std::invalid_argument{"iperm is invalid"};
  }
  return index;
}


END_NONAMESPACE

// @brief 内容を指定したコンストラクタ
Npn4::Npn4(
  bool oinv,
  const vector<bool>& iinv,
  const vector<SizeType>& iperm
)
{
  if ( iinv.size() != 4 ) {
    throw std::invalid_argument{"iinv.size() != 4"};
  }
  auto index = perm2index(iperm);
  mChunk = index;
  for ( SizeType i = 0; i < 4; ++ i ) {
    if ( iinv[i] ) {
      mChunk |= (1 << (i + 5));
    }
  }
  if ( oinv ) {
    mChunk |= (1 << 9);
  }
}

// @brief 入力の置換結果を返す．
SizeType
Npn4::iperm(
  SizeType pos
) const
{
  if ( pos < 0 || 4 <= pos ) {
    std::out_of_range{"pos is out of range"};
  }
  auto index = mChunk & 31;
  return perm_list[index][pos];
}

/// @brief 関数のNPN変換結果を求める．
Npn4::Tv4
Npn4::operator()(
  Tv4 tv
) const
{
  static std::uint8_t xform_tbl[][16] = {
#include "rwt/xform_tbl.h"
  };

  if ( mChunk & (1 << 5) ) {
    // 入力0の反転
    auto tv1 = tv & 0xAAAA;
    auto tv0 = tv & 0x5555;
    tv = (tv1 >> 1) | (tv0 << 1);
  }
  if ( mChunk & (1 << 6) ) {
    // 入力1の反転
    auto tv1 = tv & 0xCCCC;
    auto tv0 = tv & 0x3333;
    tv = (tv1 >> 2) | (tv0 << 2);
  }
  if ( mChunk & (1 << 7) ) {
    // 入力2の反転
    auto tv1 = tv & 0xF0F0;
    auto tv0 = tv & 0x0F0F;
    tv = (tv1 >> 4) | (tv0 << 4);
  }
  if ( mChunk & (1 << 8) ) {
    // 入力3の反転
    auto tv1 = tv & 0xFF00;
    auto tv0 = tv & 0x00FF;
    tv = (tv1 >> 8) | (tv0 << 8);
  }
  Tv4 new_tv = 0;
  for ( SizeType b = 0; b < 16; ++ b ) {
    if ( tv & b ) {
      auto new_b = xform_tbl[_index()][b];
      new_tv |= new_b;
    }
  }
  return new_tv;
}

// @brief 逆変換を返す．
Npn4
Npn4::operator~() const
{
  auto inv_index = inv_tbl[_index()];
  std::uint16_t chunk = inv_index;
  auto& inv_iperm = perm_list[inv_index];
  if ( oinv() ) {
    chunk |= (1 << 9);
  }
  for ( SizeType i = 0; i < 4; ++ i ) {
    if ( iinv(inv_iperm[i]) ) {
      chunk |= (1 << (i + 5));
    }
  }
  return Npn4(chunk);
}

// @brief 合成を返す．
Npn4
Npn4::operator*(
  const Npn4& right
) const
{
  static std::uint8_t compose_tbl[][24] = {
#include "rwt/compose_tbl.h"
  };
  auto index1 = _index();
  auto index2 = right._index();
  auto index = compose_tbl[index1][index2];
  std::uint16_t chunk = index;
  if ( oinv() != right.oinv() ) {
    chunk |= (1 << 9);
  }
  auto inv_index = inv_tbl[_index()];
  auto& inv_iperm = perm_list[inv_index];
  for ( SizeType i = 0; i < 4; ++ i ) {
    if ( iinv(i) != right.iinv(inv_iperm[i]) ) {
      chunk |= (1 << (i + 5));
    }
  }
  return Npn4(chunk);
}

END_NAMESPACE_YM_AIG
