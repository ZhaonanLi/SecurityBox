<?php
// URL: http://hostname:port_num/security_box_api/generate_file_encrypt.php
$request_method = $_SERVER["REQUEST_METHOD"];

if ($request_method == "GET")
{
    echo "Please use GET request.";
}
else
{
    echo "This is generate_file_encrypt serice.";
}
