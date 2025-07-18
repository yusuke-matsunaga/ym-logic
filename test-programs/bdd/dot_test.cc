
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
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM

int
dot_test(
  int argc,
  char** argv
)
{
  for ( int i = 1; i < argc; ++ i ) {
    std::string filename = argv[i];
    std::ifstream s{filename};
    if ( !s ) {
      std::cerr << filename << ": No such file." << std::endl;
      return 1;
    }

    BddMgr mgr;
    std::string buf;
    std::vector<Bdd> func_list;
    while ( getline(s, buf) ) {
      auto f = mgr.from_truth(buf);
      func_list.push_back(f);
    }
    auto nv = mgr.variable_num();
    std::vector<JsonValue> label_array(nv);
#if 1
    for ( SizeType i = 0; i < nv; ++ i ) {
      std::ostringstream buf;
      buf << "$X_{" << (i + 1) << "}$";
      auto label = buf.str();
      label_array[i] = JsonValue{label};
    }
    auto label_array_obj = JsonValue{label_array};
    std::unordered_map<std::string, JsonValue> option_dict;
    option_dict.emplace("var_texlbl", label_array_obj);
#else
    for ( SizeType i = 0; i < nv; ++ i ) {
      char c = 'a' + i;
      std::string label;
      label += c;
      label_array[i] = JsonValue{label};
    }
    auto label_array_obj = JsonValue{label_array};
    std::unordered_map<string, JsonValue> option_dict;
    option_dict.emplace("var_label", label_array_obj);
#endif
    auto option = JsonValue{option_dict};
    Bdd::gen_dot(std::cout, func_list, option);
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
