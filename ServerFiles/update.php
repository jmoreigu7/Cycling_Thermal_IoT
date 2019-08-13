<?php
header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$dbh=mysql_connect("localhost", "tesmater_cicla01", "Diopma2203*") or die ('I cannot connect to the database because: ' . mysql_error());
mysql_select_db ("tesmater_ciclado01");
// to insert all values of parameters
$v_status =         $_GET['status'];
$v_total_c =        $_GET['total_c'];
$v_performed_c =    $_GET['performed_c'];
$v_tmax_c =         $_GET['tmax_c'];
$v_tmin_c =         $_GET['tmin_c'];
$v_tmax_h =         $_GET['tmax_h'];
$v_tmin_h =         $_GET['tmin_h'];
$v_tmax_cu =        $_GET['tmax_cu'];
$v_t4max =          $_GET['t4max'];
$v_t4min =          $_GET['t4min'];
$v_tubposition =    $_GET['tub_position'];
$v_rgbcolor =       $_GET['rgbcolor'];
$v_furnancepow =    $_GET['furnancepow'];
$v_fan =            $_GET['fan'];
$v_airpump =        $_GET['airpump'];
$v_motorstatus =    $_GET['motorstatus'];
$v_generalstatus =  $_GET['generalstatus'];
// commands
$v_id_comm = $_GET['ID'];
$v_data_comm = $_GET['DATA'];
$v_done = $_GET['DONE'];

if($_GET['option']!='no'){
    if($_GET['option']=='commands'){
        $query = mysql_query("UPDATE commands SET id_comm=$v_id_comm, data_comm='$v_data_comm', done=$v_done where id=1;") or die ('I cannot connect to the database because: ' . mysql_error()); // retorna 1 si hace el insert
        if ($query){
            echo "&200!";
        } else {
            echo "&500!";//"&Error when inserting the parameters!";
        }
    }
    if($_GET['option']=='updateParam'){
        $query = mysql_query("UPDATE tesmater_ciclado01.parameters SET status_id=$v_status, tub_position=$v_tubposition, rgb_color=$v_rgbcolor, status_furnance=$v_furnancepow, status_fan=$v_fan, status_airpump=$v_airpump, status_motor=$v_motorstatus, status_general=$v_generalstatus where id=1;") or die ('I cannot connect to the database because: ' . mysql_error()); // retorna 1 si hace el insert
        if ($query){
            echo "&200!";
        } else {
            echo "&500!";//"&Error when inserting the parameters!";
        }
    }
}
?>
