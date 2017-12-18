describe('toolbar service', () => {
  var $rootScope;
  var toolbar

  beforeEach(angular.mock.module('karma-backend'));
  beforeEach(angular.mock.module('html-templates'));
  beforeEach(angular.mock.module('Xivo'));

  beforeEach(angular.mock.inject((_$rootScope_, _toolbar_) =>{
    $rootScope = _$rootScope_;
    toolbar = _toolbar_;
  }));

  it('retrieves url parameters from window.location.search', () => {
    expect(toolbar.parseParams("?param1=value1&param2=value2")).toEqual({param1:'value1', param2:'value2' });
  });

});
