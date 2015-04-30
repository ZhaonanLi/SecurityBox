<?php
// URL: http://hostname:port_num/security_box_api/upload_file_service.php
$request_method = $_SERVER["REQUEST_METHOD"];

if ($request_method == "GET")
{
    echo "Please use GET request.";
}
else
{
    echo "This is upload_file_service serice.";
}
