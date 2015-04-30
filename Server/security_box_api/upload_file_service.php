<?php
// URL: http://hostname:port_num/security_box_api/upload_file_service.php
$request_method = $_SERVER["REQUEST_METHOD"];

if ($request_method == "GET")
{
    echo "Please use POST request.";
}
else
{
    $post_raw_data = $HTTP_RAW_POST_DATA;
    echo $post_raw_data;
}
