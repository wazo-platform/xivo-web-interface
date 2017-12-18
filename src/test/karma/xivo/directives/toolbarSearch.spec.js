describe('toolbar-search directive', () => {
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

    let elem = angular.element('<toolbar-search></toolbar-search>');
    scope = $rootScope.$new();

    let elemCompiled = $compile(elem)(scope);
    $rootScope.$digest();

    isolatedScope =  elemCompiled.isolateScope();
    isolatedScope.displayOn = 'list';
  }));

  it('it can be instantiated', () => {
    expect(isolatedScope).toBeDefined();
  });

  it('checks if must be displayed', () => {
    spyOn(toolbar, 'isDisplayed');
    isolatedScope.isDisplayed();
    expect(toolbar.isDisplayed).toHaveBeenCalledWith('','list');
  });

});
