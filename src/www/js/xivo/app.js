import angular from 'angular';
import uibootstrap from 'angular-ui-bootstrap';
import angulartranslate from 'angular-translate';

import config from './xivo.config';
import run from './xivo.run';

/*Module dependencies */
import toolbarButtons from './directives/toolbarButtons.directive';
import toolbarSearch from './directives/toolbarSearch.directive';


angular.module('Xivo', [angulartranslate, uibootstrap])
.config(config)
.directive('toolbarButtons',toolbarButtons)
.directive('toolbarSearch',toolbarSearch)
.run(run);
