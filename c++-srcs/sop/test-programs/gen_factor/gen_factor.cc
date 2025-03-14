
/// @file gen_factor.cc
/// @brief gen_factor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <unistd.h>
#include <libgen.h>
#include "ym/SopCover.h"


BEGIN_NAMESPACE_YM

void
usage(
  string argv0
)
{
  cerr << "Usage: "
       << basename(const_cast<char*>(argv0.c_str()))
       << "[-q|-g|-b] <filename>"
       << endl;
  exit(1);
}

int
gen_factor(
  int argc,
  char** argv
)
{
  bool quick_factor = false;
  bool good_factor = false;
  bool bool_factor = false;

  auto argv0 = string{argv[0]};

  int ch;
  while ( (ch = getopt(argc, argv, "qgb")) != -1 ) {
    switch (ch) {
    case 'q': quick_factor = true; break;
    case 'g': good_factor = true; break;
    case 'b': bool_factor = true; break;
    case '?':
    default: usage(argv0); break;
    }
  }
  argc -= optind;
  argv += optind;

  if ( argc != 1 ) {
    usage(argv0);
  }

  auto filename = string{argv[0]};
  ifstream s{filename};
  if ( !s ) {
    cerr << filename << ": No such file";
    return 2;
  }

  if ( !quick_factor && !good_factor && !bool_factor ) {
    good_factor = true;
  }

  auto cover = SopCover::read(s);

  cout << "Initial Cover: "
       << setw(10) << cover.cube_num() << " cubes, "
       << setw(10) << cover.literal_num() << " literals" << endl;

  Expr expr;
  if ( quick_factor ) {
    expr = cover.quick_factor();
  }
  else if ( good_factor ) {
    expr = cover.good_factor();
  }
  else if ( bool_factor ) {
    expr = cover.bool_factor();
  }
  cout << "Factored Form:                   "
       << setw(10) << expr.literal_num() << " literals" << endl;
  cout << expr << endl;
  return 0;
}

END_NAMESPACE_YM


int
main(
  int argc,
  char** argv
)
{
  return YM_NAMESPACE::gen_factor(argc, argv);
}
