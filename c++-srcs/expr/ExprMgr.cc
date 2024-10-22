
/// @file ExprMgr.cc
/// @brief ExprMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ExprMgr.h"
#include "ExprNode.h"
#include "RepStringParser.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
// クラス ExprMgr
//////////////////////////////////////////////////////////////////////

// 唯一のオブジェクト
static ExprMgr* mTheObj = nullptr;

// @brief 唯一のインスタンスを返す．
ExprMgr&
ExprMgr::the_obj()
{
  if ( mTheObj == nullptr ) {
    mTheObj = new ExprMgr;
  }
  return *mTheObj;
}

// @brief コンストラクタ
ExprMgr::ExprMgr(
) : mNodeNum{0},
    mMaxNodeNum{0},
    mStuckNodeNum{0}
{
}

// デストラクタ
ExprMgr::~ExprMgr()
{
}

// @brief 確保したメモリを開放する．
void
ExprMgr::clear_memory()
{
  delete mTheObj;
  mTheObj = nullptr;
}

// 恒偽関数を作る．
ExprNodePtr
ExprMgr::zero()
{
  if ( !mConst0 ) {
    mConst0 = alloc_node(ExprNode::Const0);
    ++ mStuckNodeNum;
  }
  return mConst0;
}

// 恒真関数を作る．
ExprNodePtr
ExprMgr::one()
{
  if ( !mConst1 ) {
    mConst1 = alloc_node(ExprNode::Const1);
    ++ mStuckNodeNum;
  }
  return mConst1;
}

// 肯定のリテラルを作る．
ExprNodePtr
ExprMgr::posi_literal(
  SizeType id
)
{
  make_literals(id);
  SizeType idx = id << 1;
  ASSERT_COND( idx < mLiteralArray.size() );
  return mLiteralArray[idx];
}

// 否定のリテラルを作る．
ExprNodePtr
ExprMgr::nega_literal(
  SizeType id
)
{
  make_literals(id);
  SizeType idx = id << 1 | 1;
  ASSERT_COND( idx < mLiteralArray.size() );
  return mLiteralArray[idx];
}

// chd_list の要素を子供とする AND ノードを作る．
// 子供も AND ノードの場合にはマージする．
// 子供が定数ノードの場合には値に応じた簡単化を行う．
ExprNodePtr
ExprMgr::and_op(
  SizeType begin
)
{
  ASSERT_COND( begin >= 0 && begin < nodestack_top() );

  bool const0 = false;
  SizeType end = nodestack_top();
  mTmpNodeList.clear();
  mTmpNodeList.reserve(end - begin);
  for ( SizeType i = begin; i < end; ++ i ) {
    auto node{mNodeStack[i]};
    auto type{node->type()};
    if ( type == ExprNode::Const0 ) {
      const0 = true;
      break;
    }
    if ( type == ExprNode::And ) {
      SizeType ni{node->child_num()};
      for ( SizeType j = 0; j < ni; ++ j ) {
	ExprNodePtr node1{node->child(j)};
	if ( check_node(node1) ) {
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

  SizeType n{mTmpNodeList.size()};
  ExprNodePtr node;
  if ( const0 ) {
    node = zero();
  }
  else if ( n == 0 ) {
    node = one();
  }
  else if ( n == 1 ) {
    node = mTmpNodeList.front();
  }
  else {
    node = alloc_node(ExprNode::And);
  }
  return node;
}

// chd_list の要素を子供に持つ OR を表す式を作る．
// 子供も OR ノードの場合にはマージする．
// 子供が定数ノードの場合には値に応じた簡単化を行う．
ExprNodePtr
ExprMgr::or_op(
  SizeType begin
)
{
  ASSERT_COND( begin >= 0 && begin < nodestack_top() );

  bool const1 = false;
  SizeType end{nodestack_top()};
  mTmpNodeList.clear();
  mTmpNodeList.reserve(end - begin);
  for ( SizeType i = begin; i < end; ++ i ) {
    auto node{mNodeStack[i]};
    auto type{node->type()};
    if ( type == ExprNode::Const1 ) {
      const1 = true;
      break;
    }
    if ( type == ExprNode::Or ) {
      SizeType ni{node->child_num()};
      for ( SizeType j = 0; j < ni; ++ j ) {
	ExprNodePtr node1{node->child(j)};
	if ( check_node(node1) ) {
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

  ExprNodePtr node;
  SizeType n{mTmpNodeList.size()};
  if ( const1 ) {
    node = one();
  }
  else if ( n == 0 ) {
    node = zero();
  }
  else if ( n == 1 ) {
    node = mTmpNodeList.front();
  }
  else {
    node = alloc_node(ExprNode::Or);
  }
  return node;
}

// chd_list の要素を子供に持つ XOR を表す式を作る．
// 子供も XOR ノードの場合にはマージする．
// 子供が定数ノードの場合には値に応じた簡単化を行う．
ExprNodePtr
ExprMgr::xor_op(
  SizeType begin
)
{
  ASSERT_COND( begin >= 0 && begin < nodestack_top() );

  bool inv = false;
  SizeType end{nodestack_top()};
  mTmpNodeList.clear();
  mTmpNodeList.reserve(end - begin);
  for ( SizeType i = begin; i < end; ++ i ) {
    auto node{mNodeStack[i]};
    auto type{node->type()};
    if ( type == ExprNode::Const1 ) {
      inv = !inv;
    }
    else if ( type == ExprNode::Xor ) {
      SizeType ni{node->child_num()};
      for ( SizeType j = 0; j < ni; ++ j ) {
	ExprNodePtr node1{node->child(j)};
	if ( check_node2(node1) ) {
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

  ExprNodePtr node;
  SizeType n{mTmpNodeList.size()};
  if ( n == 0 ) {
    node = zero();
  }
  else {
    if ( n == 1 ) {
      node = mTmpNodeList.front();
    }
    else {
      node = alloc_node(ExprNode::Xor);
    }
  }
  if ( inv ) {
    return complement(node);
  }
  else {
    return node;
  }
}

// @brief rep_string 形式の文字列を読み込む．
ExprNodePtr
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
      return zero();
    }
    if ( c == '1' ) {
      return one();
    }
    // エラー
    break;
  case 'P':
    val = parser.read_int();
    return posi_literal(val);
  case 'N':
    val = parser.read_int();
    return nega_literal(val);
  case 'A':
    {
      SizeType n = parser.read_int();
      SizeType begin = nodestack_top();
      for ( SizeType i = 0; i < n; ++ i ) {
	auto child_expr = from_rep_string(parser);
	nodestack_push(child_expr);
      }
      return and_op(begin);
    }
    break;
  case 'O':
    {
      SizeType n = parser.read_int();
      SizeType begin = nodestack_top();
      for ( SizeType i = 0; i < n; ++ i ) {
	auto child_expr = from_rep_string(parser);
	nodestack_push(child_expr);
      }
      return or_op(begin);
    }
    break;
  case 'X':
    {
      SizeType n = parser.read_int();
      SizeType begin = nodestack_top();
      for ( SizeType i = 0; i < n; ++ i ) {
	auto child_expr = from_rep_string(parser);
	nodestack_push(child_expr);
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
  const ExprNode* node
)
{
  for ( auto node1: mTmpNodeList ) {
    if ( posi_equiv(node, node1) ) {
      // 同相で重なっていた場合
      return false;
    }
    if ( nega_equiv(node, node1) ) {
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
  const ExprNode* node
)
{
  for ( auto p = mTmpNodeList.begin(); p != mTmpNodeList.end(); ) {
    auto node1{*p};
    // ループ中で削除する場合があるので反復子をコピーしてから進めておく
    auto q = p;
    ++ p;

    if ( posi_equiv(node, node1) ) {
      // 同相で重なっていたら両方取り除く
      mTmpNodeList.erase(q);
      return false;
    }
    if ( nega_equiv(node, node1) ) {
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
  const ExprNode* node
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
ExprNodePtr
ExprMgr::complement(
  const ExprNode* node
)
{
  switch ( node->type() ) {
  case ExprNode::Const0:      return one();
  case ExprNode::Const1:      return zero();
  case ExprNode::PosiLiteral: return nega_literal(node->varid());
  case ExprNode::NegaLiteral: return posi_literal(node->varid());
  default: break;
  }

  SizeType n{node->child_num()};
  SizeType begin{nodestack_top()};
  for ( SizeType i = 0; i < n; ++ i ) {
    // child の型を ExprNode* にすると途中で削除されてしまうおそれがある．
    ExprNodePtr child{node->child(i)};
    if ( node->type() != ExprNode::Xor || i == 0 ) {
      child = complement(child);
    }
    nodestack_push(child);
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
ExprNodePtr
ExprMgr::compose(
  const ExprNode* node,
  SizeType id,
  const ExprNodePtr& sub
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

  SizeType n{node->child_num()};
  SizeType begin{nodestack_top()};
  bool ident = true;
  for ( SizeType i = 0; i < n; ++ i ) {
    ExprNodePtr chd{compose(node->child(i), id, sub)};
    if ( chd != node->child(i) ) {
      ident = false;
    }
    nodestack_push(chd);
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
ExprNodePtr
ExprMgr::compose(
  const ExprNode* node,
  const unordered_map<SizeType, Expr>& comp_map
)
{
  switch ( node->type() ) {
  case ExprNode::Const0:
  case ExprNode::Const1:
    return node;

  case ExprNode::PosiLiteral:
    if ( comp_map.count(node->varid()) > 0 ) {
      auto ans{comp_map.at(node->varid())};
      return ans.root();
    }
    return node;

  case ExprNode::NegaLiteral:
    if ( comp_map.count(node->varid()) > 0 ) {
      auto ans{comp_map.at(node->varid())};
      return complement(ans.root());
    }
    return node;

  default:
    break;
  }

  SizeType n{node->child_num()};
  SizeType begin{nodestack_top()};
  bool ident = true;
  for ( SizeType i = 0; i < n; ++ i ) {
    ExprNodePtr chd{compose(node->child(i), comp_map)};
    if ( chd != node->child(i) ) {
      ident = false;
    }
    nodestack_push(chd);
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
ExprNodePtr
ExprMgr::remap_var(
  const ExprNode* node,
  const unordered_map<SizeType, SizeType>& varmap
)
{
  switch ( node->type() ) {
  case ExprNode::Const0:
  case ExprNode::Const1:
    return node;

  case ExprNode::PosiLiteral:
    if ( varmap.count(node->varid()) > 0 ) {
      auto ans = varmap.at(node->varid());
      return posi_literal(ans);
    }
    return node;

  case ExprNode::NegaLiteral:
    if ( varmap.count(node->varid()) > 0 ) {
      auto ans = varmap.at(node->varid());
      return nega_literal(ans);
    }
    return node;

  default: break;
  }

  SizeType n{node->child_num()};
  SizeType begin{nodestack_top()};
  bool ident = true;
  for ( SizeType i = 0; i < n; ++ i ) {
    ExprNodePtr chd{remap_var(node->child(i), varmap)};
    if ( chd != node->child(i) ) {
      ident = false;
    }
    nodestack_push(chd);
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
ExprNodePtr
ExprMgr::simplify(
  const ExprNode* node
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

  SizeType n{node->child_num()};
  SizeType begin{nodestack_top()};
  bool ident = true;
  for ( SizeType i = 0; i < n; ++ i ) {
    ExprNodePtr chd{simplify(node->child(i))};
    if ( chd != node->child(i) ) {
      ident = false;
    }
    nodestack_push(chd);
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

// @brief 使用されているノード数を返す．
SizeType
ExprMgr::node_num()
{
  return mNodeNum;
}

// id 番めまでのリテラルノードを作る．
void
ExprMgr::make_literals(
  SizeType id
)
{
  SizeType last = mLiteralArray.size() / 2;
  while ( last <= id ) {
    auto posi{alloc_node(ExprNode::PosiLiteral)};
    posi->mNc = last;
    mLiteralArray.push_back(posi);
    ++ mStuckNodeNum;

    auto nega{alloc_node(ExprNode::NegaLiteral)};
    nega->mNc = last;
    mLiteralArray.push_back(nega);
    ++ mStuckNodeNum;
    ++ last;
  }
}

// ExprNode を確保して内容を設定する．
ExprNode*
ExprMgr::alloc_node(
  ExprNode::Type type
)
{
  ++ mNodeNum;
  if ( mMaxNodeNum < mNodeNum ) {
    mMaxNodeNum = mNodeNum;
  }

  SizeType nc = 0;
  if ( type == ExprNode::And || type == ExprNode::Or || type == ExprNode::Xor ) {
    nc = mTmpNodeList.size();
  }

  SizeType req_size{calc_size(nc)};
  void* p{new char[req_size]};
  auto node{new (p) ExprNode};
  node->mRefType = static_cast<int>(type);
  node->mNc = nc;
  for ( SizeType i = 0; i < nc; ++ i ) {
    node->mChildArray[i] = mTmpNodeList[i];
    node->child(i)->inc_ref();
  }
  return node;
}

// ExprNode を削除する．
void
ExprMgr::free_node(
  ExprNode* node
)
{
  SizeType n{node->child_num()};
  for ( SizeType i = 0; i < n; ++ i ) {
    node->child(i)->dec_ref();
  }

  -- mNodeNum;

  char* p = reinterpret_cast<char*>(node);
  delete [] p;
}

// ExprNode の入力数から必要なサイズを計算する．
SizeType
ExprMgr::calc_size(
  SizeType nc
)
{
  return sizeof(ExprNode) + sizeof(ExprNode*) * (nc - 1);
}


//////////////////////////////////////////////////////////////////////
// RepStringParser
//////////////////////////////////////////////////////////////////////

// @brief 一文字読み出す．
char
RepStringParser::read_char()
{
  ASSERT_COND( mPos < mString.size() );
  auto c = mString[mPos];
  ++ mPos;
  return c;
}

// @brief 整数を読み出す．
int
RepStringParser::read_int()
{
  int ans = 0;
  while ( mPos < mString.size() ) {
    char c = read_char();
    if ( !isdigit(c) ) {
      -- mPos;
      break;
    }
    ans = ans * 10 + static_cast<int>(c) - static_cast<int>('0');
  }
  return ans;
}

END_NAMESPACE_YM_LOGIC
