# Usage information for CreateFC-nnnPlatformProject.cmd and corresponding TCL script files

- Where nnn refers to the FC LRU type variant (100, 200, etc)


## Introduction
- Each cmd file will by default expect a corresponding TCL file to be present in the same folder as the cmd file


```mermaid
flowchart TD;
  subgraph idStart [Users]
    Developer((Developer));

    ContinuousIntegration((Continuous<br />Integration<br />e.g. Jenkins));
  end

  subgraph idCmdFileProcessing [1#46; CMD Processing]
    CreateFC-nnnPlatformProject.cmd[[<nobr>CreateFC-nnnPlatformProject.cmd</nobr>]];

    CommandLineParameters{{<nobr>Command Line Parameters</nobr><br />#40;Required and Optional#41;}};

    CreateFC-nnnPlatformProject.cmd-. 1a#46; parses<br />1b#46; validates<br /><nobr>1c#46; optionally applies default values</nobr> .->CommandLineParameters;
  end

  subgraph idTclProcessing [2#46; TCL Script Processing]
    direction LR
    xsct[[<nobr>Xilinx Software Command-line Tool for Vitis:</nobr><br />xsct<br /><nobr>#40;optional parameter available#41;</nobr>]];

    TCLScript{{<nobr>TCL Script file</nobr><br /><nobr>#40;optional parameter available#41;</nobr>}};

    subgraph idTclCommandLineParameters [Command Line Parameters]
      direction LR
      vitisWorkspacePath{{<nobr>$vitisWorkspacePath</nobr><br />is $argv 0}};

      platformXsaPathAndFilename{{<nobr>$platformXsaPathAndFilename</nobr><br />is $argv 1}};

      platformProjectName{{<nobr>$platformProjectName</nobr><br />is $argv 2}};

      vitisWorkspacePath~~~platformXsaPathAndFilename~~~platformProjectName;
    end

    xsct-. 2b#46; parses and executes tcl script<br />with command line parameter values .->TCLScript;

    xsct---idTclCommandLineParameters;

    idTclCommandLineParameters-->TCLScript;
  end

  subgraph idPatchProcessing [3#46; Patch Processing]
    PatchExecutable[[<nobr>Patch Executable</nobr><br /><nobr>#40;optional parameter available#41;</nobr>]];

    PatchFilename{{<nobr>PatchFilename</nobr><br /><nobr>#40;optional parameter available#41;</nobr>}};

    PatchExecutable-. 3b#46; applies the patch .->PatchFilename;
  end

  idStart-. <nobr>executes cmd file</nobr><br /><nobr>with command line parameters</nobr><br /><nobr>and/or pre-defined environment variables</nobr> .->idCmdFileProcessing;

  CreateFC-nnnPlatformProject.cmd-. 2a#46; executes xsct with command line parameters .->idTclProcessing;

  CreateFC-nnnPlatformProject.cmd-. 3a#46; executes patch executable<br />#40;optional#41; .->idPatchProcessing;

  idTclProcessing~~~idPatchProcessing;
```

## About the CreateFC-nnnPlatformProject.tcl files
The tcl files are intended to be specific to the FC LRU type variant (the target platform).

The tcl script files are parsed by xsct (the Xilinx Software Command-line Tool for Vitis), xsct executes the instructions contained in the tcl script file.

The tcl script filename can be specified using an optional command line parameter to the cmd file, when not specified the tcl filename is derived from the cmd filename.

## About the CreateFC-nnnPlatformProject.cmd files
The cmd files are designed and intended to:
- Allow for separation between target platform variants such that divergent changes are isolated in separate files
- Never require any editing by the user (please do not edit, ask for help if anything is unclear)
- Never require any editing when a new platform xsa file is available
- Accept inputs from the user or Continuous Integration as command line parameters
- Allow the user or CI to use an alternate method of specifying one or more parameter values using environment variables in a user-defined batch file
- Allow for required and optional parameters
- Allow for using a default value when an optional parameter is not specified
- Allow the user to specify command line parameters in any order such that the sequential ordering of parameters is irrelevant
- Allow for installation folder path variations where default installation folders have not been used



## How to specify CreateFC-nnnPlatformProject.cmd command line parameters
- Each command line parameter starts and ends with double quotation marks, using the following syntax:

```mermaid
flowchart LR;
  subgraph idSg1 [ ];
    direction LR
    OpeningDoubleQuote{{"&quot;"}}---ParameterName{{"ParameterName"}}---EqualsCharacter{{"="}}---Value{{"Value"}}---ClosingDoubleQuote{{"&quot;"}};
  end
```

- Where the rules for ParameterName are the same rules applicable to environment variable names
- Parameters must be separated by a space character, for example: "ParameterXName=XValue" "ParameterYName=YValue"


## CreateFC-nnnPlatformProject.cmd Required Parameters
- The following parameters must be provided, either using the abovementioned command line parameter syntax, or pre-defined using environment variables prior to calling CreateFC-nnnPlatformProject.cmd:


```mermaid
flowchart LR;
  subgraph idSg1 [ ];
    direction LR
    OpeningDoubleQuote{{"&quot;"}}---ParameterName{{"PathToVitisWorkspaceFolder"}}---EqualsCharacter{{"="}}---Value{{"Path to your Vitis Workspace folder"}}---ClosingDoubleQuote{{"&quot;"}};
  end
```

```mermaid
flowchart LR;
  subgraph idSg1 [ ];
    direction LR
    OpeningDoubleQuote{{"&quot;"}}---ParameterName{{"PathAndFilenameOfPlatformXsa"}}---EqualsCharacter{{"="}}---Value{{"Path and filename of the Platform XSA file"}}---ClosingDoubleQuote{{"&quot;"}};
  end
```


## CreateFC-nnnPlatformProject.cmd Optional Parameters
- The following parameters are optional and will use a default value if not specified prior to calling CreateFC-nnnPlatformProject.cmd:


```mermaid
flowchart LR;
  subgraph idSg1 [ ];
    direction LR
    OpeningDoubleQuote{{"&quot;"}}---ParameterName{{"PlatformProjectName"}}---EqualsCharacter{{"="}}---Value{{"A name that is assigned to the platform project within the Vitis workspace, usually derived from the XSA filename"}}---ClosingDoubleQuote{{"&quot;"}};
  end
```

```mermaid
flowchart LR;
  subgraph idSg1 [ ];
    direction LR
    OpeningDoubleQuote{{"&quot;"}}---ParameterName{{"PathToVitisBinFolder"}}---EqualsCharacter{{"="}}---Value{{"Path to installed Vitis bin folder"}}---ClosingDoubleQuote{{"&quot;"}};
  end
```

```mermaid
flowchart LR;
  subgraph idSg1 [ ];
    direction LR
    OpeningDoubleQuote{{"&quot;"}}---ParameterName{{"CreatePlatformProjectTclFilename"}}---EqualsCharacter{{"="}}---Value{{"Path and filename of TCL file"}}---ClosingDoubleQuote{{"&quot;"}};
  end
```

```mermaid
flowchart LR;
  subgraph idSg1 [ ];
    direction LR
    OpeningDoubleQuote{{"&quot;"}}---ParameterName{{"PatchFilename"}}---EqualsCharacter{{"="}}---Value{{"Path and filename of the patch file to be applied"}}---ClosingDoubleQuote{{"&quot;"}};
  end
```

```mermaid
flowchart LR;
  subgraph idSg1 [ ];
    direction LR
    OpeningDoubleQuote{{"&quot;"}}---ParameterName{{"PatchExecutable"}}---EqualsCharacter{{"="}}---Value{{"Path and filename of the patch executable that should be used to apply the patch"}}---ClosingDoubleQuote{{"&quot;"}};
  end
```
