
/// @file rwt_test.cc
/// @brief rwt_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigMgr.h"
#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ym/BnFunc.h"
#include "ym/SopCover.h"
#include "ym/SopCube.h"
#include "ym/Expr.h"


BEGIN_NAMESPACE_YM

AigHandle
bn2aig(
  const BnNode& node,
  AigMgr& mgr,
  std::unordered_map<SizeType, AigHandle>& aig_map
)
{
  if ( !node.is_logic() ) {
    throw std::invalid_argument{"node must be logic type"};
  }
  auto nfi = node.fanin_num();
  std::vector<AigHandle> fanin_list(nfi);
  for ( SizeType i = 0; i < nfi; ++ i ) {
    auto inode = node.fanin(i);
    if ( aig_map.count(inode.id()) == 0 ) {
      std::ostringstream buf;
      buf << "Node#" << inode.id() << " is not defined";
      throw std::invalid_argument{buf.str()};
    }
    auto ih = aig_map.at(inode.id());
    fanin_list[i] = ih;
  }

  AigHandle h;
  if ( node.is_primitive() ) {
    h = mgr.primitive_op(node.primitive_type(), fanin_list);
  }
  else if ( node.is_func() ) {
    auto func = node.local_func();
    if ( func.is_cover() ) {
      auto& cover = func.input_cover();
      h = mgr.from_cover(cover, fanin_list);
    }
    else if ( func.is_expr() ) {
      h = mgr.from_expr(func.expr(), fanin_list);
    }
    else {
      cout << "FuncType: " << func.type() << endl;
      goto not_implemented;
    }
  }
  else {
    cout << "NodeType: " << node.type() << endl;
    goto not_implemented;
  }
  aig_map.emplace(node.id(), h);
  return h;

not_implemented:
  throw std::invalid_argument{"not supported"};
  return AigHandle::zero();
}

int
rwt_test(
  int argc,
  char** argv
)
{
  if ( argc != 2 ) {
    cerr << "USAGE: rwt_test <blif-file>" << endl;
    return -1;
  }
  std::string filename = argv[1];
  try {
    auto model = BnModel::read_blif(filename);
    AigMgr mgr;
    auto no = model.output_num();
    std::vector<AigHandle> output_list(no);
    {
      // BnNode のノード番号をキーにして AigHandle を持つ辞書
      std::unordered_map<SizeType, AigHandle> aig_map;
      // 入力ノードを作る．
      auto ni = model.input_num();
      for ( SizeType i = 0; i < ni; ++ i ) {
	auto node = model.input(i);
	auto h = mgr.make_input();
	aig_map.emplace(node.id(), h);
      }
      // 論理ノードを作る．
      auto nl = model.logic_num();
      for ( SizeType i = 0; i < nl; ++ i ) {
	auto node = model.logic(i);
	auto h = bn2aig(node, mgr, aig_map);
	aig_map.emplace(node.id(), h);
      }
      // 出力ノードを作る．
      for ( SizeType i = 0; i < no; ++ i ) {
	auto node = model.output(i);
	if ( aig_map.count(node.id()) == 0 ) {
	  std::ostringstream buf;
	  buf << "Node#" << node.id() << " is not defined";
	  throw std::invalid_argument{buf.str()};
	}
	auto h = aig_map.at(node.id());
	output_list[i] = h;
      }
    }
    mgr.print(cout);
    cout << "Before rewrite: " << mgr.and_num() << endl;
    mgr.rewrite();
    cout << "After rewrite: " << mgr.and_num() << endl;
  }
  catch ( std::invalid_argument err ) {
    cerr << err.what() << endl;
    return -1;
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
  return YM_NAMESPACE::rwt_test(argc, argv);
}
