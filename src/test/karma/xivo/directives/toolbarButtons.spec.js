describe('toolbar-buttons directive', () => {
  var $compile;
  var $rootScope;
  var scope;

  beforeEach(angular.mock.module('karma-backend'));
  beforeEach(angular.mock.module('html-templates'));
  beforeEach(angular.mock.module('Xivo'));

  beforeEach(angular.mock.inject((_$compile_, _$rootScope_) =>{
    $compile = _$compile_;
    $rootScope = _$rootScope_;

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

  it('retrieves url parameters from window.location.search', () => {
    expect(scope.parseParams("?param1=value1&param2=value2")).toEqual({param1:'value1', param2:'value2' });
  });

  it('checks if we are listing entities', () => {
    spyOn(scope, 'parseParams').and.returnValue({act: 'list'});
    expect(scope.isList()).toBe(true);
  });

  it('checks if we are editing entity', () => {
    spyOn(scope, 'parseParams').and.returnValue({act: 'edit'});
    expect(scope.isList()).toBe(false);
  });
});
