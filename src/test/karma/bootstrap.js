var karmaBackend = angular.module('karma-backend', ['ngMockE2E']);

karmaBackend.run(function($httpBackend) {
  $httpBackend.whenGET('/service/ipbx/json.php/public/i18n/translate/').respond('');
  $httpBackend.whenGET(/^i18n\//).passThrough();
});


// DWHO specific mocks
var dwho_form_class_error	= 'fm-error';
