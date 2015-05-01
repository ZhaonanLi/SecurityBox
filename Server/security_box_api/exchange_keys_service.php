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
    $post_json_arr = json_decode($raw_post_body, true);

    $client_timestamp = $post_json_arr["timestamp"];
    $is_valid_timestamp = verify_timestamp($curr_timestamp, $client_timestamp);

    $rep_json_arr = array();
    if (!$is_valid_timestamp)
    {
        $rep_json_arr["timestamp"] = (int)$curr_timestamp;
        $rep_json_arr["error_code"] = 4;

        echo json_encode($rep_json_arr);
        return;
    }

    $client_rsa_public_key = $post_json_arr["client_rsa_public_key"];
    $client_dsa_public_key = $post_json_arr["client_dsa_public_key"];

    // Write client rsa public key to file.
    $client_rsa_public_key_filename = "./security_file/client_rsa_public_key.txt";
    $client_rsa_public_key_file = fopen($client_rsa_public_key_filename, "w") or
                                  die("Cannot open the file $client_rsa_public_key_filename");
    fwrite($client_rsa_public_key_file, $client_rsa_public_key);
    fclose($client_rsa_public_key_file);

    // Write client dsa public key to file.
    $client_dsa_public_key_filename = "./security_file/client_dsa_public_key.txt";
    $client_dsa_public_key_file = fopen($client_dsa_public_key_filename, "w") or
                                  die("Cannot open the file $client_dsa_public_key_filename");
    fwrite($client_dsa_public_key_file, $client_dsa_public_key);
    fclose($client_dsa_public_key_file);


    // Read server rsa public key from file.
    $server_rsa_public_key_filename = "./security_file/server_rsa_public_key.txt";
    $server_rsa_public_key_file = fopen($server_rsa_public_key_filename, "r") or
                                  die("Cannot open server rsa public key file.");

    $server_rsa_public_key = fread(
        $server_rsa_public_key_file,
        filesize($server_rsa_public_key_filename)
    );

    // Read server dsa public key from file.
    $server_dsa_public_key_filename = "./security_file/server_dsa_public_key.txt";
    $server_dsa_public_key_file = fopen($server_dsa_public_key_filename, "r") or
                                  die("Cannot open server dsa public key file.");

    $server_dsa_public_key = fread(
        $server_dsa_public_key_file,
        filesize($server_dsa_public_key_filename)
    );

    $rep_json_arr = array();
    if ($server_rsa_public_key)
    {
        $rep_json_arr["server_rsa_public_key"] = $server_rsa_public_key;
        $rep_json_arr["server_dsa_public_key"] = $server_dsa_public_key;
        $rep_json_arr["timestamp"] = (int)$curr_timestamp;
        $rep_json_arr["error_code"] = 0;
    }
    else
    {
        $rep_json_arr["timestamp"] = (int)$curr_timestamp;
        $rep_json_arr["error_code"] = 1;
    }

    echo json_encode($rep_json_arr);

    /*
    Request:
    {
        "client_rsa_public_key":"xxxxxx",
        "client_dsa_public_key":"xxxxxx",
        "timestamp":xxxxxxx
    }
    Response:
    {
        "server_rsa_public_key":"xxxxxxx",
        "server_dsa_public_key":"xxxxxxx",
        "timestamp":xxxxx,
        "error_code":XXX
    }
    */
}
