# @brief ファイル群を特定のフォルダに収集する

Set-Location (Split-Path -Parent $MyInvocation.MyCommand.Path)
$ErrorActionPreference = "Stop"

$tofrom =
@(".\src\", ".\actuator\motor\Motor\Motor.hpp"),
@(".\src\", ".\algorithm\Utility.hpp"),
@(".\src\com\", ".\com\common\BasicReader.hpp"),
@(".\src\", ".\com\common\BasicWriter.hpp"),
@(".\src\", ".\com\can\CANBus\CANBus.hpp"),
@(".\src\", ".\com\can\CANBus\CANBusInterface.hpp"),
@(".\src\", ".\com\can\CANBus\CANBusSPI.hpp"),
@(".\src\", ".\com\can\CANBus\CANBusPico.hpp"),
@(".\src\", ".\com\can\CANBus\CANBusTeensy.hpp"),
@(".\src\", ".\com\can\CANBus\CANCommon.hpp"),
@(".\src\", ".\com\can\CANBus\CANInfo.hpp"),
@(".\src\", ".\com\can\CANBus\CANReader.hpp"),
@(".\src\", ".\com\can\CANBus\CANWriter.hpp"),
@(".\src\", ".\com\i2c\I2CSlaveReader\I2CSlaveReader.hpp"),
@(".\src\", ".\com\i2c\I2CSlaveWriter\I2CSlaveWriter.hpp"),
@(".\src\", ".\com\stdmessage\Message.hpp"),
@(".\src\", ".\sensor\Gyro\Gyro.hpp"),
@(".\src\", ".\sensor\Measure\Measure.hpp"),
@(".\src\", ".\stl\list.hpp"),
@(".\src\", ".\stl\memory.hpp"),
@(".\src\", ".\stl\functional.hpp")

Write-Host "`n-------------------------- Source ---------------------------`n"

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

Write-Host "`n-------------------------- Output ---------------------------`n"

# Write-Host $target

## ファイル収集

foreach ($source in $tofrom) {

	$copydir = $source[0]
	$filename = Split-Path $source[1] -Leaf
	$filedir = Split-Path $source[1] -Parent

	try {
		robocopy /E /XC $filedir $copydir $filename
	}
	catch {
		# ぐしゃり✊
	}

}

Write-Host "`n"
