
/// @file Bdd_gen_dot.cc
/// @brief Bdd::gen_dot() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "BddEdge.h"
#include "DotGen.h"


BEGIN_NAMESPACE_YM_BDD

// @brief dot 形式で出力する．
void
Bdd::gen_dot(
  ostream& s,
  const unordered_map<string, string>& attr_dict
) const
{
  DotGen dg{s, attr_dict};
  dg.write({mRoot});
}

// @brief 複数のBDDを dot 形式で出力する．
void
Bdd::gen_dot(
  ostream& s,
  const vector<Bdd>& bdd_list,
  const unordered_map<string, string>& attr_dict
)
{
  vector<BddEdge> edge_list;
  (void) root_list(bdd_list, edge_list);
  DotGen dg{s, attr_dict};
  dg.write(edge_list);
}

END_NAMESPACE_YM_BDD
