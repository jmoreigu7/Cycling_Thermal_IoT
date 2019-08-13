<?php
header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$dbh=mysql_connect("localhost", $_GET['usr'], $_GET['pwd']) or die ('I cannot connect to the database because: ' . mysql_error());
mysql_select_db ("tesmater_ciclado01");
if ($_GET['select']!='no'){
	if ($_GET['select']=='parameters'){
		$Result=mysql_query($_GET['query']);
		$row = mysql_fetch_assoc($Result);
		echo "&";
		echo $row["id"].";"; 				// 1
		echo $row["status_id"].";";			// 2 - TEXT --> INT
		echo $row["total_cycles"].";";		// 3
		echo $row["performed_cycles"].";";	// 4
		echo $row["t_max_cycles"].";";		// 5
		echo $row["t_min_cycles"].";";		// 6
		echo $row["t_max_hf"].";";			// 7
		echo $row["t_min_hf"].";";			// 8
		echo $row["t4_max"].";";			// 9
		echo $row["t4_min"].";";			// 10
		echo $row["tub_position"].";";		// 11
		echo $row["rgb_color"].";";			// 12
		echo $row["status_furnance"].";";	// 13
		echo $row["status_fan"].";";		// 14
		echo $row["status_airpump"].";";	// 15
		echo $row["status_motor"].";";		// 16
		echo $row["status_general"]."!";	// 17
	}
	if ($_GET['select']=='commands'){
		$Result=mysql_query($_GET['query']);
		$row = mysql_fetch_assoc($Result);
		echo "&";
		echo $row["id"].";";				// 1 - todas han de venir con algun valor
		echo $row["id_comm"].";";			// 2 - todas han de venir con algun valor
		echo $row["data_comm"].";";			// 4 - no podremos reconstruir el string 
		echo $row["done"]."!";			// 5 - en el microcontrolador
	}
	if ($_GET['select']=='credential'){
		$Result=mysql_query($_GET['query']);
		$row = mysql_fetch_assoc($Result);
		echo "&";
		echo $row["user"].";";			// todas han de venir con algun valor
		echo $row["password"]."!";		// debido a que si hay alguna sin contenido
		}
	if ($_GET['select']=='temperature'){
		$Result=mysql_query($_GET['query']);
		$row = mysql_fetch_assoc($Result);
		echo "&";
		echo $row["id"].";";				// 1 - 
		echo $row["sample_name"].";";		// 2 - 
		echo $row["t1"].";";				// 3 - 
		echo $row["t2"].";";				// 4 - 
		echo $row["t3"].";";				// 5 - 
		echo $row["t4"].";";				// 6 - 
		echo $row["cycles"].";";			// 7 - 
		echo $row["status"].";";			// 8 - 
		echo $row["time"]."!";				// 9 - 
	}
} else {
	echo mysql_query($_GET['query']); // retorna 1 si hace insert
}

//www.tesmaterials.com/ciclado/comlink.php?usr=tesmater_cicla01&pwd=Diopma2203*&select=order&query=SELECT * FROM tesmater_ciclado01.order
//https://www.tesmaterials.com/ciclado/comlink.php?usr=tesmater_cicla01&pwd=Diopma2203*&select=order&query=SELECT%20*%20FROM%20tesmater_ciclado01.order
?>