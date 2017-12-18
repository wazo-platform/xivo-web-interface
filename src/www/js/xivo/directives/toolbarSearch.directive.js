export default function toolbarSearch($window, toolbar) {

  return {
    restrict: 'E',
    templateUrl: '/js/xivo/directives/toolbar-search.html',
    scope: {
      displayOn: '@'
    },
    link: (scope) => {

      scope.getSearchValue = () => {
        return toolbar.getSearchValue();
      };

      scope.isDisplayed = () => {
        if (angular.isUndefined(scope.displayOn)) {
          return true;
        }
        else {
          return toolbar.isDisplayed($window.location.search, scope.displayOn);
        }
      };
    }
  };
}
