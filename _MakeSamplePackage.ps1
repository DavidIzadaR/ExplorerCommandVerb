#Write-Host "Enable running .ps1 scripts"
#Set-ExecutionPolicy -ExecutionPolicy Bypass -Scope Process

Clear

# Change to the script's directory
Set-Location $PSScriptRoot

Write-Host "Copy binaries to Output folder"
Copy-Item .\x64\Debug\ExplorerCommandVerb.dll Appx\ExplorerCommandVerb.dll

Write-Host "Sign binaries"
.\SignFile.bat .\Appx\ExplorerCommandVerb.dll

Write-Host "Make MSIX package"
MakeAppx.exe pack /f .\Appx\Mapping.txt /p Output\SampleShellEx.msix /o

Write-Host "Sign MSIX package"
.\SignFile.bat .\Output\SampleShellEx.msix

Get-AppxPackage | where-object {$_.Name -like 'ScooterSoftware.SampleShellExt'} | Remove-AppPackage

Add-AppPackage -Path .\Output\SampleShellEx.msix -ForceUpdateFromAnyVersion
