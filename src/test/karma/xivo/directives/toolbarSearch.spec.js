describe('toolbar-search directive', () => {
  var $compile;
  var $rootScope;
  var scope;

  beforeEach(angular.mock.module('karma-backend'));
  beforeEach(angular.mock.module('html-templates'));
  beforeEach(angular.mock.module('Xivo'));

  beforeEach(angular.mock.inject((_$compile_, _$rootScope_) =>{
    $compile = _$compile_;
    $rootScope = _$rootScope_;

    var elem = angular.element('<toolbar-search></toolbar-search>');
    scope = $rootScope.$new();
    $compile(elem)(scope);
    $rootScope.$digest();
  }));

  it('it can be instantiated', () => {
    expect(scope).toBeDefined();
  });

});
