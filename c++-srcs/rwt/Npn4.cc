
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
  if ( iperm.size() != 4 ) {
    throw std::invalid_argument{"iperm.size() != 4"};
  }
  std::uint16_t index = 0;
  bool found = false;
  for ( ; index < 24; ++ index ) {
    auto& perm = perm_list[index];
    bool ok = true;
    for ( SizeType i = 0; i < 4; ++ i ) {
      if ( perm[i] != iperm[i] ) {
	ok = false;
	break;
      }
    }
    if ( ok ) {
      found = true;
      break;
    }
  }
  if ( !found ) {
    throw std::invalid_argument{"iperm is invalid"};
  }

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
}

// @brief 逆変換を返す．
Npn4
Npn4::operator~() const
{
}

// @brief 合成を返す．
Npn4
Npn4::operator*(
  const Npn4& right
) const
{
}

END_NAMESPACE_YM_AIG
