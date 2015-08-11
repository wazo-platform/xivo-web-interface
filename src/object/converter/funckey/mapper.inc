<?php
#
# XiVO Web-Interface
# Copyright (C) 2015  Avencall
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
#


class FuncKeyMapper {

	protected $user_dso = null;
	protected $group_dso = null;
	protected $meetme_dso = null;
	protected $queue_dso = null;
	protected $agent_dso = null;
	protected $paging_dso = null;
	protected $callfilter_dso = null;
	protected $callfiltermember_dso = null;

	protected $confd_transfers = array(
		'blind' => 'blindxfer',
		'attended' => 'atxfer'
	);

	protected $confd_forwards = array(
		'noanswer' => 'fwdrna',
		'busy' => 'fwdbusy',
		'unconditional' => 'fwdunc'
	);

	protected $confd_agent_actions = array(
		'login' => 'agentstaticlogin',
		'logout' => 'agentstaticlogoff',
		'toggle' => 'agentstaticlogtoggle'
	);

	protected $legacy_transfers = array(
		'blindxfer' => 'blind',
		'atxfer' => 'attended'
	);

	protected $legacy_forwards = array(
		'fwdrna' => 'noanswer',
		'fwdbusy' => 'busy',
		'fwdunc' => 'unconditional'
	);

	protected $legacy_agent_actions = array(
		'agentstaticlogin' => 'login',
		'agentstaticlogoff' => 'logout',
		'agentstaticlogtoggle' => 'toggle'
	);

	protected $legacy_services = array(
		'vmuserpurge',
		'vmusermsg',
		'enablevm',
		'phonestatus',
		'recsnd',
		'calllistening',
		'directoryaccess',
		'fwdundoall',
		'pickup',
		'callrecord',
		'incallfilter',
		'enablednd'
	);

	protected $legacy_types = array(
		'user' => array(
			'name' => 'user',
			'destination' => true,
			'supervisable' => true,
		),
		'group' => array(
			'name' => 'group',
			'destination' => true,
			'supervisable' => false,
		),
		'queue' => array(
			'name' => 'queue',
			'destination' => true,
			'supervisable' => false,
		),
		'meetme' => array(
			'name' => 'meetme',
			'destination' => true,
			'supervisable' => true,
		),
		'custom' => array(
			'name' => 'custom',
			'destination' => true,
			'supervisable' => true,
		),
		'featuremap-automon' => array(
			'category' => 'extension-general',
			'name' => 'featuremap-automon',
			'extension' => true,
		),
		'extenfeatures-phonestatus' => array(
			'category' => 'extension-general',
			'name' => 'extenfeatures-phonestatus',
			'extension' => true,
		),
		'extenfeatures-recsnd' => array(
			'category' => 'extension-general',
			'name' => 'extenfeatures-recsnd',
			'extension' => true,
		),
		'extenfeatures-callrecord' => array(
			'category' => 'extension-general',
			'name' => 'extenfeatures-callrecord',
			'extension' => true,
			'supervisable' => true,
			'progfunckey' => true,
		),
		'extenfeatures-incallfilter' => array(
			'category' => 'extension-general',
			'name' => 'extenfeatures-incallfilter',
			'extension' => true,
			'supervisable' => true,
			'progfunckey' => true,
		),
		'extenfeatures-enablednd' => array(
			'category' => 'extension-general',
			'name' => 'extenfeatures-enablednd',
			'extension' => true,
			'supervisable' => true,
			'progfunckey' => true,
		),
		'extenfeatures-pickup' => array(
			'category' => 'extension-general',
			'name' => 'extenfeatures-pickup',
			'extension' => true,
		),
		'extenfeatures-calllistening' => array(
			'category' => 'extension-general',
			'name' => 'extenfeatures-calllistening',
			'extension' => true,
		),
		'extenfeatures-directoryaccess' => array(
			'category' => 'extension-general',
			'name' => 'extenfeatures-directoryaccess',
			'extension' => true,
		),
		'extenfeatures-bsfilter' => array(
			'category' => 'extension-general',
			'name' => 'extenfeatures-bsfilter',
			'extension' => false,
			'supervisable' => true,
		),
		'featuremap-blindxfer' => array(
			'category' => 'extension-transfers',
			'name' => 'featuremap-blindxfer',
			'extension' => true,
		),
		'featuremap-atxfer' => array(
			'category' => 'extension-transfers',
			'name' => 'featuremap-atxfer',
			'extension' => true,
		),
		'extenfeatures-fwdundoall' => array(
			'category' => 'extension-forwards',
			'name' => 'extenfeatures-fwdundoall',
			'extension' => true,
		),
		'extenfeatures-fwdrna' => array(
			'category' => 'extension-forwards',
			'name' => 'extenfeatures-fwdrna',
			'extension' => true,
			'destination' => true,
			'supervisable' => true,
			'progfunckey' => true,
		),
		'extenfeatures-fwdbusy' => array(
			'category' => 'extension-forwards',
			'name' => 'extenfeatures-fwdbusy',
			'extension' => true,
			'destination' => true,
			'supervisable' => true,
			'progfunckey' => true,
		),
		'extenfeatures-fwdunc' => array(
			'category' => 'extension-forwards',
			'name' => 'extenfeatures-fwdunc',
			'extension' => true,
			'destination' => true,
			'supervisable' => true,
			'progfunckey' => true,
		),
		'extenfeatures-enablevm' => array(
			'category' => 'extension-voicemail',
			'name' => 'extenfeatures-enablevm',
			'extension' => true,
			'supervisable' => true,
			'progfunckey' => true,
		),
		'extenfeatures-vmusermsg' => array(
			'category' => 'extension-voicemail',
			'name' => 'extenfeatures-vmusermsg',
			'extension' => true,
		),
		'extenfeatures-vmuserpurge' => array(
			'category' => 'extension-voicemail',
			'name' => 'extenfeatures-vmuserpurge',
			'extension' => true,
		),
		'extenfeatures-agentstaticlogtoggle' => array(
			'category' => 'extension-agent',
			'name' => 'extenfeatures-agentstaticlogtoggle',
			'extension' => false,
			'supervisable' => true,
			'destination' => true,
			'progfunckey' => true,
		),
		'extenfeatures-agentstaticlogin' => array(
			'category' => 'extension-agent',
			'name' => 'extenfeatures-agentstaticlogin',
			'extension' => false,
			'supervisable' => true,
			'destination' => true,
			'progfunckey' => true,
		),
		'extenfeatures-agentstaticlogoff' => array(
			'category' => 'extension-agent',
			'name' => 'extenfeatures-agentstaticlogoff',
			'extension' => false,
			'supervisable' => true,
			'destination' => true,
			'progfunckey' => true,
		),
		'generalfeatures-parkext' => array(
			'category' => 'extension-parking',
			'name' => 'generalfeatures-parkext',
			'extension' => true,
		),
		'generalfeatures-parkpos' => array(
			'category' => 'extension-parking',
			'name' => 'generalfeatures-parkpos',
			'destination' => true,
			'supervisable' => true,
			'extension' => true,
		),
		'extenfeatures-paging' => array(
			'destination' => true,
			'name' => 'extenfeatures-paging',
			'category' => 'extension-paging',
		)
	);

	public function __construct($user_dso, $group_dso, $meetme_dso, $queue_dso, $agent_dso, $paging_dso, $callfilter_dso, $callfiltermember_dso) {
		$this->user_dso = $user_dso;
		$this->group_dso = $group_dso;
		$this->meetme_dso = $meetme_dso;
		$this->queue_dso = $queue_dso;
		$this->agent_dso = $agent_dso;
		$this->paging_dso = $paging_dso;
		$this->callfilter_dso = $callfilter_dso;
		$this->callfiltermember_dso = $callfiltermember_dso;
	}

	public function get_legacy_types() {
		return $this->legacy_types;
	}

	public function map_to_legacy($user_id, $funckeys) {
		$legacy_funckeys = array();
		foreach ($funckeys as $position => $funckey) {
			$legacy_funckeys[] = $this->map_legacy_funckey($user_id, $position, $funckey);
		}
		return $legacy_funckeys;
	}

	private function map_legacy_funckey($user_id, $position, $funckey) {
		$legacy_funckey = array(
			'fknum' => (string) $position,
			'supervision' => (bool) $funckey['blf'],
			'label' => $funckey['label'],
			'type' => '',
			'typeval' => '',
			'identity' => null
		);

		$legacy_destination = $this->map_legacy_destination($user_id, $funckey['destination']);
		$merged_funckey = array_merge($legacy_funckey, $legacy_destination);
		return $merged_funckey;
	}

	private function map_legacy_destination($user_id, $destination) {
		switch ($destination['type']) {
		case 'user':
			$extra = array(
				'type' => 'user',
				'typeval' => (string) $destination['user_id'],
				'identity' => $this->user_identity($destination['user_id'])
			);
			break;
		case 'group':
			$extra = array(
				'type' => 'group',
				'typeval' => (string) $destination['group_id'],
				'identity' => $this->group_identity($destination['group_id'])
			);
			break;
		case 'conference':
			$extra = array(
				'type' => 'meetme',
				'typeval' => (string) $destination['conference_id'],
				'meetme' => (string) $destination['conference_id'],
				'identity' => $this->conference_identity($destination['conference_id'])
			);
			break;
		case 'queue':
			$extra = array(
				'type' => 'queue',
				'typeval' => (string) $destination['queue_id'],
				'identity' => $this->queue_identity($destination['queue_id'])
			);
			break;
		case 'custom':
			$extra = array(
				'type' => 'custom',
				'typeval' => $destination['exten'],
				'custom' => $destination['exten'],
				'identity' => array(
					'identity' => $destination['exten']
				)
			);
			break;
		case 'onlinerec':
			$extra = array(
				'type' => 'featuremap-automon',
			);
			break;
		case 'service':
			$extra = array(
				'type' => 'extenfeatures-' . $destination['service'],
			);
			break;
		case 'bsfilter':
			$extra = $this->map_legacy_bsfilter($user_id, $destination);
			break;
		case 'transfer':
			$transfer_type = $this->confd_transfers[$destination['transfer']];
			$extra = array(
				'type' => 'featuremap-' . $transfer_type,
			);
			break;
		case 'forward':
			$forward_type = $this->confd_forwards[$destination['forward']];
			$extra = array(
				'type' => 'extenfeatures-' . $forward_type,
				'typeval' => $destination['exten'],
				'identity' => array(
					'identity' => $destination['exten']
				)
			);
			break;
		case 'agent':
			$action_type = $this->confd_agent_actions[$destination['action']];
			$extra = array(
				'type' => 'extenfeatures-' . $action_type,
				'typeval' => (string) $destination['agent_id'],
				'identity' => $this->agent_identity($destination['agent_id'])
			);
			break;
		case 'parking':
			$extra = array(
				'type' => 'generalfeatures-parkext',
			);
			break;
		case 'park_position':
			$extra = array(
				'type' => 'generalfeatures-parkpos',
				'typeval' => (string) $destination['position'],
				'identity' => array(
					'identity' => (string) $destination['position']
				)
			);
			break;
		case 'paging':
			$extra = array(
				'type' => 'extenfeatures-paging',
				'typeval' => (string) $destination['paging_id'],
				'identity' => $this->paging_identity($destination['paging_id'])
			);
			break;
		default:
			$extra = array();
			break;
		}

		return $extra;
	}

	private function map_legacy_bsfilter($user_id, $destination) {
		$filtermember = $this->callfiltermember_dso->get($destination['filter_member_id']);
		$callfilter = $this->callfilter_dso->get($filtermember['callfilterid']);
		$boss_id = $callfilter['callfiltermember']['boss']['typeval'];

		if($user_id == $boss_id) {
			$member_id = $filtermember['typeval'];
		} else {
			$member_id = $boss_id;
		}

		return array(
			'type' => 'extenfeatures-bsfilter',
			'typeval' => $member_id,
		);

	}

	private function user_identity($user_id) {
		$user = $this->user_dso->get($user_id);
		return array(
			'id' => $user_id,
			'identity' => $user['firstname'] . " " . $user['lastname']
		);
	}

	private function group_identity($group_id) {
		$group = $this->group_dso->get((string)$group_id);
		return array(
			'id' => $group_id,
			'identity' => $group['name']
		);
	}

	private function conference_identity($conference_id) {
		$conference = $this->meetme_dso->get($conference_id);
		return array(
			'id' => $conference_id,
			'identity' => $conference['name']
		);
	}

	private function queue_identity($queue_id) {
		$queue = $this->queue_dso->get($queue_id);
		return array(
			'id' => $queue_id,
			'identity' => $queue['displayname']
		);
	}

	private function agent_identity($agent_id) {
		$agent = $this->agent_dso->get($agent_id);
		return array(
			'id' => $agent_id,
			'identity' => $agent['firstname'] . " " . $agent['lastname']
		);
	}

	private function paging_identity($paging_id) {
		$paging = $this->paging_dso->get($paging_id);
		return array(
			'id' => $paging_id,
			'identity' => $paging['number']
		);
	}

	public function map_to_confd($user_id, $legacy_funckeys) {
		$funckeys = array();
		foreach($legacy_funckeys as $legacy_funckey) {
			$position = $legacy_funckey['fknum'];
			$funckeys[$position] = $this->map_confd_funckey($user_id, $legacy_funckey);
		}
		return $funckeys;
	}

	private function map_confd_funckey($user_id, $legacy_funckey) {
		$destination = $this->map_destination($user_id, $legacy_funckey);

		$label = $legacy_funckey['label'];
		if(trim($label) === '') {
			$label = null;
		}

		return array(
			'label' => $label,
			'blf' => (bool) ((int) $legacy_funckey['supervision']),
			'destination' => $destination
		);
	}

	private function map_destination($user_id, $ref)
	{
		if ($ref['type'] === 'user')
		{
			return array(
				'type' => 'user',
				'user_id' => (int) $ref['typeval']
			);
		}
		elseif ($ref['type'] === 'group')
		{
			return array(
				'type' => 'group',
				'group_id' => (int) $ref['typeval']
			);
		}
		elseif ($ref['type'] === 'queue')
		{
			return array(
				'type' => 'queue',
				'queue_id' => (int) $ref['typeval']
			);
		}
		elseif ($ref['type'] === 'meetme')
		{
			return array(
				'type' => 'conference',
				'conference_id' => (int) $ref['typeval']
			);
		}
		elseif ($ref['type'] === 'custom')
		{
			return array(
				'type' => 'custom',
				'exten' => $ref['typeval']
			);
		}
		elseif ($ref['type'] === 'extenfeatures-paging') {
			return array(
				'type' => 'paging',
				'paging_id' => (int) $ref['typeval']
			);
		}
		elseif (strpos($ref['type'], 'extenfeatures') === 0)
		{
			$exten_type = substr($ref['type'], 14);

			if (array_key_exists($exten_type, $this->legacy_forwards))
			{
				$forward_type = $this->legacy_forwards[$exten_type];
				$forward = array('type' => 'forward', 'forward' => $forward_type);

				if((string) $ref['typeval'] !== '') {
					$forward['exten'] = $ref['typeval'];
				}

				return $forward;
			}
			elseif (in_array($exten_type, $this->legacy_services))
			{
				return array(
					'type' => 'service',
					'service' => $exten_type
				);
			}
			elseif (array_key_exists($exten_type, $this->legacy_agent_actions))
			{
				$agent_action = $this->legacy_agent_actions[$exten_type];
				return array(
					'type' => 'agent',
					'agent_id' => (int) $ref['typeval'],
					'action' => $agent_action
				);
			}
			elseif ($exten_type == 'bsfilter')
			{
				$filter_member_id = $this->map_bsfilter_member_id($user_id, $ref);
				return array(
					'type' => 'bsfilter',
					'filter_member_id' => $filter_member_id
				);
			}
		}
		elseif (strpos($ref['type'], 'featuremap') === 0)
		{
			$feature_type = substr($ref['type'], 11);

			if($feature_type === 'automon')
			{
				return array('type' => 'onlinerec');
			}
			elseif (array_key_exists($feature_type, $this->legacy_transfers))
			{
				$transfer_type = $this->legacy_transfers[$feature_type];
				return array(
					'type' => 'transfer',
					'transfer' => $transfer_type
				);
			}
		}
		elseif(strpos($ref['type'], 'generalfeatures') === 0)
		{
			$general_type = substr($ref['type'], 16);

			if ($general_type === 'parkext')
			{
				return array('type' => 'parking');
			}
			elseif ($general_type == 'parkpos')
			{
				return array(
					'type' => 'park_position',
					'position' => (int) $ref['typeval']
				);
			}
		}

		dwho_report::push('error', "FK ${ref['fknum']}: cannot determine confd destination");
		return array();
	}

	private function map_bsfilter_member_id($user_id, $phonefunckey) {
		$user_id = (string) $user_id;
		$member_id = (string) $phonefunckey['typeval'];

		$callfiltermember = $this->callfiltermember_dso->get_by_boss_secretary($user_id, $member_id);
		if($callfiltermember === false) {
			$callfiltermember = $this->callfiltermember_dso->get_by_boss_secretary($member_id, $user_id);
		}

		if($callfiltermember === false) {
			dwho_report::push('error', "Cannot find callfilter member for users $user_id and $member_id");
			return null;
		}
		return (int) $callfiltermember['id'];
	}

	public function map_legacy_return($user_id, $funckeys) {
		$legacy_return = array();
		foreach(array_keys($funckeys) as $position) {
			$legacy_return[] = array(
				'iduserfeatures' => (string) $user_id,
				'fknum' =>  (string) $position
			);
		}
		return $legacy_return;
	}

}

?>
