import pyariadne as foundation


def test_boolean_bindings():
    assert bool(foundation.Boolean(True))
    assert not bool(foundation.Boolean(False))
    assert isinstance(foundation.true, foundation.Boolean)
    assert isinstance(foundation.false, foundation.Boolean)


def test_effort_binding():
    effort = foundation.Effort(1)
    effort.work()


def test_string_binding():
    value = foundation.String("1.375")
    assert isinstance(value, foundation.String)
