#! /usr/bin/env python3

""" ファイルを生成するスクリプト

:file: mk_all.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

import os
import sys
from argparse import ArgumentParser
from pathlib import Path


# このファイルのあるディレクトリ
curdir = os.path.dirname(__file__)
rootdir = os.path.join(curdir, '..', '..', '..')
module_path = os.path.join(rootdir, 'ym-common', 'utils')

sys.path.append(module_path)

parser = ArgumentParser(prog='mk_ymlogic',
                        description="make 'ymlogic' module")

parser.add_argument('-i', '--include_dir', type=str)
parser.add_argument('-s', '--source_dir', type=str)

args = parser.parse_args()

if args.include_dir is None:
    include_dir = os.path.join(curdir, '..', '..', 'include', 'pym')
else:
    include_dir = args.include_dir
if args.source_dir is None:
    source_dir = os.path.join(curdir, '..')
else:
    source_dir = args.source_dir

from primtype_gen import PrimTypeGen
primtype_gen = PrimTypeGen()

from literal_gen import LiteralGen
literal_gen = LiteralGen()

from npnmap_gen import NpnMapGen
npnmap_gen = NpnMapGen()

from expr_gen import ExprGen
expr_gen = ExprGen()

from tvfunc_gen import TvFuncGen
tvfunc_gen = TvFuncGen()

from bdd_gen import BddGen
bdd_gen = BddGen()

from bddmgr_gen import BddMgrGen
bddmgr_gen = BddMgrGen()

from bddvarset_gen import BddVarSetGen
bddvarset_gen = BddVarSetGen()

from aighandle_gen import AigHandleGen
aighandle_gen = AigHandleGen()

from aigmgr_gen import AigMgrGen
aigmgr_gen = AigMgrGen()

from sopcube_gen import SopCubeGen
sopcube_gen = SopCubeGen()

from sopcover_gen import SopCoverGen
sopcover_gen = SopCoverGen()

gen_list = [primtype_gen, literal_gen, npnmap_gen,
            tvfunc_gen, expr_gen,
            bdd_gen, bddmgr_gen,
            sopcover_gen, sopcube_gen,
            aighandle_gen, aigmgr_gen]

from mk_py_capi import ModuleGen
module_gen = ModuleGen(modulename='ymlogic',
                       pyclass_gen_list=gen_list,
                       namespace='YM')

module_gen.make_all(include_dir=include_dir,
                    source_dir=source_dir)
