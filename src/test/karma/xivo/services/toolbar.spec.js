describe('toolbar service', () => {
  var $window;
  var toolbar;

  beforeEach(angular.mock.module('karma-backend'));
  beforeEach(angular.mock.module('html-templates'));
  beforeEach(angular.mock.module('Xivo'));

  beforeEach(angular.mock.inject((_toolbar_, _$window_) =>{
    $window = _$window_;
    toolbar = _toolbar_;
    spyOn($window, 'xivo_toolbar_init');
    spyOn($window, 'init_update_plugin');
  }));

  it('init dwho implementation when instantiated', () => {
    toolbar.registerDwho();
    expect($window.xivo_toolbar_init).toHaveBeenCalled();
  });


  it('retrieves url parameters from window.location.search', () => {
    expect(toolbar.parseParams("?param1=value1&param2=value2")).toEqual({param1:'value1', param2:'value2' });
  });

  it('checks if we are listing entities', () => {
    expect(toolbar.isDisplayed("?act=list", "list")).toBe(true);
  });

  it('checks if we are editing entity', () => {
    expect(toolbar.isDisplayed("?act=edit", "list")).toBe(false);
  });

  it('returns PHP label key for an input if exist', () => {
    expect(toolbar.getLabelKey("fakePHPBundle")).toBeUndefined();
    expect(toolbar.getLabelKey("add")).toBe('toolbar_add_menu_add');
    expect(toolbar.getLabelKey("add", "musiconhold")).toBe('toolbar_adv_menu_add-category');
  });

  it('updates plugins', () => {
    toolbar.updatePlugins();
    expect($window.init_update_plugin).toHaveBeenCalled();
  });

});
