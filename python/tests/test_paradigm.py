import pyariadne_paradigm as paradigm


def test_boolean_bindings():
    assert bool(paradigm.Boolean(True))
    assert not bool(paradigm.Boolean(False))
    assert isinstance(paradigm.true, paradigm.Boolean)
    assert isinstance(paradigm.false, paradigm.Boolean)


def test_effort_binding():
    effort = paradigm.Effort(1)
    effort.work()


def test_string_binding():
    value = paradigm.String("1.375")
    assert str(value) == "1.375"
