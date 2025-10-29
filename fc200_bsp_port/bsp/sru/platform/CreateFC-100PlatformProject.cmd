@echo off

:: /*********************************************************************//**
::  <!-- setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION -->
::  *************************************************************************/
verify OTHER 2>nul
setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION
if ERRORLEVEL 1 (
  echo ERROR: Command Extensions are required but are NOT Supported in this context, unable to continue.
  timeout /T 30 /NOBREAK
  exit /B 9001
)

:: /*********************************************************************//**
::  <!-- change to the directory/folder where this batch file is stored -->
::  *************************************************************************/
pushd "%~dp0"

:: /*********************************************************************//**
::  <!-- Define the parameter names accepted by this batch file-->
::  *************************************************************************/
set ParamNamePathToVitisWorkspaceFolder=PathToVitisWorkspaceFolder
set ParamNamePathAndFilenameOfPlatformXsa=PathAndFilenameOfPlatformXsa
set ParamNameCreatePlatformProjectTclFilename=CreatePlatformProjectTclFilename
set ParamNamePathToVitisBinFolder=PathToVitisBinFolder
set ParamNamePatchFilename=PatchFilename
set ParamNamePatchExecutable=PatchExecutable
set ParamNamePlatformProjectName=PlatformProjectName

:: /*********************************************************************//**
::  <!-- Define the default values used by this batch file-->
::  *************************************************************************/
set DefaultParamValuePathToVitisBinFolder=C:/Xilinx/Vitis/2020.2/bin

:: Change the following as required, or specify as a command line parameter and value
set DefaultPatchFilename=%~dp0XilinxBspFixes.patch
set DefaultPatchFilename=%DefaultPatchFilename:\=/%

set DefaultCreatePlatformProjectTclFilenameWithoutPath=%~dp0%~n0.tcl
set DefaultCreatePlatformProjectTclFilenameWithoutPath=%DefaultCreatePlatformProjectTclFilenameWithoutPath:\=/%

set DefaultPatchExecutable=C:/Program Files/Git/usr/bin/patch.exe

echo INFO: {%~nx0}, executing on {%COMPUTERNAME%}

set NumberOfParamsParsed=0

goto lParseParameters

:: Failsafe, should be unreachable
goto :EOF

:: /*********************************************************************//**
:lParseParameters
::  *************************************************************************/
set ParamNameAndValue=%~1
shift /1

:: NB, it is not feasible to sanitise what was passed on the command line in a generic way as that will likely affect valid data as well.

if "%ParamNameAndValue%" EQU "" (
  echo INFO: Number of parameters parsed is {%NumberOfParamsParsed%}
  set NumberOfParamsParsed=0
  goto lInterpretAndVerify
)

if /I "%ParamNameAndValue%" EQU "help" (
  goto lUsage
)

if /I "%ParamNameAndValue%" EQU "/help" (
  goto lUsage
)

if /I "%ParamNameAndValue%" EQU "/h" (
  goto lUsage
)

if /I "%ParamNameAndValue%" EQU "-help" (
  goto lUsage
)

if /I "%ParamNameAndValue%" EQU "--help" (
  goto lUsage
)

if /I "%ParamNameAndValue%" EQU "/?" (
  goto lUsage
)

if /I "%ParamNameAndValue%" EQU "?" (
  goto lUsage
)

if /I "%ParamNameAndValue%" EQU "usage" (
  goto lUsage
)

if /I "%ParamNameAndValue%" EQU "instructions" (
  goto lUsage
)

for /F "delims== tokens=1,2*" %%a in ("%ParamNameAndValue%") do (
  if "%%c" NEQ "" (
    echo WARNING: Unparsable parameter data detected {%%c}, ignoring
  )
  set "%%a=%%b"
  echo INFO: Detected parameter name {%%a} and value {%%b}, stored
)

set /A NumberOfParamsParsed=NumberOfParamsParsed + 1

:: Recursively parse the next parameter
goto lParseParameters

:: /*********************************************************************//**
:lInterpretAndVerify
::  *************************************************************************/

set ErrorMessage=Parameter name {%ParamNamePathToVitisWorkspaceFolder%}, Invalid value {%PathToVitisWorkspaceFolder%}
if not defined PathToVitisWorkspaceFolder (
  echo ERROR: %ErrorMessage%
  goto lUsage
)
set PathToVitisWorkspaceFolder=%PathToVitisWorkspaceFolder:"=%
if "%PathToVitisWorkspaceFolder%" EQU "" (
  echo ERROR: %ErrorMessage%
  goto lUsage
)
if not exist "%PathToVitisWorkspaceFolder%" (
  echo WARNING: %ErrorMessage%, attempting to create the folder
  mkdir "%PathToVitisWorkspaceFolder%"
  if not exist "%PathToVitisWorkspaceFolder%" (goto lUsage)
)
set PathToVitisWorkspaceFolder=%PathToVitisWorkspaceFolder:\=/%

set ErrorMessage=Parameter name {%ParamNamePathAndFilenameOfPlatformXsa%}, Invalid value {%PathAndFilenameOfPlatformXsa%}
if not defined PathAndFilenameOfPlatformXsa (
  echo ERROR: %ErrorMessage%
  goto lUsage
)
set PathAndFilenameOfPlatformXsa=%PathAndFilenameOfPlatformXsa:"=%
if "%PathAndFilenameOfPlatformXsa%" EQU "" (
  echo ERROR: %ErrorMessage%
  goto lUsage
)
if not exist "%PathAndFilenameOfPlatformXsa%" (
  echo ERROR: %ErrorMessage%
  goto lUsage
)
set PathAndFilenameOfPlatformXsa=%PathAndFilenameOfPlatformXsa:\=/%

:: NB, if the PlatformProjectName parameter is not specified, PlatformProjectName will be derived from the xsa filename
for %%d in ("%PathAndFilenameOfPlatformXsa%") do (
  set PlatformXsaFilenameWithouPath=%%~nxd

  if not defined PlatformProjectName (
    set PlatformProjectName=%%~nd
  )
)
echo INFO: {PlatformXsaFilenameWithouPath=%PlatformXsaFilenameWithouPath%}, {%ParamNamePlatformProjectName%=%PlatformProjectName%}

if not defined CreatePlatformProjectTclFilename (
  if exist "%DefaultCreatePlatformProjectTclFilenameWithoutPath%" (
    echo INFO: Optional parameter name {%ParamNameCreatePlatformProjectTclFilename%}, was not specified, defaulting to {%DefaultCreatePlatformProjectTclFilenameWithoutPath%}
    set CreatePlatformProjectTclFilename=%DefaultCreatePlatformProjectTclFilenameWithoutPath%
  ) else (
    echo ERROR: Optional parameter name {%ParamNameCreatePlatformProjectTclFilename%}, was not specified, default does not exist {%DefaultCreatePlatformProjectTclFilenameWithoutPath%}
    goto lUsage
  )
)
set CreatePlatformProjectTclFilename=%CreatePlatformProjectTclFilename:"=%
set CreatePlatformProjectTclFilename=%CreatePlatformProjectTclFilename:\=/%

if not defined PathToVitisBinFolder (
  if exist "%DefaultParamValuePathToVitisBinFolder%" (
    echo INFO: Optional parameter name {%ParamNamePathToVitisBinFolder%}, was not specified, defaulting to {%DefaultParamValuePathToVitisBinFolder%}
    set PathToVitisBinFolder=%DefaultParamValuePathToVitisBinFolder%
  ) else (
    echo ERROR: Optional parameter name {%ParamNamePathToVitisBinFolder%}, was not specified, default does not exist {%DefaultParamValuePathToVitisBinFolder%}
    goto lUsage
  )
)
set PathToVitisBinFolder=%PathToVitisBinFolder:"=%
set PathToVitisBinFolder=%PathToVitisBinFolder:\=/%
set ErrorMessage=Parameter name {%ParamNamePathToVitisBinFolder%}, Invalid value {%PathToVitisBinFolder%}
if "%PathToVitisBinFolder%" EQU "" (
  echo ERROR: %ErrorMessage%
  goto lUsage
)
if not exist "%PathToVitisBinFolder%" (
  echo ERROR: %ErrorMessage%
  goto lUsage
)

if not defined PatchFilename (
  if exist "%DefaultPatchFilename%" (
    echo INFO: Optional parameter name {%ParamNamePatchFilename%}, was not specified, defaulting to {%DefaultPatchFilename%}
    set PatchFilename=%DefaultPatchFilename%

    if not defined PatchExecutable (
      if exist "%DefaultPatchExecutable%" (
        echo INFO: Optional parameter name {%ParamNamePatchExecutable%}, was not specified, defaulting to {%DefaultPatchExecutable%}
        set PatchExecutable=%DefaultPatchExecutable%
      ) else (
        echo ERROR: Optional parameter name {%ParamNamePatchExecutable%}, was not specified, default does not exist {%DefaultPatchExecutable%}
        goto lUsage
      )
    )
  ) else (
    echo WARNING: Optional parameter name {%ParamNamePatchFilename%}, was not specified, default does not exist {%DefaultPatchFilename%}, no patch will be applied
  )
)
set PatchFilename=%PatchFilename:"=%
set PatchFilename=%PatchFilename:\=/%

call :lCreatePlatformProject

if defined PatchFilename (
  if "%PatchFilename%" NEQ "" (
    call :lApplyPatch
  )
)

goto lEnd

:: /*********************************************************************//**
:lCreatePlatformProject
::  *************************************************************************/

:: NB, xsct is usually xsct.bat, newer Vitis installations might be different
echo INFO: Executing {"%PathToVitisBinFolder%/xsct" "%CreatePlatformProjectTclFilename%" "%PathToVitisWorkspaceFolder%" "%PathAndFilenameOfPlatformXsa%" "%PlatformProjectName%"}
call "%PathToVitisBinFolder%/xsct" "%CreatePlatformProjectTclFilename%" "%PathToVitisWorkspaceFolder%" "%PathAndFilenameOfPlatformXsa%" "%PlatformProjectName%"
echo INFO: xsct process exit code was set to {%ERRORLEVEL%}
:: NB, there are issues with xsct not setting the process exit code, may be fixed in newer versions

goto :EOF

:: /*********************************************************************//**
:lApplyPatch
::  *************************************************************************/
echo INFO: Applying patch {%PatchFilename%}

copy "%PatchFilename%" "%PathToVitisWorkspaceFolder%/%PlatformProjectName%/"

pushd "%PathToVitisWorkspaceFolder%/%PlatformProjectName%/"
:: Refer to https://www.gnu.org/software/diffutils/manual/html_node/patch-Options.html
call "%PatchExecutable%" -p1 < "%PatchFilename%"
echo INFO: %PatchExecutable% process exit code was set to {%ERRORLEVEL%}
popd

goto :EOF

:: /*********************************************************************//**
:lUsage
::  *************************************************************************/
echo.
echo INFO: %~nx0 usage information 1/1
echo Detailed usage information is available in "%~dp0README.md"
goto lEnd

:: /*********************************************************************//**
:lEnd
::  *************************************************************************/
goto :EOF
