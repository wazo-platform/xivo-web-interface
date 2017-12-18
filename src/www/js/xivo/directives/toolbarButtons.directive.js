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

      scope.isListDisplayed = () => {
        return toolbar.isDisplayed($window.location.search, scope.displayAdvOn);
      };

      scope.getLabelKey = (action) => {
        return toolbar.getLabelKey(action);
      };

      scope.$on('ngRepeatActionsAdvFinished', () => {
        // register to outer dwho existing implementation
        // when DOM is ready
        toolbar.registerDwho();
      });
    }
  };
}
