import 'angular-translate-loader-url';

export default function config($translateProvider, $logProvider, $locationProvider) {

  $logProvider.debugEnabled(true);

  $translateProvider.useSanitizeValueStrategy('escape');
  $translateProvider.useUrlLoader('/service/ipbx/json.php/public/i18n/translate/');
  $translateProvider.preferredLanguage(document.getElementsByTagName('html')[0].getAttribute('lang'));
}
