
/// @file CkMatrix.cc
/// @brief CkMatrix の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CkMatrix.h"


BEGIN_NAMESPACE_YM_RC

BEGIN_NONAMESPACE

// キューブのシグネチャ(vector<SizeType>)用のハッシュ関数
struct SigHash
{
  SizeType
  operator()(
    const std::vector<SizeType>& sig_list
  ) const
  {
    SizeType ans = 0;
    for ( auto sig: sig_list ) {
      ans = ans * 1048573 + sig;
    }
    return ans;
  }
};

// キューブ(を表すリテラルのリスト)を正規化する．
std::vector<SizeType>
cube_signature(
  const std::vector<Literal>& lits,
  const std::vector<SizeType>& input_map
)
{
  std::vector<SizeType> sig_list;
  sig_list.reserve(lits.size());
  for ( auto lit: lits ) {
    auto vid = lit.varid();
    auto sig = input_map[vid] * 2;
    if ( lit.is_negative() ) {
      ++ sig;
    }
    sig_list.push_back(sig);
  }
  return sig_list;
}

END_NONAMESPACE

// @brief コンストラクタ
CkMatrix::CkMatrix(
  const std::vector<KIX::CoverInfo>& cover_list
) : mCoverList{cover_list}
{
  // 同一のカーネルキューブを列に対応させるための辞書
  // カバーごとに入力が異なるので正規化を行う．
  std::unordered_map<std::vector<SizeType>, SizeType, SigHash> kc_dict;

  auto cover_num = cover_list.size();
  for ( SizeType cover_id = 0; cover_id < cover_num; ++ cover_id ) {
    auto& cover_info = cover_list[cover_id];
    auto& cover = cover_info.cover;
    auto& inputs = cover_info.inputs;
    // 元のキューブにID番号を割り振る．
    std::unordered_map<SopCube, SizeType> cube_dict;
    {
      SizeType nc = cover.cube_num();
      for ( SizeType cube_id = 0; cube_id < nc; ++ cube_id ) {
	auto cube = cover.get_cube(cube_id);
	// キューブの価値はリテラル数
	auto value = cube.literal_num();
	auto vid = add_value(value);
	mValueArray.push_back(ValueInfo{cover_id, cube_id});
	cube_dict.emplace(cube, vid);
      }
    }
    // カーネルを列挙する．
    auto kernel_list = cover.all_kernels();
    for ( auto& p: kernel_list ) {
      auto& kernel = p.first;
      auto& cokernel_list = p.second;
      if ( cokernel_list.size() == 1 &&
	   cokernel_list.front().literal_num() == 0 ) {
	// 元の式全体がカーネルの場合は除外する．
	continue;
      }
      auto nk = kernel.cube_num();
      std::vector<SopCube> kcube_array;
      kcube_array.reserve(nk);
      std::vector<SizeType> col_id_array(nk);
      // カーネルキューブを列に対応させる．
      for ( SizeType i = 0; i < nk; ++ i ) {
	kcube_array.push_back(kernel.get_cube(i));
	auto& kcube = kcube_array.back();
	auto klits = kcube.literal_list();
	auto sig = cube_signature(klits, inputs);
	SizeType col_id;
	if ( kc_dict.count(sig) == 0 ) {
	  // 新規登録
	  col_id = col_size();
	  kc_dict.emplace(sig, col_id);
	  mColArray.push_back(ColInfo{klits});
	  // 列のコストはキューブのリテラル数
	  auto col_cost = kcube.literal_num();
	  insert_col(col_cost);
	}
	else {
	  // 既に同じキューブが登録されていた．
	  col_id = kc_dict.at(sig);
	}
	col_id_array[i] = col_id;
      }
      // コカーネルキューブを行に対応させる．
      auto nc = cokernel_list.size();
      for ( SizeType j = 0; j < nc; ++ j ) {
	auto& ccube = cokernel_list[j];
	// 行のコストはキューブのリテラル数+1
	auto row_cost = ccube.literal_num() + 1;
	auto row_id = row_size();
	insert_row(row_cost);
	mRowArray.push_back(RowInfo{cover_id, ccube});
	// 要素を挿入する．
	for ( SizeType i = 0; i < nk; ++ i ) {
	  // 元のキューブを作る．
	  auto cube = ccube & kcube_array[i];
	  if ( cube_dict.count(cube) == 0 ) {
	    throw std::logic_error{"cube is not registered"};
	  }
	  auto vid = cube_dict.at(cube);
	  add_elem(vid, row_id, col_id_array[i]);
	}
      }
    }
  }
}

// @brief 内容を出力する．
void
CkMatrix::print(
  std::ostream& s
) const
{
  const char* col_sp = "";
  for ( auto& col_info: mColArray ) {
    s << col_sp;
    col_sp = "| ";
    const char* spc = "";
    for ( auto lit: col_info.cube ) {
      s << spc << "v"
	<< lit.varid();
      if ( lit.is_negative() ) {
	s << "'";
      }
      spc = " ";
    }
  }
  s << std::endl;
  s << std::endl;
  for ( auto& row_info: mRowArray ) {
    auto cover_id = row_info.cover_id;
    auto& cube = row_info.ccube;
    s << cover_id << " " << cube << std::endl;
  }
  s << std::endl;

  for ( SizeType row_id = 0; row_id < row_size(); ++ row_id ) {
    const char* spc = "";
    for ( auto elem: row_list(row_id) ) {
      s << spc << "(" << elem->row() << ", " << elem->col()
	<< ")[" << elem->val_id() << "]";
      spc = ", ";
    }
    s << std::endl;
  }
}

END_NAMESPACE_YM_RC
