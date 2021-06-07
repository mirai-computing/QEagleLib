# QEagleLib
Qt-based library for managing EagleCAD XML files.

This library allows to work with schematic (\*.sch), board (\*.brd) and library (\*.lbr) files in XML format. Latest supported DTD version is 8.0 (last completely offline version maintained up to EagleCAD 8.1.1), old binary format is not supported.

QEagleLib provides C++ classes for all entities described in DTD allowing for editing of existing files as well as creating new from a scratch.

This library also makes it possible to store both schematic and board (and library) in a single file for processing purposes, however, such file will be considered invalid by EagleCAD and should written back to separate files.

Use *CEagleDocument* class as an entry point to begin working with a file, and if you create one from a scratch, don't forget to select its kind using *.drawing().mode()* property before saving and feeding it back to EagleCAD.

In order to save a file for an earlier version of EagleCAD, please set required version via *CEagleDocument::version()* property, newer features won't be written to that file.
