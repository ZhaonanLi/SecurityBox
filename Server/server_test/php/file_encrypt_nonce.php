<?php
// URL: http://hostname:port_num/security_box_api/file_encrypt_nonce.php
$request_method = $_SERVER["REQUEST_METHOD"];

if ($request_method == "GET")
{
    echo "Please use GET request.";
}
else
{
    echo "This is file_encrypt_nonce serice.";
}
