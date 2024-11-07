from ctypes import CDLL

import pytest

libcortical = CDLL("./c/cortical.so")

#assert libcortical.fact(6) == 720
#assert libcortical.fact(0) == 1
#assert libcortical.fact(-42) == 1

@pytest.fixture
def libcortical():
    yield CDLL("./c/cortical.so")


@pytest.mark.parametrize("n,e", [(6, 720), (0, 1), (-42, 1)])
def test_cortical(libcortical, n, e):
    assert libcortical.fact(n) == e


