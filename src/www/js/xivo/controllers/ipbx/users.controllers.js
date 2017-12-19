/* global clean_ms */

export default class IpbxUsersController {

  constructor($scope, $log) {
    this.$log = $log;
    this.$scope = $scope;
    this.init_tab_group_done = false;
  }

  init_tab_group() {
    if (!this.init_tab_group_done) {
      new clean_ms('it-grouplist-finder','it-grouplist','it-group').__init();
      new clean_ms('it-queuelist-finder','it-queuelist','it-queue').__init();
      this.init_tab_group_done = true;
    }
  }
}
