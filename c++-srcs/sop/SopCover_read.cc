
/// @file SopCover_read.cc
/// @brief SopCover::read() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SopCover.h"
#include "ym/SopCube.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopCover
//////////////////////////////////////////////////////////////////////

// @brief write() で書き出した形式で読み込む．
SopCover
SopCover::read(
  std::istream& s
)
{
  // 行単位で読み込む．
  std::vector<std::string> line_list;
  std::string line;
  while ( std::getline(s, line) ) {
    line_list.push_back(line);
  }
  if ( line_list.empty() ) {
    // 空でもエラーにはならない．
    return SopCover{};
  }

  auto line0 = line_list.front();
  auto nv = line0.size();
  // 全てのサイズが等しくなければならない．
  // 現れる文字は '0', '1', '-' のいずれか
  for ( auto& line: line_list ) {
    if ( line.size() != nv ) {
      throw std::invalid_argument{"line size mismatch"};
    }
    for ( auto c: line ) {
      if ( c != '0' && c != '1' && c != '-' ) {
	throw std::invalid_argument{"invalid character"};
      }
    }
  }
  // 内容を Literal のリストのリストに変換する．
  std::vector<std::vector<Literal>> literal_list;
  literal_list.reserve(line_list.size());
  for ( auto& line: line_list ) {
    std::vector<Literal> lits;
    lits.reserve(nv);
    for ( SizeType i = 0; i < nv; ++ i ) {
      switch ( line[i] ) {
      case '0': lits.push_back(Literal(i, true)); break;
      case '1': lits.push_back(Literal(i, false)); break;
      case '-': break;
      }
    }
    literal_list.push_back(lits);
  }
  return SopCover(nv, literal_list);
}

END_NAMESPACE_YM_SOP
