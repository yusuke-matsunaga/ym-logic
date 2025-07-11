
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

static char* argv0 = nullptr;

void
usage()
{
  std::cerr << "Usage: "
	    << basename(argv0)
	    << "[-q|-g|-b] <filename>"
	    << std::endl;
  exit(1);
}

int
gen_factor(
  int argc,
  char** argv
)
{
  argv0 = argv[0];

  bool quick_factor = false;
  bool good_factor = false;
  bool bool_factor = false;

  int ch;
  while ( (ch = getopt(argc, argv, "qgb")) != -1 ) {
    switch (ch) {
    case 'q': quick_factor = true; break;
    case 'g': good_factor = true; break;
    case 'b': bool_factor = true; break;
    case '?':
    default: usage(); break;
    }
  }
  argc -= optind;
  argv += optind;

  if ( argc != 1 ) {
    usage();
  }

  auto filename = std::string{argv[0]};
  std::ifstream s{filename};
  if ( !s ) {
    std::cerr << filename << ": No such file" << std::endl;
    return 2;
  }

  if ( !quick_factor && !good_factor && !bool_factor ) {
    good_factor = true;
  }

  auto cover = SopCover::read(s);

  std::cout << "Initial Cover: "
	    << std::setw(10) << cover.cube_num() << " cubes, "
	    << std::setw(10) << cover.literal_num() << " literals" << std::endl;

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
  std::cout << "Factored Form:                   "
	    << std::setw(10) << expr.literal_num() << " literals" << std::endl
	    << expr << std::endl;
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
