import 'angular-translate-loader-partial';

export default function config($translateProvider, $translatePartialLoaderProvider, $logProvider) {

  $logProvider.debugEnabled(true);

  $translateProvider.useSanitizeValueStrategy('escape');
  $translatePartialLoaderProvider.addPart('xivo');
  $translateProvider.useLoader('$translatePartialLoader', {
    urlTemplate: '/i18n/{part}-{lang}.json'
  });
  $translateProvider.registerAvailableLanguageKeys(['en','fr'], {
    'en_*': 'en',
    'fr_*': 'fr'
  });
  $translateProvider.preferredLanguage(document.getElementsByTagName('html')[0].getAttribute('lang'));
  $translateProvider.fallbackLanguage('fr');
  $translateProvider.forceAsyncReload(true);
}
