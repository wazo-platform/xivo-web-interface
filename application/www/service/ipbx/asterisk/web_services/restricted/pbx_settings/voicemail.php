<?php

if(isset($_SERVER['REMOTE_ADDR']) === false
|| $_SERVER['REMOTE_ADDR'] !== '127.0.0.1')
	xivo_die('Error/403');

$appvoicemail = &$ipbx->get_application('voicemail',null,false);

switch($_QRY->get_qs('act'))
{
	case 'list':
	default:
		if(($voicemail = $appvoicemail->get_voicemail_list()) === false)
			xivo_die('no-data');

		$_HTML->set_var('voicemail',$voicemail);
		$_HTML->set_var('sum',$_QRY->get_qs('sum'));
		$_HTML->display('/service/ipbx/'.$ipbx->get_name().'/pbx_settings/voicemail');
}

?>
