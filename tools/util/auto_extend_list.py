class auto_extend_list(list):
    """List which automatically extend itself with a specified default values, if there is a write above bounds"""
    def __init__(self, default):
        self._default = default

    def __setitem__(self, key, value):
        if key >= len(self):
            self += [self._default] * (key - len(self) + 1)
        super(auto_extend_list, self).__setitem__(key, value)
