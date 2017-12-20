export default function toolbar($window) {

  var searchValue;

  const _registerDwho = (page) => {
    $window.xivo_toolbar_init();
    switch (page) {
    case 'queueskillrules':
    case 'users': $window.xivo_toolbar_init_adv_delete();
      break;
    case 'configuration':
      $window.xivo_toolbar_init_adv_delete(true);
      break;
    case 'queueskills':
      $window.xivo_toolbar_init_adv_delete(true);
      $window.xivo_toolbar_init_toolbar_change('it-toolbar-context');
      break;
    case 'agents': $window.xivo_toolbar_init_toolbar_change('it-toolbar-linked');
      break;
    }

    searchValue = $window.xivo_toolbar_fm_search;
  };

  const _updatePlugins = () => {
    $window.init_update_plugin();
  };

  const _parseParams = (search) => {
    return (search).replace(/(^\?)/,'').split("&").reduce((p,n) => {
      return n = n.split("="), p[n[0]] = n[1], p;
    }, {});
  };

  const _isDisplayed = (searchParams, value) => {
    let params = _parseParams(searchParams);
    return params.act === value;
  };

  const _getSearchValue = () => {
    return searchValue;
  };

  const _getLabelKey = (input, page) => {
    switch(input) {
    case 'add':
      switch (page) {
      case 'agents': return 'toolbar_adv_menu_add-group';
      case 'musiconhold': return 'toolbar_adv_menu_add-category';
      default: return 'toolbar_add_menu_add';}
    case 'addagent' : return 'toolbar_adv_menu_add-agent';
    case 'addfile' : return 'toolbar_adv_menu_add-file';
    case 'import': return 'toolbar_add_menu_import-file';
    case 'update_import': return 'toolbar_add_menu_update_import';
    case 'export': return 'toolbar_add_menu_export';
    case 'toolbar-advanced-menu-enable' : return 'toolbar_adv_menu_enable';
    case 'toolbar-advanced-menu-disable': return 'toolbar_adv_menu_disable';
    case 'toolbar-advanced-menu-select-all' : return 'toolbar_adv_menu_select-all';
    case 'toolbar-advanced-menu-delete' :
    case 'toolbar-advanced-menu-delete-agents' : return 'toolbar_adv_menu_delete'; }
  };

  return {
    registerDwho : _registerDwho,
    parseParams : _parseParams,
    isDisplayed : _isDisplayed,
    getLabelKey : _getLabelKey,
    getSearchValue : _getSearchValue,
    updatePlugins : _updatePlugins
  };
}
