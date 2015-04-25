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
[9] get_rsa_public_key
```

##### Request json format(Client --> Server):

[1] signup:
```json
    {
        "operation":"signup",
        "username":"xxx@xxx",
        "password_hash":"xxxxxxxx",
        "timestamp":"xxxxxx"
    }
```

[2] login:
```json
    {
        "operation":"login",
        "username":"xxx@xxx",
        "password_hash":"xxxxxxxx",
        "timestamp":"xxxxxx"
    }
```

[3] logout:
```json
    {
        "operation":"logout",
        "username":"xxx@xxx",
        "timestamp":"xxxxxx"
    }
```

[4] list:
```json
    {
        "operation":"list",
        "username":"xxx@xxx",
        "timestamp":"xxxxxx"
    }
```

[5] remove:
```json
    {
        "operation":"remove",
        "username":"xxx@xxx",
        "filename":"xxxxxxx",
        "timestamp":"xxxxxx"
    }
```

[6] rename:
```json
    {
        "operation":"rename",
        "username":"xxx@xxx",
        "original_filename":"xxxxxx",
        "new_filename":"xxxxxx",
        "timestamp":"xxxxxx"
    }
```

[7] upload:
```json
    {
        "operation":"upload",
        "username":"xxx@xxx",
        "filename":"xxxxxx",
        "encrypted_file":"The content of the encrypted file data.",
        "timestamp":"xxxxxx"
    }
```

[8] download:
```json
    {
        "operation":"download",
        "username":"xxx@xxx",
        "client_rsa_public_key":"xxxxxx",
        "filename":"xxxxxxx",
        "timestamp":"xxxxxx"
    }
```

[9] get_rsa_public_key:(This operation is sent by plaintext)
```json
    {
        "operation":"get_rsa_public_key",
        "timestamp":"xxxxxx"
    }
```

##### Response json format(Server --> Client):

[1] signup:
```json
    {
        "operation":"signup",
        "username":"xxx@xxx",
        "error_code":"XXX",
        "error_message":"xxxxxx",
        "timestamp":"xxxxxx"
    }
```

[2] login:
```json
    {
        "operation":"login",
        "username":"xxx@xxx",
        "error_code":"XXX",
        "error_message":"xxxxxx",
        "timestamp":"xxxxxx"
    }
```

[3] logout:
```json
    {
        "operation":"logout",
        "username":"xxx@xxx",
        "error_code":"XXX",
        "error_message":"xxxxxx",
        "timestamp":"xxxxxx"
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
        ],
        "timestamp":"xxxxxx"
    }
```

[5] remove:
```json
    {
        "operation":"remove",
        "username":"xxx@xxx",
        "error_code":"XXX",
        "error_message":"xxxxx",
        "removed_filename":"xxxxxx",
        "timestamp":"xxxxxx"
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
        "new_filename":"xxxxxx",
        "timestamp":"xxxxxx"
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
        "filesize":"xxxxxx",
        "timestamp":"xxxxxx"
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
        "encrypted_file":"The content of the encrypted file data.",
        "timestamp":"xxxxxx"
    }
```

[9] get_rsa_public_key:(This operation is sent by plaintext)
```json
    {
        "operation":"get_rsa_public_key",
        "rsa_public_key":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxxxx",
        "timestamp":"xxxxxx"
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
[15] "14": cannot answer rsa_public_key.
[16] "15": unknown error.
```


##### Security json format:
###### Request json format(Client --> Server):

```json
    {
        "HMAC":"xxxxxx",
        "signature":"xxxxxx",
        "DSA_public_key":"xxxxxx",
        "RSA_encrypted_package":"xxxxxxxxxx"
    }
```
###### Response json format(Server --> Client):

```json
    {
        "HMAC":"xxxxxxx",
        "signature":"xxxxx",
        "DSA_public_key":"xxxxxx",
        "RSA_encrypted_package":"xxxxxx"
    }
```


##### security_error_code:
```json
[1] "0": no error.
[2] "1": HMAC does not match.
[3] "2": signature does not match.
[4] "3": RSA decrypt error.
[5] "4": timestamp does not satisfy. 
[6] "5": unknown error.
```


##### Server database scheme:

[1] User:
```sql
CREATE TABLE User (
    username VARCHAR(70) NOT NULL PRIMARY KEY,
    password_hash VARCHAR(100) NOT NULL,
    signup_date DATE NOT NULL,
    signup_time TIME NOT NULL
)ENGINE=INNODB;
```
[2] Session:
```sql
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
    [1] STL
    [2] Boost
    [3] Crypto++
    [4] Qt 5.4
Network:
    [1] Protocol: TCP.
    [2] IP Version: IPv4.
    [3] Port number: 10000-20000.
Database:
    [1] Server: MySQL.
    [2] Client: SQLite.
```


##### Security:
```json
Asymmetric encryption scheme: RSA.
symmetric encryption scheme: RC4.
Hash: SHA3.
Digital Signature: DSA.
Timestamp: Time-Server: 200.20.186.76 
```
[Time server request C++ example here](http://stackoverflow.com/questions/9326677/is-there-any-c-c-library-to-connect-with-a-remote-ntp-server/19835285#19835285)

