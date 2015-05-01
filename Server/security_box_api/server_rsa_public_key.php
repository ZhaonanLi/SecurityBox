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

    // Read server rsa public key from file.
    $server_rsa_pub_key_filename = "./security_file/server_rsa_public_key.txt";
    $server_rsa_pub_key_file = fopen($server_rsa_pub_key_filename, "r") or
                               die("Cannot open server rsa public key file.");
    $server_rsa_public_key = fread($server_rsa_pub_key_file,
                                   filesize($server_rsa_pub_key_filename));

    $rep_json_arr = array();
    if ($server_rsa_public_key)
    {
        $rep_json_arr["server_rsa_public_key"] = $server_rsa_public_key;
        $rep_json_arr["timestamp"] = (string)$curr_timestamp;
        $rep_json_arr["error_code"] = "0";
        $rep_json_arr["error_message"] = "no error";
    }
    else
    {
        $rep_json_arr["timestamp"] = (string)$curr_timestamp;
        $rep_json_arr["error_code"] = "1";
        $rep_json_arr["error_message"] = "server cannot answer server's rsa public key";
    }

    echo json_encode($rep_json_arr);
}
