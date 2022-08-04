
/// @file blif2bdd.cc
/// @brief blif2bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <libgen.h>
#include "ym/BnNetwork.h"
#include "ym/Bdd.h"
#include "ym/BddMgr.h"
#include "Bnet2Bdd.h"


BEGIN_NAMESPACE_YM

int
blif2bdd(
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

    auto net = BnNetwork::read_blif(filename);

    BddMgr mgr;
    Bnet2Bdd bnet2bdd{mgr, net};
    auto global_func = bnet2bdd.make_global_func();
    cout << basename(argv[i]) << ": " << Bdd::size(global_func) << endl;
    Bdd::display(cout, global_func);
    mgr.disable_gc();
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
  return YM_NAMESPACE::blif2bdd(argc, argv);
}
