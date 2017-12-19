export default function toolbarButtons($window, toolbar) {

  return {
    restrict: 'E',
    templateUrl: '/js/xivo/directives/toolbar-buttons.html',
    scope: {
      displayAdvOn: '@',
      page: '@',
      actions: '=',
      actionsAdv: '='
    },
    link: (scope) => {

      scope.isListDisplayed = () => {
        return toolbar.isDisplayed($window.location.search, scope.displayAdvOn);
      };

      scope.getLabelKey = (action) => {
        return toolbar.getLabelKey(action);
      };

      scope.$on(scope.page+'Actions', () => {
        toolbar.registerDwho(scope.page);
      });
    }
  };
}
