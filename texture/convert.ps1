$files = Get-Item *.jpg

foreach($f in $files){
    Start-Process -FilePath TextureConverter.exe -ArgumentList "$f -ml 0" -Wait
}
pause