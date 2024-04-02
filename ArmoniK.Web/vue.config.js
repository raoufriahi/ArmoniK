const webpack = require('webpack')
module.exports = {
  lintOnSave: false,
  runtimeCompiler: true,
  configureWebpack: {
       plugins: [
          new webpack.ProvidePlugin({
            $:"jquery",
            jQuery:"jquery",
            "windows.jQuery":"jquery"
          })
        ],
      optimization: {
        runtimeChunk: 'single',
        splitChunks: {
          chunks: 'all',
          maxInitialRequests: Infinity,
          minSize: 20000,
          cacheGroups: {
            vendor: {
              test: /[\\/]node_modules[\\/]/,
              name (module) {
                // get the name. E.g. node_modules/packageName/not/this/part.js
                // or node_modules/packageName
                const packageName = module.context.match(/[\\/]node_modules[\\/](.*?)([\\/]|$)/)[1]
                // npm package names are URL-safe, but some servers don't like @ symbols
                return `npm.${packageName.replace('@', '')}`
              }
            }
          }
        }
      }
    },
    assetsDir: 'static',
    devServer: {
      port: 1007,     //Port
    },
    chainWebpack: config => {
        config
            .entry('index')
            .add('babel-polyfill')
    }
}
