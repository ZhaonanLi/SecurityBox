# SecurityBox Model.
import os
import time
import datetime
import webbrowser

import dropbox

import SecurityModel
from SecurityBoxError import NotLoginError, \
    FilenameConflictError, FileNotExistError, FileHmacNotVerifyError,\
    TimestampVerifyError


def timestamp_verify(server_time_str):
    main_part_start_index = server_time_str.find(',') + 2
    main_part_end_index = server_time_str.find('+') - 2
    server_time_str = server_time_str[main_part_start_index:main_part_end_index + 1]

    date_format = "%d %b %Y %H:%M:%S"
    server_timestamp = time.mktime(
        datetime.datetime.strptime(server_time_str, date_format).timetuple()
        )

    client_timestamp = time.time() + 10000

    timestamp_threshold = 6000
    if abs(client_timestamp - server_timestamp) <= timestamp_threshold:
        return True
    else:
        return False


def singleton(class_):
    instances = {}

    def get_instance(*args, **kwargs):
        if class_ not in instances:
            instances[class_] = class_(*args, **kwargs)
        return instances[class_]
    return get_instance


@singleton
class DropboxClient(object):

    def __init__(self):
        self.__app_key = "1lppj6v29hvy87o"
        self.__app_secret = "9ajyf01tokggadr"
        self.__access_type = "app_folder"
        self.__access_token = None
        self.__session = None
        self.__request_token = None
        self.__client = None
        self.__ui = None

    def init_ui(self, ui):
        self.__ui = ui

    def connect(self):
        self.__session = dropbox.session.DropboxSession(
            self.__app_key,
            self.__app_secret,
            self.__access_type
        )

        self.__request_token = self.__session.obtain_request_token()

        oauth_url = self.__session.build_authorize_url(self.__request_token)
        # oauth_msg = "Opening {}."
        # oauth_msg += "\nPlease make sure this application is allowed before continuing."
        # print oauth_msg.format(oauth_url)

        webbrowser.open_new_tab(oauth_url)

    def connect_confirm(self):
        self.__access_token = self.__session.obtain_access_token(self.__request_token)
        self.__client = dropbox.client.DropboxClient(self.__session)

    def list_files(self):
        # Check user login status.
        if self.__client is None:
            raise NotLoginError("You should login firstly.")

        filename_arr = []

        hmac_prefix = "hmac_"
        folder_metadata_map = self.__client.metadata('/')
        contents_arr = folder_metadata_map["contents"]
        for record in contents_arr:
            if record["is_dir"] is False:
                filename = record["path"].lstrip('/')
                if len(filename) > len(hmac_prefix) and\
                   filename[:len(hmac_prefix)] == hmac_prefix:
                    continue
                filename_arr.append(filename)

        return filename_arr

    def upload_file(self, filename):
        # Check user login status.
        if self.__client is None:
            raise NotLoginError("You should login firstly.")

        base_path, abs_filename = os.path.split(filename)
        # Check filename conflict.
        filename_arr = self.list_files()
        if abs_filename in filename_arr:
            raise FilenameConflictError("Dropbox has a file with the same name.")

        # Encrypt file.
        with open(filename, 'r') as f:
            message = f.read()

        self.__ui.show_progress_m("encrypt file by AES")
        encrypted_m = SecurityModel.aes_encrypt(message)
        self.__ui.show_progress_m("make HMAC for file")
        hmac = SecurityModel.hmac_make(encrypted_m)

        # Write encrypted_m to buffer file.
        encrypt_m_buffer_filename = "./buffer_file/encrypt_m_buffer.txt"
        with open(encrypt_m_buffer_filename, 'w') as f:
            f.write(encrypted_m)

        # Write hmac to buffer file.
        hmac_buffer_filename = "./buffer_file/hmac_buffer.txt"
        with open(hmac_buffer_filename, 'w') as f:
            f.write(hmac)

        # Upload encrypt message.
        self.__ui.show_progress_m("upload encrypted file content")
        with open(encrypt_m_buffer_filename, 'r') as f:
            upload_en_m_rep = self.__client.put_file(abs_filename, f)

        # Upload hmac.
        self.__ui.show_progress_m("upload file's HMAC")
        with open(hmac_buffer_filename, 'r') as f:
            upload_hmac_rep = self.__client.put_file("hmac_" + abs_filename + "_.hmac", f)

        if upload_en_m_rep is not None and upload_hmac_rep is not None and\
           upload_en_m_rep["bytes"] > 0 and upload_hmac_rep["bytes"] > 0:

            if timestamp_verify(upload_en_m_rep["modified"]) and \
               timestamp_verify(upload_hmac_rep["modified"]):
                return True
            else:
                raise TimestampVerifyError("Timestamp verify fail.")
        else:
            return False

    def download_file(self, filename, download_file_path):
        # Check user login status.
        if self.__client is None:
            raise NotLoginError("You should login firstly.")

        # Check whether file exists.
        self.__ui.show_progress_m("checking whether file exists")
        filename_arr = self.list_files()
        if filename not in filename_arr:
            raise FileNotExistError("File not exist.")

        # Download encrypted file.
        self.__ui.show_progress_m("download encrypted file content")
        en_file, en_metadata = self.__client.get_file_and_metadata(filename)
        en_file_content = en_file.read()

        # Download hmac file.
        self.__ui.show_progress_m("download file's HMAC")
        hmac_file, hmac_media = self.__client.get_file_and_metadata("hmac_" + filename + "_.hmac")
        hmac_file_content = hmac_file.read()

        # Verify hmac.
        self.__ui.show_progress_m("verying file's HMAC")
        is_valid = SecurityModel.hmac_verify(en_file_content, hmac_file_content)
        if not is_valid:
            raise FileHmacNotVerifyError("Download file hmac verify fail.")

        # Decrypt file.
        self.__ui.show_progress_m("decrypt file by AES")
        de_file_content = SecurityModel.aes_decrypt(en_file_content)

        self.__ui.show_progress_m("write file to local")
        download_filename = os.path.join(download_file_path, filename)
        with open(download_filename, "w") as f:
            f.write(de_file_content)

        return True

    def remove_file(self, filename):
        # Check user login status.
        if self.__client is None:
            raise NotLoginError("You should login firstly.")

        # Check whether file exist on cloud.
        self.__ui.show_progress_m("checking cloud file folder structure")
        filename_list = self.list_files()
        if filename not in filename_list:
            raise FileNotExistError("File does not exist.")

        # Delete file.
        self.__ui.show_progress_m("removing file on cloud")
        del_file_rep = self.__client.file_delete(filename)

        # Delete hmac.
        self.__ui.show_progress_m("removin file's HMAC on cloud")
        del_hmac_rep = self.__client.file_delete("hmac_" + filename + "_.hmac")

        if del_file_rep["is_deleted"] and del_hmac_rep["is_deleted"]:
            return True
        else:
            return False
