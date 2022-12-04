## ファイル群を特定のフォルダに集める

## 収集ファイルのターゲットパス配列
$sources =
	".\actuator\motor\Motor\Motor.h",
	".\algorithm\utility.h",
	".\com\can\CanReader\CanBase.h",
	".\com\can\CanReader\CanReader.h",
	".\com\can\CanWriter\CanWriter.h",
	".\com\can\CanReader\FunctionBinder.h",
	".\sensor\Gyro\Gyro.h",
	".\sensor\Measure\Measure.h"

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
