open Gl;

open GlType;

let _compileShader = (gl, glslSource: string, shader) => {
  shaderSource(shader, glslSource, gl);
  compileShader(shader, gl);
  WonderLog.Log.debugWithFunc(
    () =>
      getShaderParameter(shader, getCompileStatus(gl), gl) === false ?
        {
          let message = getShaderInfoLog(shader, gl);
          WonderLog.Log.debug(
            WonderLog.Log.buildDebugMessage(
              ~description="shader info log",
              ~params={j|$message|j},
            ),
            true,
          );
          WonderLog.Log.debug(
            WonderLog.Log.buildDebugMessage(
              ~description="glsl source",
              ~params={j|$glslSource|j},
            ),
            true,
          );
        } :
        (),
    true,
  );
  shader;
};

let _linkProgram = (program, gl) => {
  linkProgram(program, gl);
  WonderLog.Log.debugWithFunc(
    () =>
      getProgramParameter(program, getLinkStatus(gl), gl) === false ?
        {
          let message = getProgramInfoLog(program, gl);
          WonderLog.Log.fatal(
            WonderLog.Log.buildFatalMessage(
              ~title="link program error",
              ~description={j|$message|j},
              ~reason="",
              ~solution={j||j},
              ~params={j||j},
            ),
          );
        } :
        (),
    true,
  );
  ();
};

let initShader = (vsSource: string, fsSource: string, gl, program: program) => {
  let vs =
    _compileShader(gl, vsSource, createShader(getVertexShader(gl), gl));
  let fs =
    _compileShader(gl, fsSource, createShader(getFragmentShader(gl), gl));

  /* WonderLog.Log.print(("vs source: ", vsSource)) |> ignore;
     WonderLog.Log.print(("fs source: ", fsSource)) |> ignore; */

  /* dispose?
     if (this.glProgram) {
         this.dispose();
     } */
  attachShader(program, vs, gl);
  attachShader(program, fs, gl);
  /*!
    if warn:"Attribute 0 is disabled. This has significant performance penalty" when run,
    then do this before linkProgram:
    gl.bindAttribLocation( this.glProgram, 0, "a_position");



    can reference here:
    http://stackoverflow.com/questions/20305231/webgl-warning-attribute-0-is-disabled-this-has-significant-performance-penalt?answertab=votes#tab-top


    OpenGL requires attribute zero to be enabled otherwise it will not render anything.
    On the other hand OpenGL ES 2.0 on which WebGL is based does not. So, to emulate OpenGL ES 2.0 on top of OpenGL if you don't enable attribute 0 the browser has to make a buffer for you large enough for the number of vertices you've requested to be drawn, fill it with the correct value (see gl.vertexAttrib),
    attach it to attribute zero, and enable it.

    It does all this behind the scenes but it's important for you to know that it takes time to create and fill that buffer. There are optimizations the browser can make but in the general case,
    if you were to assume you were running on OpenGL ES 2.0 and used attribute zero as a constant like you are supposed to be able to do, without the warning you'd have no idea of the work the browser is doing on your behalf to emulate that feature of OpenGL ES 2.0 that is different from OpenGL.

    require your particular case the warning doesn't have much meaning. It looks like you are only drawing a single point. But it would not be easy for the browser to figure that out so it just warns you anytime you draw and attribute 0 is not enabled.
    */
  /*!
    Always have vertex attrib 0 array enabled. If you draw with vertex attrib 0 array disabled, you will force the browser to do complicated emulation when running on desktop OpenGL (e.g. on Mac OSX). This is because in desktop OpenGL, nothing gets drawn if vertex attrib 0 is not array-enabled. You can use bindAttribLocation() to force a vertex attribute to use location 0, and use enableVertexAttribArray() to make it array-enabled.
    */
  bindAttribLocation(program, 0, "a_position", gl);
  _linkProgram(program, gl);
  /*!
    should detach and delete shaders after linking the program

    explain:
    The shader object, due to being attached to the program object, will continue to exist even if you delete the shader object. It will only be deleted by the system when it is no longer attached to any program object (and when the user has asked to delete it, of course).

    "Deleting" the shader, as with all OpenGL objects, merely sets a flag that says you don't need it any more. OpenGL will keep it around for as long as it needs it itself, and will do the actual delete any time later (most likely, but not necessarily, after the program is deleted).
    */
  deleteShader(vs, gl);
  deleteShader(fs, gl);
  program;
};