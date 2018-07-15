var gulp = require("gulp");
var rollup = require("rollup");
var nodeResolve = require("rollup-plugin-node-resolve");
var commonjs = require("rollup-plugin-commonjs");

gulp.task("rollup", function () {
    var inputConfig = {
        input: "./lib/es6_global/src/Index.js",
        plugins: [
            nodeResolve({
                skip: [
                ],
                extensions: [".js"]
            }),
            commonjs({
                namedExports: {
                },
                extensions: [".js"]
            })
        ]
    };
    var outputConfig = {
        file: "./dist/wdimgui.js",
        format: "umd",
        name: "wdimgui",
        indent: '\t'
    };

    return rollup.rollup(inputConfig)
        .then(bundle => bundle.write(outputConfig))
});



var generate = require("wonder-generate-index");
var path = require("path");

gulp.task("generateIndex", function (done) {
    var rootDir = path.join(process.cwd(), "src"),
        destDir = "./src/";

    generate.generate("/", rootDir, ["**/api/*.re", "**/api/**/*.re"], destDir, {
        exclude: []
    });

    done();
});
