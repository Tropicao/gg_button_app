# How to use GG Button App

## Starting the application
You should see a gear icon in your system tray. The application offers the following features when system tray icon is clicked :
* Connection status : if the GG Buton is plugged when application is started, the status passed from "Disconnected" to "Connected"
* Generate skeleton : generate an empty configuration file to set sounds (see below)
* Sound list : each loaded side presents a short menu with following actions :
  * Play sound : play the loaded sound right now. Can be used to test if sound is properly loaded
  * Set this sound as default : set this sound as the one played when the GG Button is pushed. A gear is present next to the default sound item in the general menu
* Leave : quit the application

## Configuration file
When started, the application tries to load a configuration file with ini format. If you have no configuration available, you should start the application and ask for an empty skeleton (see appropriate action above).
Next, you can edit the configuration file. You can set up to five different sounds. For each sound, you can set the name which will appear in the application menu,
and the path to corresponding file. Please avoid special characters in path. Do not forget to escape '\' characters. If your configuiration file is invalid or if provided paths are not correct, sound items in the application will be disabled
