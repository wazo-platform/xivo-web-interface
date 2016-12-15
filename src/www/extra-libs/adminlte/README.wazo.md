Fonts
=====

AdminLTE uses Google fonts by default. Wazo modified the default CSS file to
serve local fonts instead. Here is the procedure used to modify the CSS file:

* Find the lines referencing Google fonts, e.g.:
```@import url(https://fonts.googleapis.com/css?family=Source+Sans+Pro:300,400,600,700,300italic,400italic,600italic);```
* Go to https://google-webfonts-helper.herokuapp.com
* Select the font and variant, e.g. font Source Sans Pro, variants (300,regular,600,700,300italic,italic,600italic) (400 is regular)
* Compatibility should be Best Support
* Charset should be latin-ext
* Copy the given CSS sheet in adminlte/css/fonts.css
* Download the given zip file containing font files
* Extract the zip file in adminlte/fonts/
* Replace the Google fonts import from above with the local file you just created, e.g.:
```@import 'fonts.css';```
