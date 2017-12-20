/* global clean_ms */

export default class CCAgentsController {

  constructor($scope, $log) {
    this.$log = $log;
    this.$scope = $scope;
    this.init_tab_queues_done = false;
  }

  init_tab_queues() {
    if (!this.init_tab_queues_done) {
      this.$log.info('init_tab_queues');
      this.init_tab_queues_done = new clean_ms('it-queuelist-finder','it-queuelist','it-queue').__init();
    }
  }
}
