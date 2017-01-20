# QEagleLib
Qt-based library for managing EagleCAD XML files.

This library allows to work with schematic (\*.sch), board (\*.brd) and Library (\*.lbr) files in XML format, old binary format is not supported. Latest supported DTD version is 6.6, newer features intoduced in 7.0 and 7.4 will be ignored.

QEagleLib provides C++ classes for all entities described in DTD, allowing editing of existing files as well as creating new from a scratch.

This library makes it possible to store both schematic and board (and library) in a single file for processing purposes, however, such file will be considered invalid by EagleCAD and should written back to separate files.

Use *CEagleDocument* class as an entry point to begin working with a file, and if you create one from a scratch, don't forget to select it's kind using *.drawing().mode()* property before saving and feeding back to EagleCAD.
