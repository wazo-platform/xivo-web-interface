describe('toolbar-buttons directive', () => {
  var $compile;
  var $rootScope;
  var scope;
  var toolbar;

  beforeEach(angular.mock.module('karma-backend'));
  beforeEach(angular.mock.module('html-templates'));
  beforeEach(angular.mock.module('Xivo'));

  beforeEach(angular.mock.inject((_$compile_, _$rootScope_, _toolbar_) =>{
    $compile = _$compile_;
    $rootScope = _$rootScope_;
    toolbar = _toolbar_;

    var elem = angular.element('<toolbar-buttons></toolbar-buttons>');
    scope = $rootScope.$new();
    $compile(elem)(scope);
    spyOn(window, 'xivo_toolbar_init');
    $rootScope.$digest();
  }));

  it('it can be instantiated', () => {
    expect(scope).toBeDefined();
  });

  it('init dwho implementation when instantiated', () => {
    expect(window.xivo_toolbar_init).toHaveBeenCalled();
  });

  it('checks if we are listing entities', () => {
    spyOn(toolbar, 'parseParams').and.returnValue({act: 'list'});
    expect(scope.isList()).toBe(true);
  });

  it('checks if we are editing entity', () => {
    spyOn(toolbar, 'parseParams').and.returnValue({act: 'edit'});
    expect(scope.isList()).toBe(false);
  });
});
