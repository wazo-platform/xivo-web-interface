var webpackConfig = require('./webpack.config.js');

delete webpackConfig.entry;
webpackConfig.plugins.splice(1);
//For debugging only as too slow for tdd, see https://webpack.js.org/configuration/devtool/
//webpackConfig.devtool='inline-source-map';

module.exports = function(config) {
  config.set({
    basePath: '',
    frameworks: ['jasmine'],

    reporters: ['progress'],
    port: 9876,
    colors: true,
    logLevel: config.LOG_INFO,
    autoWatch: true,
    browsers: ['Chrome'],
    singleRun: false,
    autoWatchBatchDelay: 300,

    files: [
      './node_modules/jquery/dist/jquery.js',
      './node_modules/angular/angular.js',
      './node_modules/angular-mocks/angular-mocks.js',
      './test/karma/bootstrap.js',
      './www/js/xivo/app.js',
      './www/js/xivo_toolbar.js',
      './www/js/dwho.js',
      './www/js/dwho/*.js',
      './www/js/xivo/configuration/provisioning/plugin.js',
      {pattern: './i18n/**/*', watched: false, served: true, included: false},
      './www/js/xivo/**/*.html',
      './test/karma/**/*.spec.js'
    ],

    proxies: {
      "/i18n/": "/base/i18n/",
    },

    preprocessors: {
      './www/js/xivo/app.js': ['webpack', 'sourcemap'],
      './test/karma/**/*.spec.js': ['webpack', 'sourcemap'],
      './www/js/xivo/**/*.html': ['ng-html2js']
    },

    webpack: webpackConfig,

    webpackMiddleware: {
      noInfo: 'errors-only'
    },

    ngHtml2JsPreprocessor: {
      stripPrefix: 'www',
      moduleName: 'html-templates'
    }
  });
};
