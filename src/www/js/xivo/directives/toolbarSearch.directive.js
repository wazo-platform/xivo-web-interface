export default function toolbarSearch($window, toolbar) {

  return {
    restrict: 'E',
    templateUrl: '/js/xivo/directives/toolbar-search.html',
    scope: {
      displayOn: '@',
    },
    link: (scope) => {

      scope.isDisplayed = () => {
        return toolbar.isDisplayed($window.location.search, scope.displayOn);
      };
    }
  };
}
