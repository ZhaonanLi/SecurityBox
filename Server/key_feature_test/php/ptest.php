<?php
date_default_timezone_set('UTC');
$curr_timestamp = time();
echo $curr_timestamp;
echo "<hr>";
$plaintext = '{"username":"tom", "timestamp":"12345566"}';
$encoded_text = base64_encode($plaintext);
$decoded_text = base64_decode($encoded_text);
echo "The plaintext = $plaintext<br>";
echo "The encoded_text = $encoded_text<br>";
echo "The decoded_text = $decoded_text<br>";
