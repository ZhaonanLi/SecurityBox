# UI Test.
import Tkinter as tk
from tkFileDialog import askopenfilename
from tkFileDialog import askdirectory

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
        self.filename_list_area= tk.Text(
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
        
    def choose_upload_file_call_back(self):
        self.upload_filename = askopenfilename()
        
    def upload_file_call_back(self):
        # self.filename_list_area.insert(tk.INSERT, "hello\n")
        
        
        print "hello ---->"
        
    def remove_file_call_back(self):
        pass
    
    def choose_download_file_location_call_back(self):
        self.download_file_path = askdirectory()
    
    def download_file_call_back(self):
        pass
        
    def list_files_call_back(self):
        pass
    
    def login_call_back(self):
        pass
    
    def confirm_login_call_back(self):
        pass
        
if __name__ == "__main__":
    dropbox_client = None
    ui = SecurityBoxUI(dropbox_client)
    ui.win.mainloop()
    
    