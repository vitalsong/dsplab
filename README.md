## DSPLAB

An automated environment for testing streaming devices, especially digital processing modules.

> The project is quite old and has been gathering dust for a long time. I am now ashamed of some architectural decisions. Maybe I'll fix something, maybe not.

The main inspiration for the design was VST, Matlab Simulink and GnuRadio.

Dependencies:

- Qt5

Supported platforms:

- Windows;
- Ubuntu;
- MacOS;

Build:

```
cmake . -B build -DCMAKE_PREFIX_PATH=${PATH_TO_QT5}
cmake --build build --parallel=8
```

### TODO

- move the sdk to a separate repository (build plugins outside the dsplab project);
- move the app to a separate repository (the host is more about graphics);
- isolate the assembly of plugins (targets within cmake should not overlap);
- building a simple window design with parameters according to the description (so as not to write button handlers);
- change the uuid type to string or int128 (generate through the uuid1 function);
- support multiple channels for one connection
- fix comments to eng;
- add dsplab-sdk version to plugin API;