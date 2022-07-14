
/// @file mwc_test.cc
/// @brief mwc_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <libgen.h>
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM

int
mwc_test(
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
    string buf;
    vector<SizeType> bad_outputs;
    vector<TvFunc> func_list;
    SizeType lit_num = 0;
    while ( getline(s, buf) ) {
      TvFunc f{buf};
      func_list.push_back(f);
      auto expr = f.MWC_expr();
      auto f_expr = expr.make_tv(f.input_num());
      if ( f != f_expr ) {
	bad_outputs.push_back(func_list.size() - 1);
      }
      lit_num += expr.literal_num();
    }
    SizeType no = func_list.size();
    SizeType ni = no > 0 ? func_list[0].input_num() : 0;
    cout << basename(argv[i]) << ": #i: " << ni << " #o: " << no
	 << " #L: " << lit_num << endl;
    for ( auto i: bad_outputs ) {
      cout << "O#" << i << ": " << func_list[i] << endl;
    }
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
  return YM_NAMESPACE::mwc_test(argc, argv);
}
