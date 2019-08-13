<!DOCTYPE html>
<html>
<meta http-equiv="refresh" content="0; URL=http://www.tesmaterials.com/ciclado/mainscreen.php">
<head>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.0/jquery.min.js"></script>  
<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css" integrity="sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO" crossorigin="anonymous">
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/js/bootstrap.min.js" integrity="sha384-ChfqqxuZUCnJSK3+MXmPNIyE6ZbWh2IMqE241rYiqJxyMiZ6OW/JmZQ5stwEULTy" crossorigin="anonymous"></script>
<title>Cycling Thermal (IoT) - Univeritat de Barcelona - Jefferson Moreira</title>
</head>
<body>
<?php 
$id_ok = $_POST['iOK'];
if($id_ok == 1){
    $dbh=mysql_connect("localhost", "tesmater_cicla01", "Diopma2203*") 
    or die ('I cannot connect to the database because: ' . mysql_error());
    mysql_select_db ("tesmater_ciclado01");

    $id_ok = $_POST['iOK'];
    $total_c = $_POST['total_c'];
    $performed_c = $_POST['performed_c'];
    $tmax_c = $_POST['tmax_c'];
    $tmin_c = $_POST['tmin_c'];
    $tmax_h = $_POST['tmax_h'];
    $tmin_h = $_POST['tmin_h'];

    $read_params = "SELECT * FROM tesmater_ciclado01.parameters where id=1";
    $result_param = mysql_query($read_params);
    $row = mysql_fetch_assoc($result_param);
    $v_id_params = $row["id"];
    $v_status = $row["status"];
    $v_t4_max = $row["t4_max"];
    $v_t4_min = $row["t4_min"];
    $v_tub_position = $row["tub_position"];
    $v_rgb_color = $row["rgb_color"];
    $v_furnance_power = $row["status_furnance"];
    $v_fan_status = $row["status_fan"];
    $v_air_pump_status = $row["status_airpump"];
    $v_status_motor = $row["status_motor"];
    $v_status_general = $row["status_general"];

    if($id_ok == 1){
        // echo" GAS "; // importante, este update introduce el estado "1" que permite que vaya calentando el horno
        $query = mysql_query("UPDATE tesmater_ciclado01.parameters SET total_cycles = '$total_c',
        performed_cycles = '$performed_c', t_max_cycles = '$tmax_c', t_min_cycles = '$tmin_c', t_max_hf = '$tmax_h', 
        t_min_hf = '$tmin_h' WHERE id=1;")  
        or die ('I cannot connect to the database because: ' . mysql_error()); // retorna 1 si hace el insert

        if (!$query){echo "&Error when inserting the parameters!";}
    }
}
?> 
</body>
</html>
