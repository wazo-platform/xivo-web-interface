export default function toolbarButtons($window, toolbar) {

  return {
    restrict: 'E',
    templateUrl: '/js/xivo/directives/toolbar-buttons.html',
    scope: {
      displayAdvOn: '@',
      actions: '=',
      actionsAdv: '='
    },
    link: (scope) => {
      // register to outer dwho existing implementation
      toolbar.registerDwho();

      scope.isListDisplayed = () => {
        return toolbar.isDisplayed($window.location.search, scope.displayAdvOn);
      };

      scope.getLabelKey = (action) => {
        return toolbar.getLabelKey(action);
      };
    }
  };
}
