# btplay
This filter loads textual scene descriptions - MPEG-4 BIFS in BT or XMT syntax,
VRML97 and X3D - into the scene graph of GPAC's compositor, which then composes
them straight into a canvas. There is no video track involved: the picture is
built on the spot from a text file.

It is GPAC's own `load_bt_xmt.c`, packaged as a side module because no solver
carries it. Alongside it come `clock.c` - the same file `bifsdec` carries, since
the compositor's clock helpers are not exported by the solvers - and a handful
of media-object entry points that a scene loaded from a file never reaches.

## What works today

| Format | Extension | State |
|---|---|---|
| MPEG-4 BIFS, BT syntax | `.bt` | works |
| VRML97, classic syntax | `.wrl` | works |
| X3D, XML syntax | `.x3d` | **does not render** - the compositor raises "divide by zero" and then "null function", whatever geometry the scene uses. VRML97 goes through the same loader and renders, so the gap is on the X3D XML side and is not understood yet. |
| MPEG-4 BIFS, XMT syntax | `.xmt` | untested |

Two things to know before writing a scene for it.

**The source URL must be absolute.** The compositor resolves it inside GPAC's
own virtual filesystem, where a relative path has nothing to resolve against;
the session then reports `Cannot find filter for service` and retries in a
loop, at some cost to the machine. Resolve the URL against `location.href`
before handing it over.

**Avoid `Text` nodes.** This build carries no font, and a text node makes the
compositor divide by zero - the same signature as the X3D failure above, which
may well be the same underlying cause.

## Requirements

[CMake](https://cmake.org/) is used as a build system. To install it, follow
[Debian build instructions](developing_in_debian.md).

[Emscripten SDK](https://emscripten.org/) is required for building
WebAssembly artifacts. To install it, follow the
[Download and Install](https://emscripten.org/docs/getting_started/downloads.html)
guide:

```bash
cd $OPT

# Get the emsdk repo.
git clone https://github.com/emscripten-core/emsdk.git

# Enter that directory.
cd emsdk

# Download and install the latest SDK tools.
./emsdk install latest

# Make the "latest" SDK "active" for the current user. (writes ~/.emscripten file)
./emsdk activate latest
```

## Building the accessor

```bash
# Setup EMSDK and other environment variables. In practice EMSDK is set to be
# $OPT/emsdk.
source $OPT/emsdk/emsdk_env.sh

# Assuming you are in the root level of the cloned repo :
emcmake cmake .
emmake make
```

Once built, you can use and distribute btplay_1.wasm with your universal tags.

## Documentation

For more details, please visit our documentation at https://bevara.com/documentation/develop/.
