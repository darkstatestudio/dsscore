param(
    [string]$Version = "0.1.0",
    [string[]]$EngineVersions = @("5.6", "5.7")
)

$ProjectRoot = Resolve-Path "$PSScriptRoot\.."
$PluginName = "DssCore"
$PluginRoot = "$ProjectRoot\Plugins\$PluginName"

$DistDir = "$ProjectRoot\dist"

Write-Host "Preparing release..."

# Clean dist
if (!Test-Path $DistDir) {
	New-Item -ItemType Directory -Force -Path $DistDir | Out-Null
}

$ReleaseDir = "$DistDir\$PluginName"
# Copy plugin
robocopy `
	$PluginRoot `
	$ReleaseDir `
	/E `
	/XD `
	Binaries `
	Intermediate `
	DerivedDataCache `
	Saved `
	.git `
	.idea `
	.vs `
	/XF `
	.editorconfig `
	*.sln

foreach ($EngineVersion in $EngineVersions)
{
    Write-Host ""
    Write-Host "Packaging UE $EngineVersion"

	$ZipPath = "$DistDir\${PluginName}_${EngineVersion}_${Version}.zip"

	if (Test-Path $ZipPath) {
    	Remove-Item $ZipPath -Force
    }

	Compress-Archive `
		-Path "$ReleaseDir\*" `
		-DestinationPath $ZipPath `
		-Force

	Write-Host ""
	Write-Host "Release created:"
	Write-Host $ZipPath
}

# Cleanup temp
Remove-Item $ReleaseDir -Recurse -Force

Write-Host ""
Write-Host "Done."
