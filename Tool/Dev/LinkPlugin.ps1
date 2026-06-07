# Usage: ./Tool/Dev/LinkPlugin.ps1 -PluginPath <path-to-target-plugin> -DryRun

param(
    [Parameter(Mandatory = $true)]
    [ValidateNotNullOrEmpty()]
    [string]$PluginPath,

    [string]$Branch = "main",

    [string]$WorktreesDir = "../DssCoreWorktrees",

    [switch]$MigrateContent,

    [switch]$DryRun,

    [switch]$Rebase,

    [switch]$LinkCurrent
)



function Write-Log {
    param(
        [string]$Message,
        [switch]$DryRun,
        [ValidateSet("INFO", "WARN", "ERROR")]
        [string]$Level = "INFO"

    )

    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $dryRunPart = ""
    if ($DryRun) {
        $dryRunPart = "[DryRun]"
    }
    $messageBase = "[$timestamp][$Level]$dryRunPart "

    switch ($Level) {
        "INFO" { Write-Host "$messageBase" -ForegroundColor Blue -NoNewline; Write-Host $Message }
        "WARN" { Write-Host "$messageBase" -ForegroundColor Yellow -NoNewline; Write-Host $Message }
        "ERROR" { Write-Host "$messageBase" -ForegroundColor Red -NoNewline; Write-Host $Message }
    }
}

function New-GitWorktree {
    param(
        [ValidateNotNullOrEmpty()]
        [string]$RepoPath,

        [ValidateNotNullOrEmpty()]
        [string]$WorktreePath,

        [ValidateNotNullOrEmpty()]
        [string]$Branch,

        [switch]$DryRun
    )

    if (!(Test-Path $WorktreesFull)) {
        if ($DryRun) {
            Write-Log "Create worktrees directory" -DryRun:$DryRun
        }
        else {
            New-Item -ItemType Directory -Path $WorktreesFull | Out-Null
        }
    }

    if (!(Test-Path $WorktreePath)) {
        Write-Log "Creating DssCore worktree: $Branch" -DryRun:$DryRun
        if (!$DryRun) {
            git -C $RepoPath worktree add $WorktreePath $Branch
        }
    }
    else {
        Write-Log "Using existing worktree: $WorktreePath" -DryRun:$DryRun
    }
}

function Update-GitWorktree {
    param(
        [ValidateNotNullOrEmpty()]
        [string]$WorktreePath,

        [ValidateNotNullOrEmpty()]
        [string]$Branch,

        [switch]$DryRun
    )

    Write-Log "Pulling(rebase) latest: $Branch" -DryRun:$DryRun
    if ($DryRun) {
        return;
    }

    git -C $WorktreePath pull --rebase
}

function Get-DssPluginManifest {
    param (
        [ValidateNotNullOrEmpty()]
        [string]$PluginFullPath
    )

    $PluginManifestPath = Join-Path $PluginFullPath "DssPlugin.json"
    if (!(Test-Path $PluginManifestPath)) {
        throw "Missing DssPlugin.json in plugin: $PluginFullPath"
    }

    Get-Content $PluginManifestPath -Raw | ConvertFrom-Json
}

function New-CleanSymlink {
    param(
        [ValidateNotNullOrEmpty()]
        [string]$Link,

        [ValidateNotNullOrEmpty()]
        [string]$Target,

        [switch]$DryRun
    )

    Write-Log "New-CleanSymlink $Link -> $Target" -DryRun:$DryRun

    if (!(Test-Path $Target)) {
        if (!$DryRun) {
            throw "Target does not exist: $Target"
        }
    }

    if (Test-Path $Link) {
        $Item = Get-Item $Link -Force
        if ($null -eq $Item.LinkType) {
            throw "Path exists but is not a link: $Link"
        }

        Write-Log "Removing existing link: $Link" -DryRun:$DryRun
        if (!$DryRun) {
            Remove-Item $Link -Force -Recurse
        }
    }

    $Parent = Split-Path -Parent $Link
    if (!(Test-Path $Parent)) {
        Write-Log "Creating link parent directory: $Parent" -DryRun:$DryRun
        if (!$DryRun) {
            New-Item -ItemType Directory -Path $Parent | Out-Null
        }
    }

    Write-Log "Linking: $Link -> $Target" -DryRun:$DryRun
    if (!$DryRun) {
        New-Item -ItemType Junction -Path $Link -Target (Resolve-Path $Target) | Out-Null
        #New-Item -ItemType SymbolicLink -Path $Link -Target (Resolve-Path $Target) | Out-Null
    }
}

$ErrorActionPreference = "Stop"

$DssCoreRoot = Split-Path -Parent $PSScriptRoot | Split-Path -Parent | Resolve-Path
$DssCoreRepo = $DssCoreRoot

$PluginFullPath = Resolve-Path $PluginPath
$PluginSourcePath = Join-Path $PluginFullPath "Source"

if ($Branch -eq "") {
    throw "Branch not provided"
}

$PluginManifest = Get-DssPluginManifest $PluginFullPath
Write-Log "Plugin: '$PluginFullPath'" -DryRun:$DryRun
Write-Log "Plugin Manifest: $PluginManifest" -DryRun:$DryRun

$WorktreesFull = Join-Path $DssCoreRoot $WorktreesDir | Resolve-Path
$SafeBranch = $Branch.Replace("/", "_")
$WorktreePath = Join-Path $WorktreesFull "DssCoreWT_$SafeBranch"
$DssCoreWorktreeSourcePath = Join-Path $WorktreePath "Plugins\DssCore\Source"

if ($LinkCurrent) {
	$DssCoreWorktreeSourcePath = Join-Path $DssCoreRoot "Plugins\DssCore\Source"
} else {
	New-GitWorktree $DssCoreRepo $WorktreePath $Branch -DryRun:$DryRun
	if ($Rebase) {
		Update-GitWorktree $WorktreePath $Branch -DryRun:$DryRun
	}
}

$DssCoreModules = "DssCore", "DssEditorCore"
foreach ($module in $DssCoreModules) {
    $link = Join-Path $PluginSourcePath $module
    $target = Join-Path $DssCoreWorktreeSourcePath $module
    New-CleanSymlink -Link "$link" -Target "$target" -DryRun:$DryRun
}


if ($LinkCurrent) {
	Write-Log "Linked current to plugin" -DryRun:$DryRun
} else {
	Write-Log "DssCore branch: '$Branch' Worktree: '$WorktreePath' " -DryRun:$DryRun
}
