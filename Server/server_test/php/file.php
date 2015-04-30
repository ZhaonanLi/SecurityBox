<?php
// URL: http://hostname:port_num/security_box_api/server_rsa_public_key.php
$request_method = $_SERVER["REQUEST_METHOD"];

if ($request_method == "GET")
{
    echo "Please use GET request.";
}
else
{
    echo "This is server_rsa_public_key serice.";
}
