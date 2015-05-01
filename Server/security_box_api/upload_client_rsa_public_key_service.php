<?php
require_once('timestamp.php');

$request_method = $_SERVER["REQUEST_METHOD"];
$curr_timestamp = get_curr_timestamp();

// Check request method.
if ($request_method == "GET")
{
    $rep_json_arr = array(
        "operation_package" => array(
            "timestamp" => $curr_timestamp,
            "error_code" => "7",
            "error_message" => "request error"
        )
    );
    echo json_encode($rep_json_arr);
}
else
{
    $raw_post_body = $HTTP_RAW_POST_DATA;
    $post_json_arr = json_decode($raw_post_body, true);

    $client_timestamp = $post_json_arr["timestamp"];
    $is_valid_timestamp = verify_timestamp($curr_timestamp, (int)$client_timestamp);

    $rep_json_arr = array();
    if (!$is_valid_timestamp)
    {
        $rep_json_arr["timestamp"] = $curr_timestamp;
        $rep_json_arr["error_code"] = "4";
        $rep_json_arr["error_message"] = "timestamp does not satisfy.";

        echo json_encode($rep_json_arr);
        return;
    }

    
}
