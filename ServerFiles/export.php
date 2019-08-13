<?php  
      //export.php  
 if(isset($_POST["export"]))  
 {  
    $connect=mysql_connect("localhost", "tesmater_cicla01", "Diopma2203*") 
    or die ('I cannot connect to the database because: ' . mysql_error());
    mysql_select_db ("tesmater_ciclado01");
    //   $connect = mysqli_connect("localhost", "root", "", "testing");  
    header('Content-Type: text/csv; charset=utf-8');  
    header('Content-Disposition: attachment; filename=data.csv');  
    $output = fopen("php://output", "w");  
    fputcsv(($output), (array('id','sample_name', 't1', 't2', 't3', 't4', 'cycles','status','time','date')),(";"));  
    $query = "SELECT id, sample_name, round(t1,2), round(t2,2), round(t3,2), round(t4,2), cycles, status, concat(hour(time), ':', minute(time), ':', round(second(time),2)), time from temperature ORDER BY id DESC";  
    $result = mysql_query($query);  
    while($row = mysql_fetch_assoc($result))  
    {  
        fputcsv($output, $row, ";");  
    }  
    fclose($output);  
 }  
 ?>  