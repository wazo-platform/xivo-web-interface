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

class xivo_application_service_asterisk_line_sql extends xivo_application_abstract_datastorage_sql
{
	public $_table	= array(
				'linefeatures' 	=> '',
				'userfeatures' 	=> '',
				'usersip'		=> '',
				'sccpline'		=> '',
				'usercustom'	=> '',
				'user_line'		=> '');

	public function get_nb($arr=null,$disable=null,$initialized=null)
	{
		if(is_array($arr) === false)
			$arr = array();

		$this->_dso->new_select($this->_table['linefeatures']);
		$this->_dso->leftjoin($this->_table['usersip'],
				      $this->_table['linefeatures'].'.protocolid = '.$this->_table['usersip'].'.id');
		$this->_dso->leftjoin($this->_table['sccpline'],
				      $this->_table['linefeatures'].'.protocolid = '.$this->_table['sccpline'].'.id');
		$this->_dso->leftjoin($this->_table['usercustom'],
				      $this->_table['linefeatures'].'.protocolid = '.$this->_table['usercustom'].'.id');

		$this->_dso->where($this->_table['usersip'].'.id IS NOT NULL');
		$this->_dso->orwhere($this->_table['sccpline'].'.id IS NOT NULL');
		$this->_dso->orwhere($this->_table['usercustom'].'.id IS NOT NULL');

		$this->_dso->aggregate_where();

		$sip = $sccp = $custom = array();
		$sip['category'] = $custom['category'] = 'user';

		if($disable !== null)
		{
			$disable = intval((bool) $disable);
			$arr['commented'] = $disable;
			$sip['commented'] = $disable;
			$sccp['commented'] = $disable;
			$custom['commented'] = $disable;
		}

		if (defined('ENTITY_FILTER_CONTEXT'))
			$this->_dso->wherein('context', explode(',', ENTITY_FILTER_CONTEXT), $this->_table['linefeatures']);

		$this->_dso->andwhere($arr,$this->_table['linefeatures']);
		$this->_dso->joinwhere($this->_table['usersip'] ,$sip);
		$this->_dso->joinwhere($this->_table['sccpline'] ,$sccp);
		$this->_dso->joinwhere($this->_table['usercustom'],$custom);

		$this->_dso->andjoinwhere($this->_table['usersip'],
					  $this->_table['linefeatures'].'.protocol = '.$this->_table['usersip'].'.protocol');

		$this->_dso->andjoinwhere($this->_table['sccpline'],
					  $this->_table['linefeatures'].'.protocol = '.$this->_table['sccpline'].'.protocol');

		$this->_dso->andjoinwhere($this->_table['usercustom'],
					  $this->_table['linefeatures'].'.protocol = '.$this->_table['usercustom'].'.protocol');

		if($initialized !== null)
		{
			if((bool) $initialized === false)
				$this->_dso->andjoinwhere($this->_table['usersip'],
							  'COALESCE('.$this->_table['usersip'].'.fullcontact,\'\') = \'\'');
			else
				$this->_dso->andjoinwherenot($this->_table['usersip'],
							  'COALESCE('.$this->_table['usersip'].'.fullcontact,\'\') = \'\'');
		}

		return(dwho_ulongint($this->_dso->select_count(null,'id',$this->_table['linefeatures'])));
	}

	public function get_all(
			$where=array(),
			$disable=null,
			$order=null,
			$limit=null,
			$value=null)
	{
		$this->_cnt = 0;
		$arr = array();

		$this->_dso->new_select($this->_table['linefeatures']);

		$initialized = null;
		if(isset($where['initialized']) === true)
			$initialized = $where['initialized'];

		$context = null;
		if(isset($where['context']) === true)
			$context = $where['context'];

		if(dwho_issa('protocols', $where) === false)
			$use_sip = $use_sccp = $use_custom = true;
		else
		{
			$protocols = $where['procotol'];
			$use_sip  = in_array(XIVO_SRE_IPBX_AST_PROTO_SIP, $protocols);
			$use_sccp  = in_array(XIVO_SRE_IPBX_AST_PROTO_SCCP, $protocols);
			$use_custom = in_array(XIVO_SRE_IPBX_AST_PROTO_CUSTOM, $protocols);
			$this->_dso->wherein('protocol',$protocols,$this->_table['linefeatures']);
		}

		if($use_sip === false && $use_sccp === false && $use_custom === false)
			return(false);

		$sip = $sccp = $custom = array();
		$sip['category'] = $custom['category'] = 'user';

		if($disable !== null)
		{
			$disable = intval((bool) $disable);
			$arr['commented'] = $disable;
			$sip['commented'] = $disable;
			$sccp['commented'] = $disable;
			$custom['commented'] = $disable;
		}

		if (defined('ENTITY_FILTER_CONTEXT'))
			$this->_dso->wherein('context', explode(',', ENTITY_FILTER_CONTEXT), $this->_table['linefeatures']);

		$this->_dso->andwhere($arr,$this->_table['linefeatures']);

		$this->_dso->leftjoin($this->_table['user_line'], $this->_table['user_line'].'.line_id = '. $this->_table['linefeatures'].'.id');

		$this->_dso->leftjoin($this->_table['userfeatures'],
						$this->_table['user_line'].'.user_id = '.$this->_table['userfeatures'].'.id',
						null,
						array('firstname', 'lastname'));

		if($use_sip === true)
		{
			$this->_dso->leftjoin($this->_table['usersip'],
					      $this->_table['linefeatures'].'.protocolid = '.$this->_table['usersip'].'.id',
					      null,
					      '*');

			$this->_dso->joinwhere($this->_table['usersip'],$sip);

			$this->_dso->andjoinwhere($this->_table['usersip'],
						  $this->_table['linefeatures'].'.protocol = '.$this->_table['usersip'].'.protocol');

			if($initialized !== null)
			{
				if((bool) $initialized === false)
					$this->_dso->andjoinwhere($this->_table['usersip'],
								  'COALESCE('.$this->_table['usersip'].'.fullcontact,\'\') = \'\'');
				else
					$this->_dso->andjoinwherenot($this->_table['usersip'],
								  'COALESCE('.$this->_table['usersip'].'.fullcontact,\'\') = \'\'');
			}
		}

		if($use_sccp === true)
		{
			$this->_dso->leftjoin($this->_table['sccpline'],
					      $this->_table['linefeatures'].'.protocolid = '.$this->_table['sccpline'].'.id',
					      null,
					      '*');
		}

		if($use_custom === true)
		{
			$this->_dso->leftjoin($this->_table['usercustom'],
					      $this->_table['linefeatures'].'.protocolid = '.$this->_table['usercustom'].'.id',
					      null,
					      'interface');

			$this->_dso->joinwhere($this->_table['usercustom'],$custom);

			$this->_dso->andjoinwhere($this->_table['usercustom'],
						  $this->_table['linefeatures'].'.protocol = '.$this->_table['usercustom'].'.protocol');
		}

		$this->_dso->aggregate_where();

		if (is_array($context) === true
		&& empty($context) === false)
			$this->_dso->wherein('context',$context,$this->_table['linefeatures']);
		elseif(dwho_has_len($context) === true)
			$this->_dso->where($this->_table['linefeatures'].'.context = %s',$context);

		if(dwho_has_len($value) === true)
		{
			$this->_dso->aggregate_where();
			$this->_dso->wherecontain('number',$value,true,$this->_table['linefeatures']);
			$this->_dso->orwherecontain('name',$value,true,$this->_table['linefeatures']);
		}

		$this->_dso->aggregate_where();

		if($use_sip === true)
			$this->_dso->orwhere($this->_table['usersip'].'.id IS NOT NULL');

		if($use_sccp === true)
			$this->_dso->orwhere($this->_table['sccpline'].'.id IS NOT NULL');

		if($use_custom === true)
			$this->_dso->orwhere($this->_table['usercustom'].'.id IS NOT NULL');

		$this->_dso->aggregate_where(false,'AND');

		$this->_dso->new_order($order,null,$this->_table['linefeatures']);
		$this->_dso->limit($limit);

		$r = $this->_dso->select_all(null,true,true);

		$this->_dso->reset_order();
		$this->_dso->reset_limit();

		if(($this->_cnt = $this->_dso->select_count(null,'id',$this->_table['linefeatures'])) === false
		|| isset($r[0]) === false)
			$r = false;

		return($r);
	}

	public function search($value,$where=array(),$disable=null,$order=null,$limit=null)
	{
		$this->_cnt = 0;

		if(dwho_has_len($value) === false)
			return(false);

		return($this->get_all($where,$disable,$order,$limit,$value));
	}
}

?>
