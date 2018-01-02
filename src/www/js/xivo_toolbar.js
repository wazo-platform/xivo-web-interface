var xivo_toolbar_fn_adv_menu_enable = function(e)
{
    if(dwho_is_function(e.preventDefault) === true)
        e.preventDefault();

    dwho.fm[xivo_toolbar_form_name]['act'].value = 'enables';
    dwho.fm[xivo_toolbar_form_name].submit();
}

var xivo_toolbar_fn_adv_menu_disable = function(e)
{
    if(dwho_is_function(e.preventDefault) === true)
        e.preventDefault();

    dwho.fm[xivo_toolbar_form_name]['act'].value = 'disables';
    dwho.fm[xivo_toolbar_form_name].submit();
}

var xivo_toolbar_fn_adv_menu_delete = function(e)
{
    if(dwho_is_function(e.preventDefault) === true)
        e.preventDefault();

    if(confirm(xivo_toolbar_adv_menu_delete_confirm) === true)
    {
        if(dwho_is_undef(dwho.fm[xivo_toolbar_form_name]['search']) === false
        && typeof(xivo_toolbar_fm_search) !== 'undefined')
            dwho.fm[xivo_toolbar_form_name]['search'].value = xivo_toolbar_fm_search;

        dwho.fm[xivo_toolbar_form_name]['act'].value = 'deletes';
        dwho.fm[xivo_toolbar_form_name].submit();
    }
}

var xivo_toolbar_fn_adv_menu_delete_agents = function(e)
{
    if(dwho_is_function(e.preventDefault) === true)
        e.preventDefault();

    if(confirm(xivo_toolbar_adv_menu_delete_confirm) === true)
    {
        if(dwho_is_undef(dwho.fm[xivo_toolbar_form_name]['search']) === false
        && typeof(xivo_toolbar_fm_search) !== 'undefined')
            dwho.fm[xivo_toolbar_form_name]['search'].value = xivo_toolbar_fm_search;

        dwho.fm[xivo_toolbar_form_name]['act'].value = 'deleteagents';
        dwho.fm[xivo_toolbar_form_name].submit();
    }
}

var xivo_toolbar_init = function() {
  if(typeof(xivo_toolbar_fm_search) === 'undefined' || dwho_has_len(xivo_toolbar_fm_search) === false)
      dwho.form.set_events_text_helper('it-toolbar-search');

  dwho.dom.add_event('mouseover',
             dwho_eid('toolbar-bt-add'),
             function()
             {
              if((add_menu = dwho_eid('toolbar-add-menu')) !== false)
                  add_menu.style.display = 'block';
             });

  dwho.dom.add_event('mouseout',
             dwho_eid('toolbar-bt-add'),
             function()
             {
              if((add_menu = dwho_eid('toolbar-add-menu')) !== false)
                  add_menu.style.display = 'none';
             });

  dwho.dom.add_event('mouseover',
             dwho_eid('toolbar-add-menu'),
             function()
             {
              this.style.display = 'block';
             });

  dwho.dom.add_event('mouseout',
             dwho_eid('toolbar-add-menu'),
             function()
             {
              this.style.display = 'none';
             });

  dwho.dom.add_event('mouseover',
             dwho_eid('toolbar-bt-advanced'),
             function()
             {
              if((advanced_menu = dwho_eid('toolbar-advanced-menu')) !== false)
                  advanced_menu.style.display = 'block';
             });

  dwho.dom.add_event('mouseout',
             dwho_eid('toolbar-bt-advanced'),
             function()
             {
              if((advanced_menu = dwho_eid('toolbar-advanced-menu')) !== false)
                  advanced_menu.style.display = 'none';
             });

  dwho.dom.add_event('mouseover',
             dwho_eid('toolbar-advanced-menu'),
             function()
             {
              this.style.display = 'block';
             });

  dwho.dom.add_event('mouseout',
             dwho_eid('toolbar-advanced-menu'),
             function()
             {
              this.style.display = 'none';
             });

  dwho.dom.add_event('click',
             dwho_eid('toolbar-advanced-menu-enable'),
             xivo_toolbar_fn_adv_menu_enable);

  dwho.dom.add_event('click',
             dwho_eid('toolbar-advanced-menu-disable'),
             xivo_toolbar_fn_adv_menu_disable);

  dwho.dom.add_event('click',
             dwho_eid('toolbar-advanced-menu-select-all'),
             function(e)
             {
              if(dwho_is_function(e.preventDefault) === true)
                  e.preventDefault();

              dwho.form.checked_all(xivo_toolbar_form_name,
                          xivo_toolbar_form_list);
             });

  dwho.dom.add_event('click',
             dwho_eid('toolbar-advanced-menu-delete'),
             xivo_toolbar_fn_adv_menu_delete);
  dwho.dom.add_event('click',
             dwho_eid('toolbar-advanced-menu-delete-agents'),
             xivo_toolbar_fn_adv_menu_delete_agents);
}

var xivo_toolbar_init_adv_delete = function(withContext, withDir, withAutoProv)
{
	dwho.dom.remove_event('click',
    dwho_eid('toolbar-advanced-menu-delete'),
    xivo_toolbar_fn_adv_menu_delete);

	dwho.dom.add_event('click',
   dwho_eid('toolbar-advanced-menu-delete'),
   function(e)
   {
		 if(dwho_is_function(e.preventDefault) === true)
			e.preventDefault();

			if(confirm(xivo_toolbar_adv_menu_delete_confirm) === true)
			{
        xivo_toolbar_init_form_item('search', xivo_toolbar_fm_search);

        if (withContext) {
          xivo_toolbar_init_form_item('context', xivo_toolbar_fm_context);
        }
        if (withDir) {
          xivo_toolbar_init_form_item('dir', xivo_toolbar_fm_dir);
        }
        if (withAutoProv) {
          xivo_toolbar_init_form_item('linked', xivo_toolbar_fm_linked);
        }

				dwho.fm[xivo_toolbar_form_name]['act'].value = 'deletes';
				dwho.fm[xivo_toolbar_form_name].submit();
			}
	 });

   if (withAutoProv) {
     dwho.dom.add_event('click', dwho_eid('toolbar-advanced-menu-autoprov'),
     function(e) {
       dialog = document.getElementById('autoprov_dialog');
       dialog.style.visibility= 'visible';
     });
   }
}

var xivo_toolbar_init_form_item = function(item, value) {
  if(dwho_is_undef(dwho.fm[xivo_toolbar_form_name][item]) === false)
    dwho.fm[xivo_toolbar_form_name][item].value = value;
}

var xivo_toolbar_init_toolbar_change = function(item, hasDir)
{
	dwho.dom.add_event('change',
			   dwho_eid(item),
			   function(e)
			   {
				if(xivo_toolbar_fm_search === ''
				&& dwho_has_len(dwho.form.text_helper['it-toolbar-search']) === false)
					this.form['search'].value = '';

        if (hasDir) {
          if (this.form['act'].value === 'list') {
            this.form['search'].value = '';
          }

          if(this.value === '') {
            this.form['act'].value += 'dir';
          }
        }

				this.form.submit();
			   });
}

function autoprov_cancel()
{
	dialog = document.getElementById('autoprov_dialog');
	dialog.style.visibility= 'hidden';

	return false;
}

function autoprov_validate()
{
	dialog = document.getElementById('autoprov_dialog');
	dialog.style.visibility= 'hidden';

	dwho.fm[xivo_toolbar_form_name]['act'].value = 'mass_synchronize';
	dwho.fm[xivo_toolbar_form_name].submit();

	return false;
}
