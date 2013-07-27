; jnb.nsi
;
;--------------------------------

; the name of the installer
Name "jnb"

; the file to write
; *TODO* support release configuration
; *TODO* support versioning and other metadata
; *TODO* multi-platform support
OutFile "jnb_1_00_win32_setup.exe"

; the default installation directory
InstallDir $EXEDIR

; request application privileges for Windows Vista
RequestExecutionLevel user

; comression
SetCompressor /FINAL bzip2

; autoclose ?
AutoCloseWindow true

;--------------------------------

; pages
Page directory
Page instfiles

;--------------------------------

; the stuff to install
Section "" ; no components page, name is not important

  ; put file(s) here

  ; set output path to the installation directory
  SetOutPath $INSTDIR\data 
  File data\jnb.dat
  SetOutPath $INSTDIR
  File jnb.exe
  File libmikmod-2.dll
  File SDL.dll
  File SDL_mixer.dll
  File SDL_net.dll
  File zlib1.dll

SectionEnd ; end the section
