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
			'exten' => null,
			'typeextenumbers' => null,
			'typevalextenumbers' => null,
			'typeextenumbersright' => null,
			'typevalextenumbersright' => null,
			'iduserfeatures' => $user_id,
			'progfunckey' => false,
			'type' => '',
			'typeval' => '',
			'user' => '',
			'group' => '',
			'queue' => '',
			'meetme' => '',
			'custom' => '',
			'extension' => false,
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
				'typeextenumbersright' => 'user',
				'typevalextenumbersright' => (string) $destination['user_id'],
				'type' => 'user',
				'typeval' => (string) $destination['user_id'],
				'user' => (string) $destination['user_id'],
				'identity' => $this->user_identity($destination['user_id'])
			);
			break;
		case 'group':
			$extra = array(
				'typeextenumbersright' => 'group',
				'typevalextenumbersright' => (string) $destination['group_id'],
				'type' => 'group',
				'typeval' => (string) $destination['group_id'],
				'group' => (string) $destination['group_id'],
				'identity' => $this->group_identity($destination['group_id'])
			);
			break;
		case 'conference':
			$extra = array(
				'typeextenumbersright' => 'meetme',
				'typevalextenumbersright' => (string) $destination['conference_id'],
				'type' => 'meetme',
				'typeval' => (string) $destination['conference_id'],
				'meetme' => (string) $destination['conference_id'],
				'identity' => $this->conference_identity($destination['conference_id'])
			);
			break;
		case 'queue':
			$extra = array(
				'typeextenumbersright' => 'queue',
				'typevalextenumbersright' => (string) $destination['queue_id'],
				'type' => 'queue',
				'typeval' => (string) $destination['queue_id'],
				'queue' => (string) $destination['queue_id'],
				'identity' => $this->queue_identity($destination['queue_id'])
			);
			break;
		case 'custom':
			$extra = array(
				'exten' => $destination['exten'],
				'type' => 'custom',
				'typeval' => $destination['exten'],
				'custom' => $destination['exten']
			);
			break;
		case 'onlinerec':
			$extra = array(
				'typeextenumbers' => 'featuremap',
				'typevalextenumbers' => 'automon',
				'type' => 'featuremap-automon',
				'extension' => true
			);
			break;
		case 'service':
			$extra = array(
				'typeextenumbers' => 'extenfeatures',
				'typevalextenumbers' => $destination['service'],
				'type' => 'extenfeatures-' . $destination['service'],
				'extension' => true
			);
			break;
		case 'bsfilter':
			$extra = $this->map_legacy_bsfilter($user_id, $destination);
			break;
		case 'transfer':
			$transfer_type = $this->confd_transfers[$destination['transfer']];
			$extra = array(
				'typeextenumbers' => 'featuremap',
				'typevalextenumbers' => $transfer_type,
				'type' => 'featuremap-' . $transfer_type,
				'extension' => true
			);
			break;
		case 'forward':
			$forward_type = $this->confd_forwards[$destination['forward']];
			$extra = array(
				'typeextenumbers' => 'extenfeatures',
				'typevalextenumbers' => $forward_type,
				'type' => 'extenfeatures-' . $forward_type,
				'exten' => $destination['exten'],
				'typeval' => $destination['exten'],
				'extension' => true
			);
			break;
		case 'agent':
			$action_type = $this->confd_agent_actions[$destination['action']];
			$extra = array(
				'typeextenumbers' => 'extenfeatures',
				'typevalextenumbers' => $action_type,
				'typeextenumbersright' => 'agent',
				'typevalextenumbersright' => (string) $destination['agent_id'],
				'type' => 'extenfeatures-' . $action_type,
				'typeval' => (string) $destination['agent_id'],
				'identity' => $this->agent_identity($destination['agent_id'])
			);
			break;
		case 'parking':
			$extra = array(
				'typeextenumbers' => 'generalfeatures',
				'typevalextenumbers' => 'parkext',
				'type' => 'generalfeatures-parkext',
				'extension' => true
			);
			break;
		case 'park_position':
			$extra = array(
				'typeextenumbers' => 'generalfeatures',
				'typevalextenumbers' => 'parkpos',
				'type' => 'generalfeatures-parkpos',
				'typeval' => (string) $destination['position'],
				'exten' => (string) $destination['position'],
				'extension' => true
			);
			break;
		case 'paging':
			$extra = array(
				'typeextenumbers' => 'extenfeatures',
				'typevalextenumbers' => 'paging',
				'typeextenumbersright' => 'paging',
				'typevalextenumbersright' => (string) $destination['paging_id'],
				'type' => 'extenfeatures-paging',
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
			'typeextenumbers' => 'extenfeatures',
			'typevalextenumbers' => 'bsfilter',
			'typeextenumbersright' => 'user',
			'typevalextenumbersright' => $member_id,
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
		return array(
			'label' => $legacy_funckey['label'],
			'blf' => (bool) ((int) $legacy_funckey['supervision']),
			'destination' => $destination
		);
	}

	private function map_destination($user_id, $ref)
	{
		if($ref['typeextenumbersright'] === 'user' && $ref['typevalextenumbers'] !== 'bsfilter')
		{
			return array(
				'type' => 'user',
				'user_id' => (int) $ref['typevalextenumbersright']
			);
		}
		elseif($ref['typeextenumbersright'] === 'group')
		{
			return array(
				'type' => 'group',
				'group_id' => (int) $ref['typevalextenumbersright']
			);
		}
		elseif($ref['typeextenumbersright'] === 'queue')
		{
			return array(
				'type' => 'queue',
				'queue_id' => (int) $ref['typevalextenumbersright']
			);
		}
		elseif($ref['typeextenumbersright'] === 'meetme')
		{
			return array(
				'type' => 'conference',
				'conference_id' => (int) $ref['typevalextenumbersright']
			);
		}
		elseif($ref['typeextenumbers'] === 'extenfeatures'
			  && array_key_exists($ref['typevalextenumbers'], $this->legacy_forwards)
		)
		{
			$forward_type = $this->legacy_forwards[$ref['typevalextenumbers']];
			$forward = array('type' => 'forward', 'forward' => $forward_type);

			if((string)$ref['exten'] !== '') {
				$forward['exten'] = $ref['exten'];
			}

			return $forward;
		}
		elseif($ref['typeextenumbers'] === 'extenfeatures'
				&& in_array($ref['typevalextenumbers'], $this->legacy_services)
		)
		{
			return array(
				'type' => 'service',
				'service' => $ref['typevalextenumbers']
			);
		}
		elseif($ref['typeextenumbers'] === 'extenfeatures'
			&& array_key_exists($ref['typevalextenumbers'], $this->legacy_agent_actions)
		)
		{
			$agent_action = $this->legacy_agent_actions[$ref['typevalextenumbers']];
			return array(
				'type' => 'agent',
				'agent_id' => (int) $ref['typevalextenumbersright'],
				'action' => $agent_action
			);
		}
		elseif($ref['typeextenumbers'] === 'featuremap'
				&& $ref['typevalextenumbers'] === 'automon')
		{
			return array('type' => 'onlinerec');
		}
		elseif($ref['typeextenumbers'] === 'featuremap'
				&& array_key_exists($ref['typevalextenumbers'], $this->legacy_transfers))
		{
			$transfer_type = $this->legacy_transfers[$ref['typevalextenumbers']];
			return array(
				'type' => 'transfer',
				'transfer' => $transfer_type
			);
		}
		elseif($ref['typeextenumbers'] === 'generalfeatures'
				&& $ref['typevalextenumbers'] === 'parkext')
		{
			return array('type' => 'parking');
		}
		elseif($ref['typevalextenumbers'] == 'bsfilter')
		{
			$filter_member_id = $this->map_bsfilter_member_id($user_id, $ref);
			return array(
				'type' => 'bsfilter',
				'filter_member_id' => $filter_member_id
			);
		}
		elseif($ref['typevalextenumbers'] == 'parkpos')
		{
			return array(
				'type' => 'park_position',
				'position' => (int) $ref['exten']
			);
		}
		elseif($ref['typevalextenumbers'] == 'paging')
		{
			return array(
				'type' => 'paging',
				'paging_id' => (int) $ref['typevalextenumbersright']
			);
		}
		elseif( ($ref['typeextenumbers'] === null || $ref['typeextenumbers'] === '')
				&& ($ref['typevalextenumbers'] === null || $ref['typevalextenumbers'] === ''))
		{
			return array(
				'type' => 'custom',
				'exten' => $ref['exten']
			);
		}

		dwho_report::push('error', "FK ${ref['fknum']}: cannot determine confd destination");
		return array();
	}

	private function map_bsfilter_member_id($user_id, $phonefunckey) {
		$user_id = (string) $user_id;
		$member_id = (string) $phonefunckey['typevalextenumbersright'];

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
