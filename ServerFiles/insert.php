<?php
header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$dbh=mysql_connect("localhost", "tesmater_cicla01", "Diopma2203*") or die ('I cannot connect to the database because: ' . mysql_error());
mysql_select_db ("tesmater_ciclado01");
// to insert all values of parameters
$status = $_POST['status'];
$total_c = $_POST['total_c'];
$performed_c = $_POST['performed_c'];
$tmax_c = $_POST['tmax_c'];
$tmin_c = $_POST['tmin_c'];
$tmax_h = $_POST['tmax_h'];
$tmin_h = $_POST['tmin_h'];
$tmax_cu = $_POST['tmax_cu'];
$t4max = $_POST['t4max'];
$t4min = $_POST['t4min'];
$tubposition = $_POST['tub_position'];
$rgbcolor = $_POST['rgbcolor'];
$furnancepow = $_POST['furnancepow'];
$fan = $_POST['fan'];
$airpump = $_POST['airpump'];
$motorstatus = $_POST['motorstatus'];
$generalstatus = $_POST['generalstatus'];
// commands
$v_id = $_POST['id'];
$v_id_comm = $_POST['id_comm'];
$v_command = $_POST['command'];
$v_data_comm = $_POST['data_comm'];
$v_execute = $_POST['execute'];
$v_response = $_POST['response'];
// to insert temperature
$sname = $_GET['Muestra'];
$t1 = $_GET['T1'];
$t2 = $_GET['T2'];
$t3 = $_GET['T3'];
$t4 = $_GET['T4'];
$ciclos = $_GET['Ciclos'];
$status = $_GET['Estado'];
$fecha = "CURRENT_TIMESTAMP + interval 7 hour"; // +7, servidor en USA

$xtest = $_GET['xtest'];
$ytest = $_GET['ytest'];

if($_GET['option']!='no'){
    if($_GET['option']=='allparameters'){
        $query = mysql_query("INSERT INTO tesmater_ciclado01.parameters (status_id, total_cycles, performed_cycles, t_max_cycles, t_min_cycles, t_max_hf, t_min_hf,
                        t4_max, t4_min, tub_position, rgb_color, status_furnance, status_fan, status_airpump, status_motor, status_general) 
        values ('$status','$total_c','$performed_c','$tmax_c','$tmin_c','$tmax_h','$tmin_h','$t4max','$t4min','$tubposition','$rgbcolor','$furnancepow','$fan',
        '$airpump','$motorstatus','$generalstatus');")  or die ('I cannot connect to the database because: ' . mysql_error()); // retorna 1 si hace el insert
        if ($query){
            echo "&200!";
        } else {
            echo "&500!";//"&Error when inserting the parameters!";
        }
    }
    if($_GET['option']=='updateparams1'){
        $query = mysql_query("INSERT INTO tesmater_ciclado01.parameters (status_id, tub_position, rgb_color, status_furnance, status_fan, status_airpump,
        status_motor, status_general) 
        values ('$status','$tubposition','$rgbcolor','$furnancepow','$fan','$airpump','$motorstatus','$generalstatus');")  or die ('I cannot connect to the database because: ' . mysql_error()); // retorna 1 si hace el insert
        if ($query){
            echo "&200!";
        } else {
            echo "&500!";//"&Error when inserting the principal parameters!";
        }
    }
    if($_GET['option']=='temperature'){
        $query = mysql_query("INSERT INTO tesmater_ciclado01.temperature (sample_name,t1,t2,t3,t4,cycles,status_id,time) 
        values ('$sname','$t1','$t2','$t3','$t4','$ciclos','$status',$fecha);")  
        or die ('I cannot connect to the database because: ' . mysql_error()); // retorna 1 si hace el insert
        if ($query){
            echo "&200!";
        } else {
            echo "&500!";// "&Error when inserting the temperature!";
        }
    }
/*    
    if($_GET['option']=='insertCommandsCycle'){
        $query = mysql_query("INSERT INTO tesmater_ciclado01.commands (id,id_comm,commands,data_comm,executed,response) 
        values ('$v_id','$v_id_comm','$v_command','$v_data_comm','$v_execute','$v_response');")  
        or die ('I cannot connect to the database because: ' . mysql_error()); // retorna 1 si hace el insert
        if ($query){
            echo "&200!";
        } else {
            echo "&500!";// "&Error when inserting the temperature!";
        }
    }
*/
    if($_GET['option']=='insertCommandsCycle'){
        $query = mysql_query("INSERT INTO tesmater_ciclado01.commands (id,id_comm,data_comm,done) 
        values ('$v_id','$v_id_comm','$v_data_comm','$v_execute');")  
        or die ('I cannot connect to the database because: ' . mysql_error()); // retorna 1 si hace el insert
        if ($query){
            echo "&200!";
        } else {
            echo "&500!";// "&Error when inserting the temperature!";
        }
    }
    if($_GET['option']=='test'){
        $query = mysql_query("INSERT INTO tesmater_ciclado01.test (x,y) 
        values ('$xtest','$ytest');")  
        or die ('I cannot connect to the database because: ' . mysql_error()); // retorna 1 si hace el insert
        if ($query){
            echo "&200!";
        } else {
            echo "&500!";//"&Error when inserting the temperature!";
        }
    }
}
?>