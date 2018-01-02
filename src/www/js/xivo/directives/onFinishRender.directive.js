export default function onFinishRender($timeout) {

  return {
    restrict: 'A',
    link: function (scope, element, attr) {
      if (scope.$last === true) {
        $timeout(() => {
          scope.$emit(attr.onFinishRender);
        });
      }
    }
  };
}
