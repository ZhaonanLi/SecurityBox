# SecurityBox Client.
from dropbox import client, rest, session
import webbrowser


def get_user_access_token(app_key, app_secret):
    # Include the Dropbox SDK libraries

    # Get your app key and secret from the Dropbox developer website
    APP_KEY = app_key
    APP_SECRET = app_secret

    ACCESS_TYPE = 'app_folder'

    sess = session.DropboxSession(APP_KEY, APP_SECRET, ACCESS_TYPE)
    request_token = sess.obtain_request_token()
    url = sess.build_authorize_url(request_token)

    # Make the user sign in and authorize this token
    print "url:", url

    webbrowser.open_new_tab(url)

    print "Please visit this website and press 'Allow' button, \
           then hit 'Enter' here."
    raw_input()

    # This will fail if the user didn't visit the above URL
    access_token = sess.obtain_access_token(request_token)

    return access_token


if __name__ == "__main__":
    print "Program is working..."

    app_key = "3egjek2fwb862o5"
    app_secret = "umi5kqveeekp3mx"
    access_token = get_user_access_token(app_key, app_secret)
    print "The access_token = {}".format(access_token)

    print "Program is end."
