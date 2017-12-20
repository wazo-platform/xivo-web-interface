import angular from 'angular';
import uibootstrap from 'angular-ui-bootstrap';
import angulartranslate from 'angular-translate';

import config from './xivo.config';
import run from './xivo.run';

/*Module dependencies */
import IpbxUsersController from './controllers/ipbx/users.controllers';
import CCAgentsController from './controllers/callcenter/agents.controllers';
import toolbar from './services/toolbar.service';
import toolbarButtons from './directives/toolbarButtons.directive';
import toolbarSearch from './directives/toolbarSearch.directive';
import onFinishRender from './directives/onFinishRender.directive';


angular.module('Xivo', [angulartranslate, uibootstrap])
.config(config)
.service('toolbar',toolbar)
.controller('IpbxUsersController', IpbxUsersController)
.controller('CCAgentsController', CCAgentsController)
.directive('toolbarButtons',toolbarButtons)
.directive('toolbarSearch',toolbarSearch)
.directive('onFinishRender',onFinishRender)
.run(run);
