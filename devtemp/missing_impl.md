## Missing implementation that would be nice to have

# i18n

So far translations are retrieved through a xivo WS, however we are facing FOUC (Flash of untranslated content) issue as we are not in SPA application, idea would either :

* send 304 Header server side if translations hasn't changed (maybe not sufficient to remove FOUC)
* create a script to generate static files from PHP sources that can be loaded at runtime of ng-app
