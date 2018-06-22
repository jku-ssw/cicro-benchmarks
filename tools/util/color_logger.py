import logging


# source: https://stackoverflow.com/a/384125/2967999
class ColoredFormatter(logging.Formatter):
    RESET_SEQ = "\033[0m"
    COLOR_SEQ = "\033[1;%dm"

    COLORS = {
        'WARNING': 33,
        'INFO': 37,
        'DEBUG': 34,
        'CRITICAL': 41,
        'ERROR': 31,
        'EXCEPTION': 31
    }

    def __init__(self, msg):
        logging.Formatter.__init__(self, msg)

    def format(self, record):
        lvl_name = record.levelname
        if lvl_name in ColoredFormatter.COLORS:
            record.levelname = self.COLOR_SEQ % (self.COLORS[lvl_name]) + record.levelname + self.RESET_SEQ
            record.msg = self.COLOR_SEQ % (self.COLORS[lvl_name]) + str(record.msg) + self.RESET_SEQ
        return logging.Formatter.format(self, record)

    def formatException(self, ei):
        super_ei = logging.Formatter.formatException(self, ei)
        return self.COLOR_SEQ % (self.COLORS['EXCEPTION']) + super_ei + self.RESET_SEQ


class ColoredLogger(logging.Logger):
    COLOR_FORMAT = "[%(asctime)s] [ %(levelname)-19s ]  %(message)s (%(filename)s:%(lineno)d)"

    def __init__(self, name):
        logging.Logger.__init__(self, name, logging.DEBUG)

        console = logging.StreamHandler()
        console.setFormatter(ColoredFormatter(self.COLOR_FORMAT))

        self.addHandler(console)


logging.setLoggerClass(ColoredLogger)


def get_logger(name):
    return ColoredLogger(name)
    # return logging.getLogger(name)  # Using this code can result in duplicated logs?
