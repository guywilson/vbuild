#include <string>
#include <iostream>
#include <fstream>
#include <exception>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "cmdarg.h"
#include "currenttime.h"
#include "version.h"

struct _version {
    uint32_t majorVersion;
    uint32_t minorVersion;
    uint32_t incrementalVersion;
};

typedef struct _version version_t;

void printUsage() {
    std::cout << "Usage:" << std::endl << std::endl;
    std::cout << "vbuild <options>" << std::endl;
    std::cout << "\t-v - Print version number and exit" << std::endl;
    std::cout << "\t-h - Print this help and exit" << std::endl;
    std::cout << "\t-i [incremental version file]" << std::endl;
    std::cout << "\t-t [template version source code file]" << std::endl;
    std::cout << "\t-o [version source code output]" << std::endl;
    std::cout << "\t-major [major version]" << std::endl;
    std::cout << "\t-minor [minor version]" << std::endl;
    std::cout << "\t--pre-increment - Increment the version before writing the output file, default is post increment" << std::endl << std::endl;
}

static uint32_t readIncrementalVersion(const std::string & filename) {
    FILE * fp = fopen(filename.c_str(), "rt");

    if (fp == NULL) {
        if (errno == ENOENT) {
            std::cout << "The incremental version file '" << filename << "' does not exist - creating..." << std::endl << std::endl;

            fp = fopen(filename.c_str(), "wt");

            if (fp == NULL) {
                std::cout << "Failed to create incremental version file '" << filename << "' with: " << strerror(errno) << std::endl << std::endl;
                throw std::exception();
            }

            fwrite("000", 1, 3, fp);
            fclose(fp);

            return 0U;
        }

        std::cout << "Failed to open incremental version file '" << filename << "' for reading with: " << strerror(errno) << std::endl << std::endl;
        throw std::exception();
    }

    int i = 0;
    std::string version;

    while (!feof(fp)) {
        version[i++] = fgetc(fp);
    }

    fclose(fp);

    return (uint32_t)strtoul(version.c_str(), NULL, 10);
}

static void writeIncrementalVersion(const std::string & filename, uint32_t version) {
    FILE * fp = fopen(filename.c_str(), "wt");

    if (fp == NULL) {
        std::cout << "Failed to open incremental version file '" << filename << "' for writing" << std::endl << std::endl;
        throw std::exception();
    }

    fprintf(fp, "%03ld", (unsigned long)version);

    fclose(fp);
}

static void writeVersionSourceFile(const std::string & versionTemplateFile, const std::string & versionSourceFile, const version_t & v) {
    FILE * fpTemplate = fopen(versionTemplateFile.c_str(), "rt");

    if (fpTemplate == NULL) {
        std::cout << "Failed to open template file '" << versionTemplateFile << "'" << std::endl << std::endl;
        throw std::exception();
    }

    fseek(fpTemplate, 0L, SEEK_END);
    long filelength = ftell(fpTemplate);
    rewind(fpTemplate);

    char * templateBuffer = (char *)malloc(filelength + 1);

    if (templateBuffer == NULL) {
        std::cout << "Failed to allocate memory for template file '" << versionTemplateFile << "'" << std::endl << std::endl;
        throw std::exception();
    }

    fread(templateBuffer, 1, filelength, fpTemplate);

    fclose(fpTemplate);

    FILE * fpSource = fopen(versionSourceFile.c_str(), "wt");

    if (fpSource == NULL) {
        std::cout << "Failed to open source file '" << versionSourceFile << "'" << std::endl << std::endl;
        throw std::exception();
    }

    std::string timestamp = CurrentTime::getTimeStamp();

    char versionString[256];

    snprintf(
        versionString, 
        256, 
        "%u.%u.%03u", 
        v.majorVersion, 
        v.minorVersion, 
        v.incrementalVersion);

    int i = 0;

    while (templateBuffer[i] != 0) {
        if (strncmp(&templateBuffer[i], "<BUILD_DATE>", 12) == 0) {
            fwrite(timestamp.c_str(), 1, timestamp.length(), fpSource);
            i += 12;
        }
        else if (strncmp(&templateBuffer[i], "<BUILD_VERSION>", 15) == 0) {
            fwrite(versionString, 1, strlen(versionString), fpSource);
            i += 15;
        }
        else {
            fputc((int)templateBuffer[i], fpSource);
            i++;
        }
    }

    fclose(fpSource);

    free(templateBuffer);
}

int main(int argc, char * argv[]) {
    std::string incrementalVersionFile;
    std::string versionTemplateFile;
    std::string versionSourceFile;

    version_t v;
    bool isPreIncrement = false;

    CmdArg cmd(argc, argv);

    while (cmd.hasMoreArgs()) {
        std::string arg = cmd.nextArg();

        if (arg == "-i") {
            incrementalVersionFile = cmd.nextArg();
        }
        else if (arg == "-o") {
            versionSourceFile = cmd.nextArg();
        }
        else if (arg == "-major") {
            v.majorVersion = strtoul(cmd.nextArg().c_str(), NULL, 10);
        }
        else if (arg == "-minor") {
            v.minorVersion = strtoul(cmd.nextArg().c_str(), NULL, 10);
        }
        else if (arg == "-t") {
            versionTemplateFile = cmd.nextArg();
        }
        else if (arg == "-v") {
            std::cout << "VBUILD 'v" << getVersion() << "' - built [" << getBuildDate() << "]" << std::endl << std::endl;
            return 0;
        }
        else if (arg == "-h") {
            printUsage();
            return 0;
        }
        else if (arg == "--pre-increment") {
            isPreIncrement = true;
        }
        else {
            std::cout << "Unknown command line option '" << arg << "'" << std::endl << std::endl;
            printUsage();
            return -1;
        }
    }

    if (incrementalVersionFile.length() == 0) {
        std::cout << "Incremental version file must be specified with the -i option." << std::endl;
        printUsage();
        return -1;
    }
    if (versionSourceFile.length() == 0) {
        std::cout << "Output version source file must be specified with the -o option." << std::endl;
        printUsage();
        return -1;
    }
    if (versionTemplateFile.length() == 0) {
        std::cout << "Template version file must be specified with the -t option." << std::endl;
        printUsage();
        return -1;
    }

    v.incrementalVersion = readIncrementalVersion(incrementalVersionFile);

    if (isPreIncrement) {
        v.incrementalVersion++;
        writeVersionSourceFile(versionTemplateFile, versionSourceFile, v);
    }
    else {
        writeVersionSourceFile(versionTemplateFile, versionSourceFile, v);
        v.incrementalVersion++;
    }

    writeIncrementalVersion(incrementalVersionFile, v.incrementalVersion);

    return 0;
}