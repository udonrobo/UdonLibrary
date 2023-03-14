## ファイル群を特定のフォルダに収集する
## 実行時にファイルパスが赤く出力される場合、ファイルが存在しません
## また、フォルダパスが黄色く出力される場合、コピー先フォルダがないため自動的に作成します

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
@(".\src\", ".\dev\com\stdmessage\Message.hpp"),
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
		Write-Host -NoNewline $source[0] -ForegroundColor Yellow
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
		robocopy /E $filedir $copydir $filename | Out-Null
	}
	catch {
		# ✊
	}

}

