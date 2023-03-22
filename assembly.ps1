## ファイル群を特定のフォルダに収集する
## 実行時にファイル、フォルダパスが赤く出力される場合、ファイル、フォルダが存在しません

Set-Location (Split-Path -Parent $MyInvocation.MyCommand.Path)
$ErrorActionPreference = "Stop"

$tofrom =
@(".\src\", ".\dev\actuator\motor\Motor\Motor.hpp"),
@(".\src\", ".\dev\algorithm\Utility.hpp"),
@(".\src\", ".\dev\com\common\BasicReader.hpp"),
@(".\src\", ".\dev\com\common\BasicWriter.hpp"),
@(".\src\", ".\dev\com\can\CanBusInterface.hpp"),
@(".\src\", ".\dev\com\can\CanBusSPI.hpp"),
@(".\src\", ".\dev\com\can\CanBusPico.hpp"),
@(".\src\", ".\dev\com\can\CanBusTeensy.hpp"),
@(".\src\", ".\dev\com\can\Can.hpp"),
@(".\src\", ".\dev\com\can\CanInfo.hpp"),
@(".\src\", ".\dev\com\can\CanReader.hpp"),
@(".\src\", ".\dev\com\can\CanWriter.hpp"),
@(".\src\", ".\dev\com\i2c\I2CSlaveReader\I2CSlaveReader.hpp"),
@(".\src\", ".\dev\com\i2c\I2CSlaveWriter\I2CSlaveWriter.hpp"),
@(".\src\", ".\dev\com\serializer\Serializer.hpp"),
@(".\src\", ".\dev\common\types\Message.hpp"),
@(".\src\", ".\dev\common\types\Predef.hpp"),
@(".\src\", ".\dev\common\types\Vector2D.hpp"),
@(".\src\", ".\dev\sensor\Gyro\Gyro.hpp"),
@(".\src\", ".\dev\sensor\Measure\Measure.hpp"),
@(".\src\", ".\dev\stl\list.hpp"),
@(".\src\", ".\dev\stl\memory.hpp"),
@(".\src\", ".\dev\stl\functional.hpp")

## ui
foreach ($source in $tofrom) {
	if (Test-Path $source[0]) {
		Write-Host -NoNewline $source[0]
	}
	else {
		Write-Host -NoNewline $source[0] -ForegroundColor Red
	}

	Write-Host -NoNewline "  <<  "

	if (Test-Path $source[1]) {
		Write-Host -NoNewline $source[1]
	}
	else {
		Write-Host -NoNewline $source[1] -ForegroundColor Red
	}

	Write-Host
}


## file collection
Remove-Item ".\src\" -Recurse -Force
foreach ($source in $tofrom) {

	$copydir = $source[0]
	$filename = Split-Path $source[1] -Leaf
	$filedir = Split-Path $source[1] -Parent

	try {
		robocopy $filedir $copydir $filename | Out-Null
	}
	catch {
		# ✊
	}

}

