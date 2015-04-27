## SecurityBox
### Security Cloud File Storage System.

##### System:
```json
Language:
    [1] C++11
    [2] PHP 5.4
Library: 
    [1] STL
    [2] Boost
    [3] Crypto++
    [4] Qt 5.4
Network:
    [1] Protocol: HTTP1.1.
    [2] IP Version: IPv4.
HTTP Server:
    [1] Apache2
Database:
    [1] Server: MySQL.
Time-Server: 
    [1] Host: NTP
    [2] PORT: 123
```

##### Security Algorithms:
```json
[1] Asymmetric encryption scheme: RSA.
[2] symmetric encryption scheme: RC4.
[3] Hash: SHA-3.
[4] Digital Signature: DSA.
```

##### Operation:
```json
[1] get server rsa public key
[2] upload client rsa public key
[3] get symmetric key
[4] signup
[5] login
[6] logout
[7] list
[8] remove
[9] rename
[10] get file encrypt nonce
[11] upload file
[12] download file
```

##### Security package format:
###### Request json format(Client --> Server):

Asymmetric Encryption:
```json
    {
        "HMAC":"xxxxxxx",
        "signature":"xxxxxxxx",
        "DSA_public_key":"xxxxxxx",
        "rsa_encrypted_package":"xxxxxx"
    }
```

Symmetric Encryption:
```json
    {
        "HMAC":"xxxxxx",
        "signature":"xxxxxx",
        "DSA_public_key":"xxxxxx",
        "symmetric_encrypted_package":"xxxxxxxxxx"
    }
```

###### Response json format(Server --> Client):

Asymmetric Encryption:
```json
    {
        "HMAC":"xxxxxxx",
        "signature":"xxxxxxxx",
        "DSA_public_key":"xxxxxxx",
        "rsa_encrypted_package":"xxxxxx",
        "security_error_code":"XXX",
        "security_error_message":"xxxxxxx"
    }
```

Symmetric Encryption:
```json
    {
        "HMAC":"xxxxxxx",
        "signature":"xxxxx",
        "DSA_public_key":"xxxxxx",
        "symmetric_encrypted_package":"xxxxxx",
        "security_error_code":"XXX",
        "security_error_message":"xxxxxxx"
    }
```

##### security_error_code --> security_error_message:

* "0": no error.
* "1": HMAC does not match.
* "2": signature does not match.
* "3": decrypt error.
* "4": timestamp does not satisfy. 
* "5": unknown error.


##### Message package format:
###### Request format(Client --> Server):
`All URL based on RESTful API Rules`

[1] get server rsa public key:
```json
    URL: http://hostname:port_num/server_rsa_public_key.php
    
    JSON:
    {
        "timestamp":"xxxxxxx"
    }
```

[2] upload client rsa public key:

```json
    URL: http://hostname:port_num/upload_client_rsa_public_key_service.php
    
    JSON:
    {
        "client_rsa_public_key":"xxxxxxx",
        "timestamp":"xxxxxxx"
    }
```

[3] get symmetric key:

```json
    URL: http://hostname:port_num/symmetric_key.php
    
    JSON:
    {
        "timestamp":"xxxxxxx"
    }
```

[4] signup:
```json
    URL: http://hostname:port_num/signup_service.php
    
    JSON:
    {
        "username":"xxx@xxx",
        "password_hash":"xxxxxxxx",
        "timestamp":"xxxxxx"
    }
```

[5] login:
```json
    URL: http://hostname:port_num/login_service.php
    
    JSON:
    {
        "username":"xxx@xxx",
        "password_hash":"xxxxxxxx",
        "timestamp":"xxxxxx"
    }
```

[6] logout:
```json
    URL: http://hostname:port_num/logout_service.php

    JSON:
    {
        "username":"xxx@xxx",
        "timestamp":"xxxxxx"
    }
```

[7] list:
```json
    URL: http://hostname:port_num/list_service.php
    
    JSON:
    {
        "username":"xxx@xxx",
        "timestamp":"xxxxxx"
    }
```

[8] remove:
```json
    URL: http://hostname:port_num/remove_service.php

    JSON:
    {
        "username":"xxx@xxx",
        "filename":"xxxxxxx",
        "timestamp":"xxxxxx"
    }
```

[9] rename:
```json
    URL: http://hostname:port_num/rename_service.php

    JSON:
    {
        "username":"xxx@xxx",
        "original_filename":"xxxxxx",
        "new_filename":"xxxxxx",
        "timestamp":"xxxxxx"
    }
```

[10] get file encrypt nonce:
```json
    URL: http://hostname:port_num/file_encrypt_nonce.php
    
    JSON:
    {
        "username":"xxx@xxx",
        "filename":"xxxxxxx",
        "timestamp":"xxxxxx"
    }
```

[11] upload file:
```json
    URL: http://hostname:port_num/upload_file_service.php

    {
        "username":"xxx@xxx",
        "filename":"xxxxxx",
        "encrypted_file":"The content of the encrypted file data.",
        "timestamp":"xxxxxx"
    }
```

[12] download file:
```json
    URL: http://hostname:port_num/file.php

    {
        "username":"xxx@xxx",
        "filename":"xxxxxxx",
        "timestamp":"xxxxxx"
    }
```



###### Response json format(Server --> Client):

[1] get server rsa public key:
```json
    {
        "server_rsa_public_key":"xxxxxx",
        "timestamp":"xxxxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```

[2] upload client rsa public key:
```json
    {
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxxx"
    }
```

[3] get symmetric key:
```json
    {
        "symmetric_key":"xxxxxxx",
        "timestamp":"xxxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxxx"
    }
```

[4] signup:
```json
    {
        "username":"xxx@xxx",
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```

[5] login:
```json
    {
        "username":"xxx@xxx",
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```

[6] logout:
```json
    {
        "username":"xxx@xxx",
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```

[7] list:
```json
    {
        "username":"xxx@xxx",
        "filename_list":[
            "xxxxxx",
            "xxxxxx",
            "xxxxxx"
        ],
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```

[8] remove:
```json
    {
        "username":"xxx@xxx",
        "removed_filename":"xxxxxx",
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxx"
    }
```

[9] rename:
```json
    {
        "username":"xxx@xxx",
        "original_filename":"xxxxxx",
        "new_filename":"xxxxxx",
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```

[10] get file encrypt nonce:
```json
    {
        "username":"xxx@xxx",
        "filename":"xxxxxxx",
        "file_encrypt_nonce":"xxxxxx",
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxxx"
    }
```

[11] upload file:
```json
    {
        "username":"xxx@xxx",
        "filename":"xxxxxx",
        "filesize":"xxxxxx",
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```

[12] download file:
```json
    {
        "username":"xxx@xxx",
        "filename":"xxxxxx",
        "filesize":"xxxxxx",
        "encrypted_file":"The content of the encrypted file data.",
        "timestamp":"xxxxxx",
        "error_code":"XXX",
        "error_message":"xxxxxx"
    }
```


##### error_code(type = string) --> error_message(type = string):

* "0": no error.
* "1": server cannot answer server's rsa public key.
* "2": server cannot get client rsa public key.
* "3": server cannot answer symmetric key.
* "4": username conflict.
* "5": server cannot create user folder.
* "6": server cannot register user info into db.
* "7": server cannot find username.
* "8": password_hash and username do not match.
* "9": you have been login.
* "10": cannot logout.
* "11": you have been logout.
* "12": server cannot retrieve filename list.
* "13": server cannot find file based on given username and filename.
* "14": server cannot remove file.
* "15": filename conflict.
* "16": server cannot rename file.
* "17": server cannot retrieve file encrypt nonce based on given username and filename.
* "18": unknown error.




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

[3] File:
```sql
CREATE TABLE File (
    username VARCHAR(70) NOT NULL,
    filename VARCHAR(100) NOT NULL,
    encrypt_nonce VARCHAR(100) NOT NULL,
    modified_date DATE NOT NULL,
    modified_time TIME NOT NULL,
    PRIMARY KEY(username, filename),
    FOREIGIN KEY(username) REFERENCES User(username)
)ENGINE=INNODB;
```
