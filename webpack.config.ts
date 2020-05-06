import webpack from 'webpack';
import HtmlWebpackPlugin from 'html-webpack-plugin';
import path from 'path';

const config: webpack.Configuration = {
    mode: process.env.NODE_ENV === 'development' ? 'development' : 'production',
    entry: path.join(__dirname, './client/app.tsx'),
    output: {
        path: path.join(__dirname, './dist/'),
        filename: 'app.js',
    },
    module: {
        rules: [
            {
                test: /\.tsx?$/,
                use: 'ts-loader',
            },
            {
                test: /\.(sass|scss)$/,
                use: [
                    { loader: 'style-loader' },
                    { loader: 'css-loader' },
                    { loader: 'sass-loader' },
                ],
            },
        ],
    },
    resolve: {
        extensions: ['.ts', '.js', '.tsx'],
        alias: {
            '#': path.resolve(__dirname, './client/'),
        },
    },
    plugins: [
        new HtmlWebpackPlugin({
            template: path.join(__dirname, './client/index.html'),
        }),
    ],
    devServer: {
        contentBase: path.join(__dirname, './dist/'),
        port: 3000,
        hot: true,
        inline: true,
        watchContentBase: true,
    },
};

export default config;
