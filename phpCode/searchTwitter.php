<?php
// This is an unsafe method of including the keys for now
require('authkeys.php');

// This method will shield the keys
// include($_ENV['YOUR_DOMAIN_ROOT'] . 'httpd.private/authkeys.php');

// Include the oAuth Library
require 'tmhOAuth/tmhOAuth.php';

// Setup an variable to track the number of relevant tweets
$count = 0;

// The Query String
$query = "#snowday";

// Make a Connection to the Twitter oAuth lib
// and transfer the keys from authkeys.php to this file.
$connection = new tmhOAuth(array(
    'consumer_key' => $consumer_key,
    'consumer_secret' => $consumer_secret,
    'user_token' => $user_token,
    'user_secret' => $user_secret
));

// Request API, GET the tweets
$http_code = $connection->request('GET',
    $connection->url('1.1/search/tweets'),
    array('q' => $query,'count' => '100', 'lang' => 'en'));

// Request was successful
if ($http_code == 200) {
    $count = 0;

    // Split the json
    $response = json_decode($connection->response['response'],true);
    $tweet_data = $response['statuses'];

    // Get Today's date
    $today = date("Y-m-d");

    // Loop through the tweets
    foreach ($tweet_data as $tweet) {

        // If the created_at value of the tweet is equal to today's value
        if(date("Y-m-d",strtotime($tweet['created_at'])) == $today) {
            // Increment the Counter
            $count++;
        }
    }
    // Display the end value
    print $count;
    exit;
}
// Handle errors from API request
else {
    if ($http_code == 429) {
        print 'Error: Twitter API rate limit reached';
        exit;
    }
    else {
        print 'Error: Twitter was not able to process that request';
        exit;
    }
    exit;
}
?>
