## ファイルの中身を同期
## @param $filename  同期元ファイル()
## @param $directory 監視するディレクトリ(全ての子ディレクトリも対象)

$filename  = "id.h"
$sourceDir = "."
$subscDir  = ".\node"

echo "`n--------------------- Monitoring Start ----------------------`n"

echo (Get-ChildItem -Path $subscDir -Recurse).DirectoryName

echo "`n-------------------------- Source ---------------------------`n"

"{0}{1}{2}" -f $pwd.Path, "\", $filename

echo "`n-------------------------------------------------------------`n"

for ( ; ; ) {
	foreach ($it in (Get-ChildItem -Path $sourceDir -Recurse).DirectoryName) {
		robocopy $sourceDir $it $filename | Out-Null
	}
	Start-Sleep -Seconds 1
}