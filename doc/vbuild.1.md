% VBUILD(1) VBUILD User Manual
% Guy Wilson
% Sep 10, 2026

# NAME

vbuild - version builder

# SYNOPSIS

vbuild [*options*]

# DESCRIPTION

VBUILD is a version management program to increment a software’s version number and create an output source file that can be compiled and linked with the software so that the software can display the version number (and build date/time) to the user.

I have used it from a makefile target, such that I can run the command:
```
make version
```

The makefile target can be defined like this:
```
version:
	vbuild -i $(TARGET).ver -t version.c.template -o $(SOURCE)/version.c -major 1 -minor 2
```

An example template file is included with the vbuild distribution, the important point here is that the template file must have the strings `<BUILD_VERSION>` and `<BUILD_DATE>` defined, as it is these that are replaced by vbuild when it creates the output source file.

# OPTIONS

**-i** [incremental version file]

Specifies the filename of the incremental version file, vbuild will create this if it doesn’t exist. The incremental version number in this file is incremented by vbuild.

**-o** [output source file]

Specifies the filename of the output source file.

**-t** [template file]

Specifies the filename of the template source file, vbuild replaces the strings `<BUILD_VERSION>` and `<BUILD_DATE>` in the template file to create the output source file.

**-major** [major version number]

Specifies the major version number, the version number is made up from:
```
	major.minor.incremental
```

**-minor** [minor version number]

Specifies the minor version number, the version number is made up from:
```
	major.minor.incremental
```

**`--pre-increment`**

Tells VBUILD to increment the version number in the input increment version file before writing the output source file. The default is to post-increment the version number.

**-v**

Prints the version information of the VBUILD program and exits.

**-h**

Prints the supported command line options and exits.

