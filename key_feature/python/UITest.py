# UI Test.
import Tkinter as tk
from tkFileDialog import askopenfilename
from tkFileDialog import askdirectory

import DropboxModel

from DropboxError import NotLoginError, \
    FilenameConflictError, FileNotExistError, FileHmacNotVerifyError,\
    TimestampVerifyError


class SecurityBoxUI(object):
    def __init__(self, dropbox_client):
        self.__dc = dropbox_client
        self.init_ui()

        self.upload_filename = None
        self.download_file_path = None

    def init_ui(self):
        self.win = tk.Tk()
        self.win.geometry("700x451+370+70")
        self.win.title("SecurityBox")

        self.frame1 = tk.Frame(self.win)
        self.frame2 = tk.Frame(self.win)
        self.frame3 = tk.Frame(self.win)
        self.frame4 = tk.Frame(self.win)

        # Frame 1.
        self.filename_list_area = tk.Text(
            self.frame1,
            height=23,
            width=50,
            bg="lightgray"
        )
        self.filename_list_area.pack(side=tk.LEFT)

        self.progress_list_area = tk.Text(
            self.frame1,
            height=23,
            bg="lightgray",
        )

        self.progress_list_area.insert(tk.INSERT, "Press 'login' button\n")
        self.progress_list_area.pack(side=tk.LEFT)

        # Frame 2.
        self.upload_file_chooser = tk.Button(
            self.frame2,
            width=19,
            text='choose upload file',
            command=self.choose_upload_file_call_back
        )
        self.upload_file_chooser.pack(side=tk.LEFT)

        self.upload_button = tk.Button(
            self.frame2,
            width=19,
            text="upload",
            command=self.upload_file_call_back
        )
        self.upload_button.pack(side=tk.LEFT)

        self.remove_filename_textbox = tk.Entry(
            self.frame2,
            width=19,
            fg="gray"
        )

        self.remove_filename_textbox.insert(
            0,
            "remove file name"
        )

        self.remove_filename_textbox.bind(
            "<Button-1>",
            self.click_remove_filename_textbox_call_back
        )

        self.remove_filename_textbox.pack(side=tk.LEFT)

        self.remove_file_button = tk.Button(
            self.frame2,
            width=19,
            text="remove",
            command=self.remove_file_call_back
        )
        self.remove_file_button.pack(side=tk.LEFT)

        # Frame 3.
        self.download_file_location_chooser = tk.Button(
            self.frame3,
            width=19,
            text='download file location',
            command=self.choose_download_file_location_call_back
        )
        self.download_file_location_chooser.pack(side=tk.LEFT)

        self.download_filename_textbox = tk.Entry(
            self.frame3,
            width=20,
            fg="gray"
        )

        self.download_filename_textbox.insert(
            0,
            "download file name"
        )

        self.download_filename_textbox.bind(
            "<Button-1>",
            self.click_download_filename_textbox_call_back
        )

        self.download_filename_textbox.pack(side=tk.LEFT)

        self.download_button = tk.Button(
            self.frame3,
            width=18,
            text="download",
            command=self.download_file_call_back
        )
        self.download_button.pack(side=tk.LEFT)

        self.list_files_button = tk.Button(
            self.frame3,
            width=19,
            text="list file",
            command=self.list_files_call_back
        )
        self.list_files_button.pack(side=tk.LEFT)

        # Frame 4.
        self.login_button = tk.Button(
            self.frame4,
            width=39,
            text="login",
            command=self.login_call_back
        )
        self.login_button.pack(side=tk.LEFT)

        self.confirm_login_button = tk.Button(
            self.frame4,
            width=39,
            text="confirm login",
            command=self.confirm_login_call_back
        )
        self.confirm_login_button.pack(side=tk.LEFT)

        self.frame1.pack()
        self.frame2.pack()
        self.frame3.pack()
        self.frame4.pack()

    def show_progress_m(self, error_m):
        self.progress_list_area.insert(tk.INSERT, error_m + "\n")
        self.progress_list_area.see(tk.END)

    def choose_upload_file_call_back(self):
        self.upload_filename = askopenfilename()
        self.show_progress_m("choose file:{}".format(self.upload_filename))

    def upload_file_call_back(self):
        if self.upload_filename is None or len(self.upload_filename) == 0:
            self.show_progress_m("Error::Before upload, please choose file")
            return

        try:
            self.show_progress_m("file: {} will be uploaed".format(self.upload_filename))
            self.show_progress_m("uploading......")
            upload_succ = self.__dc.upload_file(self.upload_filename)
            if upload_succ:
                self.show_progress_m("upload successfully")
                self.upload_filename = None
                self.list_files_call_back()
            else:
                self.show_progress_m("Error::upload failed")

        except NotLoginError, login_error:
            self.show_progress_m("Error::{}".format(str(login_error)))
        except FilenameConflictError, file_error:
            self.show_progress_m("Error::{}".format(str(file_error)))
        except TimestampVerifyError, time_error:
            self.show_progress_m("Error::{}".format(str(time_error)))

    def click_remove_filename_textbox_call_back(self, event):
        self.remove_filename_textbox.delete(0, tk.END)

    def remove_file_call_back(self):
        remove_filename = self.remove_filename_textbox.get()
        if remove_filename is None or len(remove_filename) == 0:
            self.show_progress_m("Error::Before remove file, please enter file name")
            return

        try:
            self.show_progress_m("file:{} will be removed".format(remove_filename))
            self.show_progress_m("removing......")
            remove_succ = self.__dc.remove_file(remove_filename)

            if remove_succ:
                self.show_progress_m("remove successfully")
                self.list_files_call_back()
            else:
                self.show_progress_m("Error::remove failed")

        except NotLoginError, login_error:
            self.show_progress_m("Error::{}".format(str(login_error)))
        except FileNotExistError, file_error:
            self.show_progress_m("Error::{}".format(str(file_error)))

    def click_download_filename_textbox_call_back(self, event):
        self.download_filename_textbox.delete(0, tk.END)

    def choose_download_file_location_call_back(self):
        self.download_file_path = askdirectory()
        if self.download_file_path is not None and len(self.download_file_path) > 0:
            self.show_progress_m("download location:{}".format(self.download_file_path))

    def download_file_call_back(self):
        if self.download_file_path is None or len(self.download_file_path) == 0:
            self.show_progress_m("Error::Before download, please choose download location")
            return

        download_filename = self.download_filename_textbox.get()
        if download_filename is None or len(download_filename) == 0:
            self.show_progress_m("Error::Before download, please enter download file name")
            return

        try:
            self.show_progress_m("file:{} will be download".format(download_filename))
            self.show_progress_m("downloading......")
            download_succ = self.__dc.download_file(
                download_filename,
                self.download_file_path
            )

            if download_succ:
                self.show_progress_m("download successfully")
                self.download_file_path = None
                self.list_files_call_back()
            else:
                self.show_progress_m("Error::download failed")

        except NotLoginError, login_error:
            self.show_progress_m("Error::{}".format(str(login_error)))
        except FileNotExistError, file_exist_error:
            self.show_progress_m("Error::{}".format(str(file_exist_error)))
        except FileHmacNotVerifyError, file_hmac_error:
            self.show_progress_m("Error::{}".format(str(file_hmac_error)))

    def list_files_call_back(self):
        try:
            self.show_progress_m("list all files on cloud")
            filename_list = self.__dc.list_files()
            self.filename_list_area.delete("1.0", tk.END)
            for filename in filename_list:
                self.filename_list_area.insert(tk.INSERT, filename + "\n")

        except NotLoginError, e:
            self.show_progress_m("Error::{}".format(str(e)))

    def login_call_back(self):
        self.__dc.connect()
        self.show_progress_m("After press 'Allow' in browser, press 'confirm login' button here")

    def confirm_login_call_back(self):
        self.__dc.connect_confirm()
        self.show_progress_m("login confirm.")
        self.list_files_call_back()

if __name__ == "__main__":
    dropbox_client = None
    dropbox_client = DropboxModel.DropboxClient()
    ui = SecurityBoxUI(dropbox_client)
    dropbox_client.init_ui(ui)
    ui.win.mainloop()
