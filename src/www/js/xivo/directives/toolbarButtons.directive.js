export default function toolbarButtons($window) {

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

      scope.parseParams = (search) => {
        return (search).replace(/(^\?)/,'').split("&").reduce((p,n) => {
          return n = n.split("="), p[n[0]] = n[1], p;
        }, {});
      };

      scope.isList = () => {
        let params = scope.parseParams($window.location.search);
        return params.act === 'list';
      };

      scope.init();
    }
  };
}
