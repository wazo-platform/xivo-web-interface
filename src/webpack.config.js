var path = require('path');
var webpack = require('webpack');

const ExtractTextPlugin = require("extract-text-webpack-plugin");

module.exports = {
  entry: {
    css: './www/css/css.js',
    vendor: ['jquery', 'jquery-ui', 'bootstrap', 'select2']
  },
  output: {
      publicPath: '/',
      filename: 'www/js/[name].js'
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
        test: require.resolve('jquery'),
        loader: 'expose-loader?jQuery!expose-loader?$'
      },
      {
        test: /\.(ttf|eot|woff|woff2|svg)$/,
        loader: "url-loader?limit=50000&name=www/fonts/[name].[ext]"
      }
    ]
  },
  plugins: [
    new webpack.optimize.CommonsChunkPlugin({ name: 'vendor', filename: 'www/js/vendor.bundle.js' }),
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
