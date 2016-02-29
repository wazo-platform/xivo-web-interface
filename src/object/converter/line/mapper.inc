<?php
#
# XiVO Web-Interface
# Copyright (C) 2016 Avencall
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

class OptionMapper {

	private $native_fields = array(
		'name',
		'type',
		'username',
		'secret',
		'md5secret',
		'context',
		'language',
		'accountcode',
		'amaflags',
		'allowtransfer',
		'fromuser',
		'fromdomain',
		'subscribemwi',
		'buggymwi',
		'call-limit',
		'callerid',
		'fullname',
		'cid_number',
		'maxcallbitrate',
		'insecure',
		'nat',
		'promiscredir',
		'usereqphone',
		'videosupport',
		'trustrpid',
		'sendrpid',
		'allowsubscribe',
		'allowoverlap',
		'dtmfmode',
		'rfc2833compensate',
		'qualify',
		'g726nonstandard',
		'disallow',
		'allow',
		'autoframing',
		'mohinterpret',
		'useclientcode',
		'progressinband',
		't38pt_udptl',
		't38pt_usertpsource',
		'rtptimeout',
		'rtpholdtimeout',
		'rtpkeepalive',
		'deny',
		'permit',
		'defaultip',
		'setvar',
		'host',
		'port',
		'regexten',
		'subscribecontext',
		'fullcontact',
		'vmexten',
		'callingpres',
		'ipaddr',
		'regseconds',
		'regserver',
		'lastms',
		'parkinglot',
		'protocol',
		'category',
		'outboundproxy',
		'transport',
		'remotesecret',
		'directmedia',
		'callcounter',
		'busylevel',
		'ignoresdpversion',
		'session-timers',
		'session-expires',
		'session-minse',
		'session-refresher',
		'callbackextension',
		'registertrying',
		'timert1',
		'timerb',
		'qualifyfreq',
		'contactpermit',
		'contactdeny',
		'unsolicited_mailbox',
		'use_q850_reason',
		'encryption',
		'snom_aoc_enabled',
		'maxforwards',
		'disallowed_methods',
		'textsupport',
	);

	public function map_form($form)
	{
		$options = array();
		if(isset($form['optionname'])) {
			for($i = 0; $i < count($form['optionname']); $i++) {
				$name = $form['optionname'][$i];
				$value = $form['optionvalue'][$i];
				if(trim($name) !== '' && trim($value) !== '') {
					$options[] = array($name, $value);
				}
			}
		} else if (is_array($form)) {
			$options = $form;
		}

		return $options;
	}

	public function merge_options($current, $added)
	{
		$native = array();

		foreach($current as $option) {
			$name = $option[0];
			if(in_array($name, $this->native_fields)) {
				$native[] = $option;
			}

		}

		return array_merge($native, $added);
	}

	public function extract_options($options)
	{
		$extra = array();
		foreach($options as $option) {
			$name = $option[0];
			if(!in_array($name, $this->native_fields)) {
				$extra[] = $option;
			}
		}

		return $extra;
	}
}

?>
