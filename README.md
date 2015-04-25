## SecurityBox
### Security Cloud File Storage System.

##### Operation:
```json
[1] signup
[2] login
[3] logout
[4] list
[5] remove
[6] rename
[7] upload
[8] download
```

##### Request json format(Client --> Server):

[1] signup:
```json
    {
        "operation":"signup",
        "username":"xxx@xxx",
        "password_hash":"xxxxxxxx"
    }
```

[2] login:
```json
    {
        "operation":"login",
        "username":"xxx@xxx",
        "password_hash":"xxxxxxxx"
    }
```

[3] logout:
```json
    {
        "operation":"logout",
        "username":"xxx@xxx"
    }
```

[4] list:
```json
    {
        "operation":"list",
        "username":"xxx@xxx"
    }
```

[5] remove:
```json
    {
        "operation":"remove",
        "username":"xxx@xxx",
        "filename":"xxxxxxx"
    }
```

[6] rename:
```json
    {
        "operation":"rename",
        "username":"xxx@xxx",
        "original_filename":"xxxxxx",
        "new_filename":"xxxxxx"
    }
```json

[7] upload:
```json
    {
        "operation":"upload",
        "username":"xxx@xxx",
        "filename":"xxxxxx",
        "encrypted_file":"The content of the encrypted file data."
    }
```json

[8] download:
```json
    {
        "operation":"download",
        "username":"xxx@xxx",
        "filename":"xxxxxxx"
    }
```

##### Response json format(Server --> Client):

[1] signup:
```json
    {
        "operation":"signup",
        "username":"xxx@xxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```

[2] login:
```json
    {
        "operation":"login",
        "username":"xxx@xxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```

[3] logout:
```json
    {
        "operation":"logout",
        "username":"xxx@xxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```

[4] list:
```json
    {
        "operation":"list",
        "username":"xxx@xxx",
        "error_code":"XXX",
        "error_message":"xxxxxx",
        "filename_list":[
            "xxxxxx",
            "xxxxxx",
            "xxxxxx"
        ]
    }
```

[5] remove:
```json
    {
        "operation":"remove",
        "username":"xxx@xxx",
        "error_code":"XXX",
        "error_message":"xxxxx",
        "removed_filename":"xxxxxx"
    }
```

[6] rename:
```json
    {
        "operation":"rename",
        "username":"xxx@xxx",
        "error_code":"XXX",
        "error_message":"xxxxxx",
        "original_filename":"xxxxxx",
        "new_filename":"xxxxxx"
    }
```

[7] upload:
```json
    {
        "operation":"upload",
        "username":"xxx@xxx",
        "error_code":"XXX",
        "error_message":"xxxxxx",
        "filename":"xxxxxx",
        "filesize":"xxxxxx"
    }
```

[8] download:
```json
    {
        "operation":"download",
        "username":"xxx@xxx",
        "error_code":"XXX",
        "error_message":"xxxxxx",
        "filename":"xxxxxx",
        "encrypted_file":"The content of the encrypted file data."
    }
```

##### error_code(type = string):
```json
[1] "0": no error(success).
[2] "1": username conflict(username has been used). 
[3] "2": cannot create user folder.
[4] "3": cannot find username(this user has not been registered).
[5] "4": password and username do not match.
[6] "5": you have been login.
[7] "6": cannot logout.
[8] "7": you have been logout.
[9] "8": cannot retrieve filename list.
[10] "9": cannot find file.
[11] "10": cannot remove file.
[12] "11": cannot rename file.
[13] "12": filename conflict.
[14] "13": cannot upload file.
[15] "14": unkown error.
```


##### Security json format:
###### Request json format(Client --> Server):

[1] Outer layer:
```json
    {
        "HMAC":"xxxxxx",
        "RSA_encrypted_package":"xxxxxx"
    }
```
[2] Inner layer(package):
```json
    {
        "message":"message json",
        "DSA_public_key":"xxxxxx",
        "DSA_signature":"xxxxxxx",
        "timestamp":"xxxxxxxxxxx"
    }
```
###### Response json format(Server --> Client):

[1] Outer layer:
```json
    {
        "HMAC":"xxxxxxx",
        "RSA_encrypted_package":"xxxxxx"
    }
```

[2] Inner layer(package):
```json
    {
        "message":"message_json",
        "DSA_pubic_key":"xxxxxx",
        "DSA_signuature":"xxxxx",
        "timestamp":"xxxxxxxxxx",
        "security_error_code":"XXX",
        "security_error_message":"xxxxxx"
    }
```


##### security_error_code:
```json
[1] "0": no error.
[2] "1": RSA decrypt error.
[3] "2": HMAC does not match.
[4] "3": timestamp does not satisfy. 
[5] "4": signature does not match.
[6] "5": unknown error.
```


##### Server database scheme:

[1] User:
```json
CREATE TABLE User (
    username VARCHAR(70) NOT NULL PRIMARY KEY,
    password_hash VARCHAR(100) NOT NULL,
    signup_date DATE NOT NULL,
    signup_time TIME NOT NULL
)ENGINE=INNODB;
```
[2] Session:
```json
CREATE TABLE Session (
    username VARCHAR(70) NOT NULL,
    login_date DATE NOT NULL,
    login_time TIME NOT NULL,
    logout_date DATE,
    logout_time TIME,
    PRIMARY KEY(username, login_date, login_time),
    FOREIGN KEY(username) REFERENCES User(username)
)ENGINE=INNODB;
```



##### System:
```json
Language:C++11
Library: 
    [a] STL
    [b] Boost
    [c] Crypto++
    [d] Qt 5.4
```


##### Security:
```json
Asymmetric encryption scheme: RSA.
symmetric encryption scheme: RC4.
Hash: SHA3.
Digital Signature: DSA.
Timestamp: Time-Server: 200.20.186.76 [C++ example here][time_server_example]
```

[time_server_example]:http://stackoverflow.com/questions/9326677/is-there-any-c-c-library-to-connect-with-a-remote-ntp-server/19835285#19835285 "C++ example here"
