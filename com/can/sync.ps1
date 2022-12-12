## ファイルの中身を同期

## 同期するファイル名
$filename = "id.h"

## 基になるファイルパス
$sources = "."

## 書き換えるファイルがあるパス(全子ディレクトリも対象)
$target  = ".\node"


echo "`n-------------------------- Source ---------------------------`n"

$path = ((Convert-Path $sources) +"\" + $filename)

if (Test-Path $path) {
	Write-Host $path
}
else {
	Write-Host $path -ForegroundColor Red
}

echo "`n-------------------------- Output ---------------------------`n"

foreach ($it in (Get-ChildItem -Path $target -Recurse).DirectoryName) {
	echo ($it +"\" + $filename)
}

## ファイル同期
foreach ($it in (Get-ChildItem -Path $target -Recurse).DirectoryName) {
	robocopy $sources $it $filename | Out-Null
}

echo "`n-------------------------- Finish ---------------------------`n"
