<?php
// URL: http://hostname:port_num/security_box_api/server_rsa_public_key.php
require_once('ntp_timestamp.php');

$request_method = $_SERVER["REQUEST_METHOD"];
$current_timestamp = get_curr_ntp_timestamp();


// Check request method.
if ($request_method == "GET")
{
    $response_json_array = array(
        "operation_package" => array(
            "error_code" => "1",
            "error_message" => "request error",
            "timestamp" => $current_timestamp
        )
    );

    $response_json_str = json_encode($response_json_array);
    echo $response_json_str;
}
else
{

}
