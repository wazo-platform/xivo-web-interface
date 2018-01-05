export default function breadcrumb() {
  return {
    restrict: 'E',
    templateUrl: '/js/xivo/directives/breadcrumb.html',
    scope: {
      page: '@',
      parent: '@',
      value: '@'
    }
  };
}
