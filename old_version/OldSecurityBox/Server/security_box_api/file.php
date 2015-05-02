<?php
// URL: http://hostname:port_num/security_box_api/server_rsa_public_key.php
$request_method = $_SERVER["REQUEST_METHOD"];

if ($request_method == "GET")
{
    echo "Please use POST request.";
}
else
{
    $post_raw_data = $HTTP_RAW_POST_DATA;
    $json_arr = json_decode($post_raw_data, TRUE);
    echo "##### username => ".$json_arr["username"].", and the password => ".$json_arr["password"];
}
