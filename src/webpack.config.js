var path = require('path');
var webpack = require('webpack');

const ExtractTextPlugin = require("extract-text-webpack-plugin");

module.exports = {
  entry: {
    css: './www/css/css.js',
    xivo: './www/js/xivo/app.js',
    vendor: ['jquery', 'jquery-ui', 'bootstrap', 'select2']
  },
  output: {
      publicPath: '/',
      filename: 'www/js/[name].bundle.js'
  },
  resolve: {
    alias: {
      "jquery-ui": 'jquery-ui-dist/jquery-ui.js'
    }
  },
  devtool: 'source-map',
  module: {
    rules: [
      {
        test: /\.less$/,
        use: ExtractTextPlugin.extract({
          fallback: 'style-loader',
          use: ['css-loader', 'less-loader']
        })
      },
      {
        enforce: "pre",
        test: /\.js$/,
        include: [/xivo/],
        exclude: [/node_modules/, /configuration/, /monitoring.js/, /wizard.js/],
        loader: "eslint-loader"
      },
      {
        test: /\.js$/,
        include: [/xivo/],
        exclude: [/node_modules/, /configuration/, /monitoring.js/, /wizard.js/],
        use: {
          loader: 'babel-loader',
          options: {
            presets: ['env'],
            cacheDirectory: true
          }
        }
      },
      {
        test: require.resolve('jquery'),
        loader: 'expose-loader?jQuery!expose-loader?$'
      },
      {
        test: require.resolve('angular'),
        loader: 'expose-loader?angular!expose-loader?angular'
      },
      {
        test: /\.(ttf|eot|woff|woff2|svg)$/,
        loader: "url-loader?limit=50000&name=www/fonts/[name].[ext]"
      }
    ]
  },
  plugins: [
    new webpack.ProvidePlugin({
      $: 'jquery',
      jQuery: 'jquery',
      "window.jQuery":"jquery"
    }),
    new ExtractTextPlugin({
      filename: 'www/css/xivo.css',
      disable: false,
      allChunks: true
    })
  ]
};
