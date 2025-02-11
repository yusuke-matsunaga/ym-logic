
/// @file ExprMgr.cc
/// @brief ExprMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ExprMgr.h"
#include "ExprNode.h"
#include "RepStringParser.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
// クラス ExprMgr
//////////////////////////////////////////////////////////////////////

// chd_list の要素を子供とする AND ノードを作る．
// 子供も AND ノードの場合にはマージする．
// 子供が定数ノードの場合には値に応じた簡単化を行う．
Expr::NodePtr
ExprMgr::and_op(
  SizeType begin
)
{
  ASSERT_COND( begin >= 0 && begin < nodestack_top() );

  bool const0 = false;
  auto end = nodestack_top();
  mTmpNodeList.clear();
  mTmpNodeList.reserve(end - begin);
  for ( SizeType i = begin; i < end; ++ i ) {
    auto node = mNodeStack[i];
    auto type = node->type();
    if ( type == ExprNode::Const0 ) {
      const0 = true;
      break;
    }
    if ( type == ExprNode::And ) {
      for ( auto& opr: node->operand_list() ) {
	if ( check_node(opr) ) {
	  // 逆相の入力があったら答は0
	  const0 = true;
	  break;
	}
      }
      if ( const0 ) {
	break;
      }
    }
    else if ( type == ExprNode::Const1 ) {
      ; // 無視
    }
    else {
      if ( check_node(node) ) {
	// 逆相の入力があったら答は0
	const0 = true;
	break;
      }
    }
  }
  nodestack_pop(begin);

  auto n = mTmpNodeList.size();
  Expr::NodePtr node;
  if ( const0 ) {
    node = ExprNode::new_zero();
  }
  else if ( n == 0 ) {
    node = ExprNode::new_one();
  }
  else if ( n == 1 ) {
    node = mTmpNodeList.front();
  }
  else {
    node = ExprNode::new_and(mTmpNodeList);
  }
  return node;
}

// chd_list の要素を子供に持つ OR を表す式を作る．
// 子供も OR ノードの場合にはマージする．
// 子供が定数ノードの場合には値に応じた簡単化を行う．
Expr::NodePtr
ExprMgr::or_op(
  SizeType begin
)
{
  ASSERT_COND( begin >= 0 && begin < nodestack_top() );

  bool const1 = false;
  auto end = nodestack_top();
  mTmpNodeList.clear();
  mTmpNodeList.reserve(end - begin);
  for ( SizeType i = begin; i < end; ++ i ) {
    auto node = mNodeStack[i];
    auto type = node->type();
    if ( type == ExprNode::Const1 ) {
      const1 = true;
      break;
    }
    if ( type == ExprNode::Or ) {
      for ( auto& opr: node->operand_list() ) {
	if ( check_node(opr) ) {
	  // 逆相の入力があったら答は1
	  const1 = true;
	  break;
	}
      }
      if ( const1 ) {
	break;
      }
    }
    else if ( type == ExprNode::Const0 ) {
      ; // 無視
    }
    else {
      if ( check_node(node) ) {
	// 逆相の入力があったら答は1
	const1 = true;
	break;
      }
    }
  }
  nodestack_pop(begin);

  Expr::NodePtr node;
  auto n = mTmpNodeList.size();
  if ( const1 ) {
    node = ExprNode::new_one();
  }
  else if ( n == 0 ) {
    node = ExprNode::new_zero();
  }
  else if ( n == 1 ) {
    node = mTmpNodeList.front();
  }
  else {
    node = ExprNode::new_or(mTmpNodeList);
  }
  return node;
}

// chd_list の要素を子供に持つ XOR を表す式を作る．
// 子供も XOR ノードの場合にはマージする．
// 子供が定数ノードの場合には値に応じた簡単化を行う．
Expr::NodePtr
ExprMgr::xor_op(
  SizeType begin
)
{
  ASSERT_COND( begin >= 0 && begin < nodestack_top() );

  bool inv = false;
  auto end = nodestack_top();
  mTmpNodeList.clear();
  mTmpNodeList.reserve(end - begin);
  for ( SizeType i = begin; i < end; ++ i ) {
    auto node = mNodeStack[i];
    auto type = node->type();
    if ( type == ExprNode::Const1 ) {
      inv = !inv;
    }
    else if ( type == ExprNode::Xor ) {
      for ( auto& opr: node->operand_list() ) {
	if ( check_node2(opr) ) {
	  inv = !inv;
	}
      }
    }
    else if ( type == ExprNode::Const0 ) {
      ; // 無視
    } else {
      if ( check_node2(node) ) {
	inv = !inv;
      }
    }
  }
  nodestack_pop(begin);

  Expr::NodePtr node;
  auto n = mTmpNodeList.size();
  if ( n == 0 ) {
    node = ExprNode::new_zero();
  }
  else if ( n == 1 ) {
    node = mTmpNodeList.front();
  }
  else {
    node = ExprNode::new_xor(mTmpNodeList);
  }
  if ( inv ) {
    return complement(node);
  }
  else {
    return node;
  }
}

// @brief rep_string 形式の文字列を読み込む．
Expr::NodePtr
ExprMgr::from_rep_string(
  RepStringParser& parser
)
{
  char c = parser.read_char();
  int val;
  switch ( c ) {
  case 'C':
    c = parser.read_char();
    if ( c == '0' ) {
      return ExprNode::new_zero();
    }
    if ( c == '1' ) {
      return ExprNode::new_one();
    }
    // エラー
    break;
  case 'P':
    val = parser.read_int();
    return ExprNode::new_literal(val, false);
  case 'N':
    val = parser.read_int();
    return ExprNode::new_literal(val, true);
  case 'A':
    {
      SizeType n = parser.read_int();
      SizeType begin = nodestack_top();
      for ( SizeType i = 0; i < n; ++ i ) {
	auto opr = from_rep_string(parser);
	nodestack_push(opr);
      }
      return and_op(begin);
    }
    break;
  case 'O':
    {
      SizeType n = parser.read_int();
      SizeType begin = nodestack_top();
      for ( SizeType i = 0; i < n; ++ i ) {
	auto opr = from_rep_string(parser);
	nodestack_push(opr);
      }
      return or_op(begin);
    }
    break;
  case 'X':
    {
      SizeType n = parser.read_int();
      SizeType begin = nodestack_top();
      for ( SizeType i = 0; i < n; ++ i ) {
	auto opr = from_rep_string(parser);
	nodestack_push(opr);
      }
      return xor_op(begin);
    }
    break;
  default:
    break;
  }
  throw std::invalid_argument{"syntax error"};
}

// node が node_list の中に含まれていないか調べる．
// 同相で同じものがふくまれていたらなにもせず false を返す．
// 逆相で同じものがふくまれていたらなにもせず true を返す．
// 同じものがふくまれていなかったら node を node_list に追加して
// false を返す．
bool
ExprMgr::check_node(
  const Expr::NodePtr& node
)
{
  for ( auto& node1: mTmpNodeList ) {
    if ( ExprNode::posi_equiv(node, node1) ) {
      // 同相で重なっていた場合
      return false;
    }
    if ( ExprNode::nega_equiv(node, node1) ) {
      // 逆相で重なっていた場合
      return true;
    }
  }
  mTmpNodeList.push_back(node);
  return false;
}

// node が node_list の中に含まれていないか調べる(XOR用)．
// 同相で同じものが含まれていたら重複したものを node_list から削除して
// false を返す．
// 逆相で同じものが含まれていたら重複したものを node_list から削除して
// true を返す．
// 同じものが含まれていなかったら node を node_list に追加して
// false を返す．
bool
ExprMgr::check_node2(
  const Expr::NodePtr& node
)
{
  for ( auto p = mTmpNodeList.begin(); p != mTmpNodeList.end(); ) {
    auto node1 = *p;
    // ループ中で削除する場合があるので反復子をコピーしてから進めておく
    auto q = p;
    ++ p;

    if ( ExprNode::posi_equiv(node, node1) ) {
      // 同相で重なっていたら両方取り除く
      mTmpNodeList.erase(q);
      return false;
    }
    if ( ExprNode::nega_equiv(node, node1) ) {
      // 逆相で重なっていたら両方取り除く
      mTmpNodeList.erase(q);
      return true;
    }
  }
  mTmpNodeList.push_back(node);
  return false;
}

// @brief ノードスタックにノードを入れる．
void
ExprMgr::nodestack_push(
  const Expr::NodePtr& node
)
{
  mNodeStack.push_back(node);
}

// @brief ノードスタックの先頭位置を返す．
SizeType
ExprMgr::nodestack_top()
{
  return mNodeStack.size();
}

// @brief ノードスタックの先頭位置を戻す．
void
ExprMgr::nodestack_pop(
  SizeType oldtop
)
{
  ASSERT_COND( oldtop >= 0 && oldtop < mNodeStack.size() );

  mNodeStack.erase(mNodeStack.begin() + oldtop, mNodeStack.end());
}

// 否定形を返す．
Expr::NodePtr
ExprMgr::complement(
  const Expr::NodePtr& node
)
{
  switch ( node->type() ) {
  case ExprNode::Const0:      return ExprNode::new_one();
  case ExprNode::Const1:      return ExprNode::new_zero();
  case ExprNode::PosiLiteral: return ExprNode::new_literal(node->varid(), true);
  case ExprNode::NegaLiteral: return ExprNode::new_literal(node->varid(), false);
  default: break;
  }

  auto begin = nodestack_top();
  bool first = true;
  for ( auto& opr: node->operand_list() ) {
    Expr::NodePtr new_opr;
    if ( node->type() != ExprNode::Xor || first ) {
      new_opr = complement(opr);
    }
    else {
      new_opr = opr;
    }
    first = false;
    nodestack_push(new_opr);
  }

  switch ( node->type() ) {
  case ExprNode::And: return or_op(begin);
  case ExprNode::Or:  return and_op(begin);
  case ExprNode::Xor: return xor_op(begin);
  default: break;
  }

  // ここに来ることはない．
  ASSERT_NOT_REACHED;

  return nullptr;
}

// id 番目の変数を sub に置き換える．
Expr::NodePtr
ExprMgr::compose(
  const Expr::NodePtr& node,
  SizeType id,
  const Expr::NodePtr& sub
)
{
  switch ( node->type() ) {
  case ExprNode::Const0:
  case ExprNode::Const1:
    return node;

  case ExprNode::PosiLiteral:
    if ( node->varid() == id ) {
      return sub;
    }
    return node;

  case ExprNode::NegaLiteral:
    if ( node->varid() == id ) {
      return complement(sub);
    }
    return node;
  default:
    break;
  }

  auto begin = nodestack_top();
  bool ident = true;
  for ( auto& opr: node->operand_list() ) {
    auto new_opr = compose(opr, id, sub);
    if ( new_opr != opr ) {
      ident = false;
    }
    nodestack_push(new_opr);
  }
  if ( ident ) {
    nodestack_pop(begin);
    return node;
  }

  switch ( node->type() ) {
  case ExprNode::And: return and_op(begin);
  case ExprNode::Or:  return or_op(begin);
  case ExprNode::Xor: return xor_op(begin);
  default: break;
  }

  ASSERT_NOT_REACHED;
  return nullptr;
}

// comp_map にしたがってリテラルを式に置き換える．
Expr::NodePtr
ExprMgr::compose(
  const Expr::NodePtr& node,
  const Expr::ComposeMap& comp_map
)
{
  switch ( node->type() ) {
  case ExprNode::Const0:
  case ExprNode::Const1:
    return node;

  case ExprNode::PosiLiteral:
    if ( comp_map.count(node->varid()) > 0 ) {
      auto ans = comp_map.at(node->varid());
      return ans.root();
    }
    return node;

  case ExprNode::NegaLiteral:
    if ( comp_map.count(node->varid()) > 0 ) {
      auto ans = comp_map.at(node->varid());
      return complement(ans.root());
    }
    return node;

  default:
    break;
  }

  auto begin = nodestack_top();
  bool ident = true;
  for ( auto& opr: node->operand_list() ) {
    auto new_opr = compose(opr, comp_map);
    if ( new_opr != opr ) {
      ident = false;
    }
    nodestack_push(new_opr);
  }
  if ( ident ) {
    nodestack_pop(begin);
    return node;
  }

  switch ( node->type() ) {
  case ExprNode::And: return and_op(begin);
  case ExprNode::Or:  return or_op(begin);
  case ExprNode::Xor: return xor_op(begin);
  default:   break;
  }

  ASSERT_NOT_REACHED;
  return nullptr;
}

// 変数番号をマッピングし直す
Expr::NodePtr
ExprMgr::remap_var(
  const Expr::NodePtr& node,
  const Expr::VarMap& varmap
)
{
  switch ( node->type() ) {
  case ExprNode::Const0:
  case ExprNode::Const1:
    return node;

  case ExprNode::PosiLiteral:
    if ( varmap.count(node->varid()) > 0 ) {
      auto ans = varmap.at(node->varid());
      return ExprNode::new_literal(ans, false);
    }
    return node;

  case ExprNode::NegaLiteral:
    if ( varmap.count(node->varid()) > 0 ) {
      auto ans = varmap.at(node->varid());
      return ExprNode::new_literal(ans, true);
    }
    return node;

  default: break;
  }

  auto begin = nodestack_top();
  bool ident = true;
  for ( auto& opr: node->operand_list() ) {
    auto new_opr = remap_var(opr, varmap);
    if ( new_opr != opr ) {
      ident = false;
    }
    nodestack_push(new_opr);
  }
  if ( ident ) {
    nodestack_pop(begin);
    return node;
  }

  switch ( node->type() ) {
  case ExprNode::And: return and_op(begin);
  case ExprNode::Or:  return or_op(begin);
  case ExprNode::Xor: return xor_op(begin);
  default:   break;
  }

  ASSERT_NOT_REACHED;
  return nullptr;
}

// 簡単化を行う．
Expr::NodePtr
ExprMgr::simplify(
  const Expr::NodePtr& node
)
{
  switch ( node->type() ) {
  case ExprNode::Const0:
  case ExprNode::Const1:
  case ExprNode::PosiLiteral:
  case ExprNode::NegaLiteral:
    return node;

  default:
    break;
  }

  auto begin = nodestack_top();
  bool ident = true;
  for ( auto& opr: node->operand_list() ) {
    auto new_opr = simplify(opr);
    if ( new_opr != opr ) {
      ident = false;
    }
    nodestack_push(new_opr);
  }
  if ( ident ) {
    nodestack_pop(begin);
    return node;
  }

  switch ( node->type() ) {
  case ExprNode::And: return and_op(begin);
  case ExprNode::Or:  return or_op(begin);
  case ExprNode::Xor: return xor_op(begin);
  default:   break;
  }

  // ここに来ることはない．
  ASSERT_NOT_REACHED;

  return nullptr;
}

END_NAMESPACE_YM_LOGIC
