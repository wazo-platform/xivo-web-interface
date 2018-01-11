export default function breadcrumb() {
  return {
    restrict: 'E',
    templateUrl: '/js/xivo/directives/breadcrumb.html',
    scope: {
      page: '@',
      parent: '@',
      value: '@',
      redirect: '@'
    },
    link: (scope) => {
      scope.redirectParent = () => {
        return (scope.redirect) ? scope.redirect : './?act=list';
      };
    }
  };
}
