#!/usr/bin/env pwsh-preview

[CmdletBinding()]
param (
    [Parameter()]
    [string]
    $File
)

$Script:ToolsBin = "$PSScriptRoot/tools-bin"
function Invoke-CoolParser {
    param (
        [string]
        $Source
    )
    
    return (& "${Script:ToolsBin}/lexer" $Source | & "${Script:ToolsBin}/parser")
}

function Test-CoolParsed {
    [CmdletBinding()]
    param (
        [Parameter()]
        [string]
        $ParsedFile
    )

    $parsed = Get-Content $ParsedFile -Raw

    $ansFile = [System.IO.Path]::ChangeExtension($ParsedFile, 'ans');
    $outFile = [System.IO.Path]::ChangeExtension($ParsedFile, 'out');
    $ans = $parsed | & "${Script:ToolsBin}/semant"
    $out = $parsed | & "$PSScriptRoot/src/semant"

    $ans > $ansFile
    $out > $outFile

    git diff --no-index $ansFile $outFile
}

New-Item -ItemType Directory -Force "$PSScriptRoot/fuck/"
$Parsed = "$PSScriptRoot/fuck/" + [System.IO.Path]::GetFileNameWithoutExtension($File) + '.parsed';

Invoke-CoolParser -Source $File > $Parsed
Test-CoolParsed -ParsedFile $Parsed

