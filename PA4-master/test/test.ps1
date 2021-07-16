#! /usr/bin/env pwsh-preview

[CmdletBinding()]
param (
    [Parameter()]
    [string]
    $Source,
    [Parameter()]
    [switch]
    $SelfOnly,
    [Parameter()]
    [switch]
    $AnsOnly,
    [Parameter()]
    [switch]
    $Clean    
)

$Source = Resolve-Path $Source
$SrcDir = Join-Path $PSScriptRoot '../src' -Resolve

function selfmake {
    if ($Clean) {
        make --directory $SrcDir clean
    }

    make --directory $SrcDir

    if (-not $?) {
        exit
    }
}

$outCgen = Join-Path $SrcDir 'cgen'

$ToolsBin = Join-Path $PSScriptRoot '../tools-bin' -Resolve
$refCgen = Join-Path $ToolsBin cgen
# $coolc = Join-Path $ToolsBin coolc
$lexer = Join-Path $ToolsBin lexer
$parser = Join-Path $ToolsBin parser
$semant = Join-Path $ToolsBin semant
$trap = Join-Path $ToolsBin 'trap.s'

$AnsTarget = [System.IO.Path]::ChangeExtension($Source, '.ans.s');
$OutTarget = [System.IO.Path]::ChangeExtension($Source, '.out.s');

function anscgen {
    & $lexer $Source | & $parser | & $semant | & $refCgen -c > $AnsTarget

    if (-not $?) {
        exit
    }

    Write-Host
    Write-Host "-> Ans: " -NoNewline
    Write-Host "$(Resolve-Path $AnsTarget)" -ForegroundColor Cyan

    if (-not $?) {
        exit
    }

    spim -exception_file $trap -file $AnsTarget
}

function selfcgen {
    & $lexer $Source | & $parser | & $semant | & $outCgen -c > $OutTarget

    if (-not $?) {
        exit
    }

    Write-Host
    Write-Host "-> Out: " -NoNewline
    Write-Host "$(Resolve-Path $OutTarget)" -ForegroundColor Magenta

    if (-not $?) {
        exit
    }

    spim -exception_file $trap -file $OutTarget

    if (-not $?) {
        exit
    }
}

function comp {
    # git diff --no-index $AnsTarget $OutTarget

    return $?
}

if ($SelfOnly) {
    selfmake
    selfcgen
}
elseif ($AnsOnly) {
    anscgen
}
else {
    selfmake
    anscgen
    selfcgen
    comp
}
