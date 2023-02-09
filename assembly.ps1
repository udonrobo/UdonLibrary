# @brief ファイル群を特定のフォルダに収集する

$ErrorActionPreference = "Stop"

$selfpath = (Get-Location).Path;

# 収集ファイルのターゲットパス配列
$sources =
"${selfpath}\actuator\motor\Motor\Motor.hpp",
"${selfpath}\algorithm\Utility.hpp",
"${selfpath}\com\common\BasicReader.hpp",
"${selfpath}\com\common\BasicWriter.hpp",
"${selfpath}\com\can\CANBus\CANBus.hpp",
"${selfpath}\com\can\CANBus\CANBusInterface.hpp",
"${selfpath}\com\can\CANBus\CANBusSPI.hpp",
"${selfpath}\com\can\CANBus\CANBusPico.hpp",
"${selfpath}\com\can\CANBus\CANBusTeensy.hpp",
"${selfpath}\com\can\CANBus\CANCommon.hpp",
"${selfpath}\com\can\CANBus\CANInfo.hpp",
"${selfpath}\com\can\CANBus\CANReader.hpp",
"${selfpath}\com\can\CANBus\CANWriter.hpp",
"${selfpath}\com\i2c\I2CSlaveReader\I2CSlaveReader.hpp",
"${selfpath}\com\i2c\I2CSlaveWriter\I2CSlaveWriter.hpp",
"${selfpath}\com\stdmessage\Message.hpp",
"${selfpath}\sensor\Gyro\Gyro.hpp",
"${selfpath}\sensor\Measure\Measure.hpp",
"${selfpath}\stl\list.hpp",
"${selfpath}\stl\memory.hpp",
"${selfpath}\stl\functional.hpp"

# 収集フォルダ
$target = "${selfpath}\src"


Write-Host "`n-------------------------- Source ---------------------------`n"

foreach ($source in $sources) {
	if (Test-Path $source) {
		Write-Host $source
	}
	else {
		Write-Host $source -ForegroundColor Red
	}
}

Write-Host "`n-------------------------- Output ---------------------------`n"

Write-Host $target

## ファイル収集

foreach ($source in $sources) {
	$filedir = Split-Path $source -Parent
	$filename = Split-Path $source -Leaf

	try {
		robocopy $filedir $target $filename | Out-Null
	}
	catch {
		# ぐしゃり✊
	}

}
Write-Host "`n"