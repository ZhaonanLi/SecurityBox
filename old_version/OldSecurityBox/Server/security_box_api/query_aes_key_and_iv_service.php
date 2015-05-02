<?php
require_once('timestamp.php');

$request_method = $_SERVER["REQUEST_METHOD"];
$curr_timestamp = get_curr_timestamp();

// Check request method.
if ($request_method == "GET")
{
    echo "";
}
else
{
    $raw_post_body = $HTTP_RAW_POST_DATA;
    $raw_post_body_base64 = base64_encode($raw_post_body);
    $decrypt_message_base64 = exec("./Decrypt --rsa --d $raw_post_body_base64");
    $decrypt_message = base64_decode($post_body_base64);
    $decrypt_json_arr = json_decode($decrypt_message);

    $rep_json_arr = array();
    $rep_json_arr["timestamp"] = $curr_timestamp;
    $rep_json_arr["security_error_code"] = "0";
    $rep_json_arr["security_error_message"] = "";
    if ($decrypt_json_arr["decrypt_error_code"] != "0")
    {
        $rep_json_arr["security_error_code"] = $decrypt_json_arr["decrypt_error_code"];
        $rep_json_arr["security_error_message"] = $decrypt_json_arr["decrypt_error_message"];
    }
    else
    {
        $client_timestamp = $message_json_arr["timestamp"];
        $is_valid_timestamp = verify_timestamp($curr_timestamp, (int)$client_timestamp);

        if (!$is_valid_timestamp)
        {
            $rep_json_arr["error_code"] = "";
            $rep_json_arr["error_message"] = "";
        }
    }
}
