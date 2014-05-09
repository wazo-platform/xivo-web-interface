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
				'func_key_destination_type'	=> '',
				'func_key_mapping'	=> '',
				'func_key_template'	=> '',
				'func_key_type'		=> '');

	function get_nb($arr=null,$disable=null,$initialized=null)
	{
		if(is_array($arr) === false)
			$arr = array();

		$this->_dso->new_select($this->_table['userfeatures']);

		if($disable !== null)
			$arr['commented'] = intval((bool) $disable);

		$this->_dso->where($arr,$this->_table['userfeatures']);

		return(dwho_ulongint($this->_dso->select_count(null,'id',$this->_table['userfeatures'])));
	}

	function get_all($disable=null,$initialized=null,$order=null,$limit=null,$value=null)
	{
		$this->_cnt = 0;

		$arr = array();

		$this->_dso->new_select($this->_table['userfeatures']);

		#TODO: entity_filter
		#if (isset($_SESSION["_USR"]->entity_id) && $_SESSION["_USR"]->entity_id !== 0)
		#	$arr['entityid'] = $_SESSION["_USR"]->entity_id;

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

		$this->_dso->leftjoin($this->_table['groupfeatures'],
					$type_id.' = '.$this->_table['groupfeatures'].'.id',
					null,
					array('id','*'));
		$this->_dso->joinwhere($this->_table['groupfeatures'],
					 $type.' = %s',
					'group');
		$this->_dso->andjoinwhere($this->_table['groupfeatures'],
					array('deleted' => 0));

		$this->_dso->leftjoin($this->_table['queuefeatures'],
					$type_id.' = '.$this->_table['queuefeatures'].'.id',
					null,
					array('id','*'));
		$this->_dso->joinwhere($this->_table['queuefeatures'],
					$type.' = %s',
					'queue');

		$this->_dso->leftjoin($this->_table['meetmefeatures'],
					$type_id.' = '.$this->_table['meetmefeatures'].'.id',
					null,
					array('id','*'));
		$this->_dso->joinwhere($this->_table['meetmefeatures'],
					$type.' = %s',
					'meetme');

		$this->_dso->new_order(array('fknum' => SORT_ASC), null, $this->_table['phonefunckey']);

		$r = $this->_dso->select_all(null, true, true);

		if(isset($r[0]) === false)
			$r = false;

		return($r);
	}

	function get_phonefunckey_v2($template_id)
	{
		$this->_dso->new_select(
			 $this->_table['func_key_mapping'],
			array(
				'phonefunckey.fknum'		=> 'position',
				'phonefunckey.supervision'	=> 'blf',
				'phonefunckey.label'		=> 'label'
			)
		);
		$this->_dso->where(array('template_id' => $template_id), $this->_table['func_key_mapping']);

		$this->_dso->innerjoin($this->_table['func_key'],
					$this->_table['func_key'].'.id = '.$this->_table['func_key_mapping'].'.func_key_id',
					null,
					array('*'));

		$this->_dso->innerjoin($this->_table['func_key_destination_type'],
					$this->_table['func_key_destination_type'].'.id = '.$this->_table['func_key'].'.destination_type_id',
					null,
					array('phonefunckey.typeextenumbersright' => 'name'));

		# USER FUNC_KEY
		$this->_dso->leftjoin($this->_table['func_key_dest_user'],
					$this->_table['func_key_dest_user'].'.func_key_id = '.$this->_table['func_key'].'.id',
					null,
					array('phonefunckey.typevalextenumbersright' => 'user_id'));
		$this->_dso->leftjoin($this->_table['userfeatures'],
					$this->_table['func_key_dest_user'].'.user_id'.' = '.$this->_table['userfeatures'].'.id',
					null,
					array('*'));

		# GROUP FUNC_KEY
		$this->_dso->leftjoin($this->_table['func_key_dest_group'],
					$this->_table['func_key_dest_group'].'.func_key_id = '.$this->_table['func_key'].'.id',
					null,
					array('phonefunckey.typevalextenumbersright' => 'group_id'));
		$this->_dso->leftjoin($this->_table['groupfeatures'],
					$this->_table['func_key_dest_group'].'.group_id'.' = '.$this->_table['groupfeatures'].'.id',
					null,
					array('*'));

		# QUEUE FUNC_KEY
		$this->_dso->leftjoin($this->_table['func_key_dest_queue'],
					$this->_table['func_key_dest_queue'].'.func_key_id = '.$this->_table['func_key'].'.id',
					null,
					array('phonefunckey.typevalextenumbersright' => 'queue_id'));
		$this->_dso->leftjoin($this->_table['queuefeatures'],
					$this->_table['func_key_dest_queue'].'.queue_id'.' = '.$this->_table['queuefeatures'].'.id',
					null,
					array('*'));

		# CONFERENCE FUNC_KEY
		$this->_dso->leftjoin($this->_table['func_key_dest_conference'],
					$this->_table['func_key_dest_conference'].'.func_key_id = '.$this->_table['func_key'].'.id',
					null,
					array('phonefunckey.typevalextenumbersright' => 'conference_id'));
		$this->_dso->leftjoin($this->_table['meetmefeatures'],
					$this->_table['func_key_dest_conference'].'.conference_id'.' = '.$this->_table['meetmefeatures'].'.id',
					null,
					array('*'));

		$this->_dso->new_order(array('position' => SORT_ASC), null, $this->_table['func_key_mapping']);

		$r = $this->_dso->select_all(null, true, true);

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
