# @brief ファイル群を特定のフォルダに集める

$ErrorActionPreference = "Stop"

# 収集ファイルのターゲットパス配列
$sources =
".\actuator\motor\Motor\Motor.hpp",
".\algorithm\Utility.hpp",
".\com\message\Message.hpp",
".\com\Common\BasicReader.hpp",
".\com\Common\BasicWriter.hpp",
".\com\CAN\CANBus\CANBus.hpp",
".\com\CAN\CANBus\CANBusInterface.hpp",
".\com\CAN\CANBus\CANBusSPI.hpp",
".\com\CAN\CANBus\CANBusPico.hpp",
".\com\CAN\CANBus\CANBusTeensy.hpp",
".\com\CAN\CANBus\CANCommon.hpp",
".\com\CAN\CANBus\CANInfo.hpp",
".\com\CAN\CANBus\CANReader.hpp",
".\com\CAN\CANBus\CANWriter.hpp",
".\com\I2C\I2CSlaveReader\I2CSlaveReader.hpp",
".\com\I2C\I2CSlaveWriter\I2CSlaveWriter.hpp",
".\sensor\Gyro\Gyro.hpp",
".\sensor\Measure\Measure.hpp",
".\stl\list.hpp",
".\stl\memory.hpp",
".\stl\functional.hpp"

# 収集フォルダ
$target = ".\src"


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

Write-Host "`n-------------------------- Finish ---------------------------`n"