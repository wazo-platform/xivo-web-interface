<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2014  Avencall
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

require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'application','abstract','datastorage','sql.inc'));

class xivo_application_service_asterisk_user_sql extends xivo_application_abstract_datastorage_sql
{
	var $_table	= array(
				'extensions'		=> '',
				'userfeatures'		=> '',
				'callfilter'		=> '',
				'callfiltermember'	=> '',
				'phonefunckey'		=> '',
				'agentfeatures'		=> '',
				'linefeatures'		=> '',
				'groupfeatures'		=> '',
				'queuefeatures'		=> '',
				'meetmefeatures'	=> '',
				'user_line'			=> '',
				'func_key'			=> '',
				'func_key_dest_user'	=> '',
				'func_key_dest_group'	=> '',
				'func_key_dest_queue'	=> '',
				'func_key_dest_conference'	=> '',
				'func_key_dest_service'	=> '',
				'func_key_dest_forward'	=> '',
				'func_key_dest_park_position'	=> '',
				'func_key_dest_features'	=> '',
				'func_key_dest_paging'	=> '',
				'func_key_dest_custom'	=> '',
				'func_key_dest_agent'	=> '',
				'func_key_dest_bsfilter'	=> '',
				'func_key_destination_type'	=> '',
				'func_key_mapping'	=> '',
				'func_key_template'	=> '',
				'func_key_type'		=> '');

	function get_all($disable=null,$initialized=null,$order=null,$limit=null,$value=null)
	{
		$this->_cnt = 0;

		$arr = array();

		$this->_dso->new_select($this->_table['userfeatures']);

		if (defined('ENTITY_FILTER_ID'))
			$arr['entityid'] = ENTITY_FILTER_ID;

		if($disable !== null)
			$arr['commented'] = intval((bool) $disable);

		$this->_dso->andwhere($arr,$this->_table['userfeatures']);

		if($initialized === true)
		{
			$this->_dso->innerjoin($this->_table['user_line'],
							$this->_table['user_line'].'.user_id = '. $this->_table['userfeatures'].'.id',
							null,
							'id');
		}
		else
		{
			$this->_dso->leftjoin($this->_table['user_line'],
							$this->_table['user_line'].'.user_id = '. $this->_table['userfeatures'].'.id',
							null,
					 		'id');
		}

		$this->_dso->leftjoin($this->_table['linefeatures'],
				 		$this->_table['user_line'].'.line_id = '. $this->_table['linefeatures'].'.id',
						null,
						'number');

		$this->_dso->innerjoin('entity',
				 		'entity.id = '. $this->_table['userfeatures'].'.entityid',
						null,
						'*');

		$this->_dso->aggregate_where();

		if(dwho_has_len($value) === true)
		{
			$this->_dso->aggregate_where();
			if(($concat = $this->_dso->concat(array((object) ($this->_table['userfeatures'].'.firstname'),
								' ',
								(object) ($this->_table['userfeatures'].'.lastname')))) !== false)
				$this->_dso->wherecontain((object) $concat,$value,true);
			else
			{
				$this->_dso->wherecontain('firstname',$value,true,$this->_table['userfeatures']);
				$this->_dso->orwherecontain('lastname',$value,true,$this->_table['userfeatures']);
			}
			$this->_dso->orwhereexact('userfield',$value,true,$this->_table['userfeatures']);
			$this->_dso->orwherecontain('number',$value,true,$this->_table['linefeatures']);
		}

		$this->_dso->new_order($order,null,$this->_table['userfeatures']);
		$this->_dso->limit($limit);

		$r = $this->_dso->select_all(null,true,true);

		$this->_dso->reset_order();
		$this->_dso->reset_limit();

		if(($this->_cnt = $this->_dso->select_count(null,'id',$this->_table['userfeatures'])) === false
		|| isset($r[0]) === false)
			$r = false;

		return($r);
	}

	function search($value,$disable=null,$initialized=null,$order=null,$limit=null)
	{
		$this->_cnt = 0;

		if(dwho_has_len($value) === false)
			return(false);

		return($this->get_all($disable,$initialized,$order,$limit,$value));
	}

	function get_bsfilter($id,$bsfilter)
	{
		if(($id = dwho_ulongint($id)) === 0)
			return(false);

		$this->_dso->new_select($this->_table['userfeatures'],null);
		$this->_dso->where(array('id' => $id, 'bsfilter' => 'boss'), $this->_table['userfeatures']);
		$this->_dso->orwhere(array('id' => $id, 'bsfilter' => 'secretary'), $this->_table['userfeatures']);

		// NOTE: userfeatures.id is an integer, callfiltermember.typeval a string
		// we must cast the second to allow comparison (required by PostgreSQL)
		$this->_dso->innerjoin($this->_table['callfiltermember'],
					$this->_table['userfeatures'].'.id = '. $this->_dso->cast($this->_table['callfiltermember'].'.typeval', 'int'),
				       null,
				       'callfilterid');

		$this->_dso->joinwhere($this->_table['callfiltermember'],array('type' => 'user','bstype' => $bsfilter));
		$this->_dso->andjoinwhere($this->_table['callfiltermember'],
					  $this->_table['userfeatures'].'.bsfilter = '.$this->_table['callfiltermember'].'.bstype');

		$callfilterids = $this->_dso->select_singlecolumn();

		if(isset($callfilterids[0]) === false)
			return(false);

		$this->_dso->new_select($this->_table['callfiltermember'],null);
		$this->_dso->where(array('type' => 'user'),$this->_table['callfiltermember']);
		$this->_dso->andwherenot(array('bstype' => $bsfilter),$this->_table['callfiltermember']);

		$this->_dso->innerjoin($this->_table['userfeatures'],
				       $this->_dso->cast($this->_table['callfiltermember'].'.typeval','int').' = '.$this->_table['userfeatures'].'.id',
				       null,
				       '*');

		$this->_dso->andjoinwhere($this->_table['userfeatures'],
					  $this->_table['callfiltermember'].'.bstype = '.$this->_table['userfeatures'].'.bsfilter');
		//$this->_dso->andjoinwherenot($this->_table['userfeatures'],
		//			     'COALESCE('.$this->_table['userfeatures'].'.number,\'\') = \'\'');

		$this->_dso->innerjoin($this->_table['callfilter'],
				       $this->_table['callfiltermember'].'.callfilterid = '.$this->_table['callfilter'].'.id',
				       null,
				       array('callfilterid'	=> 'id',
					     'callfiltername'	=> 'name'));

		$this->_dso->joinwhere($this->_table['callfilter'],array('type' => 'bosssecretary'));
		$this->_dso->andjoinwhere($this->_table['callfilter'],$this->_table['callfilter'].'.bosssecretary IS NOT NULL');
		$this->_dso->andjoinwherein($this->_table['callfilter'],'id',$callfilterids);

		$r = $this->_dso->select_all();

		if(isset($r[0]) === false)
			$r = false;

		return($r);
	}

	function get_phonefunckey($user_id)
	{
		$this->_dso->new_select($this->_table['phonefunckey']);

		$this->_dso->where(array('iduserfeatures' => $user_id), $this->_table['phonefunckey']);

		$type = $this->_table['phonefunckey'].'.typeextenumbersright';
		$type_id = $this->_dso->cast($this->_table['phonefunckey'].'.typevalextenumbersright', 'int');

		$this->_dso->leftjoin($this->_table['agentfeatures'],
					$type_id.' = '.$this->_table['agentfeatures'].'.id',
					null,
					array('id','*'));
		$this->_dso->joinwhere($this->_table['agentfeatures'],
					$type.' = %s',
					'agent');

		$this->_dso->new_order(array('fknum' => SORT_ASC), null, $this->_table['phonefunckey']);

		$r = $this->_dso->select_all(null, true, true);

		if(isset($r[0]) === false)
			$r = false;

		return($r);
	}

	function get_phonefunckey_v2($template_id)
    {
		$query = <<<EOQ
		SELECT
			"func_key_mapping"."position"	AS "phonefunckey.fknum",
			"func_key_mapping"."blf"		AS "phonefunckey.supervision",
			"func_key_mapping"."label"		AS "phonefunckey.label",
			"userfeatures"."id"				AS "userfeatures.id",
			"userfeatures"."firstname"		AS "userfeatures.firstname",
			"userfeatures"."lastname"		AS "userfeatures.lastname",
			"groupfeatures"."id"			AS "groupfeatures.id",
			"groupfeatures"."name"			AS "groupfeatures.name",
			"queuefeatures"."id"			AS "queuefeatures.id",
			"queuefeatures"."name"			AS "queuefeatures.name",
			"meetmefeatures"."id"			AS "meetmefeatures.id",
			"meetmefeatures"."name"			AS "meetmefeatures.name",
			"agentfeatures"."id"			AS "agentfeatures.id",
			"agentfeatures"."firstname"		AS "agentfeatures.firstname",
			"agentfeatures"."lastname"		AS "agentfeatures.lastname",
			"paging"."id"					AS "paging.id",
			"paging"."number"				AS "paging.number",

			COALESCE(
				"func_key_dest_forward"."number",
				"func_key_dest_park_position"."park_position",
				"func_key_dest_custom"."exten"
			) AS "phonefunckey.exten",

			CASE
				WHEN "func_key_destination_type"."name" = 'service'
					THEN "service_extensions"."type"

				WHEN "func_key_destination_type"."name" = 'forward'
					THEN "fwd_extensions"."type"

				WHEN "func_key_destination_type"."name" = 'park_position'
					THEN 'generalfeatures'

				WHEN "func_key_destination_type"."name" = 'features'
					AND "func_key_type"."name" = 'forward'
					THEN 'generalfeatures'

				WHEN "func_key_destination_type"."name" = 'features'
					AND "func_key_type"."name" = 'dtmf'
					THEN 'featuremap'

				WHEN "func_key_destination_type"."name" = 'paging'
					THEN 'extenfeatures'

				WHEN "func_key_destination_type"."name" = 'agent'
					THEN 'extenfeatures'

				WHEN "func_key_destination_type"."name" = 'bsfilter'
					THEN 'extenfeatures'
			END AS "phonefunckey.typeextenumbers",

			CASE
				WHEN "func_key_destination_type"."name" = 'service'
					THEN "service_extensions"."typeval"

				WHEN "func_key_destination_type"."name" = 'forward'
					THEN "fwd_extensions"."typeval"

				WHEN "func_key_destination_type"."name" = 'park_position'
					THEN 'parkpos'

				WHEN "func_key_destination_type"."name" = 'features'
					THEN "features"."var_name"

				WHEN "func_key_destination_type"."name" = 'paging'
					THEN 'paging'

				WHEN "func_key_destination_type"."name" = 'agent'
					THEN "agent_extensions"."typeval"

				WHEN "func_key_destination_type"."name" = 'bsfilter'
					THEN 'bsfilter'
			END AS "phonefunckey.typevalextenumbers",

			CASE "func_key_destination_type"."name"
				WHEN 'user'			THEN 'user'
				WHEN 'group'		THEN 'group'
				WHEN 'queue'		THEN 'queue'
				WHEN 'conference'	THEN 'meetme'
				WHEN 'paging'		THEN 'paging'
				WHEN 'agent'		THEN 'agent'
				WHEN 'bsfilter'		THEN 'user'
			END AS "phonefunckey.typeextenumbersright",

			CASE
				WHEN "func_key_destination_type"."name" = 'user'
					THEN CAST("func_key_dest_user"."user_id" AS VARCHAR)

				WHEN "func_key_destination_type"."name" = 'group'
					THEN CAST("func_key_dest_group"."group_id" AS VARCHAR)

				WHEN "func_key_destination_type"."name" = 'queue'
					THEN CAST("func_key_dest_queue"."queue_id" AS VARCHAR)

				WHEN "func_key_destination_type"."name" = 'conference'
					THEN CAST("func_key_dest_conference"."conference_id" AS VARCHAR)

				WHEN "func_key_destination_type"."name" = 'paging'
					THEN CAST("func_key_dest_paging"."paging_id" AS VARCHAR)

				WHEN "func_key_destination_type"."name" = 'agent'
					THEN CAST("func_key_dest_agent"."agent_id" AS VARCHAR)

				WHEN "func_key_destination_type"."name" = 'bsfilter'
					AND "user_func_key"."id" = CAST("bossmember"."typeval" AS INTEGER)
					THEN "callfiltermember"."typeval"

				WHEN "func_key_destination_type"."name" = 'bsfilter'
					THEN "bossmember"."typeval"

				ELSE NULL
			END AS "phonefunckey.typevalextenumbersright"

		FROM "func_key_mapping"

		INNER JOIN "userfeatures" AS "user_func_key" ON "user_func_key"."func_key_private_template_id" = "func_key_mapping"."template_id"

		INNER JOIN "func_key" ON func_key.id = func_key_mapping.func_key_id
			INNER JOIN "func_key_destination_type" ON func_key_destination_type.id = func_key.destination_type_id
			INNER JOIN "func_key_type" ON func_key_type.id = func_key.type_id
			LEFT JOIN "func_key_dest_user" ON func_key_dest_user.func_key_id = func_key.id
				LEFT JOIN "userfeatures" ON func_key_dest_user.user_id = userfeatures.id
			LEFT JOIN "func_key_dest_group" ON func_key_dest_group.func_key_id = func_key.id
				LEFT JOIN "groupfeatures" ON func_key_dest_group.group_id = groupfeatures.id
			LEFT JOIN "func_key_dest_queue" ON func_key_dest_queue.func_key_id = func_key.id
				LEFT JOIN "queuefeatures" ON func_key_dest_queue.queue_id = queuefeatures.id
			LEFT JOIN "func_key_dest_conference" ON func_key_dest_conference.func_key_id = func_key.id
				LEFT JOIN "meetmefeatures" ON func_key_dest_conference.conference_id = meetmefeatures.id
			LEFT JOIN "func_key_dest_service" ON func_key_dest_service.func_key_id = func_key.id
				LEFT JOIN "extensions" AS "service_extensions" ON func_key_dest_service.extension_id = service_extensions.id
			LEFT JOIN "func_key_dest_forward" ON func_key_dest_forward.func_key_id = func_key.id
				LEFT JOIN "extensions" AS "fwd_extensions" ON func_key_dest_forward.extension_id = fwd_extensions.id
			LEFT JOIN "func_key_dest_park_position" ON func_key_dest_park_position.func_key_id = func_key.id
			LEFT JOIN "func_key_dest_features" ON func_key_dest_features.func_key_id = func_key.id
				LEFT JOIN "features" ON func_key_dest_features.features_id = features.id
			LEFT JOIN "func_key_dest_paging" ON func_key_dest_paging.func_key_id = func_key.id
				LEFT JOIN "paging" ON func_key_dest_paging.paging_id = paging.id
			LEFT JOIN "func_key_dest_custom" ON func_key_dest_custom.func_key_id = func_key.id
			LEFT JOIN "func_key_dest_agent" ON func_key_dest_agent.func_key_id = func_key.id
				LEFT JOIN "agentfeatures" ON agentfeatures.id = func_key_dest_agent.agent_id
				LEFT JOIN "extensions" AS "agent_extensions" ON agent_extensions.id = func_key_dest_agent.extension_id
			LEFT JOIN "func_key_dest_bsfilter" ON func_key_dest_bsfilter.func_key_id = func_key.id
				LEFT JOIN "callfiltermember" ON callfiltermember.id = func_key_dest_bsfilter.filtermember_id
				LEFT JOIN "callfiltermember" AS "bossmember" ON bossmember.bstype = 'boss' AND bossmember.callfilterid = callfiltermember.callfilterid
		WHERE
			"func_key_mapping"."template_id" = ${template_id}

		ORDER BY
			"func_key_mapping"."position" ASC
EOQ;
		$r = $this->_dso->select_all($query);

		if(isset($r[0]) === false)
			$r = false;
		else {
			foreach($r as &$row) {
				if($row['phonefunckey.typeextenumbersright'] == 'conference') {
					$row['phonefunckey.typeextenumbersright'] = 'meetme';
				}
			}
		}

		return($r);
	}
}

?>
