<?php
function get_curr_timestamp()
{
    date_default_timezone_set('UTC');
    return time();
}

function verify_timestamp($time_1, $time_2)
{
    $diff_threshold = 60;
    $diff = abs($time_1 - $time_2);
    return $diff <= $diff_threshold;
}
