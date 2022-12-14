## ファイル群を特定のフォルダに集める

## 収集ファイルのターゲットパス配列
$sources =
	".\actuator\motor\Motor\Motor.hpp",
	".\algorithm\Utility.hpp",
	".\com\common\Message.hpp",
	".\com\can\CanReader\CanBase.hpp",
	".\com\can\CanReader\CanReader.hpp",
	".\com\can\CanWriter\CanWriter.hpp",
	".\com\can\CanReader\FunctionBinder.hpp",
	".\sensor\Gyro\Gyro.hpp",
	".\sensor\Measure\Measure.hpp"

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
