
/// @file KIX.cc
/// @brief KIX の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/KIX.h"
#include "ym/SopCover.h"
#include "ym/SopCube.h"
#include "CkMatrix.h"
#include <unordered_set>


BEGIN_NAMESPACE_YM_RC

BEGIN_NONAMESPACE

struct HashFunc
{
  SizeType
  operator()(
    const std::pair<SizeType, SizeType>& a
  ) const
  {
    return a.first * 1048573 + a.second;
  }
};

// 入力番号を正規化する．
std::vector<Literal>
normalize(
  const std::vector<Literal>& src_list,
  const std::vector<SizeType>& inputs
)
{
  std::vector<Literal> ans_list;
  for ( auto lit: src_list ) {
    auto new_vid = inputs[lit.varid()];
    auto inv = lit.is_negative();
    auto new_lit = Literal(new_vid, inv);
    ans_list.push_back(new_lit);
  }
  return ans_list;
}

// 入力番号をパックする．
KIX::CoverInfo
pack(
  const std::vector<std::vector<Literal>>& src_cube_list
)
{
  // 現れる入力番号のリストを作る．
  std::vector<SizeType> input_list;
  std::unordered_set<SizeType> input_set;
  for ( auto& lits: src_cube_list ) {
    for ( auto lit: lits ) {
      auto vid = lit.varid();
      if ( input_set.count(vid) == 0 ) {
	input_set.emplace(vid);
	input_list.push_back(vid);
      }
    }
  }
  // ソートする．
  std::sort(input_list.begin(), input_list.end());
  // 今の変数番号をキーとして変換マップを作る．
  std::unordered_map<SizeType, SizeType> varmap;
  for ( SizeType i = 0; i < input_list.size(); ++ i ) {
    auto vid = input_list[i];
    varmap.emplace(vid, i);
  }
  // varmap に従って変換する．
  std::vector<std::vector<Literal>> dst_cube_list;
  dst_cube_list.reserve(src_cube_list.size());
  for ( auto& lits: src_cube_list ) {
    std::vector<Literal> new_lits;
    new_lits.reserve(lits.size());
    for ( auto lit: lits ) {
      auto vid = lit.varid();
      auto inv = lit.is_negative();
      auto new_lit = Literal(varmap.at(vid), inv);
      new_lits.push_back(new_lit);
    }
    dst_cube_list.push_back(new_lits);
  }
  auto new_cover = SopCover(input_list.size(), dst_cube_list);
  return KIX::CoverInfo{std::move(new_cover), std::move(input_list)};
}

END_NONAMESPACE

// @brief 共通な論理式を括りだす．
std::vector<KIX::CoverInfo>
KIX::extract(
  const std::vector<CoverInfo>& cover_list
)
{
  // co-kernel/kernel cube matrix を作る．
  auto matrix = CkMatrix(cover_list);

  // cover_list に含まれる入力番号の最大値を求める．
  SizeType max_input_id = 0;
  for ( auto& cover_info: cover_list ) {
    for ( auto input_id: cover_info.inputs ) {
      max_input_id = std::max(max_input_id, input_id);
    }
  }

  // 選択した矩形に対応する新しい入力番号
  SizeType new_input_id = max_input_id + 1;
  // cover_list から削除されるキューブの情報
  std::unordered_set<CkMatrix::ValueInfo> deleted_cubes;
  // 各カバーに追加されるキューブの情報
  // キーはカバー番号
  std::vector<std::vector<std::pair<SopCube, SizeType>>> added_cube_list(cover_list.size());
  // 新しく追加されるカバーの情報
  std::vector<std::vector<std::vector<Literal>>> new_cover_list;
  // 結果のサイズ
  SizeType ans_size = cover_list.size();
  for ( ; ; ) {
    auto rect_list = matrix.enum_max_rects();
    if ( rect_list.empty() ) {
      break;
    }
    auto rect = rect_list.front();
    if ( matrix.rect_value(rect) <= 0 ) {
      // 改善がなくなったら終わり
      break;
    }
    // rect によってカバーされた部分の価値番号のリスト
    auto vid_list = matrix.rect_vid_list(rect);
    // rect によってカバーされた部分の価値を0にする．
    for ( auto vid: vid_list ) {
      matrix.set_value(vid, 0);
    }
    // rect によって削除されるキューブの情報を記録する．
    for ( auto vid: vid_list ) {
      auto& value_info = matrix.value_info(vid);
      auto cover_id = value_info.cover_id;
      auto cube_id = value_info.cube_id;
      deleted_cubes.emplace(CkMatrix::ValueInfo{cover_id, cube_id});
    }
    // 代わりに追加されるキューブの情報を記録する．
    for ( auto row_id: rect.row_list() ) {
      auto& row_info = matrix.row_info(row_id);
      auto cover_id = row_info.cover_id;
      auto& cube = row_info.ccube;
      added_cube_list[cover_id].push_back(std::make_pair(cube, new_input_id));
    }
    // 新しく追加されるカバーの情報を記録する．
    std::vector<std::vector<Literal>> new_cover;
    new_cover.reserve(rect.col_size());
    for ( auto col_id: rect.col_list() ) {
      auto& col_info = matrix.col_info(col_id);
      new_cover.push_back(col_info.cube);
    }
    new_cover_list.push_back(new_cover);
    ++ new_input_id;
    ans_size += rect.col_size();
  }

  // 結果のカバーリストを作る．
  std::vector<CoverInfo> ans_cover_list;
  ans_cover_list.reserve(ans_size);

  // もとのカバーを置き換えながらコピーする．
  // めんどくさいのはもともとのカバーがローカルな入力番号を
  // 用いているということ．
  // そのため一回正規化されたリテラルのリストの変換する．
  SizeType n = cover_list.size();
  for ( SizeType i = 0; i < n; ++ i ) {
    auto& cover_info = cover_list[i];
    auto& cover = cover_info.cover;
    auto& inputs = cover_info.inputs;
    auto cube_list = cover.literal_list();
    SizeType nc = cover.cube_num();
    std::vector<std::vector<Literal>> new_cube_list;
    for ( SizeType j = 0; j < nc; ++ j ) {
      if ( deleted_cubes.count(CkMatrix::ValueInfo{i, j}) > 0 ) {
	// 削除された．
	continue;
      }
      new_cube_list.push_back(normalize(cube_list[j], inputs));
    }
    // 置き換え用の新しいキューブを追加する．
    for ( auto& p: added_cube_list[i] ) {
      auto& cube = p.first;
      auto new_input = p.second;
      auto new_cube = normalize(cube.literal_list(), inputs);
      new_cube.push_back(Literal(new_input));
      new_cube_list.push_back(new_cube);
    }
    // 入力番号をパックする．
    auto new_cover_info = pack(new_cube_list);
    ans_cover_list.push_back(new_cover_info);
  }

  // 新しいカバーを追加する．
  for ( auto& cover: new_cover_list ) {
    auto new_cover_info = pack(cover);
    ans_cover_list.push_back(new_cover_info);
  }

  return ans_cover_list;
}

END_NAMESPACE_YM_RC
