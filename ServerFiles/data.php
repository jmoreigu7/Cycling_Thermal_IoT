<?php
header('Content-Type: application/json');
header("Cache-Control: no-cache, must-revalidate"); // HTTP/1.1
header("Expires: Sat, 1 Jul 2000 05:00:00 GMT"); // Fecha en el pasado
 
$con = mysql_connect("localhost", "tesmater_cicla01", "Diopma2203*") or die ('I cannot connect to the database because: ' . mysql_error());
mysql_select_db ("tesmater_ciclado01");

$data_points = array();
if (mysqli_connect_errno($con)) {
    echo "Failed to connect to DataBase: " . mysqli_connect_error();
} else {
    $result = mysql_query("SELECT (UNIX_TIMESTAMP(time-INTERVAL 7 HOUR)*1000) as time, t1, t2, t3, t4 FROM tesmater_ciclado01.temperature;");
    // SELECT CONCAT(HOUR(time), '.',MINUTE(time)) as time from temperature 
    while ($row = mysql_fetch_array($result)) {
        $point = array("time" => $row['time'], "valory1" => $row['t1'], "valory2" => $row['t2'], "valory3" => $row['t3'], "valory4" => $row['t4']);
        // $point = array("time" => $row['time'], "valory1" => $row['t1'], "valory2" => $row['t2'], "valory3" => $row['t3']);
        array_push($data_points, $point);
    }
    echo json_encode($data_points);
}
mysql_close($con);
?>