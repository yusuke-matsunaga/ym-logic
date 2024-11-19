
/// @file dot_test.cc
/// @brief dot_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <libgen.h>
#include "ym/Bdd.h"
#include "ym/BddMgr.h"
#include "ym/BddVar.h"


BEGIN_NAMESPACE_YM

int
dot_test(
  int argc,
  char** argv
)
{
  for ( int i = 1; i < argc; ++ i ) {
    string filename = argv[i];
    ifstream s{filename};
    if ( !s ) {
      cerr << filename << ": No such file." << endl;
      return 1;
    }

    BddMgr mgr;
    string buf;
    vector<Bdd> func_list;
    while ( getline(s, buf) ) {
      auto f = mgr.from_truth(buf);
      func_list.push_back(f);
    }
#if 0
    std::unordered_map<string, string> attr_dict;
    attr_dict["graph:bgcolor"] = "beige";
    attr_dict["edge0:color"] = "blue";
    attr_dict["edge1:color"] = "red";
    attr_dict["node:color"] = "mediumaquamarine";
    attr_dict["node:style"] = "filled";
    attr_dict["terminal:style"] = "filled";
    attr_dict["terminal0:color"] = "mediumpurple";
    attr_dict["terminal1:color"] = "lightsalmon";
    mgr.gen_dot(cout, func_list, attr_dict);
#else
    mgr.gen_dot(cout, func_list);
#endif
  }
  return 0;
}

END_NAMESPACE_YM

int
main(
  int argc,
  char** argv
)
{
  return YM_NAMESPACE::dot_test(argc, argv);
}
