/* global clean_ms */

export default class CCQueuesController {

  constructor() {
    this.init_tab_announce_done = false;
  }

  init_tab_announce() {
    if (!this.init_tab_announce_done) {
      this.init_tab_announce_done = new clean_ms('it-pannouncelist-finder','it-pannouncelist','it-queue-periodic-announce').__init();
    }
  }
}
