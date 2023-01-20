## ファイル群を特定のフォルダに集める

## 収集ファイルのターゲットパス配列
$sources =
	".\actuator\motor\Motor\Motor.hpp",
	".\algorithm\Utility.hpp",

	".\com\common\Message.hpp",
	".\com\Common\BasicReader.hpp",
	".\com\Common\BasicWriter.hpp",

	".\com\can\CanBus\CanReader.hpp",
	".\com\can\CanBus\CanWriter.hpp",
	".\com\can\CanBus\CanBus.hpp",
	".\com\can\CanBus\CanBusSpi.hpp",
	".\com\can\CanBus\CanBusTeensy.hpp",
	".\com\can\CanBus\CanCommon.hpp",

	".\com\i2c\I2cSlaveReader\I2cSlaveReader.hpp",
	".\com\i2c\I2cSlaveWriter\I2cSlaveWriter.hpp",

	".\sensor\Gyro\Gyro.hpp",
	".\sensor\Measure\Measure.hpp",

	".\stl\list.hpp",
	".\stl\memory.hpp"
	".\stl\functional.hpp"

## 収集フォルダ
$target = ".\src"


echo "`n-------------------------- Source ---------------------------`n"

foreach ($source in $sources) {
	if (Test-Path $source) {
		Write-Host $source
	}
	else {
		Write-Host $source -ForegroundColor Red
	}
}

echo "`n-------------------------- Output ---------------------------`n"

echo $target

## ファイル収集
foreach ($source in $sources) {
	$filedir  = Split-Path $source -Parent
	$filename = Split-Path $source -Leaf
	robocopy $filedir $target $filename | Out-Null
}

echo "`n-------------------------- Finish ---------------------------`n"
