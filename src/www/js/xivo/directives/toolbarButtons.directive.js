export default function toolbarButtons($location) {

  return {
    restrict: 'E',
    templateUrl: '/js/xivo/directives/toolbar-buttons.html',
    link: (scope) => {

      scope.init = () => {
        // call to outer dwho existing implementation
        /* eslint-disable */
        xivo_toolbar_init();
        /* eslint-enable */
      };

      scope.isList = () => {
        return $location.search().act === 'list';
      };

      scope.init();
    }
  };
}
