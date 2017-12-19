describe('toolbar-buttons directive', () => {
  var $compile;
  var $rootScope;
  var scope;
  var isolatedScope;
  var toolbar;

  beforeEach(angular.mock.module('karma-backend'));
  beforeEach(angular.mock.module('html-templates'));
  beforeEach(angular.mock.module('Xivo'));

  beforeEach(angular.mock.inject((_$compile_, _$rootScope_, _toolbar_) =>{
    $compile = _$compile_;
    $rootScope = _$rootScope_;
    toolbar = _toolbar_;

    let elem = angular.element('<toolbar-buttons></toolbar-buttons>');
    scope = $rootScope.$new();

    let elemCompiled = $compile(elem)(scope);
    $rootScope.$digest();

    isolatedScope =  elemCompiled.isolateScope();
    isolatedScope.displayAdvOn = 'list';
  }));

  it('it can be instantiated', () => {
    expect(isolatedScope).toBeDefined();
  });

  it('checks if must be displayed', () => {
    spyOn(toolbar, 'isDisplayed');
    isolatedScope.isListDisplayed();
    expect(toolbar.isDisplayed).toHaveBeenCalledWith('','list');
  });

  it('gets translation key', () => {
    spyOn(toolbar, 'getLabelKey');
    isolatedScope.getLabelKey('add');
    expect(toolbar.getLabelKey).toHaveBeenCalledWith('add');
  });

  it('builds params list without act param in it', () => {
    spyOn(toolbar, 'parseParams').and.returnValue({act:'list', group: 3, something: 'else'});
    let searchPath = "?act=list&group=3&something=else";
    expect(isolatedScope.getOtherParams()).toBe('&group=3&something=else');
  });

});
