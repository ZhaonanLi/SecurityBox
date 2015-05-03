# SecurityBox Client Test.
import dropbox
import webbrowser


def connect(app_key, app_secret):
    access_type = "app_folder"  # Here can be "dropbox" or "app_folder".
    session = dropbox.session.DropboxSession(app_key,
                                             app_secret,
                                             access_type)

    request_token = session.obtain_request_token()

    url = session.build_authorize_url(request_token)
    msg = "Opening %s. Please make sure this application is allowed before continuing."
    print msg % url
    webbrowser.open_new_tab(url)
    raw_input("Press enter to continue")
    access_token = session.obtain_access_token(request_token)

    client = dropbox.client.DropboxClient(session)
    return client


def upload(client, filename):
    with open(filename) as fh:
        # path = os.path.join(self.path, self.filename)
        res = client.put_file("/good_file_good_good.txt", fh)

    return res


def download(client, filename):

    file, metadata = client.get_file_and_metadata(filename)

    dst_filename = "awesome_file_awesome_awe.txt"
    with open(dst_filename, "w") as fh:
        fh.write(file.read())

    return metadata


def list_folder(client):
    folder_metadata = client.metadata('/')
    return folder_metadata


def file_delete(client, filename):
    res = client.file_delete(filename)
    return res


if __name__ == "__main__":
    print "Program is working..."

    # app_key = "tcuxv539ihkcmju"
    # app_secret = "li59ls2n4aso7ga"
    app_key = "3egjek2fwb862o5"
    app_secret = "umi5kqveeekp3mx"
    client = connect(app_key, app_secret)

    # filename = "./helloword.txt"
    # upload_res = upload(client, filename)
    # print upload_res
    #
    # folders = list_folder(client)
    # print folders

    # download_filename = "/good_file_good_good.txt"
    # download_res = download(client, download_filename)
    # print download_res

    delete_res = file_delete(client, "/good_file_good_good.txt")
    print delete_res

    print "Program is end."
