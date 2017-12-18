export default function toolbar() {

  const _registerDwho = () => {
    /* eslint-disable */
    xivo_toolbar_init();
    /* eslint-enable */
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

  const _getLabelKey = (input) => {
    switch(input) {
    case 'add': return 'toolbar_add_menu_add';
    case 'import': return 'toolbar_add_menu_import-file';
    case 'update_import': return 'toolbar_add_menu_update_import';
    case 'export': return 'toolbar_add_menu_export';
    case 'toolbar-advanced-menu-enable' : return 'toolbar_adv_menu_enable';
    case 'toolbar-advanced-menu-disable': return 'toolbar_adv_menu_disable';
    case 'toolbar-advanced-menu-select-all' : return 'toolbar_adv_menu_select';
    case 'toolbar-advanced-menu-delete' : return 'toolbar_adv_menu_delete'; }
  };

  return {
    registerDwho : _registerDwho,
    parseParams : _parseParams,
    isDisplayed : _isDisplayed,
    getLabelKey : _getLabelKey
  };
}
