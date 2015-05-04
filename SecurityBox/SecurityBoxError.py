# SecurityBoxError


class NotLoginError(Exception):
    def __init__(self, message):
        super(NotLoginError, self).__init__(message)


class FilenameConflictError(Exception):
    def __init__(self, message):
        super(FilenameConflictError, self).__init__(message)


class FileNotExistError(Exception):
    def __init__(self, message):
        super(FileNotExistError, self).__init__(message)


class FileHmacNotVerifyError(Exception):
    def __init__(self, message):
        super(FileHmacNotVerifyError, self).__init__(message)


class TimestampVerifyError(Exception):
    def __init__(self, message):
        super(TimestampVerifyError, self).__init__(message)
