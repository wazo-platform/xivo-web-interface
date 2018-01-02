import _ from 'lodash';

export default function toolbarButtons($window, toolbar) {

  return {
    restrict: 'E',
    templateUrl: '/js/xivo/directives/toolbar-buttons.html',
    scope: {
      displayAdvOn: '@',
      page: '@',
      actions: '=',
      actionsAdv: '=',
      plugins: '='
    },
    link: (scope) => {
      scope.page = angular.isDefined(scope.page) ? scope.page : 'generic';

      scope.getOtherParams = () => {
        let params = _.omit(toolbar.parseParams($window.location.search), 'act');
        return  _.reduce(params, function(result, value, key) {
          return (!_.isUndefined(value)) ? result += '&' + key + '=' + value : result;
        }, '');
      };

      scope.isListDisplayed = () => {
        return toolbar.isDisplayed($window.location.search, scope.displayAdvOn);
      };

      scope.getLabelKey = (action, page) => {
        return toolbar.getLabelKey(action, page);
      };

      scope.updatePlugins = () => {
        toolbar.updatePlugins();
      };

      scope.$on(scope.page+'ActionsLoaded', () => {
        toolbar.registerDwho(scope.page);
      });
    }
  };
}
