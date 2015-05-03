def singleton(class_):
    instances = {}

    def get_instance(*args, **kwargs):
        if class_ not in instances:
            instances[class_] = class_(*args, **kwargs)
        return instances[class_]
    return get_instance


class Record(object):
    def __init__(self, filename, aes_key_str, aes_iv_str, hmac_key_str):
        self.filename = filename
        self.aes_key_str = aes_key_str
        self.aes_iv_str = aes_iv_str
        self.hmac_key_str = hmac_key_str

    def to_str(self):
        record_str = "{0}:{1}:{2}:{3}"
        return record_str.format(
            self.filename,
            self.aes_key_str, self.aes_iv_str,
            self.hmac_key_str)


@singleton
class KeyManager(object):
    def __init__(self, db_filename):
        self.__db_filename = db_filename
        self.__db_map = self.__load_db(db_filename)

    def __load_db(self, db_filename):
        db_map = {}

        # Load db file into memory.
        lines = []
        with open(db_filename, 'r') as f:
            lines = f.readlines()

        if len(lines) == 0:
            return {}

        for line in lines:
            line = line.strip()
            if len(line) == 0:
                continue

            items_arr = line.split(':')
            record = Record(items_arr[0], items_arr[1], items_arr[2], items_arr[3])
            db_map[record.filename] = record

        return db_map

    def get_aes_key_str(self, filename):
        # Check input.
        if filename is None or len(filename) == 0:
            raise ValueError("Class::KeyManager, Member::get_aes_key_str, \
                              error::Input filename is empty")

        if filename in self.__db_map:
            record = self.__db_map[filename]
            return record.aes_key_str

        return None

    def get_aes_iv_str(self, filename):
        # Check input.
        if filename is None or len(filename) == 0:
            raise ValueError("Class::KeyManager, Member::get_aes_iv_str, \
                              error::Input filename is empty.")

        if filename in self.__db_map:
            record = self.__db_map[filename]
            return record.aes_iv_str

        return None

    def get_hmac_key_str(self, filename):
        # Check input.
        if filename is None or len(filename) == 0:
            raise ValueError("Class::KeyManager, Member::get_hmac_key_str, \
                              error::Input filename is empty.")

        if filename in self.__db_map:
            record = self.__db_map[filename]
            return record.hmac_key_str

        return None

    def add_file_record(self, filename, aes_key_str, aes_iv_str, hmac_key_str):
        # Check input.
        if filename is None or len(filename) == 0 or\
           aes_key_str is None or len(aes_key_str) == 0 or\
           aes_iv_str is None or len(aes_iv_str) == 0:
            raise ValueError("Class::KeyManager, Method::add_file_record, error::Inputs are empty.")

        # Check whether filename conflict.
        if filename in self.__db_map:
            # Filename conflict.
            return False
        else:
            record = Record(filename, aes_key_str, aes_iv_str, hmac_key_str)

            # Add record into db_map.
            self.__db_map[filename] = record

            # Add record into db file.
            with open(self.__db_filename, 'a') as f:
                f.write(record.to_str() + "\n")

            return True

    def remove_file_record(self, filename):
        # Check input.
        if filename is None or len(filename) == 0:
            raise ValueError("Class::KeyManager, Method::remove_file_record, \
                              error::Input filename is empty.")

        # Check whether the given is in the db_map.
        if filename in self.__db_map:
            # Remove this file record from db_map.
            del self.__db_map[filename]

            # Remove this file record from db_file.
            with open(self.__db_filename, 'w') as f:
                for filename in self.__db_map:
                    record = self.__db_map[filename]
                    f.write(record.to_str() + "\n")

            return True
        else:
            return False
