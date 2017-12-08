import angular from 'angular';
import uibootstrap from 'angular-ui-bootstrap';
import angulartranslate from 'angular-translate';

import config from './xivo.config';
import run from './xivo.run';

/*Module dependencies */



angular.module('Xivo', [angulartranslate, uibootstrap])
.config(config)
.run(run);
