@echo off
REM Authenticode sign and timestamp a file using dual SHA-1 and SHA-256 signatures
REM also includes fallback behavior if one of the timestamping servers is offline
REM
REM Arguments:
REM   %1 - File to sign
REM   %2 - Description (optional)

REM Add Tools\WinSDK to PATH
setlocal

set Filename=%1
set SignArgs=

REM Set %Description% to either an empty string or "/d <argv[2]>"
REM   Don't use else and () in case argv[2] contains a ")"
set Description=
if not [%2]==[] set Description=/d %2


REM ------------------------------------------------------------------
REM MSI's can't be dual-signed, so skip the SHA-1 signature
REM ------------------------------------------------------------------
for %%f in (%Filename%) do (
    if %%~xf==.msi goto :sha1skipped
    if %%~xf==.msix goto :sha1skipped
)


REM ------------------------------------------------------------------
REM Add SHA-1 signature for compatibility with WinXP SP3+ and Vista
REM ------------------------------------------------------------------

REM Add signature using Comodo's Authenticode timestamp server
REM   Retry up to 3 times in case the server is temporarily unavailable
for /L %%a in (1,1,3) do (
    echo Comodo SHA-1 attempt #%%a
    signtool sign /a %Description% /fd sha1 /t http://timestamp.comodoca.com /v %Filename%

    REM Check for success, breaking on success, waiting 15 seconds on failure
    if not ERRORLEVEL 1 GOTO sha1succeeded
    choice /N /T:15 /D:Y >NUL
)

REM Fallback using Symantec/Verisign's Authenticode timestamp server
echo Verisign SHA-1 fallback
signtool sign /a %Description% /fd sha1 /t http://timestamp.verisign.com/scripts/timestamp.dll /v %Filename%
if ERRORLEVEL 1 goto error

:sha1succeeded

REM If there isn't an existing signature, passing /as (append) to signtool makes
REM the new signature the primary one, but .msi files only support one, and it
REM exits with an error before checking if it's actually adding a second one.
set SignArgs=/as

:sha1skipped


REM ------------------------------------------------------------------
REM Add SHA-256 signature for compatibility with Win7 and above
REM ------------------------------------------------------------------

REM Append signature using Comodo's RFC 3161 server
REM   Retry up to 3 times in case the server is temporarily unavailable
for /L %%a in (1,1,3) do (
    echo Comodo SHA-256 attempt #%%a
    signtool sign /a %SignArgs% %Description% /fd sha256 /td sha256 /tr http://timestamp.comodoca.com /v %Filename%

    REM Check for success, breaking on success, waiting 15 seconds on failure
    if not ERRORLEVEL 1 GOTO sha256succeeded
    choice /N /T:15 /D:Y >NUL
)

REM Fallback to adding signature using Symantec/Verisign's RFC 3161 server
REM   This (currently) generates an SHA-1 digest for the timestamp, which will
REM   be blocked by Windows 10 after 1/1/2017
echo Verisign SHA-256 fallback
signtool sign /a %SignArgs% %Description% /fd sha256 /td sha256 /tr http://sha256timestamp.ws.symantec.com/sha256/timestamp /v %Filename%
if ERRORLEVEL 1 goto ERROR

:sha256succeeded


REM ------------------------------------------------------------------
REM Success
REM ------------------------------------------------------------------
:success
endlocal
exit /b 0


REM ------------------------------------------------------------------
REM Error handler
REM ------------------------------------------------------------------
:error
echo.
echo ERROR! Unable to sign or timestamp file
endlocal
exit /b 1
