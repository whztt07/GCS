rem
@set mcpp=C:\OpenEaagles3rdParty\bin\mcpp.exe
rem
%mcpp% configs/gcs.epp > gcs.edl ^
 -I c:\OpenEaagles\include ^
 -I c:\OpenEaaglesExamples\shared\data ^
 -DFONT_PATH="\"c:/OpenEaaglesExamples/shared/data/fonts/\"" ^
 -DFONT_11_19_PATH="\"c:/OpenEaaglesExamples/shared/data/fonts/11x19/normal/\"" ^
 -DTEXTURES_PATH="\"c:/OpenEaaglesExamples/shared/data/textures/\""
 