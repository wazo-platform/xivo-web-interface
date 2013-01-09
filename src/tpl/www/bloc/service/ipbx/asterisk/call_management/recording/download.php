<?php
//logging
$file = $this->get_var('file');
$logphrase = "File " . $file . " is being downloaded.";
$command = 'logger -t xivo-recording "' . $logphrase . '"';
exec($command);


$filepath = $this->get_var('filepath');
$size = filesize($filepath);

header('Pragma: no-cache');
header('Cache-Control: private, must-revalidate');
header('Last-Modified: '.
		date('D, d M Y H:i:s',mktime()).' '.
		dwho_i18n::strftime_l('%Z',null));
header("Content-Disposition: attachment; filename=\"" . $file . "\"");
header("Content-Type: audio/wav");
header("Content-Transfer-Encoding: binary");
header("Content-Length: " . $size);
readfile($filepath);
ob_end_flush();
die();
?>