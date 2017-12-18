export default function toolbar() {

  const _parseParams = (search) => {
    return (search).replace(/(^\?)/,'').split("&").reduce((p,n) => {
      return n = n.split("="), p[n[0]] = n[1], p;
    }, {});
  };

  return {
    parseParams : _parseParams
  };
}
