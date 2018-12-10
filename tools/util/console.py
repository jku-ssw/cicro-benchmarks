import sys


# source: https://code.activestate.com/recipes/577058/
def query_yes_no(question, default="yes", on_keyboard_int=None):
    valid = {"yes": "yes", "y": "yes", "ye": "yes",
             "no": "no", "n": "no"}
    if default is None:
        prompt = " [y/n] "
    elif default == "yes":
        prompt = " [Y/n] "
    elif default == "no":
        prompt = " [y/N] "
    else:
        raise ValueError("invalid default answer: '%s'" % default)

    sys.stdout.flush()
    sys.stderr.flush()

    while 1:
        sys.stdout.write(question + prompt)

        try:
            choice = input().lower()
        except KeyboardInterrupt:
            if on_keyboard_int is not None:
                return on_keyboard_int  # default behaviour for ^C
            else:
                raise

        if default is not None and choice == '':
            return default
        elif choice in valid.keys():
            return valid[choice]
        else:
            sys.stdout.write("Please respond with 'yes' or 'no' (or 'y' or 'n').\n")
